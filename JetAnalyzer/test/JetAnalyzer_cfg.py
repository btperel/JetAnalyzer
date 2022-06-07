import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
                                  'file:/eos/user/r/rperelma/input_root_file_from_ang.root'
                                )
                           )

#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T15', '')

process.JetAnalyzer = cms.EDAnalyzer('JetAnalyzer',
                                     genjets = cms.untracked.InputTag("ak4GenJets"),
                                    )

process.TFileService = cms.Service("TFileService", fileName = cms.string("JetTree.root") )

process.runseq = cms.Sequence()
process.runseq += process.JetAnalyzer
process.path = cms.Path(process.runseq)
process.schedule = cms.Schedule(process.path)

