# TrackAnalyzer

This analyzer reads the reconstructed tracks from simulated events in CMSSW and prints the pT, eta, phi, t0 (time at reference point), dz, particle ID, simpT, simeta, simphi, tMTD (time at MTD), and path length to MTD in each event. It also gives association information between the simulated and reconstructed tracks. 
The timing variables have units of ns, and the path length units are cm. 
Also it creats a root file storing those variables for the GenRecoTracks that can be opened without CMSSW.

To run the code:
```shell
cd CMSSW/src
cmsenv
git clone https://github.com/btperel/TrackAnalyzer.git
scram b
cd JetAnalyzer/test/
cmsRun TrackAnalyzer_cfg.py
```

The generated root file will be "TrackTree.root" under the "test" directory (where you run the code).

Before running the code, make sure this line is changed:

TrackAnalyzer/test/TrackAnalyzer_cfg.py(https://github.com/btperel/JetAnalyzer/blob/317cd4c22a2639a6cfb54dde81c973e191513390/JetAnalyzer/test/TrackAnalyzer_cfg.py#L11)

This line specifies the data file the code is running on, so change it to: 
```shell
'file:/path/to/file'
```
