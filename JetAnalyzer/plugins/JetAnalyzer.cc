#include <memory>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "TTree.h"

struct eventInfo
{
  int evt;
  std::vector <double> jet_pt;
  std::vector <double> jet_eta;
  std::vector <double> jet_phi;
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

  edm::EDGetTokenT<reco::GenJetCollection> GenJetToken_;
};

JetAnalyzer::JetAnalyzer(const edm::ParameterSet& iConfig)
    : GenJetToken_(consumes< reco::GenJetCollection >(iConfig.getUntrackedParameter<edm::InputTag>("genjets"))) 
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

  edm::Handle<reco::GenJetCollection> genjets;
  iEvent.getByToken(GenJetToken_, genjets);

  // loop over all GenJets
  for (const auto& jet : (*genjets)){
    double pt = jet.pt();
    double eta = jet.eta();
    double phi = jet.phi();
    std::cout << "  GenJet pT: " << pt << " eta: " << eta << " phi: " << phi << std::endl;
    evInfo->jet_pt.push_back(pt);
    evInfo->jet_eta.push_back(eta);
    evInfo->jet_phi.push_back(phi);
  }

  eventTree->Fill();

}

void JetAnalyzer::beginJob() {
  edm::Service<TFileService> fileService;
  if(!fileService) throw edm::Exception(edm::errors::Configuration, "TFileService is not registered in cfg file");

  eventTree = fileService->make<TTree>( "jettree", "jettree" );
  eventTree->Branch( "evt",                  &evInfo->evt);
  eventTree->Branch( "jet_pt",               &evInfo->jet_pt);
  eventTree->Branch( "jet_eta",              &evInfo->jet_eta);
  eventTree->Branch( "jet_phi",              &evInfo->jet_phi);
}

void JetAnalyzer::initEventStructure()
{
  evInfo->evt=-1;
  evInfo->jet_pt.clear();
  evInfo->jet_eta.clear();
  evInfo->jet_phi.clear();
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetAnalyzer);
