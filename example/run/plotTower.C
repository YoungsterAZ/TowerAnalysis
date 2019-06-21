//////////////////////////////////////////////////
//            TOPOTOWERS  LEGEND                //                                             
// #  :: type :: x      :: y      :: cut :: jet //
//----------------------------------------------//
// 1  :: 1D   :: N      ::        :: 0   :: t/c //
// 2  :: 1D   :: N      ::        :: 1   :: t/c //
// 3  :: 1D   :: N      ::        :: 1/2 :: t/c //
// 4  :: 1D   :: M(inv) ::        :: 0,2 :: c   //
// 5  :: 1D   :: pT     ::        :: 0   :: t/c //
// 6  :: 1D   :: pT     ::        :: 1   :: t/c //
// 7  :: 1D   :: pT     ::        :: 2   :: c   //
// 8  :: 1D   :: y      ::        :: 0   :: t/c //
// 9  :: 1D   :: y      ::        :: 1   :: t/c //
// 10 :: 1D   :: y      ::        :: 2   :: c   //
// 11 :: 2D   :: pT     :: y      :: 0   :: t   //
// 12 :: 2D   :: pT     :: y      :: 0   :: c   //
// 13 :: 2D   :: pT     :: y      :: 1   :: t   //
// 14 :: 2D   :: pT     :: y      :: 1   :: c   //
// 15 :: 2D   :: pT     :: y      :: 2   :: c   //
// 16 :: 1D   :: t      ::        :: 0   :: c   //
// 17 :: 1D   :: t      ::        :: 1   :: c   //
// 18 :: 1D   :: t      ::        :: 2   :: c   //
// 19 :: 1D   :: tsig   ::        :: 0   :: c   //
// 20 :: 1D   :: tsig   ::        :: 1   :: c   //
// 21 :: 1D   :: tsig   ::        :: 2   :: c   //
// 22 :: 2D   :: t      :: y      :: 0   :: c   //
// 23 :: 2D   :: t      :: y      :: 1   :: c   //
// 24 :: 2D   :: t      :: y      :: 2   :: c   //
// 25 :: 2D   :: tsig   :: y      :: 0   :: c   //
// 26 :: 2D   :: tsig   :: y      :: 1   :: c   //
// 27 :: 2D   :: tsig   :: y      :: 2   :: c   //
// 28 :: 1D   :: y gap  ::        :: 0,2 :: c   //
// 29 :: 2D   :: y gap  :: M(inv) :: 0   :: c   //
// 30 :: 2D   :: y gap  :: M(inv) :: 2   :: c   //
// 31 :: 1D   :: N      ::        :: 3   :: t/c //
// 32 :: 1D   :: pT     ::        :: 3   :: t/c //
// 33 :: 1D   :: y      ::        :: 3   :: t/c //
// 34 :: 2D   :: pT     :: y      :: 3   :: t   //
// 35 :: 2D   :: pT     :: y      :: 3   :: c   //
// 36 :: 1D   :: t      ::        :: 3   :: c   //
// 37 :: 1D   :: tsig   ::        :: 3   :: c   //
// 38 :: 2D   :: y      :: tsig   :: 3   :: c   //
// 39 :: 2D   :: y      :: t      :: 3   :: c   //



void plotTower()
{

  /////////////////////////////////
  //1. Number of Jets before Cuts//
  /////////////////////////////////

  // open the file
  TFile* f = new TFile("vbf2600_topotow_rapgap_2.0.root","READ");
  // check if successful
  if ( f == 0 ) { printf("cannot open file!\n"); return; }

  // fimd histos 
  TH1D* h_n_tjet_ini = (TH1D*)f->FindObjectAny("Truth/Initial/h_njet");
  TH1D* h_n_cjet_ini = (TH1D*)f->FindObjectAny("Calo/Initial/h_njet");
  if ( h_n_tjet_ini ==  0 ) { printf("cannot find truth jet multiplicity histo!\n"); return; }


  TCanvas* ca = new TCanvas("NjetsInitial","Number of Jets Before Cuts");
  ca->SetLogy();

  // just the frame
  TH1D* _frame = new TH1D("_frame","_frame",100,0.,120.);
  // fetch from input histogram  _frame->GetXaxis()->SetTitle(h_ntj->GetXaxis()->GetTitle());
  _frame->GetXaxis()->SetTitle("N_{jet}");
  _frame->GetXaxis()->SetLimits(0,100);
  _frame->GetYaxis()->SetTitle(h_n_tjet_ini->GetYaxis()->GetTitle());
  //  double bw = h_n_tjet_ini->GetBinWidth(1);
  //  TString yt = TString::Format("Entries per %.0f count(s)",bw);
  //  _frame->GetYaxis()->SetTitle(yt.Data());
  // _frame->SetMinimum(h_ntj->GetMinimum());
  // _frame->SetMaximum(h_ntj->GetMaximum()*1.10);
  _frame->SetMinimum(0.5);
  _frame->SetMaximum(10000.);
  _frame->DrawCopy("axis");

  h_n_tjet_ini->SetLineColor(kBlue);
  h_n_tjet_ini->SetFillColorAlpha(kCyan, 0.35);
  h_n_tjet_ini->SetMarkerStyle(0);
  h_n_tjet_ini->DrawCopy("hist same");

  h_n_cjet_ini->SetLineColor(kRed);
  h_n_cjet_ini->SetFillColorAlpha(kYellow, 0.35);
  h_n_cjet_ini->SetMarkerStyle(0);
  h_n_cjet_ini->DrawCopy("hist same");

  TLegend* lg = new TLegend(0.5,0.75,0.8,0.9);
  lg->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg->AddEntry(h_n_tjet_ini,"N_{jet}^{truth}","lf");
  lg->AddEntry(h_n_cjet_ini,"N_{jet}^{topo-cluster}","lf");
  lg->Draw("same");

  TH1D* h_njet_icopy = new TH1D(*h_n_tjet_ini);
  h_njet_icopy->SetFillColor(0);
  h_njet_icopy->DrawCopy("hist same");

  _frame->DrawCopy("axis same");

  ca->Print("njets_initial.pdf");

  ////////////////////////////////
  //2. Number of Jets after Cuts//
  ////////////////////////////////

  // find histos 
  TH1D* h_n_tjet_fin = (TH1D*)f->FindObjectAny("Truth/Final/h_njet");
  TH1D* h_n_cjet_fin = (TH1D*)f->FindObjectAny("Calo/Final/h_njet");
  if ( h_n_tjet_fin ==  0 ) { printf("cannot find truth jet multiplicity histo!\n"); return; }

  TCanvas* cb = new TCanvas("NjetsFinal","Number of Jets after Initial Cuts");
  cb->SetLogy();

  _frame->DrawCopy("axis");

  h_n_tjet_fin->SetLineColor(kBlue);
  h_n_tjet_fin->SetFillColorAlpha(kCyan, 0.35);
  h_n_tjet_fin->SetMarkerStyle(0);
  h_n_tjet_fin->DrawCopy("hist same");

  h_n_cjet_fin->SetLineColor(kRed);
  h_n_cjet_fin->SetFillColorAlpha(kYellow, 0.35);
  h_n_cjet_fin->SetMarkerStyle(0);
  h_n_cjet_fin->DrawCopy("hist same");

  lg->Draw("same");

  TH1D* h_njet_fcopy = new TH1D(*h_n_tjet_fin);
  h_njet_fcopy->SetFillColor(0);
  h_njet_fcopy->DrawCopy("hist same");

  _frame->DrawCopy("axis same");

  cb->Print("njets_final.pdf");

  ////////////////////////////////////////////////////////
  //3. Number of Jets after Pile Up Supression using Rho//
  ////////////////////////////////////////////////////////

  // find histos 
  // TH1D* h_n_tjet_fin = (TH1D*)f->FindObjectAny("Truth/Final/h_njet");
  TH1D* h_n_calojet_sig = (TH1D*)f->FindObjectAny("Calo/PUArea/h_njet");
  if ( h_n_calojet_sig ==  0 ) { printf("cannot find calo jet multiplicity histo!\n"); return; }

  TCanvas* cc = new TCanvas("NjetsRho","Number of Jets After Pile-Up Supression");
  cc->SetLogy();

  _frame->DrawCopy("axis");

  h_n_tjet_fin->SetLineColor(kBlue);
  h_n_tjet_fin->SetFillColorAlpha(kCyan, 0.35);
  h_n_tjet_fin->SetMarkerStyle(0);
  h_n_tjet_fin->DrawCopy("hist same");

  h_n_calojet_sig->SetLineColor(kRed);
  h_n_calojet_sig->SetFillColorAlpha(kYellow, 0.35);
  h_n_calojet_sig->SetMarkerStyle(0);
  h_n_calojet_sig->DrawCopy("hist same");                                          //////////////////////////////////////////////////////////
  lg->Draw("same");                                                                //Note: this histogram looks almost identical to hist #2//
                                                                                   //////////////////////////////////////////////////////////
  TH1D* h_njet_rcopy = new TH1D(*h_n_calojet_sig);
  h_njet_rcopy->SetFillColor(0);
  h_njet_rcopy->DrawCopy("hist same");

  _frame->DrawCopy("axis same");

  cc->Print("njets_rho.pdf");

  //////////////////////////////////////////////////
  //4. Invariant Mass before and After suppression//
  //////////////////////////////////////////////////

  // find histos 
  TH1D* h_n_cjet_im = (TH1D*)f->FindObjectAny("Inclusive/h_cjet_minv_incl");
  TH1D* h_n_cjet_im_sig = (TH1D*)f->FindObjectAny("Final/h_cjet_minv_fin");
  if ( h_n_cjet_im ==  0 ) { printf("cannot find calo jet inv mass histo!\n"); return; }

  TCanvas* cd = new TCanvas("InvM","Invariant Mass before and after Suppression");
  cd->SetLogy();

  TH1D* _frame1 = new TH1D("_frame1","_frame1",100,0.,5000.);

  _frame1->GetXaxis()->SetTitle("M_{inv}^{calo-jet} [GeV]");
  _frame1->GetXaxis()->SetLimits(-50,4500);
  _frame1->GetYaxis()->SetTitle(h_n_cjet_im->GetYaxis()->GetTitle());
  _frame1->SetMinimum(5.);
  _frame1->SetMaximum(12000.);
  _frame1->DrawCopy("axis");

  h_n_cjet_im->SetLineColor(kBlue);
  h_n_cjet_im->SetFillColorAlpha(kCyan, 0.35);
  h_n_cjet_im->SetMarkerStyle(0);
  h_n_cjet_im->DrawCopy("hist same");

  h_n_cjet_im_sig->SetLineColor(kRed);
  h_n_cjet_im_sig->SetFillColorAlpha(kYellow, 0.35);
  h_n_cjet_im_sig->SetMarkerStyle(0);
  h_n_cjet_im_sig->DrawCopy("hist same");

  TLegend* lg2 = new TLegend(0.5,0.75,0.8,0.9);
  lg2->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg2->AddEntry(h_n_cjet_im,"M_{inv}^{calo-jet,initial}","lf");
  lg2->AddEntry(h_n_cjet_im_sig,"M_{inv}^{calo-jet,final}","lf");
  lg2->Draw("same");

 _frame1->DrawCopy("axis same");
  TH1D* h_cjet_mcopy = new TH1D(*h_n_cjet_im);
  h_cjet_mcopy->SetFillColor(0);
  h_cjet_mcopy->DrawCopy("hist same");

  cd->Print("mjets.pdf");

  ///////////////////////////////////
  //5. Transverse Momentum Initial//
  //////////////////////////////////

  // find histos 
  TH1D* h_n_pttjet_ini = (TH1D*)f->FindObjectAny("Truth/Initial/h_ptjet");
  TH1D* h_n_ptcjet_ini = (TH1D*)f->FindObjectAny("Calo/Initial/h_ptjet");
  if ( h_n_pttjet_ini ==  0 ) { printf("cannot find jet pt histo!\n"); return; }

  TCanvas* ce = new TCanvas("pTjetsI","Transverse Momentum before Cuts");
  ce->SetLogy();

  TH1D* _frame2 = new TH1D("_frame2","_frame2",100,0.,5000.);

  _frame2->GetXaxis()->SetTitle("p_{T}^{jet} [GeV]");
  _frame2->GetXaxis()->SetRangeUser(-500,1500);
  _frame2->GetYaxis()->SetTitle(h_n_pttjet_ini->GetYaxis()->GetTitle());
  _frame2->SetMinimum(0.5);
  _frame2->SetMaximum(2000000.);
  _frame2->DrawCopy("axis");

  h_n_pttjet_ini->SetLineColor(kBlue);
  h_n_pttjet_ini->SetFillColorAlpha(kCyan, 0.35);
  h_n_pttjet_ini->SetMarkerStyle(0);
  h_n_pttjet_ini->DrawCopy("hist same");

  h_n_ptcjet_ini->SetLineColor(kRed);
  h_n_ptcjet_ini->SetFillColorAlpha(kYellow, 0.35);
  h_n_ptcjet_ini->SetMarkerStyle(0);
  h_n_ptcjet_ini->DrawCopy("hist same");

  TLegend* lg3 = new TLegend(0.5,0.75,0.8,0.9);
  lg3->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg3->AddEntry(h_n_pttjet_ini,"p_{T}^{jet,truth}","lf");
  lg3->AddEntry(h_n_ptcjet_ini,"p_{T}^{jet,topo-cluster}","lf");
  lg3->Draw("same");

  TH1D* h_ptjet_pcopy = new TH1D(*h_n_pttjet_ini);
  h_ptjet_pcopy->SetFillColor(0);
  h_ptjet_pcopy->DrawCopy("hist same");

  _frame2->DrawCopy("axis same");

  ce->Print("ptjets_ini.pdf");

  ////////////////////////////////
  //6. Transverse Momentum Final//
  ////////////////////////////////

  // find histos 
  TH1D* h_n_pttjet_fin = (TH1D*)f->FindObjectAny("Truth/Final/h_ptjet");
  TH1D* h_n_ptcjet_fin = (TH1D*)f->FindObjectAny("Calo/Final/h_ptjet");
  if ( h_n_pttjet_fin ==  0 ) { printf("cannot find jet pt histo!\n"); return; }

  TCanvas* cf = new TCanvas("pTjetsF","Transverse Momentum after Initial Cuts");
  cf->SetLogy();

  _frame2->DrawCopy("axis");

  h_n_pttjet_fin->SetLineColor(kBlue);
  h_n_pttjet_fin->SetFillColorAlpha(kCyan, 0.35);
  h_n_pttjet_fin->SetMarkerStyle(0);
  h_n_pttjet_fin->DrawCopy("hist same");

  h_n_ptcjet_fin->SetLineColor(kRed);
  h_n_ptcjet_fin->SetFillColorAlpha(kYellow, 0.35);
  h_n_ptcjet_fin->SetMarkerStyle(0);
  h_n_ptcjet_fin->DrawCopy("hist same");

  lg3->Draw("same");

  TH1D* h_ptjet_pfcopy = new TH1D(*h_n_pttjet_fin);
  h_ptjet_pfcopy->SetFillColor(0);
  h_ptjet_pfcopy->DrawCopy("hist same");

  _frame2->DrawCopy("axis same");

  cf->Print("ptjets_fin.pdf");

  ////////////////////////////////////////////
  //7. Transverse Momentum After Suppression//
  ////////////////////////////////////////////

  // find histos 
  // TH1D* h_n_pttjet_fin = (TH1D*)f->FindObjectAny("Truth/Final/h_ptjet");
  TH1D* h_n_pt_calojet_sig = (TH1D*)f->FindObjectAny("Calo/PUArea/h_ptjet");
  if ( h_n_pt_calojet_sig ==  0 ) { printf("cannot find jet pt histo!\n"); return; }

  TCanvas* cg = new TCanvas("pTjetsPU","Transverse Momentum After Suppression");
  cg->SetLogy();

  TH1D* _frame3 = new TH1D("_frame3","_frame3",100,0.,5000.);

  _frame3->GetXaxis()->SetTitle("p_{T}^{jet} [GeV]");
  _frame3->GetXaxis()->SetLimits(-5,30);
  _frame3->GetYaxis()->SetTitle(h_n_pt_calojet_sig->GetYaxis()->GetTitle());
  _frame3->SetMinimum(0.5);
  _frame3->SetMaximum(10000.);
  _frame3->DrawCopy("axis");

  h_n_pt_calojet_sig->SetLineColor(kRed);
  h_n_pt_calojet_sig->SetFillColorAlpha(kYellow, 0.35);                 
  h_n_pt_calojet_sig->SetMarkerStyle(0);                                 

  TLegend* lg5 = new TLegend(0.5,0.75,0.8,0.9);
  lg5->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg5->AddEntry(h_n_pt_calojet_sig,"p_{T}^{jet,calo}","lf");
  lg5->Draw("same");

  TH1D* h_ptjet_prcopy = new TH1D(*h_n_calojet_sig);
  h_ptjet_prcopy->SetFillColor(kYellow);
  h_ptjet_prcopy->DrawCopy("hist same");



  _frame3->DrawCopy("axis same");

  cf
->Print("ptjets_rho.pdf");

  ///////////////////////
  //8. Rapidity Initial//
  ///////////////////////

  // find histos 
  TH1D* h_n_raptjet_ini = (TH1D*)f->FindObjectAny("Truth/Initial/h_rapjet");
  TH1D* h_n_rapcjet_ini = (TH1D*)f->FindObjectAny("Calo/Initial/h_rapjet");
  if ( h_n_raptjet_ini ==  0 ) { printf("cannot find jet rap histo!\n"); return; }

  TCanvas* ch = new TCanvas("rapjetsI","Jet Rapidity before Cuts");
  ch->SetLogy();

  TH1D* _frame5 = new TH1D("_frame5","_frame5",100,0.,5000.);

  _frame5->GetXaxis()->SetTitle("rapidity_{jet}");
  _frame5->GetXaxis()->SetLimits(-5,5);
  _frame5->GetYaxis()->SetTitle(h_n_raptjet_ini->GetYaxis()->GetTitle());
  _frame5->SetMinimum(0.5);
  _frame5->SetMaximum(1000000.);
  _frame5->DrawCopy("axis same");
  _frame5->DrawCopy("axis");

  h_n_raptjet_ini->SetLineColor(kBlue);
  h_n_raptjet_ini->SetFillColorAlpha(kCyan, 0.35);
  h_n_raptjet_ini->SetMarkerStyle(0);
  h_n_raptjet_ini->DrawCopy("hist same");

  h_n_rapcjet_ini->SetLineColor(kRed);
  h_n_rapcjet_ini->SetFillColorAlpha(kYellow, 0.35);
  h_n_rapcjet_ini->SetMarkerStyle(0);
  h_n_rapcjet_ini->DrawCopy("hist same");

  TLegend* lg4 = new TLegend(0.5,0.75,0.8,0.9);
  lg4->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg4->AddEntry(h_n_raptjet_ini,"rap_{jet}^{truth}","lf");
  lg4->AddEntry(h_n_rapcjet_ini,"rap_{jet}^{topo-cluster}","lf");
  lg4->Draw("same");

  TH1D* h_rapjet_rcopy = new TH1D(*h_n_raptjet_ini);
  h_rapjet_rcopy->SetFillColor(0);
  h_rapjet_rcopy->DrawCopy("hist same");

  _frame5->DrawCopy("axis same");

  ch
->Print("rapjets_ini.pdf");

  //////////////////////////////////
  //9. Rapidity after initial cuts//
  //////////////////////////////////

  // find histos 
  TH1D* h_n_raptjet_fin = (TH1D*)f->FindObjectAny("Truth/Final/h_rapjet");
  TH1D* h_n_rapcjet_fin = (TH1D*)f->FindObjectAny("Calo/Final/h_rapjet");
  if ( h_n_raptjet_fin ==  0 ) { printf("cannot find jet rap histo!\n"); return; }

  TCanvas* ci = new TCanvas("rapjetsF","Jet Rapidity after Initial Cuts");
  ci->SetLogy();

  _frame5->DrawCopy("axis");

  h_n_raptjet_fin->SetLineColor(kBlue);
  h_n_raptjet_fin->SetFillColorAlpha(kCyan, 0.35);
  h_n_raptjet_fin->SetMarkerStyle(0);
  h_n_raptjet_fin->DrawCopy("hist same");

  h_n_rapcjet_fin->SetLineColor(kRed);
  h_n_rapcjet_fin->SetFillColorAlpha(kYellow, 0.35);
  h_n_rapcjet_fin->SetMarkerStyle(0);
  h_n_rapcjet_fin->DrawCopy("hist same");

  lg4->Draw("same");

  TH1D* h_rapjet_fcopy = new TH1D(*h_n_raptjet_fin);
  h_rapjet_fcopy->SetFillColor(0);
  h_rapjet_fcopy->DrawCopy("hist same");

  _frame5->DrawCopy("axis same");

  ci
->Print("rapjets_fin.pdf");

  ////////////////////////////
  //10. Rapidity after p cut//
  ////////////////////////////

  // find histos 
  TH1D* h_n_calojet_rap = (TH1D*)f->FindObjectAny("Calo/PUArea/h_rapjet");
  if ( h_n_calojet_rap ==  0 ) { printf("cannot find jet rap histo!\n"); return; }

  TCanvas* cj = new TCanvas("rapjetsp","Jet Rapidity after Suppression");
  cj->SetLogy();

  TH1D* _frame6 = new TH1D("_frame6","_frame6",100,0.,5000.);

  _frame6->GetXaxis()->SetTitle("rapidity_{jet}");
  _frame6->GetXaxis()->SetLimits(-5,5);
  _frame6->GetYaxis()->SetTitle(h_n_raptjet_ini->GetYaxis()->GetTitle());
  _frame6->SetMinimum(0.5);
  _frame6->SetMaximum(100000.);
  _frame6->DrawCopy("axis same");                                                ///////////////////////////////////////////////////////
  _frame6->DrawCopy("axis");                                                     //Note: Does rho suppression effect y?              ///
                                                                                 //looks like it doesn't or not finding the right data//
  h_n_calojet_rap->SetLineColor(kRed);                                           //////////////////////////////////////////////////////      
  h_n_calojet_rap->SetFillColorAlpha(kYellow, 0.35);
  h_n_calojet_rap->SetMarkerStyle(0);
  h_n_calojet_rap->DrawCopy("hist same");

  TLegend* lg6 = new TLegend(0.5,0.75,0.8,0.9);
  lg6->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg6->AddEntry(h_n_calojet_rap,"rap_{jet}^{topo-cluster}","lf");
  lg6->Draw("same");

  TH1D* h_rapjet_pcopy = new TH1D(*h_n_calojet_rap);
  h_rapjet_pcopy->SetFillColor(0);
  h_rapjet_pcopy->DrawCopy("hist same");

  _frame5->DrawCopy("axis same");

  cj
->Print("rapjets_p.pdf");

  ///////////////////////////////////////////////////////////////
  //11. Transverse momentum vs. rapidity before Cuts, Truth Jets//
  ///////////////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_pt_raptjet_ini = (TH2D*)f->FindObjectAny("Truth/Initial/d_pt_rapjet");
  if ( d_pt_raptjet_ini ==  0 ) { printf("cannot find truth jet pt vs. rap histo!\n"); return; }


  TCanvas* ck = new TCanvas("pt_rap_tjetsInitial","pT vs. y, Truth Jets, Initial");
  ck->SetLogy();

  // just the frame
  TH2D* _frame7 = new TH2D("_frame","_frame",100,0.,120.,100,0.,120.);
 
  _frame7->SetTitle(0);
  _frame7->GetXaxis()->SetTitle("rapidity");
  _frame7->GetXaxis()->SetLimits(-5,5);
  _frame7->GetYaxis()->SetTitle("p^{jet} [GeV]");
  _frame7->GetYaxis()->SetLimits(-10,2000); 
  _frame7->DrawCopy("axis");

  d_pt_raptjet_ini->SetLineColor(kBlue);
  d_pt_raptjet_ini->SetFillColorAlpha(kCyan, 0.35);
  d_pt_raptjet_ini->SetMarkerStyle(0);
  d_pt_raptjet_ini->DrawCopy("hist same");

  TLegend* lg7 = new TLegend(0.5,0.75,0.8,0.9);
  lg7->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg7->AddEntry(d_pt_raptjet_ini,"Truth Jets","lf");
  lg7->Draw("same");

  TH2D* d_pt_rapjet_itcopy = new TH2D(*d_pt_raptjet_ini);
  d_pt_rapjet_itcopy->SetFillColor(0);
  d_pt_rapjet_itcopy->DrawCopy("hist same");

  _frame7->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_pt_raptjet_ini->Draw("CONTZ");

  ck->Print("pt_rap_tinitial.pdf");

  //////////////////////////////////////////////////////////////
  //12. Transverse momentum vs. rapidity before Cuts, CaloJets//
  //////////////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_pt_rapcjet_ini = (TH2D*)f->FindObjectAny("Calo/Initial/d_pt_rapjet");
  if ( d_pt_rapcjet_ini ==  0 ) { printf("cannot find calo jet pt vs. rap histo!\n"); return; }


  TCanvas* cl = new TCanvas("pt_rap_cjetsInitial","pT vs. y, TopoCluster Jets, Initial");
  cl->SetLogy();

  // just the frame

  _frame7->DrawCopy("axis");

  d_pt_rapcjet_ini->SetLineColor(kRed);
  d_pt_rapcjet_ini->SetFillColorAlpha(kYellow, 0.35);
  d_pt_rapcjet_ini->SetMarkerStyle(0);
  d_pt_rapcjet_ini->DrawCopy("hist same");

  TLegend* lg8 = new TLegend(0.5,0.75,0.8,0.9);
  lg8->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg8->AddEntry(d_pt_rapcjet_ini,"Calorimeter Jets","lf");
  lg8->Draw("same");

  TH2D* d_pt_rapjet_iccopy = new TH2D(*d_pt_rapcjet_ini);
  d_pt_rapjet_iccopy->SetFillColor(0);
  d_pt_rapjet_iccopy->DrawCopy("hist same");

  _frame7->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_pt_rapcjet_ini->Draw("CONTZ");

  cl->Print("pt_rap_cinitial.pdf");

  ////////////////////////////////////////////////////////////////.
  //13. Transverse momentum vs. rapidity after Cut 1, Truth Jets//
  ////////////////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_pt_raptjet_fin = (TH2D*)f->FindObjectAny("Truth/Final/d_pt_rapjet");
  if ( d_pt_raptjet_fin ==  0 ) { printf("cannot find truth jet pt vs. rap histo!\n"); return; }


  TCanvas* cm = new TCanvas("pt_rap_tjetsFinal","pT vs. y, Truth Jets, Final");
  cm->SetLogy();

  // just the frame
 
  _frame7->DrawCopy("axis");

  d_pt_raptjet_fin->SetLineColor(kBlue);
  d_pt_raptjet_fin->SetFillColorAlpha(kCyan, 0.35);
  d_pt_raptjet_fin->SetMarkerStyle(0);
  d_pt_raptjet_fin->DrawCopy("hist same");

  TLegend* lg9 = new TLegend(0.5,0.75,0.8,0.9);
  lg9->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg9->AddEntry(d_pt_raptjet_ini,"Truth Jets","lf");
  lg9->Draw("same");

  TH2D* d_pt_rapjet_ftcopy = new TH2D(*d_pt_raptjet_fin);
  d_pt_rapjet_ftcopy->SetFillColor(0);
  d_pt_rapjet_ftcopy->DrawCopy("hist same");

  _frame7->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_pt_raptjet_fin->Draw("CONTZ");

  cm->Print("pt_rap_tfinal.pdf");

  //////////////////////////////////////////////////////////////
  //14. Transverse momentum vs. rapidity after Cut 1, CaloJets//
  //////////////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_pt_rapcjet_fin = (TH2D*)f->FindObjectAny("Calo/Final/d_pt_rapjet");
  if ( d_pt_rapcjet_fin ==  0 ) { printf("cannot find calo jet pt vs. rap histo!\n"); return; }


  TCanvas* cn = new TCanvas("pt_rap_cjetsFinal","pT vs. y, TopoCluster Jets, Final");
  cn->SetLogy();

  // just the frame

  _frame7->DrawCopy("axis");

  d_pt_rapcjet_fin->SetLineColor(kRed);
  d_pt_rapcjet_fin->SetFillColorAlpha(kYellow, 0.35);
  d_pt_rapcjet_fin->SetMarkerStyle(0);
  d_pt_rapcjet_fin->DrawCopy("hist same");

  TLegend* lg10 = new TLegend(0.5,0.75,0.8,0.9);
  lg10->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg10->AddEntry(d_pt_rapcjet_fin,"Calorimeter Jets","lf");
  lg10->Draw("same");

  TH2D* d_pt_rapjet_fccopy = new TH2D(*d_pt_rapcjet_fin);
  d_pt_rapjet_fccopy->SetFillColor(0);
  d_pt_rapjet_fccopy->DrawCopy("hist same");

  _frame7->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_pt_rapcjet_fin->Draw("CONTZ");

  cn->Print("pt_rap_cfinal.pdf");

  ////////////////////////////////////////////////////////////////
  //15. Transverse momentum vs. rapidity after rho cut, CaloJets//
  ////////////////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_pt_rapcjet_rho = (TH2D*)f->FindObjectAny("Calo/PUArea/d_pt_rapjet");
  if ( d_pt_rapcjet_rho ==  0 ) { printf("cannot find calo jet pt vs. rap histo!\n"); return; }


  TCanvas* co = new TCanvas("pt_rap_jetsRho","pT vs. y, TopoCluster Jets, After Suppression");
  co->SetLogy();

  // just the frame

  _frame7->DrawCopy("axis");

  d_pt_rapcjet_rho->SetLineColor(kRed);
  d_pt_rapcjet_rho->SetFillColorAlpha(kYellow, 0.35);
  d_pt_rapcjet_rho->SetMarkerStyle(0);
  d_pt_rapcjet_rho->DrawCopy("hist same");

  lg10->Draw("same");

  TH2D* d_pt_rapjet_rcopy = new TH2D(*d_pt_rapcjet_rho);
  d_pt_rapjet_rcopy->SetFillColor(0);
  d_pt_rapjet_rcopy->DrawCopy("hist same");

  _frame7->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_pt_rapcjet_rho->Draw("CONTZ");

  co->Print("pt_rap_crho.pdf");


  ////////////////////////////////////
  //16. Jet Timing Initial, CaloJets//
  ////////////////////////////////////


  // find histos 
  TH1D* h_t_cjet_ini = (TH1D*)f->FindObjectAny("Calo/Initial/h_tjet");
  if ( h_t_cjet_ini ==  0 ) { printf("cannot find jet t histo!\n"); return; }

  TCanvas* cp = new TCanvas("tJetsIni","Jet Timing Before Cuts, TopoCluster Jets");
  cp->SetLogy();

  TH1D* _frame8 = new TH1D("_frame8","_frame8",100,0.,5000.);

  _frame8->GetXaxis()->SetTitle("t_{calojet} [ns]");
  _frame8->GetXaxis()->SetLimits(-120.,80.);
  _frame8->GetYaxis()->SetTitle("#partialN/#partialt ns^{-1}");
  _frame8->SetMinimum(0.5);
  _frame8->SetMaximum(100000.);
  _frame8->DrawCopy("axis");

  h_t_cjet_ini->SetLineColor(kRed);
  h_t_cjet_ini->SetFillColorAlpha(kYellow, 0.35);                 
  h_t_cjet_ini->SetMarkerStyle(0);                                 

  TLegend* lg11 = new TLegend(0.5,0.75,0.8,0.9);
  lg11->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg11->AddEntry(h_n_pt_calojet_sig,"Timing^{TopoCluster Jet}","lf");
  lg11->Draw("same");

  TH1D* h_t_cjet_icopy = new TH1D(*h_t_cjet_ini);
  h_t_cjet_icopy->SetFillColor(kYellow);
  h_t_cjet_icopy->DrawCopy("hist same");



  _frame8->DrawCopy("axis same");

  cp
->Print("tcjets_ini.pdf");

  ////////////////////////////////////////
  //17. Jet Timing After cut 1, CaloJets//
  ///////////////////////////////////////


  // find histos 
  TH1D* h_t_cjet_fin = (TH1D*)f->FindObjectAny("Calo/Final/h_tjet");
  if ( h_t_cjet_fin ==  0 ) { printf("cannot find jet t histo!\n"); return; }

  TCanvas* cq = new TCanvas("tJetsFin","Jet Timing After Cut 1, TopoCluster Jets");
  cq->SetLogy();

  _frame8->DrawCopy("axis");

  h_t_cjet_fin->SetLineColor(kRed);
  h_t_cjet_fin->SetFillColorAlpha(kYellow, 0.35);                 
  h_t_cjet_fin->SetMarkerStyle(0);                                 

  lg11->Draw("same");

  TH1D* h_t_cjet_fcopy = new TH1D(*h_t_cjet_fin);
  h_t_cjet_fcopy->SetFillColor(kYellow);
  h_t_cjet_fcopy->DrawCopy("hist same");



  _frame8->DrawCopy("axis same");

  cq
->Print("tcjets_fin.pdf");

  //////////////////////////////////////////////////
  //18. Jet Timing After rho suppression, CaloJets//
  //////////////////////////////////////////////////


  // find histos 
  TH1D* h_t_cjet_rho = (TH1D*)f->FindObjectAny("Calo/PUArea/h_tjet");
  if ( h_t_cjet_rho ==  0 ) { printf("cannot find jet t histo!\n"); return; }

  TCanvas* cr = new TCanvas("tJetsRho","Jet Timing After Suppression, TopoCluster Jets");
  cr->SetLogy();

  _frame8->DrawCopy("axis");

  h_t_cjet_rho->SetLineColor(kRed);
  h_t_cjet_rho->SetFillColorAlpha(kYellow, 0.35);                 
  h_t_cjet_rho->SetMarkerStyle(0);                                 

  lg11->Draw("same");

  TH1D* h_t_cjet_rcopy = new TH1D(*h_t_cjet_rho);
  h_t_cjet_rcopy->SetFillColor(kYellow);
  h_t_cjet_rcopy->DrawCopy("hist same");



  _frame8->DrawCopy("axis same");

  cr
->Print("tcjets_rho.pdf");

  //////////////////////////////////////////////
  //19. Signal in Jet Timing Initial, CaloJets//
  //////////////////////////////////////////////


  // find histos 
  TH1D* h_tsig_cjet_ini = (TH1D*)f->FindObjectAny("Calo/Initial/h_tsig");
  if ( h_tsig_cjet_ini ==  0 ) { printf("cannot find jet tsig histo!\n"); return; }

  TCanvas* cs = new TCanvas("tsigJetsIni","Sigal in Jet Timing Before Cuts, TopoCluster Jets");
  cs->SetLogy();

  TH1D* _frame9 = new TH1D("_frame9","_frame9",100,0.,5000.);

  _frame9->GetXaxis()->SetTitle("t_{calo-signal} [ns]");
  _frame9->GetXaxis()->SetLimits(-120.,120.);
  _frame9->GetYaxis()->SetTitle("#partialN/#partialt ns^{-1}");
  _frame9->SetMinimum(0.5);
  _frame9->SetMaximum(1000000.);
  _frame9->DrawCopy("axis");

  h_tsig_cjet_ini->SetLineColor(kRed);
  h_tsig_cjet_ini->SetFillColorAlpha(kYellow, 0.35);                 
  h_tsig_cjet_ini->SetMarkerStyle(0);                                 

  TLegend* lg12 = new TLegend(0.5,0.75,0.8,0.9);
  lg12->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg12->AddEntry(h_n_pt_calojet_sig,"Timing^{TopoCluster Jet}","lf");
  lg12->Draw("same");

  TH1D* h_tsig_cjet_icopy = new TH1D(*h_tsig_cjet_ini);
  h_tsig_cjet_icopy->SetFillColor(kYellow);
  h_tsig_cjet_icopy->DrawCopy("hist same");



  _frame9->DrawCopy("axis same");

  cs
->Print("tsig_cjets_ini.pdf");

  //////////////////////////////////////////////////
  //20. Signal in Jet Timing After cut 1, CaloJets//
  //////////////////////////////////////////////////


  // find histos 
  TH1D* h_tsig_cjet_fin = (TH1D*)f->FindObjectAny("Calo/Final/h_tsig");
  if ( h_tsig_cjet_fin ==  0 ) { printf("cannot find jet tsig histo!\n"); return; }

  TCanvas* ct = new TCanvas("tsigJetsFin","Jet Timing After Cut 1, TopoCluster Jets");
  ct->SetLogy();

  _frame9->DrawCopy("axis");

  h_tsig_cjet_fin->SetLineColor(kRed);
  h_tsig_cjet_fin->SetFillColorAlpha(kYellow, 0.35);                 
  h_tsig_cjet_fin->SetMarkerStyle(0);                                 

  lg12->Draw("same");

  TH1D* h_tsig_cjet_fcopy = new TH1D(*h_tsig_cjet_fin);
  h_tsig_cjet_fcopy->SetFillColor(kYellow);
  h_tsig_cjet_fcopy->DrawCopy("hist same");



  _frame9->DrawCopy("axis same");

  cs
->Print("tsig_cjets_fin.pdf");

  ////////////////////////////////////////////////////////////
  //21. Signal in Jet Timing After rho suppression, CaloJets//
  ////////////////////////////////////////////////////////////


  // find histos 
  TH1D* h_tsig_cjet_rho = (TH1D*)f->FindObjectAny("Calo/PUArea/h_tsig");
  if ( h_tsig_cjet_rho ==  0 ) { printf("cannot find jet tsig histo!\n"); return; }

  TCanvas* cu = new TCanvas("tsigJetsRho","Signal in Jet Timing After Suppression, TopoCluster Jets");
  cu->SetLogy();

  _frame9->DrawCopy("axis");

  h_tsig_cjet_rho->SetLineColor(kRed);
  h_tsig_cjet_rho->SetFillColorAlpha(kYellow, 0.35);                 
  h_tsig_cjet_rho->SetMarkerStyle(0);                                 

  lg12->Draw("same");

  TH1D* h_tsig_cjet_rcopy = new TH1D(*h_tsig_cjet_rho);
  h_tsig_cjet_rcopy->SetFillColor(kYellow);
  h_tsig_cjet_rcopy->DrawCopy("hist same");



  _frame9->DrawCopy("axis same");

  cu
->Print("tsig_cjets_rho.pdf");

  /////////////////////////////////////////////////
  //22. Timing vs. rapidity before Cuts, CaloJets//
  /////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_tjet_rapcjet_ini = (TH2D*)f->FindObjectAny("Calo/Initial/d_tjet_rapjet");
  if ( d_tjet_rapcjet_ini ==  0 ) { printf("cannot find calo jet t vs. rap histo!\n"); return; }


  TCanvas* cv = new TCanvas("t_rap_cjetsInitial","t vs. y, TopoCluster Jets, Initial");
  cv->SetLogy();

  // just the frame
  TH2D* _frame10 = new TH2D("_frame10","_frame10",100,0.,5000.,100,0,5000);

  _frame10->GetXaxis()->SetTitle("Rapidity y");
  _frame10->GetXaxis()->SetLimits(-5,5);
  _frame10->GetYaxis()->SetTitle("Jet Timing [ns]");
  _frame10->SetMinimum(1);
  _frame10->SetMaximum(50.);
  _frame10->DrawCopy("axis");

  d_tjet_rapcjet_ini->SetLineColor(kRed);
  d_tjet_rapcjet_ini->SetFillColorAlpha(kYellow, 0.35);
  d_tjet_rapcjet_ini->SetMarkerStyle(0);
  d_tjet_rapcjet_ini->DrawCopy("hist same");

  TLegend* lg13 = new TLegend(0.5,0.75,0.8,0.9);
  lg13->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg13->AddEntry(d_tjet_rapcjet_ini,"Calorimeter Jets","lf");
  lg13->Draw("same");

  TH2D* d_tjet_rapjet_iccopy = new TH2D(*d_tjet_rapcjet_ini);
  d_tjet_rapjet_iccopy->SetFillColor(0);
  d_tjet_rapjet_iccopy->DrawCopy("hist same");

  _frame10->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_tjet_rapcjet_ini->Draw("CONTZ");

  cv->Print("tjet_rap_cinitial.pdf");


  /////////////////////////////////////////////////
  //23. Timing vs. rapidity after Cut 1, CaloJets//
  /////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_tjet_rapcjet_fin = (TH2D*)f->FindObjectAny("Calo/Final/d_tjet_rapjet");
  if ( d_tjet_rapcjet_fin ==  0 ) { printf("cannot find calo jet t vs. rap histo!\n"); return; }


  TCanvas* cw = new TCanvas("t_rap_cjetsFinal","t vs. y, TopoCluster Jets, Final");
  cw->SetLogy();

  // just the frame

  _frame10->DrawCopy("axis");

  d_tjet_rapcjet_fin->SetLineColor(kRed);
  d_tjet_rapcjet_fin->SetFillColorAlpha(kYellow, 0.35);
  d_tjet_rapcjet_fin->SetMarkerStyle(0);
  d_tjet_rapcjet_fin->DrawCopy("hist same");

  TLegend* lg14 = new TLegend(0.5,0.75,0.8,0.9);
  lg14->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg14->AddEntry(d_tjet_rapcjet_fin,"Calorimeter Jets","lf");
  lg14->Draw("same");

  TH2D* d_tjet_rapjet_fccopy = new TH2D(*d_tjet_rapcjet_fin);
  d_tjet_rapjet_fccopy->SetFillColor(0);
  d_tjet_rapjet_fccopy->DrawCopy("hist same");

  _frame10->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_tjet_rapcjet_fin->Draw("CONTZ");

  cw->Print("tjet_rap_cfinal.pdf");

  ///////////////////////////////////////////////////
  //24. Timing vs. rapidity after rho cut, CaloJets//
  ///////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_tjet_rapcjet_rho = (TH2D*)f->FindObjectAny("Calo/PUArea/d_tjet_rapjet");
  if ( d_tjet_rapcjet_rho ==  0 ) { printf("cannot find calo jet t vs. rap histo!\n"); return; }


  TCanvas* cx = new TCanvas("t_rap_jetsRho","t vs. y, TopoCluster Jets, After Suppression");
  cx->SetLogy();

  // just the frame

  _frame10->DrawCopy("axis");

  d_tjet_rapcjet_rho->SetLineColor(kRed);
  d_tjet_rapcjet_rho->SetFillColorAlpha(kYellow, 0.35);
  d_tjet_rapcjet_rho->SetMarkerStyle(0);
  d_tjet_rapcjet_rho->DrawCopy("hist same");

  lg14->Draw("same");

  TH2D* d_tjet_rapjet_rcopy = new TH2D(*d_tjet_rapcjet_rho);
  d_tjet_rapjet_rcopy->SetFillColor(0);
  d_tjet_rapjet_rcopy->DrawCopy("hist same");

  _frame10->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_tjet_rapcjet_rho->Draw("CONTZ");

  cx->Print("tjet_rap_crho.pdf");

  ///////////////////////////////////////////////////////////////
  //25.Signal in Jet  Timing vs. rapidity before Cuts, CaloJets//
  ///////////////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_tsig_rapcjet_ini = (TH2D*)f->FindObjectAny("Calo/Initial/d_tsig_rapsig");
  if ( d_tsig_rapcjet_ini ==  0 ) { printf("cannot find calo jet tsig vs. rap histo!\n"); return; }


  TCanvas* cy = new TCanvas("tsig_rap_cjetsInitial","tsig vs. y, TopoCluster Jets, Initial");

  // just the frame
  TH2D* _frame12 = new TH2D("_frame12","_frame12",100,0.,50.,100,0,50);

  _frame12->GetXaxis()->SetTitle("Rapidity y");                                       //////////////////////////
  // _frame12->GetXaxis()->SetLimits(-3,3);                                           // Note: See Note on 29.//
  _frame12->GetYaxis()->SetTitle("Signal in Jet Timing [ns]");                        //////////////////////////
  // _frame12->SetMinimum(0.9);
  // _frame12->SetMaximum(15.);
  _frame12->SetTitleOffset(0.1);
  _frame12->DrawCopy("axis");

  d_tsig_rapcjet_ini->SetLineColor(kRed);
  d_tsig_rapcjet_ini->SetFillColorAlpha(kYellow, 0.35);
  d_tsig_rapcjet_ini->SetMarkerStyle(0);
  d_tsig_rapcjet_ini->DrawCopy("hist same");

  TLegend* lg15 = new TLegend(0.5,0.75,0.8,0.9);
  lg15->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg15->AddEntry(d_tsig_rapcjet_ini,"Calorimeter Jets","lf");
  lg15->Draw("same");

  TH2D* d_tsig_rapjet_iccopy = new TH2D(*d_tsig_rapcjet_ini);
  d_tsig_rapjet_iccopy->SetFillColor(0);
  d_tsig_rapjet_iccopy->DrawCopy("hist same");

  _frame12->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_tsig_rapcjet_ini->Draw("CONTZ");
  d_tsig_rapcjet_ini->GetXaxis()->SetLimits(-3,3);
  d_tsig_rapcjet_ini->GetYaxis()->SetLimits(.9,15);

  cy->Print("tsig_rap_cinitial.pdf");


  ///////////////////////////////////////////////////////////////
  //26. Signal in Jet Timing vs. rapidity after Cut 1, CaloJets//
  ///////////////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_tsig_rapcjet_fin = (TH2D*)f->FindObjectAny("Calo/Final/d_tsig_rapsig");
  if ( d_tsig_rapcjet_fin ==  0 ) { printf("cannot find calo jet t vs. rap histo!\n"); return; }


  TCanvas* cz = new TCanvas("tsig_rap_cjetsFinal","tsig vs. y, TopoCluster Jets, Final");

  // just the frame

  _frame12->DrawCopy("axis");

  d_tsig_rapcjet_fin->SetLineColor(kRed);
  d_tsig_rapcjet_fin->SetFillColorAlpha(kYellow, 0.35);
  d_tsig_rapcjet_fin->SetMarkerStyle(0);
  d_tsig_rapcjet_fin->DrawCopy("hist same");

  lg15->Draw("same");

  TH2D* d_tsig_rapjet_fccopy = new TH2D(*d_tsig_rapcjet_fin);
  d_tsig_rapjet_fccopy->SetFillColor(0);
  d_tsig_rapjet_fccopy->DrawCopy("hist same");

  _frame12->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_tsig_rapcjet_fin->Draw("CONTZ");

  cz->Print("tsig_rap_cfinal.pdf");

  /////////////////////////////////////////////////////////////
  //27. Signal in Timing vs. rapidity after rho cut, CaloJets//
  /////////////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_tsig_rapcjet_rho = (TH2D*)f->FindObjectAny("Calo/PUArea/d_tsig_rapsig");
  if ( d_tsig_rapcjet_rho ==  0 ) { printf("cannot find calo jet tsig vs. rap histo!\n"); return; }


  TCanvas* caa = new TCanvas("tsig_rap_jetsRho","tsig vs. y, TopoCluster Jets, After Suppression");

  // just the frame

  _frame12->DrawCopy("axis");

  d_tsig_rapcjet_rho->SetLineColor(kRed);
  d_tsig_rapcjet_rho->SetFillColorAlpha(kYellow, 0.35);
  d_tsig_rapcjet_rho->SetMarkerStyle(0);
  d_tsig_rapcjet_rho->DrawCopy("hist same");

  lg15->Draw("same");

  TH2D* d_tsig_rapjet_rcopy = new TH2D(*d_tsig_rapcjet_rho);
  d_tsig_rapjet_rcopy->SetFillColor(0);
  d_tsig_rapjet_rcopy->DrawCopy("hist same");

  _frame12->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_tsig_rapcjet_rho->Draw("CONTZ");

  caa->Print("tsig_rap_crho.pdf");

  //////////////////////////////////////////////////
  //28. Rapidity Gap  before and After suppression//
  //////////////////////////////////////////////////

  // find histos 
  TH1D* h_n_cjet_rg = (TH1D*)f->FindObjectAny("Inclusive/h_cjet_rapgap_incl");
  TH1D* h_n_cjet_rg_sig = (TH1D*)f->FindObjectAny("Final/h_cjet_rapgap_fin");
  if ( h_n_cjet_rg ==  0 ) { printf("cannot find calo jet rap gap histo!\n"); return; }

  TCanvas* cab = new TCanvas("RG","Rapidity Gap before and after Suppression");

  TH1D* _frame13 = new TH1D("_frame13","_frame13",100,0.,5000.);

  _frame13->GetXaxis()->SetTitle("Rapidity Gap^{calo-jet}");
  _frame13->GetXaxis()->SetLimits(-1,10);
  _frame13->GetYaxis()->SetTitle(h_n_cjet_rg->GetYaxis()->GetTitle());
  _frame13->SetMinimum(0.);
  _frame13->SetMaximum(500.);
  _frame13->DrawCopy("axis");

  h_n_cjet_rg->SetLineColor(kBlue);
  h_n_cjet_rg->SetFillColorAlpha(kCyan, 0.35);
  h_n_cjet_rg->SetMarkerStyle(0);
  h_n_cjet_rg->DrawCopy("hist same");

  h_n_cjet_rg_sig->SetLineColor(kRed);
  h_n_cjet_rg_sig->SetFillColorAlpha(kYellow, 0.35);
  h_n_cjet_rg_sig->SetMarkerStyle(0);
  h_n_cjet_rg_sig->DrawCopy("hist same");

  TLegend* lg16 = new TLegend(0.5,0.75,0.8,0.9);
  lg16->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg16->AddEntry(h_n_cjet_rg,"Rapidity Gap^{calo-jet,initial}","lf");
  lg16->AddEntry(h_n_cjet_rg_sig,"Rapidity Gap^{calo-jet,final}","lf");
  lg16->Draw("same");

  TH1D* h_rg_icopy = new TH1D(*h_n_cjet_rg);
  h_rg_icopy->SetFillColor(0);
  h_rg_icopy->DrawCopy("hist same");
  _frame13->DrawCopy("axis same");

  cab->Print("rapgapjets.pdf");

  //////////////////////////////////////////////////////
  //29. Rapidity Gap vs. Inv. Mass  before suppression//
  //////////////////////////////////////////////////////

  // find histos 
  TH2D* d_cjet_rg_im_ini = (TH2D*)f->FindObjectAny("Inclusive/d_cjet_rapgap_minv_incl");
  if ( d_cjet_rg_im_ini ==  0 ) { printf("cannot find calo jet rap gap vs im histo!\n"); return; }

  TCanvas* cac = new TCanvas("RG vs IM, Inital","Rapidity Gap vs. Invariant Mass before Suppression");

  TH2D* _frame14 = new TH2D("_frame14","_frame14",100,0.,500.,100,0.,500.);

  d_cjet_rg_im_ini->GetXaxis()->SetTitle("Invariant Mass^{calo-jet}");
  d_cjet_rg_im_ini->GetXaxis()->SetRangeUser(800,6000);
  d_cjet_rg_im_ini->GetYaxis()->SetTitle(d_cjet_rg_im_ini->GetYaxis()->GetTitle());
  d_cjet_rg_im_ini->GetYaxis()->SetRangeUser(-1000,9);
  //d_cjet_rg_im_ini->SetMinimum(-10.);
  //d_cjet_rg_im_ini->SetMaximum(10.);
  d_cjet_rg_im_ini->DrawCopy("axis");

  d_cjet_rg_im_ini->SetLineColor(kBlue);
  d_cjet_rg_im_ini->SetFillColorAlpha(kCyan, 0.35);
  d_cjet_rg_im_ini->SetMarkerStyle(0);
  d_cjet_rg_im_ini->DrawCopy("hist same");


  TLegend* lg17 = new TLegend(0.5,0.75,0.8,0.9);
  lg17->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg17->AddEntry(d_cjet_rg_im_ini,"TopoCluster Jet, Initial","lf");
  lg17->Draw("same");

  TH2D* d_cjet_rg_im_icopy = new TH2D(*d_cjet_rg_im_ini);
  d_cjet_rg_im_icopy->SetFillColor(0);
  d_cjet_rg_im_icopy->DrawCopy("hist same");

  // _frame14->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_cjet_rg_im_ini->Draw("CONTZ");
  // d_cjet_rg_im_ini->GetXaxis()->SetLimits(0,7000);                          /////////////////////////////////////////////////////////////////
  //d_cjet_rg_im_ini->GetYaxis()->SetLimits(-1000,9);                         //Note: Very Strange Behavior - no matter how far you zoom out //
                                                                            //the shape of the plot looks the same and you can't get to the// 
  cac->Print("rapgap_invm_ini.pdf");                                        //edge on the +x and -y sides of the distributions             //
                                                                            /////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////
  //30. Rapidity Gap vs. Inv. Mass after  suppression//
  //////////////////////////////////////////////////////

  // find histos 
  TH2D* d_cjet_rg_im_fin = (TH2D*)f->FindObjectAny("Final/d_cjet_rapgap_minv_fin");
  if ( d_cjet_rg_im_fin ==  0 ) { printf("cannot find calo jet rap gap vs im histo!\n"); return; }

  TCanvas* cad = new TCanvas("RG vs IM, Final","Rapidity Gap vs. Invariant Mass after Suppression");

  _frame14->DrawCopy("axis");

  d_cjet_rg_im_fin->SetLineColor(kBlue);
  d_cjet_rg_im_fin->SetFillColorAlpha(kCyan, 0.35);
  d_cjet_rg_im_fin->SetMarkerStyle(0);
  d_cjet_rg_im_fin->DrawCopy("hist same");


  TLegend* lg18 = new TLegend(0.5,0.75,0.8,0.9);
  lg18->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg18->AddEntry(d_cjet_rg_im_ini,"TopoCluster Jet, Final","lf");
  lg18->Draw("same");

  TH2D* d_cjet_rg_im_fcopy = new TH2D(*d_cjet_rg_im_fin);
  d_cjet_rg_im_fcopy->SetFillColor(0);
  d_cjet_rg_im_fcopy->DrawCopy("hist same");

  d_cjet_rg_im_ini->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_cjet_rg_im_ini->Draw("CONTZ");

  cad->Print("rapgap_invm_ini.pdf");

 ///////////////////////////////
  //31. Number of Matched Jet//
  /////////////////////////////

  // open the file
  //  TFile* f1 = new TFile("vbf2600_topocl_rapgap_2.0.root","READ");
  // check if successfu
  // if ( f1 == 0 ) { printf("cannot open file!\n"); return; }

  // fimd histos 
  TH1D* h_n_tjet_match = (TH1D*)f->FindObjectAny("MatchedTruth/Final/h_njet");
  if ( h_n_tjet_match ==  0 ) { printf("cannot find matched truth jet multiplicity histo!\n"); return; }


  TCanvas* cae = new TCanvas("NjetsMatched","Number of Matched Jets");
  cae->SetLogy();

  // just the frame
  TH1D* _frame15 = new TH1D("_frame15","_frame15",100,0.,120.);

  _frame15->GetXaxis()->SetTitle("N_{jet}");
  _frame15->GetXaxis()->SetRangeUser(-12,12);
  _frame15->GetYaxis()->SetTitle(h_n_tjet_match->GetYaxis()->GetTitle());
  _frame15->SetMinimum(0.5);
  _frame15->SetMaximum(10000.);
  _frame15->DrawCopy("axis");

  h_n_tjet_match->SetLineColor(kBlue);
  h_n_tjet_match->SetFillColorAlpha(kCyan, 0.35);
  h_n_tjet_match->SetMarkerStyle(0);
  h_n_tjet_match->DrawCopy("hist same");


  TLegend* lg19 = new TLegend(0.5,0.75,0.8,0.9);
  lg19->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg19->AddEntry(h_n_tjet_ini,"N_{jet}^{matched}","lf");
  lg19->Draw("same");

  TH1D* h_njet_mcopy = new TH1D(*h_n_tjet_match);
  h_njet_mcopy->SetFillColor(0);
  h_njet_mcopy->DrawCopy("hist same");

  _frame15->DrawCopy("axis same");

  cae->Print("njets_matched.pdf");

  /////////////////////////////////////////
  //32. Transverse Momentum, Matched Jets//
  /////////////////////////////////////////

  // find histos 
  TH1D* h_n_pttjet_match = (TH1D*)f->FindObjectAny("Truth/Final/h_ptjet");
  TH1D* h_n_ptcjet_match = (TH1D*)f->FindObjectAny("Calo/Initial/h_ptjet");
  if ( h_n_pttjet_match ==  0 ) { printf("cannot find match jet pt histo!\n"); return; }

  TCanvas* caf = new TCanvas("pTjetsM","Transverse Momentum, Matched Jets");
  caf->SetLogy();

  TH1D* _frame16 = new TH1D("_frame16","_frame16",100,0.,5000.);

  _frame16->GetXaxis()->SetTitle("p_{T}^{jet} [GeV]");
  _frame16->GetXaxis()->SetRangeUser(-500,2000);
  _frame16->GetYaxis()->SetTitle(h_n_pttjet_match->GetYaxis()->GetTitle());
  _frame16->SetMinimum(0.5);
  _frame16->SetMaximum(500000.);
  _frame16->DrawCopy("axis");

  h_n_pttjet_match->SetLineColor(kBlue);
  h_n_pttjet_match->SetFillColorAlpha(kCyan, 0.35);
  h_n_pttjet_match->SetMarkerStyle(0);
  h_n_pttjet_match->DrawCopy("hist same");

  h_n_ptcjet_match->SetLineColor(kRed);
  h_n_ptcjet_match->SetFillColorAlpha(kYellow, 0.35);
  h_n_ptcjet_match->SetMarkerStyle(0);
  h_n_ptcjet_match->DrawCopy("hist same");

  TLegend* lg20 = new TLegend(0.5,0.75,0.8,0.9);
  lg20->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg20->AddEntry(h_n_pttjet_ini,"p_{T}^{jet,truth}","lf");
  lg20->AddEntry(h_n_ptcjet_ini,"p_{T}^{jet,topo-cluster}","lf");
  lg20->Draw("same");

  TH1D* h_ptjet_mcopy = new TH1D(*h_n_pttjet_match);
  h_ptjet_mcopy->SetFillColor(0);
  h_ptjet_mcopy->DrawCopy("hist same");

  _frame16->DrawCopy("axis same");

  caf->Print("ptjets_matched.pdf");

  //////////////////////////////
  //33. Rapidity, Matched Jets//
  //////////////////////////////

  // find histos 
  TH1D* h_n_raptjet_match = (TH1D*)f->FindObjectAny("MatchedTruth/Final/h_rapjet");
  TH1D* h_n_rapcjet_match = (TH1D*)f->FindObjectAny("MatchedCalo/Final/h_rapjet");
  if ( h_n_raptjet_match ==  0 ) { printf("cannot find match jet rap histo!\n"); return; }

  TCanvas* cag = new TCanvas("rapjetsM","Matched Jet Rapidity");


  TH1D* _frame17 = new TH1D("_frame17","_frame17",100,0.,5000.);

  _frame17->GetXaxis()->SetTitle("rapidity_{jet}");
  _frame17->GetXaxis()->SetLimits(-5,5);
  _frame17->GetYaxis()->SetTitle(h_n_raptjet_match->GetYaxis()->GetTitle());
  _frame17->SetMinimum(0);
  _frame17->SetMaximum(800.);
  _frame17->DrawCopy("axis same");
  _frame17->DrawCopy("axis");

  h_n_raptjet_match->SetLineColor(kBlue);
  h_n_raptjet_match->SetFillColorAlpha(kCyan, 0.35);
  h_n_raptjet_match->SetMarkerStyle(0);
  h_n_raptjet_match->DrawCopy("hist same");

  h_n_rapcjet_match->SetLineColor(kRed);
  h_n_rapcjet_match->SetFillColorAlpha(kYellow, 0.35);
  h_n_rapcjet_match->SetMarkerStyle(0);
  h_n_rapcjet_match->DrawCopy("hist same");

  TLegend* lg21 = new TLegend(0.5,0.75,0.8,0.9);
  lg21->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg21->AddEntry(h_n_raptjet_ini,"y_{jet}^{truth}","lf");
  lg21->AddEntry(h_n_rapcjet_ini,"y_{jet}^{topo-cluster}","lf");
  lg21->Draw("same");

  TH1D* h_rapjet_mcopy = new TH1D(*h_n_raptjet_match);
  h_rapjet_mcopy->SetFillColor(0);
  h_rapjet_mcopy->DrawCopy("hist same");

  _frame17->DrawCopy("axis same");

  cag
->Print("rapjets_matched.pdf");

  ////////////////////////////////////////////////////////////
  //34. Transverse momentum vs. rapidity, Matched Truth Jets//
  ////////////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_pt_raptjet_match = (TH2D*)f->FindObjectAny("MatchedTruth/Final/d_pt_rapjet");
  if ( d_pt_raptjet_match ==  0 ) { printf("cannot find matched truth jet pt vs. rap histo!\n"); return; }


  TCanvas* cah = new TCanvas("pt_rap_tjetsM","pT vs. y, Matched Truth Jets");

  // just the frame
  TH2D* _frame18 = new TH2D("_frame18","_frame18",100,0.,120.,100,0.,120.);
 
  _frame18->SetTitle(0);
  _frame18->GetXaxis()->SetTitle("rapidity");
  _frame18->GetXaxis()->SetLimits(-5,5);
  _frame18->GetYaxis()->SetTitle("p^{jet} [GeV]");
  _frame18->GetYaxis()->SetLimits(-10,1200); 
  _frame18->DrawCopy("axis");

  d_pt_raptjet_match->SetLineColor(kBlue);
  d_pt_raptjet_match->SetFillColorAlpha(kCyan, 0.35);
  d_pt_raptjet_match->SetMarkerStyle(0);
  d_pt_raptjet_match->DrawCopy("hist same");


  TH2D* d_pt_rapjet_mcopy = new TH2D(*d_pt_raptjet_match);
  d_pt_rapjet_mcopy->SetFillColor(0);
  d_pt_rapjet_mcopy->DrawCopy("hist same");

  _frame18->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_pt_raptjet_match->Draw("CONTZ");

  cah->Print("pt_rap_tmatched.pdf");

  ////////////////////////////////////////////////////////////
  //35. Transverse momentum vs. rapidity, Matched Calo Jets//
  ////////////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_pt_rapcjet_match = (TH2D*)f->FindObjectAny("MatchedCalo/Final/d_pt_rapjet");
  if ( d_pt_rapcjet_match ==  0 ) { printf("cannot find matched truth jet pt vs. rap histo!\n"); return; }


  TCanvas* cai = new TCanvas("pt_rap_cjetsM","pT vs. y, Matched Calo Jets");

  _frame18->DrawCopy("axis");

  d_pt_rapcjet_match->SetLineColor(kBlue);
  d_pt_rapcjet_match->SetFillColorAlpha(kCyan, 0.35);
  d_pt_rapcjet_match->SetMarkerStyle(0);
  d_pt_rapcjet_match->DrawCopy("hist same");


  TH2D* d_pt_rapjet_mccopy = new TH2D(*d_pt_rapcjet_match);
  d_pt_rapjet_mccopy->SetFillColor(0);
  d_pt_rapjet_mccopy->DrawCopy("hist same");

  _frame18->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_pt_rapcjet_match->Draw("CONTZ");

  cai->Print("pt_rap_cmatched.pdf");

  ////////////////////////////////////
  //36. Jet Timing, Matched CaloJets//
  ////////////////////////////////////


  // find histos 
  TH1D* h_t_cjet_match = (TH1D*)f->FindObjectAny("MatchedCalo/Final/h_tjet");
  if ( h_t_cjet_match ==  0 ) { printf("cannot find matched jet t histo!\n"); return; }

  TCanvas* caj = new TCanvas("tJetsMatched","Jet Timing, Matched TopoCluster Jets");
  caj->SetLogy();

  _frame8->DrawCopy("axis");

  h_t_cjet_match->SetLineColor(kRed);
  h_t_cjet_match->SetFillColorAlpha(kYellow, 0.35);                 
  h_t_cjet_match->SetMarkerStyle(0);                                 

  TLegend* lg22 = new TLegend(0.5,0.75,0.8,0.9);
  lg22->SetTextSize(_frame->GetXaxis()->GetLabelSize());
  lg22->AddEntry(h_n_pt_calojet_sig,"Timing^{Matched TopoCluster Jet}","lf");
  lg22->Draw("same");

  TH1D* h_t_cjet_mcopy = new TH1D(*h_t_cjet_match);
  h_t_cjet_mcopy->SetFillColor(kYellow);
  h_t_cjet_mcopy->DrawCopy("hist same");



  _frame8->DrawCopy("axis same");

  caj->Print("tcjets_ini.pdf");

  //////////////////////////////////////////////
  //37. Signal in Jet Timing, Matched CaloJets//
  //////////////////////////////////////////////


  // find histos 
  TH1D* h_tsig_cjet_match = (TH1D*)f->FindObjectAny("MatchedCalo/Final/h_tsig");
  if ( h_tsig_cjet_match ==  0 ) { printf("cannot find matched jet tsig histo!\n"); return; }

  TCanvas* cak = new TCanvas("tsigJetsMatched","Signal in Jet Timing, Matched TopoCluster Jets");
  cak->SetLogy();

  _frame9->DrawCopy("axis");

  h_tsig_cjet_match->SetLineColor(kRed);
  h_tsig_cjet_match->SetFillColorAlpha(kYellow, 0.35);                 
  h_tsig_cjet_match->SetMarkerStyle(0);                                 

  lg12->Draw("same");

   TH1D* h_tsig_cjet_mcopy = new TH1D(*h_tsig_cjet_match);
   h_tsig_cjet_mcopy->SetFillColor(kYellow);
   h_tsig_cjet_mcopy->DrawCopy("hist same");



  _frame9->DrawCopy("axis same");

  cak
->Print("tsig_cjets_matched.pdf");

  ///////////////////////////////////////////////////////
  //38. Signal in Timing vs. rapidity, Matched CaloJets//
  ///////////////////////////////////////////////////////

  // fimd histos 
  TH2D* d_tsig_rapcjet_match = (TH2D*)f->FindObjectAny("MatchedCalo/Final/d_tsig_rapsig");
  if ( d_tsig_rapcjet_match ==  0 ) { printf("cannot find matched calo jet tsig vs. rap histo!\n"); return; }


  TCanvas* cal = new TCanvas("tsig_rap_jetsMatched","tsig vs. y, Matched TopoCluster Jets");

  // just the frame

  _frame12->DrawCopy("axis");

  d_tsig_rapcjet_match->SetLineColor(kRed);
  d_tsig_rapcjet_match->SetFillColorAlpha(kYellow, 0.35);
  d_tsig_rapcjet_match->SetMarkerStyle(0);
  d_tsig_rapcjet_match->DrawCopy("hist same");

  lg15->Draw("same");

  TH2D* d_tsig_rapjet_mcopy = new TH2D(*d_tsig_rapcjet_match);
  d_tsig_rapjet_mcopy->SetFillColor(0);
  d_tsig_rapjet_mcopy->DrawCopy("hist same");

  _frame12->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_tsig_rapcjet_match->Draw("CONTZ");

  cal->Print("tsig_rap_cmatched.pdf");

  /////////////////////////////////////////////
  //23. Timing vs. rapidity, Matched CaloJets//
  /////////////////////////////////////////////

  // fimd histos 
  TH2D* d_tjet_rapcjet_match = (TH2D*)f->FindObjectAny("MatchedCalo/Final/d_tjet_rapjet");
  if ( d_tjet_rapcjet_match ==  0 ) { printf("cannot find matched calo jet t vs. rap histo!\n"); return; }


  TCanvas* cam = new TCanvas("t_rap_cjetsMatchd","t vs. y, Matched TopoCluster Jets");
  cw->SetLogy();

  // just the frame

  _frame10->DrawCopy("axis");

  d_tjet_rapcjet_match->SetLineColor(kRed);
  d_tjet_rapcjet_match->SetFillColorAlpha(kYellow, 0.35);
  d_tjet_rapcjet_match->SetMarkerStyle(0);
  d_tjet_rapcjet_match->DrawCopy("hist same");



  TH2D* d_tjet_rapjet_mccopy = new TH2D(*d_tjet_rapcjet_match);
  d_tjet_rapjet_mccopy->SetFillColor(0);
  d_tjet_rapjet_mccopy->DrawCopy("hist same");

  _frame10->DrawCopy("axis same");
  gStyle->SetPalette(1);
  d_tjet_rapcjet_fin->Draw("CONTZ");

  cw->Print("tjet_rap_cmatched.pdf");


}
