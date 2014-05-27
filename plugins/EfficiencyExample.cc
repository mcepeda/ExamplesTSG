/*
 * =====================================================================================
 *
 *       Filename:  EfficiencyExample.cc
 *
 *    Description:  Produce a tree for making efficiencies
 *
 * =====================================================================================
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <TTree.h>

typedef std::vector<edm::InputTag> VInputTag;
typedef std::vector<unsigned int> PackedUIntCollection;

class EfficiencyExample : public edm::EDAnalyzer {
        public:
                EfficiencyExample(const edm::ParameterSet& pset);
                virtual ~EfficiencyExample(){}
                void analyze(const edm::Event& evt, const edm::EventSetup& es);
        private:
                VInputTag recoSrc_;
                VInputTag l1Src_;
                edm::InputTag pvSrc_;
                double maxDR_;

                TTree* tree_;
                std::vector<Float_t>* ptsL1_;
                std::vector<Float_t>* etasL1_;
                std::vector<Float_t>* phisL1_;
                std::vector<Float_t>* ptsReco_;
                std::vector<Float_t>* etasReco_;
                std::vector<Float_t>* phisReco_;
                UInt_t run_;
                UInt_t lumi_;
                ULong64_t event_;
                int nvtx_;

};

EfficiencyExample::EfficiencyExample(const edm::ParameterSet& pset){
        // input collections
        recoSrc_ = pset.getParameter<VInputTag>("recoSrc");
        l1Src_ = pset.getParameter<VInputTag>("l1Src");
        pvSrc_ = pset.getParameter<edm::InputTag>("pvSrc");
        maxDR_ = pset.getParameter<double>("maxDR");

        // output tree   
        edm::Service<TFileService> fs;
        tree_ = fs->make<TTree>("Ntuple", "Ntuple");
        ptsReco_ = new std::vector<Float_t>();
        etasReco_ = new std::vector<Float_t>();
        phisReco_ = new std::vector<Float_t>();
        ptsL1_ = new std::vector<Float_t>();
        etasL1_ = new std::vector<Float_t>();
        phisL1_ = new std::vector<Float_t>();
        tree_->Branch("ptReco", "std::vector<float>", &ptsReco_);
        tree_->Branch("etaReco", "std::vector<float>", &etasReco_);
        tree_->Branch("phiReco", "std::vector<float>", &phisReco_);
        tree_->Branch("ptL1", "std::vector<float>", &ptsL1_);
        tree_->Branch("etaL1", "std::vector<float>", &etasL1_);
        tree_->Branch("phiL1", "std::vector<float>", &phisL1_);
        tree_->Branch("run", &run_, "run/i");
        tree_->Branch("lumi", &lumi_, "lumi/i");
        tree_->Branch("evt", &event_, "evt/l");
        tree_->Branch("nvtx", &nvtx_, "nvtx/i");
}




namespace {

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

        // Find the reco::Candidate in the [l1Collection] closes in DeltaR to
        // [recoObject].  Only objects within [maxDR] are considered.  If no match
        // is found, NULL is returned.
        const reco::Candidate* findBestMatch(const reco::Candidate* recoObject,
                        std::vector<const reco::Candidate*>& l1Collection, double maxDR) {
                const reco::Candidate* output = NULL;
                double bestDeltaR = -1;
                for (size_t i = 0; i < l1Collection.size(); ++i) {
                        double deltaR = reco::deltaR(*recoObject, *l1Collection[i]);
                        if (deltaR < maxDR) {
                                if (!output || deltaR < bestDeltaR) {
                                        output = l1Collection[i];
                                        bestDeltaR = deltaR;

                                }
                        }
                }
                return output;
        }

}

void EfficiencyExample::analyze(const edm::Event& evt, const edm::EventSetup& es) {
        // Get the RECO and regular L1 corrections
        std::vector<const reco::Candidate*> recoObjects = getCollections(
                        evt, recoSrc_);
        std::vector<const reco::Candidate*> l1Objects = getCollections(
                        evt, l1Src_);

        // Get PV collection
//        edm::Handle<reco::VertexCollection> vertices;
//        evt.getByLabel(pvSrc_, vertices);


        // general collections
//        nvtx_ = vertices->size();
        run_ = evt.id().run();
        lumi_ = evt.id().luminosityBlock();
        event_ = evt.id().event();

        // L1-Reco match

        ptsL1_->clear(); etasL1_->clear(); phisL1_->clear();
        ptsReco_->clear(); etasReco_->clear(); phisReco_->clear();

        for (size_t i = 0; i < recoObjects.size(); ++i) {
                const reco::Candidate* recoObject = recoObjects[i];
                const reco::Candidate* bestL1 = findBestMatch(recoObject, l1Objects, maxDR_);

                if(bestL1!=NULL){
                        ptsL1_->push_back(bestL1->pt());
                        etasL1_->push_back(bestL1->eta());
                        phisL1_->push_back(bestL1->phi());
                }else{
                        ptsL1_->push_back(-10);
                        etasL1_->push_back(-10);
                        phisL1_->push_back(-10);
                }

                ptsReco_->push_back(recoObject->pt());
                etasReco_->push_back(recoObject->eta());
                phisReco_->push_back(recoObject->phi());
        }

        // fill!

        tree_->Fill();

}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EfficiencyExample);
