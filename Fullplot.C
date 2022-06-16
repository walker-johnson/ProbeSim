#include "TFile.h"
#include "TTree.h"

//Running this root macro after executing the GEANT4 macro AnalysisRun.mac will produce plots for seven different
//thicknesses. One plot each for gamma dose rate and neutron dose rate.

void Fullplot()
{

  gStyle->SetHistMinimumZero();
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
  
  //variables used to store the data                                                                                                                                                                         
  Int_t num_trans = 0;
  Int_t gnum_trans = 0;
  //open the file


  TString files[8] = {"noShield.root","10cmLiPoly.root", "15cmLiPoly.root", "20cmLiPoly.root", "25cmLiPoly.root", "30cmLiPoly.root", "35cmLiPoly.root", "40cmLiPoly.root"};
  TString titles[8] = {" (no shielding)", " (10cm LiPoly)", " (15cm LiPoly)", " (20cm LiPoly)", " (25cm LiPoly)", " (30cm LiPoly)", " (35cm LiPoly)", " (40cm LiPoly)"};

  for(Int_t i = 0; i<8; i++){ //go through each root file and produce plots
      
  TFile* f =  new TFile(files[i]);

  //Create the tree reader and its containers
  
  TTreeReader myReader("ntransport", f);
  TTreeReader myGReader("gtransport", f);

  TTreeReaderValue<Double_t> x(myReader, "x");
  TTreeReaderValue<Double_t> y(myReader, "y");
  TTreeReaderValue<Double_t> z(myReader, "z");

  TTreeReaderValue<Double_t> gx(myGReader, "x");
  TTreeReaderValue<Double_t> gy(myGReader, "y");
  TTreeReaderValue<Double_t> gz(myGReader, "z");

  TH3F* xyz = new TH3F("3D", "D_n on Lab Boundary  [#muSv/h]" + titles[i], 100, -5, 5, 100, -5, 5, 100, -5, 5);
  TH2F* nWall = new TH2F("North Wall", "D_n on North Wall [#muSv/h]" + titles[i],  100, -5, 5, 100, -5, 5);
  TH2F* eWall = new TH2F("East Wall", "D_n on East Wall [#muSv/h]" + titles[i],  100, -5, 5, 100, -5, 5);
  TH2F* sWall = new TH2F("South Wall", "D_n on South Wall [#muSv/h]" + titles[i],  100, -5, 5, 100, -5, 5);
  TH2F* wWall = new TH2F("WestWall", "D_n on West Wall [#muSv/h]" + titles[i],  100, -5, 5, 100, -5, 5);
  TH2F* ceiling = new TH2F("Ceiling", "D_n on Ceiling [#muSv/h]" + titles[i],  100, -5, 5, 100, -5, 5);

  TH3F* gxyz = new TH3F("3D Gamma", "D_{#gamma} on Lab Boundary  [#muSv/h]" + titles[i], 100, -5, 5, 100, -5, 5, 100, -5, 5);
  TH2F* gnWall = new TH2F("North Wall Gamma", "D_{#gamma} on North Wall [#muSv/h]" + titles[i],  100, -5, 5, 100, -5, 5);
  TH2F* geWall = new TH2F("East Wall Gamma", "D_{#gamma} on East Wall [#muSv/h]" + titles[i],  100, -5, 5, 100, -5, 5);
  TH2F* gsWall = new TH2F("South Wall Gamma", "D_{#gamma} on South Wall [#muSv/h]" + titles[i],  100, -5, 5, 100, -5, 5);
  TH2F* gwWall = new TH2F("WestWall Gamma", "D_{#gamma} on West Wall [#muSv/h]" + titles[i],  100, -5, 5, 100, -5, 5);
  TH2F* gceiling = new TH2F("Ceiling Gamma", "D_{#gamma} on Ceiling [#muSv/h]" + titles[i],  100, -5, 5, 100, -5, 5);



  while(myReader.Next()){
    //Get the data from the crrent TTree entry by getting
    //the value from the connected reader.
    if(abs(*x) > 3.565 || abs(*y) > 3.665 || *z > .445){
       xyz->Fill(*x,*y, *z);
       num_trans += 1;
    }
    if(*x > 3.565){
      eWall->Fill(*y,*z);
    }
    if(*y > 3.665){
      nWall->Fill(*x,*z);
    }
    if(*x < -3.565){
      wWall->Fill(*y,*z);
    }
    if(*y < -3.665){
      sWall->Fill(*x,*z);
    }
    if(*z > .445){
      ceiling->Fill(*x,*y);
    }
    
      
  }

  while(myGReader.Next()){
    //Get the data from the crrent TTree entry by getting
    //the value from the connected reader.
    if(abs(*gx) > 3.565 || abs(*gy) > 3.665 || *gz > .445){
       gxyz->Fill(*gx,*gy, *gz);
       gnum_trans += 1;
    }
    if(*gx > 3.565){
      geWall->Fill(*gy,*gz);
    }
    if(*gy > 3.665){
      gnWall->Fill(*gx,*gz);
    }
    if(*gx < -3.565){
      gwWall->Fill(*gy,*gz);
    }
    if(*gy < -3.665){
      gsWall->Fill(*gx,*gz);
    }
    if(*gz > .445){
      gceiling->Fill(*gx,*gy);
    }
    
      
  }

  TCanvas *c1=new TCanvas("c1"+titles[i], "Neutron Dose Rate"+titles[i], 1500, 1000);
  c1->Divide(3,2);
  TCanvas *c2=new TCanvas("c2"+titles[i], "Gamma Dose Rate"+titles[i], 1500, 1000);
  c2->Divide(3,2);

  Double_t flux = 0.01; // (counts/cm^2)
  Double_t secPerHr = 3600; // (s/hr)
  Double_t picoToMilli = .000001; // (milliSv/pSv)
  Double_t Neutron_D_factor = flux*secPerHr*420*picoToMilli;
  Double_t Gamma_D_factor = flux*secPerHr*2.2*0.014*1.6*0.0001;

  Double_t peakN = xyz->GetMaximum();
  Double_t peakG = gxyz->GetMaximum();

  printf("Peak Neutron dose rate"+titles[i]+": %f  [miliSv/h]\n",peakN*Neutron_D_factor);
  printf("Peak Gamma dose rate"+titles[i]+": %f  [miliSv/h]\n",peakG*Gamma_D_factor);


  
  nWall->SetStats(0);
  ceiling->SetStats(0);
  eWall->SetStats(0);
  sWall->SetStats(0);
  xyz->SetStats(0);
  wWall->SetStats(0);

  gnWall->SetStats(0);
  gceiling->SetStats(0);
  geWall->SetStats(0);
  gsWall->SetStats(0);
  gxyz->SetStats(0);
  gwWall->SetStats(0);
  
  c1->cd(1);
  nWall->Scale(Neutron_D_factor);
  nWall->Draw("colz");
  c1->cd(2);
  ceiling->Scale(Neutron_D_factor);
  ceiling->Draw("colz");
  c1->cd(3);
  eWall->Scale(Neutron_D_factor);
  eWall->Draw("colz");
  c1->cd(4);
  sWall->Scale(Neutron_D_factor);
  sWall->Draw("colz");
  c1->cd(5);
  xyz->Scale(Neutron_D_factor);
  xyz->Draw("BOX2Z");
  c1->cd(6);
  wWall->Scale(Neutron_D_factor);
  wWall->Draw("colz");

  c2->cd(1);
  gnWall->Scale(Gamma_D_factor);
  gnWall->Draw("colz");
  c2->cd(2);
  gceiling->Scale(Gamma_D_factor);
  gceiling->Draw("colz");
  c2->cd(3);
  geWall->Scale(Gamma_D_factor);
  geWall->Draw("colz");
  c2->cd(4);
  gsWall->Scale(Gamma_D_factor);
  gsWall->Draw("colz");
  c2->cd(5);
  gxyz->Scale(Gamma_D_factor);
  gxyz->Draw("BOX2Z");
  c2->cd(6);
  gwWall->Scale(Gamma_D_factor);
  gwWall->Draw("colz");
    }
	}
