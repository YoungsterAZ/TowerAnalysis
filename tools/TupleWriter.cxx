
#include "TupleWriter.h"

#include "AnalysisHelper.h"

#include <TString.h>

#include <limits>
#include <cmath>

Analysis::TupleWriter::TupleWriter(const std::string& fname)
  : _fname(fname)
  , _file((TFile*)0)
  , _tree((TTree*)0)
  , _nEvts(0)
  , _oEvts(std::numeric_limits<int>::max())
  , _iFiles(-1)
{
  static int         _resrv = 10000;
  _truthJet_e.reserve(_resrv);
  _truthJet_pt.reserve(_resrv);
  _truthJet_rap.reserve(_resrv);
  _truthJet_phi.reserve(_resrv);
  _truthJet_m.reserve(_resrv);
  _truthJet_nc.reserve(_resrv);
  _truthJet_idx.reserve(_resrv);
  _truthJet_matchIdx.reserve(_resrv);
  _truthJet_matchR.reserve(_resrv);
  _recoJet_e.reserve(_resrv);
  _recoJet_pt.reserve(_resrv);
  _recoJet_rap.reserve(_resrv);
  _recoJet_phi.reserve(_resrv);
  _recoJet_m.reserve(_resrv);
  _recoJet_nc.reserve(_resrv);
  _recoJet_idx.reserve(_resrv);
  _recoJet_matchIdx.reserve(_resrv);
  _recoJet_matchR.reserve(_resrv);
  _recoJet_ptd.reserve(_resrv);
  _recoJet_width.reserve(_resrv);
}

Analysis::TupleWriter::~TupleWriter()
{ }

bool Analysis::TupleWriter::book(const std::string& tname,int evtsPerFile)
{
  this->setEvtsPerFile(evtsPerFile);
  return this->book(tname);
}

bool Analysis::TupleWriter::book(const std::string& tname)
{
  static std::string _mname = "TupleWriter::book";
  static std::vector<std::string> _tstr = {
    "TTTTT U   U PPPP  L     EEEEE W   W RRRR  III TTTTT EEEEE RRRR ",
    "  T   U   U P   P L     E     W   W R   R  I    T   E     R   R",
    "  T   U   U PPPP  L     EEE   W W W RRRR   I    T   EEEE  RRRR ",
    "  T   U   U P     L     E     WW Ww R  R   I    T   E     R  R ",
    "  T    UUU  P     LLLLL EEEEE  W W  R   R  I    T   EEEEE R   R"  
  };

  std::string fn;

  if ( _file != 0 ) {
    PRINT_INFO( _mname, "# events written to file \042%s\042: %i", _file->GetName(), (int)this->write() );
    if ( _iFiles < 0 ) { _iFiles = 0; } else { ++_iFiles; }
    fn = f_fname();
    PRINT_INFO( _mname, "file \042%s\042 closed, open new file \042%s\042", _file->GetName(), fn.c_str() );
    f_fclose();
  } else { 
    fn = f_fname();
    PRINT_INFO( _mname, " " );
    Analysis::Banner::print(_mname,_tstr); 
    PRINT_INFO( _mname, " " );
    PRINT_INFO( _mname, "book tuple in file \042%s\042", fn.c_str() );
  }

  if ( _tree != 0 ) {
    PRINT_WARNING( _mname, "TTree <%s> already booked", _tree->GetName() ); 
    return false;
  } else { 
    PRINT_INFO( _mname, "Attempt to book tree <%s>", tname.c_str() );
  }

  if ( !f_fopen(fn) || _file == 0 ) { 
    PRINT_ERROR( _mname, "cannot open file \042%s\042 for tuple output", fn.c_str() );
    return false;
  }

  PRINT_INFO( _mname, "change to tuple directory" );
  _file->cd();

  _tree = new TTree(tname.c_str(),tname.c_str());
  _tree->SetDirectory(_file);
  PRINT_INFO( _mname, "booked tree <%s> in directory \042%s\042", _tree->GetName(), _file->GetName() );

  _tree->Branch("idxEvt",     &_ievt,     "idxEvt/I");
  _tree->Branch("idxLead",    &_ilead,    "idxLead/I");
  _tree->Branch("idxSublead", &_isublead, "idxSublead/I");

  _tree->Branch("minv",       &_minv,     "minv/F");
  _tree->Branch("rapgap",     &_rapgap,   "rapgap/F");

  _tree->Branch("truthJet_mult",&_truthJet_mult,"truthJet_mult/I");
  _tree->Branch<std::vector<float> >("truthJet_e",       &_truthJet_e);
  _tree->Branch<std::vector<float> >("truthJet_pt",      &_truthJet_pt);
  _tree->Branch<std::vector<float> >("truthJet_rap",     &_truthJet_rap);
  _tree->Branch<std::vector<float> >("truthJet_phi",     &_truthJet_phi);
  _tree->Branch<std::vector<float> >("truthJet_m",       &_truthJet_m);
  _tree->Branch<std::vector<int>   >("truthJet_nc",      &_truthJet_nc);
  _tree->Branch<std::vector<int>   >("truthJet_idx",     &_truthJet_idx);
  _tree->Branch<std::vector<int>   >("truthJet_matchIdx",&_truthJet_matchIdx);
  _tree->Branch<std::vector<float> >("truthJet_matchR",  &_truthJet_matchR);

  _tree->Branch("recoJet_mult",&_recoJet_mult,"recoJet_mult/I");
  _tree->Branch<std::vector<float> >("recoJet_e",        &_recoJet_e);
  _tree->Branch<std::vector<float> >("recoJet_pt",       &_recoJet_pt);
  _tree->Branch<std::vector<float> >("recoJet_rap",      &_recoJet_rap);
  _tree->Branch<std::vector<float> >("recoJet_phi",      &_recoJet_phi);
  _tree->Branch<std::vector<float> >("recoJet_m",        &_recoJet_m);
  _tree->Branch<std::vector<int>   >("recoJet_nc",       &_recoJet_nc);
  _tree->Branch<std::vector<int>   >("recoJet_idx",      &_recoJet_idx);
  _tree->Branch<std::vector<int>   >("recoJet_matchIdx", &_recoJet_matchIdx);
  _tree->Branch<std::vector<float> >("recoJet_matchR",   &_recoJet_matchR);
  _tree->Branch<std::vector<float> >("recoJet_ptd",      &_recoJet_ptd);
  _tree->Branch<std::vector<float> >("recoJet_width",    &_recoJet_width);

  return true;
}

bool Analysis::TupleWriter::fill(int evtIdx,const Matching::JetRelationContainer& jetRel,const std::vector<fastjet::PseudoJet>& truthJet,const std::vector<fastjet::PseudoJet>& recoJet,
				 int leadIdx,int subleadIdx)
{
  ++_nEvts;
  if ( _oEvts < std::numeric_limits<int>::max() && _oEvts > 0 && _nEvts % _oEvts == 0 ) {
    this->book(_tree->GetName());
  }

  this->f_reset();

  ////////////////
  // Event data //
  ////////////////

  _ievt     = evtIdx;
  bool idxOkLead( leadIdx    > 0    && leadIdx    < (int)recoJet.size() );
  bool idxOkSublead( subleadIdx > 0 && subleadIdx < (int)recoJet.size() );
  if ( idxOkLead )    { _ilead    = leadIdx;    }
  if ( idxOkSublead ) { _isublead = subleadIdx; }

  if ( idxOkLead && idxOkSublead ) { 
    _minv   = (recoJet.at(_ilead)+recoJet.at(_isublead)).m()/Analysis::Units::GeV;
    _rapgap = std::abs(recoJet.at(_ilead).rap()-recoJet.at(_isublead).rap());
  }

  ////////////////////
  // Truth jet data //
  ////////////////////

  _truthJet_mult = (int)truthJet.size();
  for ( size_t i(0); i<truthJet.size(); ++i ) {
    const auto& pj = truthJet.at(i);
    _truthJet_e.push_back(pj.e()/Analysis::Units::GeV);
    _truthJet_pt.push_back(pj.pt()/Analysis::Units::GeV);
    _truthJet_rap.push_back(pj.rap());
    _truthJet_phi.push_back(pj.phi_std());
    _truthJet_m.push_back(pj.m()/Analysis::Units::GeV);
    _truthJet_nc.push_back((int)AH::numConstits(pj));
    _truthJet_idx.push_back((int)Analysis::FastJet::UserInfo::dataIndex(pj));
    if ( jetRel.checkTruth(i) ) { 
      double deltaR(0.);
      _truthJet_matchIdx.push_back(jetRel.caloIndex(i,deltaR));
      _truthJet_matchR.push_back(deltaR);
    } else { 
      _truthJet_matchIdx.push_back(-1);
      _truthJet_matchR.push_back(-1.);
    }
  }

  ///////////////////
  // Reco jet data //
  ///////////////////

  _recoJet_mult = (int)recoJet.size();
  for ( size_t i(0); i<recoJet.size(); ++i ) {
    const auto& pj = recoJet.at(i);
    _recoJet_e.push_back(pj.e()/Analysis::Units::GeV);
    _recoJet_pt.push_back(pj.pt()/Analysis::Units::GeV);
    _recoJet_rap.push_back(pj.rap());
    _recoJet_phi.push_back(pj.phi_std());
    _recoJet_m.push_back(pj.m()/Analysis::Units::GeV);
    _recoJet_nc.push_back((int)AH::numConstits(pj));
    _recoJet_idx.push_back((int)Analysis::FastJet::UserInfo::dataIndex(pj));
    if ( jetRel.checkCalo(i) ) { 
      double deltaR(0.);
      _recoJet_matchIdx.push_back(jetRel.truthIndex(i,deltaR));
      _recoJet_matchR.push_back(deltaR);
    } else { 
      _recoJet_matchIdx.push_back(-1);
      _recoJet_matchR.push_back(-1.);
    }
    _recoJet_ptd.push_back(AH::JetProperties::ptD(pj));
    _recoJet_width.push_back(AH::JetProperties::width(pj));
  }

  // fill the tree
  _tree->Fill();

  return true;
}

int Analysis::TupleWriter::write()
{ _file->cd(); _tree->Write(); return (int)_tree->GetEntries(); }

bool Analysis::TupleWriter::close()
{ 
  static std::string _mname = "TupleWriter::close";
  std::string fn = _file != 0 ? _file->GetName() : "UNKNOWN";
  if ( this->f_fclose(false) )  {
    PRINT_INFO( _mname, "successfully closed root tuple file \042%s\042 with %i entries", fn.c_str(), (int)_tree->GetEntries() );
    return true; 
  } else { 
    PRINT_WARNING( _mname, "problems closing root tuple file \042%s\042", fn.c_str() );
    return false;
  }
}

void Analysis::TupleWriter::setEvtsPerFile(int evtsPerFile ) {
  _oEvts = evtsPerFile;
}

std::string Analysis::TupleWriter::f_fname() {
  if ( _oEvts < std::numeric_limits<int>::max() && _oEvts > 0 ) { 
    if ( _iFiles < 0 ) { _iFiles = 0; }
    return std::string(TString::Format("%s_%04i.root",_fname.c_str(),_iFiles).Data());
  } else { 
    return std::string(TString::Format("%s.root",_fname.c_str()).Data());
  }
}

bool Analysis::TupleWriter::f_fopen(const std::string& fname ) {
  _file = new TFile(fname.c_str(),"RECREATE");
  return _file != 0 && _file->IsOpen(); 
}

bool Analysis::TupleWriter::f_fclose(bool delFlg) {
  if ( _file != 0 ) { 
    _file->Close(); if ( delFlg ) { delete _file; _file = (TFile*)0; } return true; 
  } else {
    return false;
  }
}

void Analysis::TupleWriter::f_reset() {
  _ievt     = -1;
  _ilead    = -1;
  _isublead = -1;

  _minv     = -1.;
  _rapgap   =  0.;

  _truthJet_mult = 0;
  _truthJet_e.clear();
  _truthJet_pt.clear();
  _truthJet_rap.clear();
  _truthJet_phi.clear();
  _truthJet_m.clear();
  _truthJet_nc.clear();
  _truthJet_idx.clear();
  _truthJet_matchIdx.clear();
  _truthJet_matchR.clear();

  _recoJet_mult = 0;
  _recoJet_e.clear();
  _recoJet_pt.clear();
  _recoJet_rap.clear();
  _recoJet_phi.clear();
  _recoJet_m.clear();
  _recoJet_nc.clear();
  _recoJet_idx.clear();
  _recoJet_matchIdx.clear();
  _recoJet_matchR.clear();
  _recoJet_ptd.clear();
  _recoJet_width.clear();
}
