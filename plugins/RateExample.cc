/*
 * =====================================================================================
 *
 *       Filename:  RateExample.cc
 *
 *    Description:  Produce a tree for computing rates.
 *
 *         Author:  Evan Friis, evan.friis@cern.ch
 *        Company:  UW Madison
 *
 * =====================================================================================
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticleFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Scalers/interface/LumiScalers.h"

#include "TTree.h"

typedef std::vector<edm::InputTag> VInputTag;

class RateExample : public edm::EDAnalyzer {
        public:
                RateExample(const edm::ParameterSet& pset);
                virtual ~RateExample();
                void analyze(const edm::Event& evt, const edm::EventSetup& es);
        private:
                VInputTag src_;
                VInputTag srcEGIso_;
                VInputTag srcJET_;

                TTree* tree;
                std::vector<Float_t>* pts_;
                std::vector<Float_t>* etas_;
                std::vector<Float_t>* phis_;

                std::vector<Float_t>* ptsIso_;
                std::vector<Float_t>* etasIso_;
                std::vector<Float_t>* phisIso_;

                std::vector<Float_t>* ptsJET_;
                std::vector<Float_t>* etasJET_;
                std::vector<Float_t>* phisJET_;

                UInt_t run_;
                UInt_t lumi_;
                ULong64_t event_;
};

RateExample::RateExample(const edm::ParameterSet& pset) {
        // Initialize the ntuple builder
        edm::Service<TFileService> fs;
        tree = fs->make<TTree>("Ntuple", "Ntuple");

        pts_ = new std::vector<Float_t>();
        etas_ = new std::vector<Float_t>();
        phis_ = new std::vector<Float_t>();

        ptsIso_ = new std::vector<Float_t>();
        etasIso_ = new std::vector<Float_t>();
        phisIso_ = new std::vector<Float_t>();

        ptsJET_ = new std::vector<Float_t>();
        etasJET_ = new std::vector<Float_t>();
        phisJET_ = new std::vector<Float_t>();

        tree->Branch("pt", "std::vector<float>", &pts_);
        tree->Branch("eta", "std::vector<float>", &etas_);
        tree->Branch("phi", "std::vector<float>", &phis_);

        tree->Branch("ptIso", "std::vector<float>", &ptsIso_);
        tree->Branch("etaIso", "std::vector<float>", &etasIso_);
        tree->Branch("phiIso", "std::vector<float>", &phisIso_);

        tree->Branch("ptJET", "std::vector<float>", &ptsJET_);
        tree->Branch("etaJET", "std::vector<float>", &etasJET_);
        tree->Branch("phiJET", "std::vector<float>", &phisJET_);

        tree->Branch("run", &run_, "run/i");
        tree->Branch("lumi", &lumi_, "lumi/i");
        tree->Branch("evt", &event_, "evt/l");

        src_ = pset.getParameter<VInputTag>("src");
        srcEGIso_ = pset.getParameter<VInputTag>("srcEGIso");
        srcJET_ = pset.getParameter<VInputTag>("srcJET");
}

RateExample::~RateExample() {
}


namespace {

        // Predicate to sort candidates by descending pt
        class CandPtSorter {
                public:
                        bool operator()(const reco::Candidate* candA, const reco::Candidate* candB)
                                const {
                                        return candA->pt() > candB->pt();
                                }
        };

        // Turn a set of InputTags into a colleciton of candidate pointers.
        std::vector<const reco::Candidate*> getCollections(
                        const edm::Event& evt, const VInputTag& collections) {
                std::vector<const reco::Candidate*> output;
                // Loop over collections
                for (size_t i = 0; i < collections.size(); ++i) {
                        edm::Handle<edm::View<reco::Candidate> > handle;
                        evt.getByLabel(collections[i], handle);
                        // Loop over objects in current collection
                        for (size_t j = 0; j < handle->size(); ++j) {
                                const reco::Candidate& object = handle->at(j);
                                output.push_back(&object);
                        }
                }
                return output;
        }

        void getValue(const edm::Event& evt, const edm::InputTag& tag, Float_t& et, Float_t& phi) {
                edm::Handle<edm::View<reco::Candidate> > handle;
                evt.getByLabel(tag, handle);
                et = handle->at(0).pt();
                phi = handle->at(0).phi();
        }

        void getSumEtL1(const edm::Event& evt, const edm::InputTag& tag, Float_t& sumet,bool upgrade) {
                if(!upgrade) {
                        edm::Handle<l1extra::L1EtMissParticleCollection> handle;
                        evt.getByLabel(tag, handle);
                        sumet = handle->at(0).etTotal();
                } else{
                        edm::Handle<edm::View<reco::Candidate> > handle;
                        evt.getByLabel(tag, handle);
                        sumet = handle->at(0).pt();
                }
        }
}

void RateExample::analyze(const edm::Event& evt, const edm::EventSetup& es) {

        // Get the objects.
        std::vector<const reco::Candidate*> objects = getCollections(
                        evt, src_);
        std::vector<const reco::Candidate*> objectsEGIso = getCollections(
                        evt, srcEGIso_);
        std::vector<const reco::Candidate*> JETobjects = getCollections(
                        evt, srcJET_);

        // Clear previous event's objects
        pts_->clear();
        etas_->clear();
        phis_->clear();

        ptsIso_->clear();
        etasIso_->clear();
        phisIso_->clear();

        ptsJET_->clear();
        etasJET_->clear();
        phisJET_->clear();

        // Setup meta info
        run_ = evt.id().run();
        lumi_ = evt.id().luminosityBlock();
        event_ = evt.id().event();


        // Loop

        for (size_t i = 0; i < objects.size(); ++i) {
                pts_->push_back(objects[i]->pt());
                etas_->push_back(objects[i]->eta());
                phis_->push_back(objects[i]->phi());
        }

        for (size_t i = 0; i < objectsEGIso.size(); ++i) {
                ptsIso_->push_back(objectsEGIso[i]->pt());
                etasIso_->push_back(objectsEGIso[i]->eta());
                phisIso_->push_back(objectsEGIso[i]->phi());
        }

        for (size_t i = 0; i < JETobjects.size(); ++i) {
                ptsJET_->push_back(JETobjects[i]->pt());
                etasJET_->push_back(JETobjects[i]->eta());
                phisJET_->push_back(JETobjects[i]->phi());
        }

        // Fill the trees

        tree->Fill();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RateExample);
