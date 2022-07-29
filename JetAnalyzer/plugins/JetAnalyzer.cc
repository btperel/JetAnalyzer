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
#include "DataFormats/TrackReco/interface/Track.h"
#include "TTree.h"

struct eventInfo
{
  int evt;
  std::vector <double> track_pt;
  std::vector <double> track_eta;
  std::vector <double> track_phi;
};

class JetAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit JetAnalyzer(const edm::ParameterSet&);
  ~JetAnalyzer();

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void initEventStructure();

  TTree *eventTree;
  eventInfo *evInfo;

  edm::EDGetTokenT<reco::TrackCollection> TrackToken_;
};

JetAnalyzer::JetAnalyzer(const edm::ParameterSet& iConfig)
    : TrackToken_(consumes< reco::TrackCollection >(iConfig.getUntrackedParameter<edm::InputTag>("tracks"))) 
{
  evInfo = new eventInfo;
}

JetAnalyzer::~JetAnalyzer() {
}

// ------------ method called for each event  ------------
void JetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  initEventStructure();
  std::cout << "Processing event: " << iEvent.id().event() << std::endl;
  evInfo->evt = iEvent.id().event();

  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByToken(TrackToken_, tracks);

  // loop over all GenJets
  for (const auto& track : (*tracks)){
    double pt = track.pt();
    double eta = track.eta();
    double phi = track.phi();
    std::cout << "  Track pT: " << pt << " eta: " << eta << " phi: " << phi << std::endl;
    evInfo->track_pt.push_back(pt);
    evInfo->track_eta.push_back(eta);
    evInfo->track_phi.push_back(phi);
  }

  eventTree->Fill();

}

void JetAnalyzer::beginJob() {
  edm::Service<TFileService> fileService;
  if(!fileService) throw edm::Exception(edm::errors::Configuration, "TFileService is not registered in cfg file");

  eventTree = fileService->make<TTree>( "jettree", "jettree" );
  eventTree->Branch( "evt",                  &evInfo->evt);
  eventTree->Branch( "track_pt",               &evInfo->track_pt);
  eventTree->Branch( "track_eta",              &evInfo->track_eta);
  eventTree->Branch( "track_phi",              &evInfo->track_phi);
}

void JetAnalyzer::initEventStructure()
{
  evInfo->evt=-1;
  evInfo->track_pt.clear();
  evInfo->track_eta.clear();
  evInfo->track_phi.clear();
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetAnalyzer);
