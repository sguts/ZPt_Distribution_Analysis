#include "TFile.h"
#include "TTree.h"

#include "TH1D.h"
#include "TH2D.h"

#include "TLorentzVector.h"
#include "TVector3.h"

#include <iostream>
#include <map>

#define _USE_MATH_DEFINES
#include <math.h>

#include "CollinsSopperAngle.h"

using namespace std;
  
void FillSlicedHistograms (std::vector<TH1D*> target, double value, double ZPtMC, std::vector<double> ZPtSeparators) 
{
  //std::vector<TH1D*> SANITYCHECK;
  //SANITYCHECK.size();
  /*
    for (int i=0; i<(int)(target.size()) && i<(int)(ZPtSeparators.size())-1; i++) {
      if (ZPtMC>ZPtSeparators[i] && ZPtMC < ZPtSeparators[i+1]) {
	//target[i]->Fill(value);
	break;
      }
    }*/
}
  
void MyLoader (int maxEntries=-1, int outputEvery=100000) {
     /*
 
  cout<<"Hi!"<<endl;
  //input and output files
  TFile *input_file = new TFile ("/local/cms/user/guts/ZPtDistribution/IntermidiateFiles/CombinedTree2016DataMay30.root");
  TFile *output_file = new TFile ("/local/cms/user/guts/ZPtDistribution/IntermidiateFiles/TreeGeneratedHistogramms.root","RECREATE");
  if(!input_file){cout<<"Couldn't find the file"<<endl;return;}
  if(!output_file){cout<<"Couldn't create output file"<<endl;return;}
  //TTree *tree1 = (TTree*) input_file->Get("dataEE");
  //input tree
  TTree *tree1;
  input_file->GetObject("AnalyzerRunner/dataEE",tree1);
  if(!input_file){cout<<"Tree not found"<<endl;return;}
  //output histogramss
  TH1D *electron_phi_ColSop_histo = new TH1D ("e+_phi__ColSop_dist","Electron angular distribution in Collins_Soper Frame",100,-M_PI,M_PI);
  
  int bins = 100;
  double angleMin=-M_PI*1.1, angleMax=M_PI*1.1;
  
  double ColSopAngle, ZPt;

  vector<double> ZPtSeparators {0,3,6,10,20,35,50,100};
  
  vector<TH1D*> ColSopSliced;
  for (auto i=0; i<(int)(ZPtSeparators.size())-1;i++) {
    TH1D* tempTH1D = new TH1D(("Sliced histogram info number "+to_string(i+1)).c_str(),
    ("Collins Soper angle distribution of events with " + to_string(ZPtSeparators[i]) + " < ZP_t < " + to_string(ZPtSeparators[i+1]) ).c_str(),bins, angleMin, angleMax);
    //tempTH1D->GetXaxis()->SetTitle("MC - Reco, GeV");
    ColSopSliced.push_back(tempTH1D);
  }
  
  double epx=0, epy=0, epz=0, eE=0;
  double ppx=0, ppy=0, ppz=0, pE=0;
  tree1->SetBranchAddress("electron px",&epx);
  tree1->SetBranchAddress("electron py",&epy);
  tree1->SetBranchAddress("electron pz",&epz);
  tree1->SetBranchAddress("electron E",&eE);
  tree1->SetBranchAddress("positron px",&ppx);
  tree1->SetBranchAddress("positron py",&ppy);
  tree1->SetBranchAddress("positron pz",&ppz);
  tree1->SetBranchAddress("positron E",&pE);
  
  TLorentzVector electron4Vec(0,0,0,0);
  TLorentzVector positron4Vec(0,0,0,0);
  
  
  //if(!&protonMomentum){cout<<"And the branch wasn't found?"<<endl;return;}
  Int_t nentries= tree1->GetEntries();
  for (Int_t i=0; i<nentries && i!=maxEntries; i++) {
      if (i % outputEvery == 0) {cout << "\nProcessing entry " << i;}
      tree1->GetEntry(i);
      electron4Vec.SetXYZT(epx,epy,epz,eE);
      positron4Vec.SetXYZT(ppx,ppy,ppz,pE);
      
      ColSopAngle = CollinsSopperAngle(electron4Vec,positron4Vec);
      ZPt= (electron4Vec+ positron4Vec).Perp();
      
      electron_phi_ColSop_histo -> Fill (ColSopAngle);
      FillSlicedHistograms (ColSopSliced,ColSopAngle,ZPt,ZPtSeparators);
      
      
  }
  
  //Writing results
  output_file->WriteObject(electron_phi_ColSop_histo, "phi_ColSop_hist");
  for  (int i=0; i<(int)(ColSopSliced.size());i++) {
    output_file->WriteObject(ColSopSliced[i],( "Sliced ColSop Distribution"+to_string(ZPtSeparators[i]) + "<ZPtMC<" +to_string(ZPtSeparators[i+1])).c_str());
  }
  //Closing files
  input_file->Close();
  output_file->Close();*/
}