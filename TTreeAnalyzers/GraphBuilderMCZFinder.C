#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TDirectory.h"

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

double e_mass = 511/1000000;

int maxEntries=-1, outputEvery=500000;

TFile *input_file, *output_file;
//TDirectory *current_directory;
TBranch *Reco, *MCTruth;
TTree *inputDataTree;

TLorentzVector e_4Vec0_Reco, e_4Vec1_Reco, e_4Vec0_MC, e_4Vec1_MC;

vector<double> ZPtSeparators {0,3,6,10,20,35,50,100};

void SetOutputParameters (int newMaxEntries, int newOutputEvery) {
  maxEntries = newMaxEntries;
  outputEvery = newOutputEvery;
}

void OpenFiles () {
  input_file = new TFile("/data/whybee0a/user/lesko_2/fermi/Output/Zfinder2MCPDFweightBorn15-7-2/Powheg15-7-04.root","READ");
  output_file = new TFile("/local/cms/user/guts/ZPtDistribution/IntermidiateFiles/GraphsMC2012_8_TeV_Zee.root","RECREATE");
  if(!input_file->IsOpen()){
    cout<<"Couldn't find the file"<<endl;
    return;}
  if(!output_file->IsOpen()){cout<<"Couldn't create output file"<<endl;return;}  
  input_file->GetObject("ZFinder/Combined Single Reco/Combined Single Reco",inputDataTree);
  if(!inputDataTree){cout<<"Tree not found"<<endl;return;}
  Reco = inputDataTree->GetBranch("reco");
  MCTruth = inputDataTree->GetBranch("truth");
}

void CloseFiles () {
  output_file->Write();
  cout << "Saved objects" <<endl;
  input_file->Close();
  output_file->Close();
}

TLorentzVector ReconstructElectron4Vector (double pt, double eta, double phi) {
  TLorentzVector electron4Vec;
  electron4Vec.SetXYZM(pt * cos (phi), pt * sin (phi), pt * sinh (eta), e_mass);
  return electron4Vec;
}

void RestoreData (Int_t entryNumber) {
  //Leaf loading initialization
  double e_pt0_Reco, e_eta0_Reco, e_phi0_Reco;
  double e_pt1_Reco, e_eta1_Reco, e_phi1_Reco;
  
  double e_pt0_MC, e_eta0_MC, e_phi0_MC;
  double e_pt1_MC, e_eta1_MC, e_phi1_MC;
  
  inputDataTree->GetEntry(entryNumber);
  
  e_pt0_MC= MCTruth->GetLeaf("e_pt0")->GetValue();
  e_eta0_MC= MCTruth->GetLeaf("e_eta0")->GetValue();
  e_phi0_MC= MCTruth->GetLeaf("e_phi0")->GetValue();
      
  e_pt1_MC= MCTruth->GetLeaf("e_pt1")->GetValue();
  e_eta1_MC= MCTruth->GetLeaf("e_eta1")->GetValue();
  e_phi1_MC= MCTruth->GetLeaf("e_phi1")->GetValue();
  
  e_pt0_Reco= Reco->GetLeaf("e_pt0")->GetValue();
  e_eta0_Reco= Reco->GetLeaf("e_eta0")->GetValue();
  e_phi0_Reco= Reco->GetLeaf("e_phi0")->GetValue();
      
  e_pt1_Reco= Reco->GetLeaf("e_pt1")->GetValue();
  e_eta1_Reco= Reco->GetLeaf("e_eta1")->GetValue();
  e_phi1_Reco= Reco->GetLeaf("e_phi1")->GetValue();
      
  
  
  e_4Vec0_MC = ReconstructElectron4Vector (e_pt0_MC, e_eta0_MC, e_phi0_MC);
  e_4Vec1_MC = ReconstructElectron4Vector (e_pt1_MC, e_eta1_MC, e_phi1_MC);
  e_4Vec0_Reco = ReconstructElectron4Vector (e_pt0_Reco, e_eta0_Reco, e_phi0_Reco);
  e_4Vec1_Reco = ReconstructElectron4Vector (e_pt1_Reco, e_eta1_Reco, e_phi1_Reco);
  
}

//Template for future functions, including a standart loop and a saver, commented out for future use

/*
void INSERT_NAME () {
  //make directory
  TDirectory *current_directory=output_file->mkdir("DirName");
  current_directory->cd();
  //Initialize nessesary stuff
  Int_t nentries= inputDataTree->GetEntries();
  for (Int_t i=0; i<nentries && i!=maxEntries; i++) {
      //Getting entry
      if (i % outputEvery == 0) {cout << "Processing entry " << i << endl;}
      RestoreData(i);
      //Filling out structures
      
  }
  //Grooming results
  
  //Saving objects
  output_file->Write();
}
*/

void FillSlicedHistograms (vector<TH1D*> target, double value, double ZPtMC) 
{
    for (int i=0; i<(int)(target.size()) && i<(int)(ZPtSeparators.size())-1; i++) {
      if (ZPtMC>ZPtSeparators[i] && ZPtMC < ZPtSeparators[i+1]) {
	target[i]->Fill(value);
	break;
      }
    }
}

void HistogramFillerZptError () {
  //make directories
  TDirectory *current_directory=output_file->mkdir("ZptError");
  current_directory->cd();
  //Initialize nessesary stuff
  
  //hist variables
  int bins = 100;
  double minZPtError = -4, maxZPtError = 4;
  double minZPt=0, maxZPt=50;
  
  double ZPtMC, ZPtReco;
  
  //hist with all results
  TH1D* ZPtTotal_Error= new TH1D("Total error","ZPt error",bins,minZPtError,maxZPtError);
  ZPtTotal_Error->GetXaxis()->SetTitle("MC - Reco, GeV");
  
  TH2D* ZPtDistribution_Error = new TH2D("ZPtDistributionError","ZPt error distribution",bins,minZPt,maxZPt,bins,minZPtError,maxZPtError);
  ZPtDistribution_Error->GetXaxis()->SetTitle("Z p_t MC, GeV");
  ZPtDistribution_Error->GetYaxis()->SetTitle("Z p_t, MC-Reco, GeV");
  
  // vector of slised histograms
  vector<TH1D*> ZPtSliced_Error;
  for (auto i=0; i<(int)(ZPtSeparators.size())-1;i++) {
    TH1D* tempTH1D = new TH1D(("Histogram info number "+to_string(i+1)).c_str(),
    ("ZPt error of events with " + to_string(ZPtSeparators[i]) + " < ZP_t MC < " + to_string(ZPtSeparators[i+1]) ).c_str(),bins,minZPtError,maxZPtError);
    tempTH1D->GetXaxis()->SetTitle("MC - Reco, GeV");
    ZPtSliced_Error.push_back(tempTH1D);
  }
  
  Int_t nentries= inputDataTree->GetEntries();
  for (Int_t i=0; i<nentries && i!=maxEntries; i++) {
      //Getting entry
      if (i % outputEvery == 0) {cout << "Processing entry " << i << " for HistogramFillerZptError" << endl;}
      RestoreData(i);
      //Filling out structures
      ZPtReco= (e_4Vec0_Reco + e_4Vec1_Reco).Perp();
      ZPtMC= (e_4Vec0_MC + e_4Vec1_MC).Perp();
      
      FillSlicedHistograms (ZPtSliced_Error, ZPtMC-ZPtReco, ZPtMC);
      ZPtTotal_Error->Fill(ZPtMC-ZPtReco);
      ZPtDistribution_Error->Fill(ZPtMC, ZPtMC-ZPtReco);
      
  }
  //Grooming results
  cout << "Left the loop" <<endl;
  //Saving objects
  /*output_file->WriteObject(ZPtTotal_Error, "ZPtError");
  output_file->WriteObject(ZPtDistribution_Error, "ZPtDistribution_Error");
  for  (int i=0; i<(int)(ZPtSliced_Error.size());i++) {
    output_file->WriteObject(ZPtSliced_Error[i],("ZPtError_Sliced, "+to_string(ZPtSeparators[i]) + "<ZPtMC<" +to_string(ZPtSeparators[i+1])).c_str());
  }*/
  

}

void HistogramFillerCollinsSoper () {
  TDirectory *current_directory=output_file->mkdir("CollinsSoperDistributions");
  current_directory->cd();
  //Initialize nessesary stuff
  int bins = 100;
  double angleMin=-M_PI/2*1.1, angleMax=M_PI/2*1.1;
  
  double ColSopAngle, ZPtMC;
  
  
  TH1D* ColSopTotal= new TH1D("Total ColSopDistribution","Collins Soper angle distribution",bins, angleMin, angleMax);
  //ZPtTotal_Error->GetXaxis()->SetTitle("100 bins");
  
  vector<TH1D*> ColSopSliced;
  for (auto i=0; i<(int)(ZPtSeparators.size())-1;i++) {
    TH1D* tempTH1D = new TH1D(("Sliced histogram info number "+to_string(i+1)).c_str(),
    ("Collins Soper angle distribution of events with " + to_string(ZPtSeparators[i]) + " < ZP_t MC < " + to_string(ZPtSeparators[i+1]) ).c_str(),bins, angleMin, angleMax);
    //tempTH1D->GetXaxis()->SetTitle("MC - Reco, GeV");
    ColSopSliced.push_back(tempTH1D);
  }
  
  Int_t nentries= inputDataTree->GetEntries();
  for (Int_t i=0; i<nentries && i!=maxEntries; i++) {
      //Getting entry
      if (i % outputEvery == 0) {cout << "Processing entry " << i << " for HistogramFillerCollinsSoper" << endl;}
      RestoreData(i);
      
      ColSopAngle = CollinsSopperAngle (e_4Vec0_MC, e_4Vec1_MC );
      ZPtMC= (e_4Vec0_MC + e_4Vec1_MC).Perp();
      
      ColSopTotal->Fill(ColSopAngle);
      FillSlicedHistograms (ColSopSliced,ColSopAngle,ZPtMC);
      
      //Filling out structures
      
  }
  //Grooming results
  
  //Saving objects
}

void HistogramFillerZPt () {
  TDirectory *current_directory=output_file->mkdir("ZptDistributions");
  current_directory->cd();
  //Initialize nessesary stuff
  int bins = 100;
  double minZPt=0, maxZPt=50;
  
  double ZPtMC, ZPtReco;
  
  TH1D* ZPtDistributionReco = new TH1D ("ZPt Reco","ZPt Reco",bins,minZPt,maxZPt);
  ZPtDistributionReco->GetXaxis()->SetTitle("GeV");
  TH1D* ZPtDistributionMC = new TH1D ("ZPt MC","ZPt MC",bins,minZPt,maxZPt);
  ZPtDistributionMC->GetXaxis()->SetTitle("GeV");
  
  
  Int_t nentries= inputDataTree->GetEntries();
  for (Int_t i=0; i<nentries && i!=maxEntries; i++) {
      //Getting entry
      if (i % outputEvery == 0) {cout << "Processing entry " << i << " for HistogramFillerZPt" << endl;}
      RestoreData(i);
      //Filling out structures
      ZPtReco= (e_4Vec0_Reco + e_4Vec1_Reco).Perp();
      ZPtMC= (e_4Vec0_MC + e_4Vec1_MC).Perp();
      
      ZPtDistributionReco->Fill(ZPtReco);
      ZPtDistributionMC->Fill(ZPtMC);
      
      
  }
  //Grooming results
  TH1D* ZPtDiff = (TH1D*)ZPtDistributionReco->Clone("ZptDiff");
  ZPtDiff->SetTitle("Difference");
  ZPtDiff->Add(ZPtDistributionMC,-1);
  
}


void HistogramFillerElectronErrors () {
  //mkae directory
  TDirectory *current_directory=output_file->mkdir("ElectronErrors");
  current_directory->cd();
  //Initialize nessesary stuff
  int binsAngle = 100;
  double angleErrorRange = 0.0002;
  int binsEnergy = 100;
  double dEnergyLimit=5, EnergyLimitMin=30, EnergyLimitMax=130;
  TH2D* AngleDistribution_Lead_dPhi_dTheta = new TH2D("AngleDistribution_Lead_dPhi_dTheta","dPhi-dTheta error distribution",
	binsAngle,-M_PI*angleErrorRange,M_PI*angleErrorRange,
	binsAngle,-M_PI*angleErrorRange,M_PI*angleErrorRange);
  AngleDistribution_Lead_dPhi_dTheta->GetXaxis()->SetTitle("dPhi");
  AngleDistribution_Lead_dPhi_dTheta->GetYaxis()->SetTitle("dTheta");

  TH2D* AngleDistribution_Second_dPhi_dTheta = new TH2D("AngleDistribution_Second_dPhi_dTheta","dPhi-dTheta error distribution",
	binsAngle,-M_PI*angleErrorRange,M_PI*angleErrorRange,
	binsAngle,-M_PI*angleErrorRange,M_PI*angleErrorRange);
  AngleDistribution_Second_dPhi_dTheta->GetXaxis()->SetTitle("dPhi");
  AngleDistribution_Second_dPhi_dTheta->GetYaxis()->SetTitle("dTheta");
  
  TH1D* AngleDistribution_Lead_dPhi = new TH1D("AngleDistribution_Lead_dPhi","Lead e dPhi error distribution",
	binsAngle,-M_PI*angleErrorRange,M_PI*angleErrorRange);
  AngleDistribution_Lead_dPhi->GetXaxis()->SetTitle("dPhi");
  TH1D* AngleDistribution_Second_dPhi = new TH1D("AngleDistribution_Second_dPhi","Second e dPhi error distribution",
	binsAngle,-M_PI*angleErrorRange,M_PI*angleErrorRange);
  AngleDistribution_Second_dPhi->GetXaxis()->SetTitle("dPhi");
  
  TH1D* AngleDistribution_Lead_dTheta = new TH1D("AngleDistribution_Lead_dTheta","Lead e dTheta error distribution",
	binsAngle,-M_PI*angleErrorRange,M_PI*angleErrorRange);
  AngleDistribution_Lead_dTheta->GetXaxis()->SetTitle("dTheta");
  TH1D* AngleDistribution_Second_dTheta = new TH1D("AngleDistribution_Second_dTheta","Second e dTheta error distribution",
	binsAngle,-M_PI*angleErrorRange,M_PI*angleErrorRange);
  AngleDistribution_Second_dTheta->GetXaxis()->SetTitle("dTheta");
  
  TH1D* EnergyDistribution_Error_Lead = new TH1D("EnergyDistribution_Error_Lead ","Lead e E error distribution",
	binsEnergy,-dEnergyLimit,dEnergyLimit);
  EnergyDistribution_Error_Lead ->GetXaxis()->SetTitle("dE, GeV");
  TH1D* EnergyDistribution_Error_Second = new TH1D("EnergyDistribution_Error_Second","Second e E error distribution",
	binsEnergy,-dEnergyLimit,dEnergyLimit);
  EnergyDistribution_Error_Second->GetXaxis()->SetTitle("dE, Gev");
  
  TH1D* EnergyDistribution_Lead = new TH1D("EnergyDistribution_Lead ","Lead e E distribution",
	binsEnergy,EnergyLimitMin,EnergyLimitMax);
  EnergyDistribution_Error_Lead ->GetXaxis()->SetTitle("E, GeV");

  double dPhiLead, dThetaLead, dAngleLead, dELead, ELead;
  double dPhiSecond, dThetaSecond, dAngleSecond, dESecond;
  
  Int_t nentries= inputDataTree->GetEntries();
  for (Int_t i=0; i<nentries && i!=maxEntries; i++) {
      //Getting entry
      if (i % outputEvery == 0) {cout << "Processing entry " << i << " for HistogramFillerElectronErrors"<< endl;}
      RestoreData(i);
      //Filling out structures
      
      dPhiLead = deltaPhi (e_4Vec0_MC, e_4Vec0_Reco);
      dPhiSecond = deltaPhi (e_4Vec1_MC, e_4Vec1_Reco);
      dThetaLead = deltaTheta (e_4Vec0_MC, e_4Vec0_Reco);
      dThetaSecond = deltaTheta (e_4Vec1_MC, e_4Vec1_Reco);
      dAngleLead = deltaAngle (e_4Vec0_MC, e_4Vec0_Reco);
      dAngleSecond = deltaAngle (e_4Vec1_MC, e_4Vec1_Reco);
      dELead = e_4Vec0_MC.E() - e_4Vec0_Reco.E();
      dESecond = e_4Vec1_MC.E() - e_4Vec1_Reco.E();
      ELead = e_4Vec0_MC.E();
      
      AngleDistribution_Lead_dPhi_dTheta->Fill(dPhiLead,dThetaLead);
      AngleDistribution_Second_dPhi_dTheta->Fill(dPhiSecond,dThetaSecond);
      
      AngleDistribution_Lead_dPhi->Fill(dPhiLead);
      AngleDistribution_Second_dPhi->Fill(dPhiSecond);
      AngleDistribution_Lead_dTheta->Fill(dThetaLead);
      AngleDistribution_Second_dTheta->Fill(dThetaSecond);
      EnergyDistribution_Error_Lead->Fill(dELead);
      EnergyDistribution_Error_Second->Fill(dESecond);
      EnergyDistribution_Lead->Fill(ELead);
  }
  //Grooming results
  
}

void CovariationFinder () {
  //Initialize nessesary stuff
  double dPhiLead, dThetaLead, dAngleLead;
  double dPhiSecond, dThetaSecond, dAngleSecond;

  //initialization to calculate covariance coefficient?
  double total_dPhiLead=0, total_dThetaLead=0, total_dPhiLead_dThetaLead=0, total_dPhiLead2=0, total_dThetaLead2=0;
  double total_events=0;
  
  Int_t nentries= inputDataTree->GetEntries();
  for (Int_t i=0; i<nentries && i!=maxEntries; i++) {
      //Getting entry
      if (i % outputEvery == 0) {cout << "Processing entry " << i << " for CovariationFinder"<<endl;}
      RestoreData(i);
      //Filling out structures
      dPhiLead = deltaPhi (e_4Vec0_MC, e_4Vec0_Reco);
      dPhiSecond = deltaPhi (e_4Vec1_MC, e_4Vec1_Reco);
      dThetaLead = deltaTheta (e_4Vec0_MC, e_4Vec0_Reco);
      dThetaSecond = deltaTheta (e_4Vec1_MC, e_4Vec1_Reco);
      dAngleLead = deltaAngle (e_4Vec0_MC, e_4Vec0_Reco);
      dAngleSecond = deltaAngle (e_4Vec1_MC, e_4Vec1_Reco);
      
      //covariance adding
      total_events++;
      total_dPhiLead+=dPhiLead;
      total_dPhiLead2+=dPhiLead*dPhiLead;
      total_dThetaLead+=dThetaLead;
      total_dThetaLead2+=dThetaLead*dThetaLead;
      total_dPhiLead_dThetaLead+=dPhiLead*dThetaLead;
  }
  total_dPhiLead/=total_events;
  total_dPhiLead2/=total_events;
  total_dPhiLead_dThetaLead/=total_events;
  total_dThetaLead/=total_events;
  total_dThetaLead2/=total_events;
  double total_dPhiLead_disp2= total_dPhiLead2 - total_dPhiLead * total_dPhiLead;
  double total_dThetaLead_disp2= total_dThetaLead2 - total_dThetaLead * total_dThetaLead;
  cout << "Total events "<< total_events <<endl;
  cout << "Average dPhi" << total_dPhiLead << " dispersion of dPhi "<< sqrt(total_dPhiLead_disp2) << endl;
  cout << "Average dTheta" << total_dThetaLead << " dispersion of dTheta "<< sqrt(total_dThetaLead_disp2) << endl;
  cout << "Covariance coefficient " << ((total_dPhiLead_dThetaLead - total_dPhiLead * total_dThetaLead)/(sqrt(total_dPhiLead_disp2 * total_dThetaLead_disp2))) << endl;

}






void GraphBuilderMC2012_8_teV_Zee () {
  using namespace std;
  cout<<"\nHi!"<<endl;
  OpenFiles();
  //ZPtSeparators ;
  HistogramFillerZptError();
  HistogramFillerCollinsSoper();
  HistogramFillerZPt ();
  HistogramFillerElectronErrors ();
  //CovariationFinder ();
  
  CloseFiles();
  
  //TH1D* ZPtDiff = (TH1D*)ZPtDistributionReco->Clone("ZptDiff");
  //ZPtDiff->Add(ZPtDistributionMC,-1);
  
  //cut0->Fit("gaus");
  
  

}