#include <memory>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoMTD/TransientTrackingRecHit/interface/MTDTransientTrackingRecHit.h"
#include "TTree.h"

struct eventInfo
{
  int evt;
  std::vector <double> track_pt;
  std::vector <double> track_eta;
  std::vector <double> track_phi;
  std::vector <double> track_t0;
  std::vector <double> track_dz; 
  std::vector <int> tpr_pID;
  std::vector <double> tpr_simpt; 
  std::vector <double> tpr_simeta; 
  std::vector <double> tpr_simphi; 
  std::vector <double> track_tracktMTD;
  std::vector <double> track_trackpathlength;    
};

class TrackAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit TrackAnalyzer(const edm::ParameterSet&);
  ~TrackAnalyzer();

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void initEventStructure();

  TTree *eventTree;
  eventInfo *evInfo;

  edm::EDGetTokenT<edm::View<reco::Track>> TrackToken_;
  edm::EDGetTokenT<reco::RecoToSimCollection> rectosimToken_; 
  edm::EDGetTokenT<edm::View<reco::Track>> MTDTrackToken_;
  edm::EDGetTokenT<edm::ValueMap<int>> TrackAssocToken_;
  edm::EDGetTokenT<edm::ValueMap<float> > trackstMTDToken_;
  edm::EDGetTokenT<edm::ValueMap<float>>trackspathlengthToken_; 
};

TrackAnalyzer::TrackAnalyzer(const edm::ParameterSet& iConfig)
    : TrackToken_(consumes< edm::View<reco::Track> >(iConfig.getUntrackedParameter<edm::InputTag>("tracks"))),
      rectosimToken_(consumes<reco::RecoToSimCollection>(iConfig.getUntrackedParameter<edm::InputTag>("rectosim"))),
      MTDTrackToken_(consumes< edm::View<reco::Track> >(iConfig.getUntrackedParameter<edm::InputTag>("mtdtracks"))),
      TrackAssocToken_(consumes<edm::ValueMap<int>>(iConfig.getUntrackedParameter<edm::InputTag>("TrackAssoc"))),
      trackstMTDToken_(consumes<edm::ValueMap<float> >(iConfig.getUntrackedParameter<edm::InputTag>("trackstMTD"))),
      trackspathlengthToken_(consumes<edm::ValueMap<float>>(iConfig.getUntrackedParameter<edm::InputTag>("pathlength")))
{
  evInfo = new eventInfo;
}

TrackAnalyzer::~TrackAnalyzer() {
}


// ------------ method called for each event  ------------
void TrackAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  initEventStructure();
  std::cout << "Processing event: " << iEvent.id().event() << std::endl;
  evInfo->evt = iEvent.id().event();

//  edm::Handle<reco::TrackCollection> tracks;
//  iEvent.getByToken(TrackToken_, tracks);

//  edm::Handle<edm::ValueMap<float> > trackstMTDHandle_;
 // iEvent.getByToken(trackstMTDToken_,trackstMTDHandle_); 

  const auto& tMtd = iEvent.get(trackstMTDToken_); 
  const auto& trackAssoc = iEvent.get(TrackAssocToken_);
  const auto& pathlength = iEvent.get(trackspathlengthToken_);

  edm::Handle<reco::RecoToSimCollection > rectosimCollection;
  iEvent.getByToken(rectosimToken_,rectosimCollection);
  //iEvent.getByLabel("trackingParticleRecoTrackAsssociation", rectosimCollection);
  auto& recSimColl = *(rectosimCollection.product());

  edm::Handle<edm::View<reco::Track>> trackCollectionH;
  iEvent.getByToken(TrackToken_, trackCollectionH);
  const edm::View<reco::Track>& trackCollection = *(trackCollectionH.product()); 

  edm::Handle<edm::View<reco::Track>> mtdtrackCollectionH;
  iEvent.getByToken(MTDTrackToken_, mtdtrackCollectionH);
  const edm::View<reco::Track>& mtdtrackCollection = *(mtdtrackCollectionH.product());

 for(edm::View<reco::Track>::size_type i=0; i<trackCollection.size(); ++i){
    edm::RefToBase<reco::Track> track(trackCollectionH, i);
    edm::Ptr< reco::Track > ptr( trackCollectionH, i);
    auto& ti = trackCollection[i];
//    if (!(ti.isTimeOk())){
//       continue; 
//    } 
    double pt = ti.pt(); 
    double eta = ti.eta(); 
    double phi = ti.phi(); 
    double t0 = 0; 
    double dz = ti.dz(); 
    double tracktMTD = tMtd[ptr];
    double trackpathlength = pathlength[ptr]; 
    double simpt = -99;
    double simeta = -99; 
    double simphi = -99; 
    int pID = 0;
   // std::cout << " Track pT: " << pt << " eta: " << eta << "phi: " << phi << "t0: " << t0 << std::endl; 
    std::vector<std::pair<TrackingParticleRef, double> > tp;
    if(recSimColl.find(track) != recSimColl.end()){
      tp = recSimColl[track];
    // std::cout << "checkpoint1" << std::endl;  
     if (tp.size()!=0) {
    // std::cout << "checkpoint2" << std::endl; 
              TrackingParticleRef tpr = tp.begin()->first;
              double associationQuality = tp.begin()->second;
              simpt = tpr->pt();
              simeta = tpr->eta(); 
              simphi = tpr->phi();  
              pID = tpr->pdgId(); 
              std::cout << "asso. particle " << tpr->pdgId() << " with quality " << associationQuality << std::endl;
          }
    }
      if (trackAssoc[ptr] == -1) {
      continue;
    } 
      const auto& mtdtrack = mtdtrackCollection.at(trackAssoc[ptr]);
        
      if (!(mtdtrack.isTimeOk())){
         continue; 
      } 

      t0 = mtdtrack.t0(); 

      std::cout << "Track pT: " << pt << "eta: " << eta << "phi: " << phi << "dz: " << dz << "tracktMTD: " << tracktMTD << "pID: " << pID << " t0: " << mtdtrack.t0() << "trackpathlength: " << trackpathlength << "simpt: "<< simpt << "simeta: " << simeta << "simphi: " << simphi << std::endl; 

      evInfo->track_pt.push_back(pt); 
      evInfo->track_eta.push_back(abs(eta)); 
      evInfo->track_phi.push_back(phi);
      evInfo->track_t0.push_back(t0);
      evInfo->track_dz.push_back(dz); 
      evInfo->tpr_simpt.push_back(simpt); 
      evInfo->tpr_simeta.push_back(simeta); 
      evInfo->tpr_simphi.push_back(simphi); 
      evInfo->track_tracktMTD.push_back(tracktMTD); 
      evInfo->track_trackpathlength.push_back(trackpathlength); 
      evInfo->tpr_pID.push_back(abs(pID)); 


  }



  // loop over all GenJets
 // for (const auto& track : (*tracks)){
   // if (!(track.isTimeOk())){
    //	continue; 
   // }
   // double pt = track.pt();
  // double eta = track.eta();
   // double phi = track.phi();
   // double t0 = track.t0();
   // std::cout << "  Track pT: " << pt << " eta: " << eta << " phi: " << phi << " t0: " << t0 << std::endl;
  //  evInfo->track_pt.push_back(pt);
  //  evInfo->track_eta.push_back(eta);
  //  evInfo->track_phi.push_back(phi);
  //  evInfo->track_t0.push_back(t0); 
//  }

  eventTree->Fill();
    
}


void TrackAnalyzer::beginJob() {
  edm::Service<TFileService> fileService;
  if(!fileService) throw edm::Exception(edm::errors::Configuration, "TFileService is not registered in cfg file");

  eventTree = fileService->make<TTree>( "jettree", "jettree" );
  eventTree->Branch( "evt",                    &evInfo->evt);
  eventTree->Branch( "track_pt",               &evInfo->track_pt);
  eventTree->Branch( "track_eta",              &evInfo->track_eta);
  eventTree->Branch( "track_phi",              &evInfo->track_phi);
  eventTree->Branch( "track_t0",	       &evInfo->track_t0);
  eventTree->Branch( "track_dz",               &evInfo->track_dz); 
  eventTree->Branch( "tpr_simpt",              &evInfo->tpr_simpt); 
  eventTree->Branch( "tpr_simeta",             &evInfo->tpr_simeta); 
  eventTree->Branch( "tpr_simphi",             &evInfo->tpr_simphi); 
  eventTree->Branch( "track_tracktMTD",        &evInfo->track_tracktMTD);  
  eventTree->Branch( "tpr_pID",                &evInfo->tpr_pID);
  eventTree->Branch( "track_trackpathlength",  &evInfo->track_trackpathlength);   
}

void TrackAnalyzer::initEventStructure()
{
  evInfo->evt=-1;
  evInfo->track_pt.clear();
  evInfo->track_eta.clear();
  evInfo->track_phi.clear();
  evInfo->track_t0.clear();
  evInfo->track_dz.clear(); 
  evInfo->tpr_simpt.clear();
  evInfo->tpr_simeta.clear();
  evInfo->tpr_simphi.clear(); 
  evInfo->track_tracktMTD.clear(); 
  evInfo->tpr_pID.clear(); 
  evInfo->track_trackpathlength.clear();  
}


//define this as a plug-in
DEFINE_FWK_MODULE(TrackAnalyzer);
