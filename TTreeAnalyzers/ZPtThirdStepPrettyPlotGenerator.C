#include "TLorentzVector.h"
#include "TMath.h"
#include <TFile.h>
#include <TStyle.h>
#include <TH1D.h>
#include <TTree.h>
#include <TCanvas.h>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>


void PrettyPlotGenerator () {
	cout << "1" << endl;
	TFile *inputFile = new TFile ("/local/cms/user/guts/ZPtDistribution/ZFinderTreesAnalysis/SecondStepZeeHisto.root","READ");
	TH1D *ZPt_data_raw = (TH1D *) inputFile->Get("ZPt data raw");
	TH1D *ZPt_MC_signal = (TH1D *) inputFile->Get("ZPt MC signal");
	TH1D *ZPt_MC_t_BG = (TH1D *) inputFile->Get("ZPt combined t backgrounds");
	TH1D *ZPt_MC_nont_BG = (TH1D *) inputFile->Get("ZPt combined non-t backgrounds");

	gStyle->SetOptStat(0);
	cout << "2" << endl;
	string canvasNameData = string("DataCanvas");
	string canvasNameMC = string("MCCanvas");
	int canvasX = 750, canvasY =700;
	TCanvas * canvasData = new TCanvas(canvasNameData.c_str(),canvasNameData.c_str(),canvasX,canvasY);
	canvasData->cd();

	cout << "3" << endl;
	ZPt_data_raw->SetLineWidth(3);
	ZPt_data_raw->SetLineColor(1);
	ZPt_data_raw->SetTitle("Data");
	ZPt_data_raw->GetXaxis()->SetTitle("p_T, GeV");
	ZPt_data_raw->GetYaxis()->SetTitle("Differential cross section, pb/GeV");
	ZPt_data_raw->Scale(0.001);
	
	ZPt_data_raw->Draw();

	canvasData->Update();
	canvasData->RedrawAxis();


	canvasData->SaveAs("/local/cms/user/guts/ZPtDistribution/ZFinderTreesAnalysis/PrettyPictures/ZPt_data.png","recreate");
	ZPt_data_raw->Delete();
	canvasData->Close();



	TCanvas * canvasMC = new TCanvas(canvasNameMC.c_str(),canvasNameMC.c_str(),canvasX,canvasY);

	canvasMC->cd();



	ZPt_MC_signal->SetLineWidth(2);
	ZPt_MC_signal->SetLineColor(2);
	ZPt_MC_signal->GetXaxis()->SetTitle("p_T, GeV");
	ZPt_MC_signal->GetYaxis()->SetTitle("nEvents");
	ZPt_MC_signal->SetTitle("MC Signal");
	ZPt_MC_signal->GetYaxis()->SetRangeUser(1, 900000);

	ZPt_MC_signal->Draw();
	

	
	ZPt_MC_t_BG->SetLineWidth(2);
	ZPt_MC_t_BG->SetLineColor(9);
	ZPt_MC_t_BG->SetTitle("t quark backgrounds");

	ZPt_MC_t_BG->Draw("same");

	ZPt_MC_nont_BG->SetLineWidth(2);
	ZPt_MC_nont_BG->SetLineColor(8);
	ZPt_MC_nont_BG->SetTitle("non-quark backgrounds");

	ZPt_MC_nont_BG->Draw("same");
	TLegend *leg = canvasMC->BuildLegend();

	canvasMC->SetLogy();
	canvasMC->Update();
	canvasMC->RedrawAxis();

	canvasMC->SaveAs("/local/cms/user/guts/ZPtDistribution/ZFinderTreesAnalysis/PrettyPictures/ZPt_MC_Signal_BG.png","recreate");
	ZPt_MC_signal->Delete();
	ZPt_MC_t_BG->Delete();
	ZPt_MC_nont_BG->Delete();
	canvasMC->Close();

	cout<<"\nDone!"<<endl;

}
