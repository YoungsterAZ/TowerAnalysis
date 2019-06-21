// -*- c++ -*-
#ifndef TUPLEWRITER_H
#define TUPLEWRITER_H

#include <TFile.h>
#include <TTree.h>

#include "fastjet/PseudoJet.hh"
#include "AnalysisData.h"

#include <string>
#include <vector>

namespace Analysis {
  class TupleWriter
  {
  public:
    TupleWriter(const std::string& fname = "tuple.root");
    virtual ~TupleWriter();

    virtual bool book(const std::string& tname = "OutputTree");
    virtual bool book(const std::string& tname,int evtsPerFile);
    virtual bool fill(int evtIdx,const Matching::JetRelationContainer& jetRel,const std::vector<fastjet::PseudoJet>& truthJet,const std::vector<fastjet::PseudoJet>& recoJet,
		      int leadIdx=-1, int subleadIdx=-1);
    virtual int  write();
    virtual bool close();

    virtual void setEvtsPerFile(int evtsPerFile=-1);

  protected:

    int   _ievt;
    int   _ilead;
    int   _isublead;
    float _minv;
    float _rapgap;
    
    int                 _truthJet_mult;
    std::vector<float> _truthJet_e;
    std::vector<float> _truthJet_pt;
    std::vector<float> _truthJet_rap;
    std::vector<float> _truthJet_phi;
    std::vector<float> _truthJet_m;
    std::vector<int>   _truthJet_nc;
    std::vector<int>   _truthJet_idx;
    std::vector<int>   _truthJet_matchIdx;
    std::vector<float> _truthJet_matchR;

    int                 _recoJet_mult;
    std::vector<float> _recoJet_e;
    std::vector<float> _recoJet_pt;
    std::vector<float> _recoJet_rap;
    std::vector<float> _recoJet_phi;
    std::vector<float> _recoJet_m;
    std::vector<int>   _recoJet_nc;
    std::vector<int>   _recoJet_idx;
    std::vector<int>   _recoJet_matchIdx;
    std::vector<float> _recoJet_matchR;

    std::vector<float> _recoJet_ptd;
    std::vector<float> _recoJet_width;

    void f_reset();

    bool f_fopen(const std::string& fname);
    bool f_fclose(bool delFlg=false);

    std::string f_fname();

  private:
    
    std::string _fname;
    
    TFile* _file;
    TTree* _tree;

    int    _nEvts;
    int    _oEvts;
    int    _iFiles;
  };
}

#endif
