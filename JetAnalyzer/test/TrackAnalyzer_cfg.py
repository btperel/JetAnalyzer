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

process.load("SimTracker.TrackerHitAssociation.tpClusterProducer_cfi")
process.load("SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi")
process.load('SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi')

process.TrackAnalyzer = cms.EDAnalyzer('TrackAnalyzer',
                                     tracks = cms.untracked.InputTag("generalTracks"),
                                     rectosim = cms.untracked.InputTag("trackingParticleRecoTrackAsssociation"), 
                                     trackstMTD = cms.untracked.InputTag("trackExtenderWithMTD", "generalTracktmtd", "RECO"),  
                                     pathlength = cms.untracked.InputTag("trackExtenderWithMTD", "generalTrackPathLength", "RECO"),
                                     mtdtracks = cms.untracked.InputTag("trackExtenderWithMTD"),
                                     TrackAssoc = cms.untracked.InputTag("trackExtenderWithMTD:generalTrackassoc"),        
     )

process.TFileService = cms.Service("TFileService", fileName = cms.string("TrackTree.root") )

process.runseq = cms.Sequence()
#process.runseq += process.trackingParticleRecoTrackAsssociation
process.runseq += process.TrackAnalyzer
process.path = cms.Path(process.runseq)
process.schedule = cms.Schedule(process.path)

