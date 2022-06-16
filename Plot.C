#include "TFile.h"
#include "TTree.h"

void Plot()
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
  TFile* f =  new TFile("BoratedPoly.root");

  //Create the tree reader and its containers                                                                                                                                                              
  TTreeReader myReader("ntransport", f);
  TTreeReader myGReader("gtransport", f);

  TTreeReaderValue<Double_t> x(myReader, "x");
  TTreeReaderValue<Double_t> y(myReader, "y");
  TTreeReaderValue<Double_t> z(myReader, "z");

  TTreeReaderValue<Double_t> gx(myGReader, "x");
  TTreeReaderValue<Double_t> gy(myGReader, "y");
  TTreeReaderValue<Double_t> gz(myGReader, "z");

  TH3F* xyz = new TH3F("3D", "D_{n} on Lab Boundary  [#muSv/h]", 100, -5, 5, 100, -5, 5, 100, -5, 5);
  TH2F* nWall = new TH2F("North Wall", "D_{n} on North Wall [#muSv/h]",  100, -5, 5, 100, -5, 5);
  TH2F* eWall = new TH2F("East Wall", "D_{n} on East Wall [#muSv/h]",  100, -5, 5, 100, -5, 5);
  TH2F* sWall = new TH2F("South Wall", "D_{n} on South Wall [#muSv/h]",  100, -5, 5, 100, -5, 5);
  TH2F* wWall = new TH2F("WestWall", "D_{n} on West Wall [#muSv/h]",  100, -5, 5, 100, -5, 5);
  TH2F* ceiling = new TH2F("Ceiling", "D_{n} on Ceiling [#muSv/h]",  100, -5, 5, 100, -5, 5);

  TH3F* gxyz = new TH3F("3D Gamma", "D_{#gamma} on Lab Boundary  [#muSv/h]", 100, -5, 5, 100, -5, 5, 100, -5, 5);
  TH2F* gnWall = new TH2F("North Wall Gamma", "D_{#gamma} on North Wall [#muSv/h]",  100, -5, 5, 100, -5, 5);
  TH2F* geWall = new TH2F("East Wall Gamma", "D_{#gamma} on East Wall [#muSv/h]",  100, -5, 5, 100, -5, 5);
  TH2F* gsWall = new TH2F("South Wall Gamma", "D_{#gamma} on South Wall [#muSv/h]",  100, -5, 5, 100, -5, 5);
  TH2F* gwWall = new TH2F("WestWall Gamma", "D_{#gamma} on West Wall [#muSv/h]",  100, -5, 5, 100, -5, 5);
  TH2F* gceiling = new TH2F("Ceiling Gamma", "D_{#gamma} on Ceiling [#muSv/h]",  100, -5, 5, 100, -5, 5);



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

  TCanvas *c1=new TCanvas();
  c1->Divide(3,2);
  TCanvas *c2=new TCanvas();
  c2->Divide(3,2);

  Double_t Neutron_D_factor = 10*420*.01*3600*.000001; //B x (divide by 100 sq cm) x (seconds in 1 hr) x (pSv/h ->#muSv/h) 
  Double_t Gamma_D_factor = 10*2.2*.014*.01*3600*1.6*.0001;

  //  printf("Total neutrons transported: %d\n", num_trans);
  //  printf("Total gammas across boundary: %d\n", gnum_trans);

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

  Double_t peak_gamma = gxyz->GetMaximum();
  Double_t peak_neutron = xyz->GetMaximum();

  printf("Peak neutron dose rate: %f miliSv/hr\n", peak_neutron);
  printf("Peak gamma dose rate: %f miliSv/hr\n", peak_gamma);
    
}
