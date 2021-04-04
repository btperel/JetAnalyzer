# JetAnalyzer

This analyzer read GenJet information in CMSSW and prints the pT, eta, phi for every GenJet in each event. 
Also it creats a root file storing the pT, eta, phi for those GenJets that can be opened without CMSSW.

To run the code:
```shell
cd CMSSW/src
cmsenv
git clone https://github.com/Ang-Li-95/JetAnalyzer.git
cd JetAnalyzer/test/
cmsRun JetAnalyzer_cfg.py
```

The generated root file will be "JetTree.root" under the "test" directory (where you run the code).

Before running the code, make sure this line is changed:

JetAnalyzer/test/JetAnalyzer_cfg.py(https://github.com/Ang-Li-95/JetAnalyzer/blob/master/JetAnalyzer/test/JetAnalyzer_cfg.py#L11)

This line specifies the data file the code is running on, so change it to: 
```shell
'file:/path/to/file'
```
