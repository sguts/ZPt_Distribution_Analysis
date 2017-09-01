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

#include "CollinsSopperAngle.h"
#include "AngularErrors.h"

//This file contains code for class used to generate histograms for the first step of Zpt ananlysis. So far shit only includes Z->ee, adding muon will be a pain in the ass 

class ZFinderTreeLooper {
  public:
    ZFinderTreeLooper (std::string inputFileNameVariable, std::string outputFileNameVariable, std::string dataTitleVariable, 
	  double crossSectionUncutVariable, std::string treeAdressVariable=string("ZFinder/Combined Single Reco/Combined Single Reco"), bool isMCVariable=true, bool recreateFile=true) {
      cout << "Class created!" << endl;
      RestartClass(inputFileNameVariable, outputFileNameVariable, dataTitleVariable, crossSectionUncutVariable, treeAdressVariable, isMCVariable, recreateFile);
    }
  
    ~ZFinderTreeLooper () {}

    //essentially a constructor without deleting and making class anew
    void RestartClass (std::string inputFileNameVariable, std::string outputFileNameVariable, std::string dataTitleVariable,  double crossSectionUncutVariable, std::string treeAdressVariable, bool isMCVariable, bool recreateFile);
    
    void recreateOutputFile ();
    void MakeMCHistograms ();
    void MakeDataHistograms ();
    void SetLoopParameters (int newMaxEntries, int newOutputEvery);

  private:
    bool isMC;
    bool Zee=true;
    bool Zmumu=false;
    //Physics constants
    double e_mass = 510.998/1000000; 
    double mu_mass = 105.658/1000;
    
    double e_Energy_sigma = 0;
    double Z_Pt_sigma = 0;

    //Looping variables
    int maxEntries=-1, outputEvery=100000;
    int nEntries, nEntriesUncut;
    double crossSectionUncut;
    
    TFile *input_file, *output_file;
    //TDirectory *current_directory;
    TBranch *Reco, *MCTruth;
    TTree *inputDataTree;

    TLorentzVector e_4Vec0_Reco, e_4Vec1_Reco, e_4Vec0_MC, e_4Vec1_MC;
    string inputFileName, outputFileName, dataTitle, treeAdress;

    bool OpenFiles ();
    void CloseFiles ();
    void RestoreData (Int_t entryNumber);
    TLorentzVector ReconstructParticle4Vector (double pt, double eta, double phi);
    void MakeBinningArray ();
    
};

void ZFinderTreeLooper::RestartClass (std::string inputFileNameVariable, std::string outputFileNameVariable, 
	std::string dataTitleVariable, double crossSectionUncutVariable, std::string treeAdressVariable=string("ZFinder/Combined Single Reco/Combined Single Reco"), bool isMCVariable=true, bool recreateFile=false) {
  isMC=isMCVariable;
  inputFileName=inputFileNameVariable;
  outputFileName=outputFileNameVariable;
  dataTitle=dataTitleVariable;
  crossSectionUncut=crossSectionUncutVariable;
  treeAdress=treeAdressVariable;

  if (recreateFile) recreateOutputFile();
  //MakeBinningArray ();//does nothing so far
  cout << "Variables reinitialized, proceeding to run ";
  if (isMCVariable) {
    cout << "MakeMCHistograms for " << dataTitle << endl;
    MakeMCHistograms ();
  } else {
    cout << "MakeDataHistograms for " << dataTitle << endl;
    MakeDataHistograms ();
  }
}

void ZFinderTreeLooper::SetLoopParameters (int newMaxEntries, int newOutputEvery){
  maxEntries=newMaxEntries;
  outputEvery=newOutputEvery;
}

void ZFinderTreeLooper::SetLoopParameters () {
  e_Energy_sigma = 
}

void ZFinderTreeLooper::RestoreData (Int_t entryNumber) {
  //Leaf loading initialization
  double e_pt0_Reco, e_eta0_Reco, e_phi0_Reco;
  double e_pt1_Reco, e_eta1_Reco, e_phi1_Reco;
  
  double e_pt0_MC, e_eta0_MC, e_phi0_MC;
  double e_pt1_MC, e_eta1_MC, e_phi1_MC;
  
  inputDataTree->GetEntry(entryNumber);

  if (isMC) {
  
    e_pt0_MC= MCTruth->GetLeaf("e_pt0")->GetValue();
    e_eta0_MC= MCTruth->GetLeaf("e_eta0")->GetValue();
    e_phi0_MC= MCTruth->GetLeaf("e_phi0")->GetValue();
      
    e_pt1_MC= MCTruth->GetLeaf("e_pt1")->GetValue();
    e_eta1_MC= MCTruth->GetLeaf("e_eta1")->GetValue();
    e_phi1_MC= MCTruth->GetLeaf("e_phi1")->GetValue();

    e_4Vec0_MC = ReconstructParticle4Vector (e_pt0_MC, e_eta0_MC, e_phi0_MC);
    e_4Vec1_MC = ReconstructParticle4Vector (e_pt1_MC, e_eta1_MC, e_phi1_MC);
  }

  e_pt0_Reco= Reco->GetLeaf("e_pt0")->GetValue();
  e_eta0_Reco= Reco->GetLeaf("e_eta0")->GetValue();
  e_phi0_Reco= Reco->GetLeaf("e_phi0")->GetValue();
    
  e_pt1_Reco= Reco->GetLeaf("e_pt1")->GetValue();
  e_eta1_Reco= Reco->GetLeaf("e_eta1")->GetValue();
  e_phi1_Reco= Reco->GetLeaf("e_phi1")->GetValue();

  e_4Vec0_Reco = ReconstructParticle4Vector (e_pt0_Reco, e_eta0_Reco, e_phi0_Reco);
  e_4Vec1_Reco = ReconstructParticle4Vector (e_pt1_Reco, e_eta1_Reco, e_phi1_Reco);
}

TLorentzVector ZFinderTreeLooper::ReconstructParticle4Vector (double pt, double eta, double phi) {
  TLorentzVector particle4Vec;
  particle4Vec.SetXYZM(pt * cos (phi), pt * sin (phi), pt * sinh (eta), e_mass);
  return particle4Vec;
}

bool ZFinderTreeLooper::OpenFiles () {
  input_file = new TFile(inputFileName.c_str(),"READ");
  output_file = new TFile(outputFileName.c_str(),"UPDATE");
  if(!input_file->IsOpen()){ cout<<"Couldn't find the file"<<endl; return false;}
  if(!output_file->IsOpen()){cout<<"Couldn't create output file"<<endl; return false;}  
  
  //IMPORTANT
  //Check that this particular cut (Combined Single Reco/Combined Single Reco) is what you want!!!!
  
  input_file->GetObject(treeAdress.c_str(),inputDataTree);
  if(!inputDataTree){cout<<"Tree not found"<<endl; return false;}
  
  Reco = inputDataTree->GetBranch("reco");
  if (isMC) {MCTruth = inputDataTree->GetBranch("truth");}
  
  nEntries = inputDataTree->GetEntries();
  
  if (isMC) {
    TH1I* nEntriesUncutHisto;
    input_file->GetObject("ZFinder/unweighted_counter",nEntriesUncutHisto);
    nEntriesUncut = (double)( nEntriesUncutHisto->GetEntries() );
  }
  
  return true;
}

void ZFinderTreeLooper::CloseFiles () {
  output_file->Write();
  cout << "Saved objects" <<endl;
  input_file->Close();
  output_file->Close();
}

void ZFinderTreeLooper::recreateOutputFile () {
  cout << "Recreating output file"<<endl;
  output_file = new TFile(outputFileName.c_str(),"RECREATE");
  output_file->Close();
}

void ZFinderTreeLooper::MakeMCHistograms () {
  if(!OpenFiles()) cout<< "Reopening class failed" <<endl;
  //initializing histograms
  TDirectory *current_directory=output_file->mkdir((std::string("Histograms for ")+dataTitle).c_str() );
  current_directory->cd();

  double minZPt=0, maxZPt=100;
  int bins=100;

  TH1D *ZPtDistributionReco = new TH1D ("ZPt_Reco",("ZPt Reco distribution for "+dataTitle).c_str(),bins,minZPt,maxZPt);
  ZPtDistributionReco->GetXaxis()->SetTitle("ZPt Reco, GeV");
  TH1D *ZPtDistributionMC = new TH1D ("ZPt_MC",("ZPt MC distribution for "+dataTitle).c_str(),bins,minZPt,maxZPt);
  ZPtDistributionMC->GetXaxis()->SetTitle("ZPt MC, GeV");
  TH2D *ZPtRotationalMatrixRecoMC = new TH2D ("ZPt_rotational_matrix_Reco_MC",("ZPt Rotational matrix for "+dataTitle).c_str(),
    bins,minZPt,maxZPt,bins,minZPt,maxZPt);
  ZPtRotationalMatrixRecoMC->GetXaxis()->SetTitle("ZPt Reco, GeV");
  ZPtRotationalMatrixRecoMC->GetYaxis()->SetTitle("ZPt MC, GeV");

  TVectorD v(3);
  
  v[0]=crossSectionUncut;
  v[1]=nEntries;
  v[2]=nEntriesUncut;
  
  v.Write("sigmaNEntCutNEntUncut");
  //initializing variables.
  double ZPtReco, ZPtMC;
  //loop
    for (Int_t i=0; i<nEntries && i!=maxEntries; i++) {
      //Getting entry
      if (i % outputEvery == 0) {cout << "Processing entry " << i << endl;}
      RestoreData(i);
      //Filling out structures
      ZPtReco= (e_4Vec0_Reco + e_4Vec1_Reco).Perp();
      ZPtMC= (e_4Vec0_MC + e_4Vec1_MC).Perp();
      ZPtDistributionReco->Fill(ZPtReco);
      ZPtDistributionMC->Fill(ZPtMC);
      ZPtRotationalMatrixRecoMC->Fill(ZPtReco,ZPtMC);
      
  }
  //Grooming results
  cout<< "Processed "<< nEntries << " entries" << endl;
  
  CloseFiles ();
}

void ZFinderTreeLooper::MakeDataHistograms () {
  if(!OpenFiles()) cout<< "Reopening class failed" <<endl;
  //initializing histograms
  TDirectory *current_directory=output_file->mkdir((std::string("Histograms for ")+dataTitle).c_str() );
  current_directory->cd();

  double minZPt=0, maxZPt=100;
  int bins=100;

  TH1D *ZPtDistributionReco = new TH1D ("ZPt_Reco",("ZPt Reco distribution for "+dataTitle).c_str(),bins,minZPt,maxZPt);
  ZPtDistributionReco->GetXaxis()->SetTitle("ZPt Reco, GeV");
  
  double ZPtReco, ZPtMC;
  //loop
    for (Int_t i=0; i<nEntries && i!=maxEntries; i++) {
      //Getting entry
      if (i % outputEvery == 0) {cout << "Processing entry " << i << endl;}
      RestoreData(i);
      //Filling out structures
      ZPtReco= (e_4Vec0_Reco + e_4Vec1_Reco).Perp();
      ZPtDistributionReco->Fill(ZPtReco);
      
  }
  //Grooming results
  
  CloseFiles ();
  
}