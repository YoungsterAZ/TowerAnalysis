// -*- c++ -*-

///@mainpage CaloSignals Analysis Software
///
/// The software documented on these pages is developed for the analysis of calometer signals in ATLAS.
/// The input data are tuples extracted from the xAOD files described in 
/// <a href="https://twiki.cern.ch/twiki/bin/view/AtlasSandboxProtected/CaloTowerPerformance">calorimeter tower performance twiki</a>
/// (accessible for registered ATLAS user only).
///
///@section m0 Input data
///
/// The tuples contain three calorimeter signal definitions for each event. The clusters of topologically connected cells ( @a topo-cluster ) are   
/// described in <a href="https://link.springer.com/article/10.1140%2Fepjc%2Fs10052-017-5004-5">this public paper</a>. They are the standard
/// calorimeter signals used in ATLAS.
///
/// The two alternative signal definitions are calorimeter towers in a regular @f$ (\eta,\phi) @f$ grid. They are filled with cells from the topo-clusters 
/// ( @a topo-tower ). The grid sizes considered are @f$ \Delta\eta\times\Delta\phi = 0.1\times 0.1 @f$ and @f$ \Delta\eta\times\Delta\phi = 0.05\times 0.05 @f$.   
///
///@section m1 Common codebase
///
///@section m2 User template
///
