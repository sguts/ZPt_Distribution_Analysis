#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TDirectory.h"
#include "TVectorD.h"

#include "TH1D.h"
#include "TH2D.h"


#include "TLorentzVector.h"
#include "TVector3.h"

#include <iostream>
#include <map>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

void RescaleToDataLuminosity (TH1D *Histo_Signal, TH1D *Histo_BG, TVectorD *v_Signal, TVectorD *v_BG) {
  double crossSectionUncut_Signal = (*v_Signal)[0];
  double nEntries_Signal =  (*v_Signal)[1];
  double nEntriesUncut_Signal = (*v_Signal)[2];
  double crossSectionUncut_BG = (*v_BG)[0];
  double nEntries_BG =  (*v_BG)[1];
  double nEntriesUncut_BG = (*v_BG)[2];

  double lumi_Signal = nEntriesUncut_Signal/crossSectionUncut_Signal;
  double lumi_BG = nEntriesUncut_BG/crossSectionUncut_BG;

  cout << "-------------------------------------------" << endl;
  cout << "Signal has title: " << Histo_Signal->GetTitle() << endl;
  cout << "Background has title: " << Histo_BG->GetTitle() << endl;

  cout << "\n\n" << endl;
  Histo_BG->Scale(lumi_Signal/lumi_BG);


  

  return;
}

void SecondStepGeneratorRunner () {
  TFile *input_file, *output_file;
  TDirectory *current_output_direcory;
  input_file = new TFile("/local/cms/user/guts/ZPtDistribution/ZFinderTreesAnalysis/FirstStepZeeHisto.root","READ");
  
  double scalingFactor;
  TVectorD *v_Sig, *v;
  
  TH1D *ZPt_Signal_Z_ee_Cut_1 = (TH1D *)input_file->Get("Histograms for Z_ee signal MC Cut 1/ZPt_Reco");
  //ZPt_Signal_Z_ee_Cut_1->Sumw2();
  v_Sig = (TVectorD *)input_file->Get("Histograms for Z_ee signal MC Cut 1/sigmaNEntCutNEntUncut");
  RescaleToDataLuminosity(ZPt_Signal_Z_ee_Cut_1,ZPt_Signal_Z_ee_Cut_1,v_Sig,v_Sig);
 
  
  
  /* //Temp removed
  TH1D *ZPt_Signal_Z_ee_Cut_2 = (TH1D *)input_file->Get("Histograms for Z_ee signal MC Cut 2/ZPt_Reco");
  //ZPt_Signal_Z_ee_Cut_2->Sumw2();
  v = (TVectorD *)input_file->Get("Histograms for Z_ee signal MC Cut 2/sigmaNEntCutNEntUncut");
  scalingFactor = (double)((*v)[0]*(*v)[1]/(*v)[2]);
  cout << "For signal 2 scaling factor is "<< scalingFactor << "\ncrossSectionUncut is " << (*v)[0] << "\nnEntries is " << (*v)[1] <<
  "\nnEntriesUncut is " << (*v)[2] << "\n \n";
  ZPt_Signal_Z_ee_Cut_2->Scale(scalingFactor);
  */

  TH1D *ZPt_Signal_Z_ee_Cut_total = (TH1D *)ZPt_Signal_Z_ee_Cut_1->Clone("ZPt Signal total normalized");
  //ZPt_Signal_Z_ee_Cut_total->Add(ZPt_Signal_Z_ee_Cut_2);//Temp removed
  //total signal generated
  
  TH1D *ZPt_BG_Tau_ee_Cut = (TH1D *)input_file->Get("Histograms for Tau_ee BG MC Cut/ZPt_Reco");
  //ZPt_BG_Tau_ee_Cut->Sumw2();
  v = (TVectorD *)input_file->Get("Histograms for Tau_ee BG MC Cut/sigmaNEntCutNEntUncut");
  RescaleToDataLuminosity(ZPt_Signal_Z_ee_Cut_total,ZPt_BG_Tau_ee_Cut,v_Sig,v);
  
  TH1D *ZPt_BG_WW_ee_Cut = (TH1D *)input_file->Get("Histograms for WW_ee BG MC Cut/ZPt_Reco");
  //ZPt_BG_WW_ee_Cut->Sumw2();
  v = (TVectorD *)input_file->Get("Histograms for WW_ee BG MC Cut/sigmaNEntCutNEntUncut");
  RescaleToDataLuminosity(ZPt_Signal_Z_ee_Cut_total,ZPt_BG_WW_ee_Cut,v_Sig,v);
  
  TH1D *ZPt_BG_WZ_ee_Cut = (TH1D *)input_file->Get("Histograms for WZ_ee BG MC Cut/ZPt_Reco");
  //ZPt_BG_WZ_ee_Cut->Sumw2();
  v = (TVectorD *)input_file->Get("Histograms for WZ_ee BG MC Cut/sigmaNEntCutNEntUncut");
  RescaleToDataLuminosity(ZPt_Signal_Z_ee_Cut_total,ZPt_BG_WZ_ee_Cut,v_Sig,v);
  
  TH1D *ZPt_BG_ZZ_ee_Cut = (TH1D *)input_file->Get("Histograms for ZZ_ee BG MC Cut/ZPt_Reco");
  //ZPt_BG_ZZ_ee_Cut->Sumw2();
  v = (TVectorD *)input_file->Get("Histograms for ZZ_ee BG MC Cut/sigmaNEntCutNEntUncut");
  RescaleToDataLuminosity(ZPt_Signal_Z_ee_Cut_total,ZPt_BG_ZZ_ee_Cut,v_Sig,v);
  
  TH1D *ZPt_BG_t_ee_Cut = (TH1D *)input_file->Get("Histograms for t_ee BG MC Cut/ZPt_Reco");
  //ZPt_BG_t_ee_Cut->Sumw2();
  v = (TVectorD *)input_file->Get("Histograms for t_ee BG MC Cut/sigmaNEntCutNEntUncut");
  RescaleToDataLuminosity(ZPt_Signal_Z_ee_Cut_total,ZPt_BG_t_ee_Cut,v_Sig,v);
  
  TH1D *ZPt_BG_tbar_ee_Cut = (TH1D *)input_file->Get("Histograms for tbar_ee BG MC Cut/ZPt_Reco");
  //ZPt_BG_tbar_ee_Cut->Sumw2();
  v = (TVectorD *)input_file->Get("Histograms for tbar_ee BG MC Cut/sigmaNEntCutNEntUncut");
  RescaleToDataLuminosity(ZPt_Signal_Z_ee_Cut_total,ZPt_BG_tbar_ee_Cut,v_Sig,v);
  
  TH1D *ZPt_BG_ttbar_ee_Cut = (TH1D *)input_file->Get("Histograms for ttbar_ee BG MC Cut/ZPt_Reco");
  //ZPt_BG_ttbar_ee_Cut->Sumw2();
  v = (TVectorD *)input_file->Get("Histograms for ttbar_ee BG MC Cut/sigmaNEntCutNEntUncut");
  RescaleToDataLuminosity(ZPt_Signal_Z_ee_Cut_total,ZPt_BG_ttbar_ee_Cut,v_Sig,v);
  
  TH1D *ZPt_BG_t_combined_ee_Cut = (TH1D *)ZPt_BG_t_ee_Cut->Clone("ZPt t backgrounds total normalized");
  ZPt_BG_t_combined_ee_Cut->Add(ZPt_BG_tbar_ee_Cut);
  ZPt_BG_t_combined_ee_Cut->Add(ZPt_BG_ttbar_ee_Cut);
  
  TH1D *ZPt_BG_nont_combined_ee_Cut = (TH1D *)ZPt_BG_Tau_ee_Cut->Clone("ZPt non-t backgrounds total normalized");
  ZPt_BG_nont_combined_ee_Cut->Add(ZPt_BG_WW_ee_Cut);
  ZPt_BG_nont_combined_ee_Cut->Add(ZPt_BG_WZ_ee_Cut);
  ZPt_BG_nont_combined_ee_Cut->Add(ZPt_BG_ZZ_ee_Cut);
  
  TH1D *ZPt_BG_all_combined_ee_Cut = (TH1D *)ZPt_BG_t_combined_ee_Cut->Clone("ZPt all backgrounds total normalized");
  ZPt_BG_all_combined_ee_Cut->Add(ZPt_BG_nont_combined_ee_Cut);
  
  
  
  
  
  cout<<"1"<<endl;
  TH1D *ZPt_Signal_BG_all_combined_ee_Cut = (TH1D *)ZPt_BG_all_combined_ee_Cut->Clone("ZPt S+B");
  ZPt_Signal_BG_all_combined_ee_Cut->Add(ZPt_Signal_Z_ee_Cut_total);
  TH1D *ZPt_BG_efficiency = (TH1D *) ZPt_Signal_Z_ee_Cut_total->Clone("ZPt ratio S/(S+B)");
  
  ZPt_BG_efficiency->Divide(ZPt_Signal_BG_all_combined_ee_Cut);
  
  TH1D *ZPt_Data_Zcandidate_ee_Cut = (TH1D *)input_file->Get("Histograms for Zcandidate_ee Signal Data Cut/ZPt_Reco");
  double dataLumi = 19.7;
  ZPt_Data_Zcandidate_ee_Cut->Scale(1/dataLumi);
  cout << "Scaled data by luminosity "<< dataLumi << " fb^-1" <<endl;

  
  //Finished making stuff
  output_file = new TFile ("/local/cms/user/guts/ZPtDistribution/ZFinderTreesAnalysis/SecondStepZeeHisto.root","RECREATE");
  ZPt_BG_efficiency->Write("BG efficiency");
  ZPt_Data_Zcandidate_ee_Cut->Write("ZPt data raw");
  ZPt_Signal_Z_ee_Cut_total->Write("ZPt MC signal");
  ZPt_BG_t_combined_ee_Cut->Write("ZPt combined t backgrounds");
  ZPt_BG_nont_combined_ee_Cut->Write("ZPt combined non-t backgrounds");
  ZPt_BG_all_combined_ee_Cut->Write("ZPt combined all backgrounds");
  
  
  input_file->Close();
  output_file->Close();
}

void SlideGeneratorPDF () {
  
}