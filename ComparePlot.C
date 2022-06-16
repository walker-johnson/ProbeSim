#include "TFile.h"
#include "TTree.h"

/*
This macro takes three root files output from the Monitor_Lab Hadr04 application, 
and compares the dose rates in a one dimensional histogram that intersects the center
of the flux peak. 

The files are hardcoded in the filenames variable, and should be changed to work with 
the necessary files

*/

void ComparePlot()
{
  //Define the style for Histograms
  gStyle->SetHistMinimumZero();
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
  gStyle->SetTitleFont(22, "");
  gStyle->SetTitleSize(.08, "");
  gStyle->SetTitleFont(22, "xyz");;
  gStyle->SetTitleSize(0.05,"xyz");
  gStyle->SetOptStat("");


  //Files to compare go here
  const char * filenames[3] = {"../DATA/30cmBPoly.root",
			       "../DATA/40cmBPoly.root",
			       "../DATA/50cmBPoly.root"};


  //gamma flux histograms
  TH1F *xh[3];
  xh[0] = new TH1F("1D flux", "D_{#gamma} @ PE surface", 40, -2, 2);
  xh[1] = new TH1F("1D flux 2", "Flux Comparison 2", 40, -2, 2);
  xh[2] = new TH1F("1D flux 3", "Flux Comparison 3", 40, -2, 2);

  //neutron flux histograms
  TH1F *xhn[3];
  xhn[0] = new TH1F("1D flux  ", "D_{n} @ PE Surface", 40, -2, 2);
  xhn[1] = new TH1F("1D flux 2  ", "Flux Comparison 2  ", 40, -2, 2);
  xhn[2] = new TH1F("1D flux 3  ", "Flux Comparison 3  ", 40, -2, 2);

    

  //Here we loop through the files in filenames, and fill the histograms
  for(int i = 0 ; i < 3; i++ ){
    TFile* f = new TFile(filenames[i]);
    printf("%s \n", filenames[i]);

    //Create reader objects
    TTreeReader *testReader = new TTreeReader("gPoly", f);
    TTreeReader *testReaderN = new TTreeReader("nPoly", f);
    TTreeReader *planeReader = new TTreeReader("PlaneLoc", f);

    //This variable is used to hold the equations for the planes of the
    //Test volumes, and is read from the TTree
    double planeLoc[8][6];
    int l = 3;

    TTreeReaderValue<Double_t> x1(*planeReader, "x1");
    TTreeReaderValue<Double_t> x2(*planeReader, "x2");
    TTreeReaderValue<Double_t> y1(*planeReader, "y1");
    TTreeReaderValue<Double_t> y2(*planeReader, "y2");
    TTreeReaderValue<Double_t> z1(*planeReader, "z1");
    TTreeReaderValue<Double_t> z2(*planeReader, "z2");
  
    while(planeReader->Next()){
      planeLoc[l][0] = *x1; //TV[i] positive x-plane
      planeLoc[l][1] = *x2; //TV[i] negative y-plane
      planeLoc[l][2] = *y1; //etc
      planeLoc[l][3] = *y2; //etc
      planeLoc[l][4] = *z1; //etc
      planeLoc[l][5] = *z2; //etc
      planeLoc[4+l][0] = *x1; //copies of the pevious values
      planeLoc[4+l][1] = *x2;
      planeLoc[4+l][2] = *y1;
      planeLoc[4+l][3] = *y2;
      planeLoc[4+l][4] = *z1;
      planeLoc[4+l][5] = *z2;
      l--;
    }

    //defines the location of the center of the planes
    //the x center is zero, and is not needed
    double zCenter = planeLoc[0][5];
    double yCenter = (planeLoc[0][2] + planeLoc[0][3])/2;

    //Reader objects to read the positions of neutrons/gammas
    //that have crossed a boundary
    TTreeReaderValue<Double_t> x(*testReader, "x");
    TTreeReaderValue<Double_t> y(*testReader, "y");
    TTreeReaderValue<Double_t> z(*testReader, "z");

    TTreeReaderValue<Double_t> xN(*testReaderN, "x");
    TTreeReaderValue<Double_t> yN(*testReaderN, "y");
    TTreeReaderValue<Double_t> zN(*testReaderN, "z");

    //Filter out only the positions which lie on the top plane of the polyethelyne
    while(testReader->Next()){
      if(*z >= planeLoc[0][4] - 0.526 && *y-yCenter >= -0.3 && *y-yCenter <= -0.2){
	xh[i]->Fill(*x);  
      }
    }

    while(testReaderN->Next()){
      if(*zN >= planeLoc[0][4] - 0.526 && *yN-yCenter >= -0.3 && *yN-yCenter <= -0.2){
	xhn[i]->Fill(*xN);
      }
    }
    
  }



  double B = 420; 
  double perA = .01; // cm^2
  double h_to_s = 3600; // s/h
  double pSv_to_microSv = 0.000001; // uSv/pSv
  double sim_run_time_factor = 50; // 1s/.10s  if we simulate 10E7 neutrons it is like simulating .1s of running time
  
  //scaling factors to convert flux to dose rate in uSv/h
  double gammaScale = sim_run_time_factor*2.2*0.014*.01*3600*1.6*.0001;
  double neutronScale = B*perA*h_to_s*pSv_to_microSv*sim_run_time_factor;
  double scale;

  TCanvas *c = new TCanvas("1", "Test");

  scale = gammaScale;
  
  xh[0]->Scale(scale);
  xh[0]->Draw("HIST");
  xh[0]->GetXaxis()->SetTitle("x [m]");
  xh[0]->GetYaxis()->SetTitle("Dose Rate [#mu Sv/h]");
  xh[1]->SetLineColor(kRed);
  xh[1]->Scale(scale);
  xh[1]->Draw("HIST same");
  xh[2]->SetLineColor(kViolet);
  xh[2]->Scale(scale);
  xh[2]->Draw("HIST same");

  auto legend = new TLegend(0.1, 0.75, 0.4, 0.9);
  legend->AddEntry(xh[0], "30 cm Borated Polyethelyne", "l");
  legend->AddEntry(xh[1], "40 cm Borated Polyethelyne", "l");
  legend->AddEntry(xh[2], "50 cm Borated Polyethelyne", "l");
  legend->Draw();

  TCanvas *c2 = new TCanvas("2", "Test2");

  scale = neutronScale;
  
  xhn[0]->Scale(scale);
  xhn[0]->Draw("HIST");
  xhn[0]->GetXaxis()->SetTitle("x [m]");
  xhn[0]->GetYaxis()->SetTitle("Dose Rate [#mu Sv/h]");
  xhn[1]->SetLineColor(kRed);
  xhn[1]->Scale(scale);
  xhn[1]->Draw("HIST same");
  xhn[2]->SetLineColor(kViolet);
  xhn[2]->Scale(scale);
  xhn[2]->Draw("HIST same");

  auto legend2 = new TLegend(0.1, 0.75, 0.4, 0.9);
  legend2->AddEntry(xhn[0], "30 cm Borated Polyethelyne", "l");
  legend2->AddEntry(xhn[1], "40 cm Borated Polyethelyne", "l");
  legend2->AddEntry(xhn[2], "50 cm Borated Polyethelyne", "l");
  legend2->Draw();

  TH1F *x1 = new TH1F("Dose Rate", "D_{net} 1 m From Shielding Surface", 40, -2, 2);
  x1->Add(xh[0], xhn[0]);
  TH1F *x2 = new TH1F("Dose Rate   ", "D_{net} 50 cm From Shielding Surface  ", 40, -2, 2);
  x2->Add(xh[1], xhn[1]);
  TH1F *x3 = new TH1F("Dose Rate    ", "D_{net} 50 cm From Shielding Surface   ", 40, -2, 2);
  x3->Add(xh[2], xhn[2]);

  TCanvas *c3 = new TCanvas("3", "Test3");

  x1->Draw("HIST");
  x1->GetXaxis()->SetTitle("x [m]");
  x1->GetYaxis()->SetTitle("Dose Rate [#mu Sv/h]");
  x2->SetLineColor(kRed);
  x2->Draw("HIST same");
  x3->SetLineColor(kViolet);
  x3->Draw("HIST same");

  auto legend3 = new TLegend(0.1, 0.75, 0.4, 0.9);
  legend3->AddEntry(x1, "30 cm Borated Polyethelyne", "l");
  legend3->AddEntry(x2, "40 cm Borated Polyethelyne", "l");
  legend3->AddEntry(x3, "50 cm Borated Polyethelyne", "l");
  legend3->Draw();

  printf("30cm DER1 = %f uSv/h\n", xhn[0]->GetMaximum());
  printf("40cm DER1 = %f uSv/h\n", xhn[1]->GetMaximum());
  printf("50cm DER1 = %f uSv/h\n", xhn[2]->GetMaximum());

  printf("30cm DER3 = %f uSv/h\n", xh[0]->GetMaximum());
  printf("40cm DER3 = %f uSv/h\n", xh[1]->GetMaximum());
  printf("50cm DER3 = %f uSv/h\n", xh[2]->GetMaximum());

  printf("30cm DER = %f uSv/h\n", x1->GetMaximum());
  printf("40cm DER = %f uSv/h\n", x2->GetMaximum());
  printf("50cm DER = %f uSv/h\n", x3->GetMaximum());


  TCanvas *c4 = new TCanvas("4", "Test4");

  std::array<Double_t, 3> x = {30, 40, 50};
  std::array<Double_t, 3> y = {xhn[0]->GetMaximum(), xhn[1]->GetMaximum(), xhn[2]->GetMaximum()};

  auto g1 = new TGraph(3, x.data(), y.data());
  g1->SetMarkerStyle(21);
  g1->SetMarkerSize(1);


  std::array<Double_t, 2> limx = {0,55};
  std::array<Double_t, 2> limy = {25,25};
  auto g2 = new TGraph(2, limx.data(), limy.data());
  g2->SetMarkerStyle(1);

  auto mg = new TMultiGraph();
  mg->Add(g1);
  mg->Add(g2);
  mg->SetTitle("DER1 for Varying Poly B Thicknesses");
  mg->GetYaxis()->SetTitle("DER1 (#muSv/h)");
  mg->GetHistogram()->SetMinimum(1.);
  mg->GetHistogram()->SetMaximum(1000000.);
  mg->GetXaxis()->SetTitle("Depth (cm)");
  mg->GetXaxis()->SetLimits(0., 55.);
  mg->Draw("ALP");

  
  c4->SetLogy();


  

  

}
				 
