'''

Simple Rate trees for testing 
For a more detailed look at rates, use the https://github.com/cms-l1-dpg/L1Menu package


'''

import FWCore.ParameterSet.Config as cms
import os

from FWCore.ParameterSet.VarParsing import VarParsing
process = cms.Process("ReRunningL1")

process.source = cms.Source ("PoolSource",
fileNames = cms.untracked.vstring(
       #"root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00005/02B79593-F47F-E311-8FF6-003048FFD796.root"
       #'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00005/00995EB2-F57F-E311-8C2A-002618943945.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00005/021BEF5B-B07F-E311-A51E-0025905A4964.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00005/02202685-C87F-E311-9CED-003048FF86CA.root',
)
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('RateExample.root')
)

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'POSTLS161_V2::All'

# Load sequences
process.load("L1Trigger.UCT2015.emulationMC_cfi") 
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("L1Trigger.UCT2015.uctl1extraparticles_cfi")

# Change the configuration of the UCT parameters
#process.UCT2015Producer.relativeJetIsolationCut = cms.double(0.5) # electron isolation
#process.UCT2015Producer.jetSeed = cms.uint32(10) # Jet Seed
#process.puMultCorrect = cms.bool(True) # Disable PU subtraction

# Little Rate Example

process.RateExample=cms.EDAnalyzer(
    "RateExample",
    src = cms.VInputTag(cms.InputTag("l1extraParticlesUCT", "NonIsolated"),cms.InputTag("l1extraParticlesUCT", "Isolated")),
    srcEGIso = cms.VInputTag(cms.InputTag("l1extraParticlesUCT", "Isolated")),
    srcJET = cms.VInputTag(cms.InputTag("l1extraParticlesUCT", "Central"),cms.InputTag("l1extraParticlesUCT", "Forward")),
)

process.p1 = cms.Path(
    process.emulationSequence *
    process.uct2015L1Extra 
    *process.RateExample    
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


