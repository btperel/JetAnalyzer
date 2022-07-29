# JetAnalyzer

This analyzer reads the GenRecoTrack information in CMSSW and prints the pT, eta, phi, t0, dz, particle ID, simpT, simeta, simphi, tMTD, and path length for every GenRecoTrack in each event. 
Also it creats a root file storing those variables for the GenRecoTracks that can be opened without CMSSW.

To run the code:
```shell
cd CMSSW/src
cmsenv
git clone https://github.com/btperel/JetAnalyzer.git
scram b
cd JetAnalyzer/test/
cmsRun TrackAnalyzer_cfg.py
```

The generated root file will be "TrackTree.root" under the "test" directory (where you run the code).

Before running the code, make sure this line is changed:

JetAnalyzer/test/JetAnalyzer_cfg.py(https://github.com/btperel/JetAnalyzer/blob/317cd4c22a2639a6cfb54dde81c973e191513390/JetAnalyzer/test/TrackAnalyzer_cfg.py#L11)

This line specifies the data file the code is running on, so change it to: 
```shell
'file:/path/to/file'
```
