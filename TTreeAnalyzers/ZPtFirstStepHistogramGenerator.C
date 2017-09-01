#include "ZPtFirstStepZFinderTreeLooper.h"

//Class definition over, we are actually using it for something

/*Generate_MC_Cut_tContaining_ee_Historgrams (){
  Generate_MC_Cut_t_ee_Historgrams ();
  Generate_MC_Cut_bart_ee_Historgrams ();
  Generate_MC_Cut_tbart_ee_Historgrams ();
}

void Generate_MC_Cut_BG_ee_Historgrams () {
  Generate_MC_Cut_ZZ_ee_Historgrams ();
  Generate_MC_Cut_ZW_ee_Historgrams ();
  Generate_MC_Cut_WW_ee_Historgrams ();
  Generate_MC_Cut_Ditau_ee_Historgrams ();
  Generate_MC_Cut_tContaining_ee_Historgrams ();
}*/

//Reminder:
//void ZFinderTreeLooper::RestartClass (std::string inputFileNameVariable, std::string outputFileNameVariable, 
//std::string dataTitleVariable, bool isMCVariable=true, bool recreateFile=false)

void FirstStepGeneratorRunner () {
  using namespace std;
  string combinedOutputFile("/local/cms/user/guts/ZPtDistribution/ZFinderTreesAnalysis/FirstStepZeeHisto.root");

  //MC for the cut signal
  string inputFile_MC_Signal_Z_ee_1 ("/data/whybee0a/user/lesko_2/fermi/Output/Zfinder2MCPDFweightBorn15-7-2/Powheg15-7-04.root"); 
  string title_MC_Signal_Z_ee_Cut_1 ("Z_ee signal MC Cut 1");
  string title_MC_Signal_Z_ee_Uncut_1 ("Z_ee signal MC Uncut 1");//not in use for oral stuff, we go without them
  double crossSection_MC_Signal_Z_ee_total_1=1966.7;//Zach told me

  string treeName_MC_Signal_Z_ee_Uncut ("ZFinder/All Electrons Reco/All Electrons Reco"); 
  string treeName_MC_Signal_Z_ee_Cut ("ZFinder/Combined Single Reco/Combined Single Reco"); 
  
  
  
  string inputFile_MC_Signal_Z_ee_2 ("/data/whybee0a/user/lesko_2/fermi/Output/ZfinderMad15-9-8/MadgraphDressed15-9-8.root"); 
  string title_MC_Signal_Z_ee_Cut_2 ("Z_ee signal MC Cut 2");
  string title_MC_Signal_Z_ee_Uncut_2 ("Z_ee signal MC Uncut 2");//not in use for oral stuff, we go without them
  double crossSection_MC_Signal_Z_ee_total_2=1177.3*3;// 

  
  
  //MC for the cut background
  string inputFile_MC_BG_Tau_ee_Cut ("/data/whybee0a/user/lesko_2/fermi/Output/ZfinderBG15-9-8/BGDitau.root");
  string title_MC_BG_Tau_ee_Cut ("Tau_ee BG MC Cut");
  double crossSection_MC_BG_Tau_ee_total=1966.7;
  string inputFile_MC_BG_WW_ee_Cut ("/data/whybee0a/user/lesko_2/fermi/Output/ZfinderBG15-9-8/BGWW.root");
  string title_MC_BG_WW_ee_Cut ("WW_ee BG MC Cut");
  double crossSection_MC_BG_WW_ee_total=54.84;
  string inputFile_MC_BG_WZ_ee_Cut ("/data/whybee0a/user/lesko_2/fermi/Output/ZfinderBG15-9-8/BGWZ.root");
  string title_MC_BG_WZ_ee_Cut ("WZ_ee BG MC Cut");
  double crossSection_MC_BG_WZ_ee_total=33.21;
  string inputFile_MC_BG_ZZ_ee_Cut ("/data/whybee0a/user/lesko_2/fermi/Output/ZfinderBG15-9-8/BGZZ.root");
  string title_MC_BG_ZZ_ee_Cut ("ZZ_ee BG MC Cut");
  double crossSection_MC_BG_ZZ_ee_total=17.7;
  
  //t-backgrounds
  string inputFile_MC_BG_tbar_ee_Cut ("/data/whybee0a/user/lesko_2/fermi/Output/ZfinderBG15-9-8/BGSingletbar.root");
  string title_MC_BG_tbar_ee_Cut ("tbar_ee BG MC Cut");
  double crossSection_MC_BG_tbar_ee_total=11.1;
  string inputFile_MC_BG_t_ee_Cut ("/data/whybee0a/user/lesko_2/fermi/Output/ZfinderBG15-9-8/BGSinglet.root");
  string title_MC_BG_t_ee_Cut ("t_ee BG MC Cut");
  double crossSection_MC_BG_t_ee_total=11.1;
  string inputFile_MC_BG_ttbar_ee_Cut ("/data/whybee0a/user/lesko_2/fermi/Output/ZfinderBG15-9-8/BGttbar.root");
  string title_MC_BG_ttbar_ee_Cut ("ttbar_ee BG MC Cut");
  double crossSection_MC_BG_ttbar_ee_total=23.64;
  
  
  //Data
  string inputFile_Data_Signal_Z_ee_Cut ("/data/whybee0a/user/lesko_2/fermi/Output/ZfinderoutputData15-9-8/2012ABCD15-9-8.root");
  string title_Data_Signal_Z_ee_Cut ("Zcandidate_ee Signal Data Cut");
  double crossSection_Data_Signal_Z_ee_Cut = 1;
  
  
  ZFinderTreeLooper Runner(inputFile_MC_Signal_Z_ee_1,combinedOutputFile,title_MC_Signal_Z_ee_Cut_1,crossSection_MC_Signal_Z_ee_total_1);
  Runner.RestartClass(inputFile_MC_Signal_Z_ee_2,combinedOutputFile,title_MC_Signal_Z_ee_Cut_2,crossSection_MC_Signal_Z_ee_total_2);
  //Runner.RestartClass(inputFile_MC_Signal_Z_ee_2,combinedOutputFile,title_MC_Signal_Z_ee_Uncut_2,crossSection_MC_Signal_Z_ee_total_2,treeName_MC_Signal_Z_ee_Uncut);
  //Runner.RestartClass(inputFile_MC_Signal_Z_ee_1,combinedOutputFile,title_MC_Signal_Z_ee_Uncut_1,crossSection_MC_Signal_Z_ee_total_1,treeName_MC_Signal_Z_ee_Uncut);
  
  //non-t BGs
  Runner.RestartClass(inputFile_MC_BG_Tau_ee_Cut,combinedOutputFile,title_MC_BG_Tau_ee_Cut,crossSection_MC_BG_Tau_ee_total);
  Runner.RestartClass(inputFile_MC_BG_WW_ee_Cut,combinedOutputFile,title_MC_BG_WW_ee_Cut,crossSection_MC_BG_WW_ee_total);
  Runner.RestartClass(inputFile_MC_BG_WZ_ee_Cut,combinedOutputFile,title_MC_BG_WZ_ee_Cut,crossSection_MC_BG_WZ_ee_total);
  Runner.RestartClass(inputFile_MC_BG_ZZ_ee_Cut,combinedOutputFile,title_MC_BG_ZZ_ee_Cut,crossSection_MC_BG_ZZ_ee_total);
  
  //t BGs
  Runner.RestartClass(inputFile_MC_BG_t_ee_Cut,combinedOutputFile,title_MC_BG_t_ee_Cut,crossSection_MC_BG_t_ee_total);
  Runner.RestartClass(inputFile_MC_BG_tbar_ee_Cut,combinedOutputFile,title_MC_BG_tbar_ee_Cut,crossSection_MC_BG_tbar_ee_total);
  Runner.RestartClass(inputFile_MC_BG_ttbar_ee_Cut,combinedOutputFile,title_MC_BG_ttbar_ee_Cut,crossSection_MC_BG_ttbar_ee_total);
  
  //Data
  Runner.RestartClass(inputFile_Data_Signal_Z_ee_Cut,combinedOutputFile,title_Data_Signal_Z_ee_Cut,crossSection_Data_Signal_Z_ee_Cut,treeName_MC_Signal_Z_ee_Cut,false);
  
}