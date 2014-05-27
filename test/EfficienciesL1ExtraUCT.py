'''

Creates L1ExtraNtuples (L1 Style) using a UCT->GT jump

Authors: L. Dodd, N. Woods, T. Perry, A. Levine,, S. Dasu, M. Cepeda, E. Friis (UW Madison)

'''

import FWCore.ParameterSet.Config as cms
import os

from FWCore.ParameterSet.VarParsing import VarParsing
process = cms.Process("ReRunningL1")

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/666531DD-9D75-E311-B95D-0025907DC9DC.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/D83DBA64-DE75-E311-88CD-0025907DC9CC.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/8EDF073A-8275-E311-80F7-00266CFFA25C.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/3415AE4D-B075-E311-865F-003048F0E194.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/B8AF4854-8275-E311-886D-00266CF327C4.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/9E9E1E8A-B075-E311-BFA2-00266CFFA048.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/88B7F378-9E75-E311-9F87-0025904B0FB6.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/606195D5-AF75-E311-8399-0025907DCA7E.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/C611F153-D375-E311-9436-003048F0E5B4.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/D28900FF-9E75-E311-9425-00266CF32F00.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/E29B3934-8275-E311-9202-0025901D42BC.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/C2BA874A-B075-E311-8964-0025901D4C92.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/A808CBDF-8F75-E311-8365-003048F0EBBE.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/7C3B05F1-BA75-E311-9495-003048D4610C.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/9E2AD326-B075-E311-B2AD-002481E0D6A6.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/06C609FB-B075-E311-8F3C-003048F0E59E.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/5A2B6818-DB75-E311-96CA-003048D4365C.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/866703E3-8F75-E311-A836-0025904B12B2.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/B6DD64ED-AF75-E311-8B96-00266CF32A00.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/F61F7047-8275-E311-A2D2-0025904B12F0.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/D285F57B-A475-E311-B032-00266CFFB868.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/F285AC91-DA75-E311-803F-003048C6903C.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/2A1BB24D-B075-E311-A5E9-003048F0E18C.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/144FD0F7-BA75-E311-8A18-00266CFFB390.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/20000/CCFD4893-4075-E311-8A60-0025907FD2BA.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/20000/BC8576EE-3575-E311-8AA7-00266CF33340.root",
"/store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/20000/D8EE3250-9775-E311-9096-002481E0D9BC.root",
                             )
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20000)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('efficiencyExample.root')
)

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'POSTLS161_V2::All'

# Load emulation sequences
process.load("L1Trigger.UCT2015.emulationMC_cfi") 
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("L1Trigger.UCT2015.uctl1extraparticles_cfi")

# Change the configuration of the UCT parameters
#process.UCT2015Producer.relativeJetIsolationCut = cms.double(0.5) # electron isolation
#process.UCT2015Producer.jetSeed = cms.uint32(10) # Jet Seed
#process.puMultCorrect = cms.bool(True) # Disable PU subtraction

# Define the generator level particle used to compute the efficiencies
process.createGenParticlesEle =cms.EDProducer("FilterGenParticles",
        MinPtThreshold=cms.untracked.double(5),
        MaxEtaThreshold=cms.untracked.double(2.5),
        GenLevelSelect=cms.untracked.int32(11),
)

#Efficiency modules
process.rlxEGEfficiency = cms.EDAnalyzer(
    "EfficiencyExample",
    recoSrc = cms.VInputTag("createGenParticlesEle"),
    l1Src = cms.VInputTag(
        # These two collections need to be combined:
        # NonIsolated literally means "failing isolation" 
        cms.InputTag("l1extraParticlesUCT", "NonIsolated"),
        cms.InputTag("l1extraParticlesUCT", "Isolated"),  
    ),
    pvSrc = cms.InputTag("offlinePrimaryVertices"),
    # Max DR for trigger matching
    maxDR = cms.double(0.5),
)

process.isoEGEfficiency = cms.EDAnalyzer(
    "EfficiencyExample",
    recoSrc = cms.VInputTag("createGenParticlesEle"),
    l1Src = cms.VInputTag(
        # These two collections
        cms.InputTag("l1extraParticlesUCT", "Isolated"),
    ),
    pvSrc = cms.InputTag("offlinePrimaryVertices"),
    # Max DR for trigger matching
    maxDR = cms.double(0.5),
)

#Run!
process.p1 = cms.Path(
    process.emulationSequence *
    process.uct2015L1Extra 
    *process.createGenParticlesEle
    *process.rlxEGEfficiency
    *process.isoEGEfficiency  
)

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('out.root'),
    outputCommands = cms.untracked.vstring('drop *',
          'keep *_*_*_ReRunningL1',
          'keep *_l1extraParticles*_*_*') 
)

#process.out = cms.EndPath(process.output)


