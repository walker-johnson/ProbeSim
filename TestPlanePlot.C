#include "TFile.h"
#include "TTree.h"

void TestPlanePlot()
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

  //open the file
  char file[50];
  printf("Enter the file name: ");
  scanf("%[^\n]%*c", file);
  TFile* f = new TFile(file);

  //an array to store all of the readers
  TTreeReader *myReaders[8];
  TTreeReader *planeReader = new TTreeReader("PlaneLoc",f);

  //create tree readers
  myReaders[0] = new TTreeReader("nTV1", f);   //neutron test plane n tuples
  myReaders[1] = new TTreeReader("nTV2", f);
  myReaders[2] = new TTreeReader("nTV3", f);
  myReaders[3] = new TTreeReader("nTV4", f);
  myReaders[4] = new TTreeReader("gTV1", f);   //gamma test plane n tuples
  myReaders[5] = new TTreeReader("gTV2", f);
  myReaders[6] = new TTreeReader("gTV3", f);
  myReaders[7] = new TTreeReader("gTV4", f);




  //create an array of canvases
  TCanvas  *c[8];
  c[0] = new TCanvas("3", "TV1 Neutrons");
  c[1] = new TCanvas("4", "TV2 Neutrons");
  c[2] = new TCanvas("5", "TV3 Neutrons");
  c[3] = new TCanvas("6", "TV4 Neutrons");
  c[4] = new TCanvas("7", "TV1 Gammas");
  c[5] = new TCanvas("8", "TV2 Gammas");
  c[6] = new TCanvas("9", "TV3 Gammas");
  c[7] = new TCanvas("10", "TV4 Gammas");


  //create an array to be used for naming the histograms
  
  const char *names[8] =  {"D_{n} on Test Volume 1",
			   "D_{n} on Test Volume 2",
			   "D_{n} on Test Volume 3",
			   "D_{n} on Test Volume 4",
			   "D_{#gamma} on Test Volume 1",
			   "D_{#gamma} on Test Volume 2",
			   "D_{#gamma} on Test Volume 3",
			    "D_{#gamma} on Test Volume 4"};
  	   
  //create arrays for each histogram to contain the various plots
  TH3F *xyz[8];
  TH2F *n[8];
  TH2F *e[8];
  TH2F *s[8];
  TH2F *w[8];
  TH2F *t[8];

  //locations of test volume plane locations {+x, -x, +y, -y, +z, -z}
  double planeLoc[8][6];

  TTreeReaderValue<Double_t> x1(*planeReader, "x1");
  TTreeReaderValue<Double_t> x2(*planeReader, "x2");
  TTreeReaderValue<Double_t> y1(*planeReader, "y1");
  TTreeReaderValue<Double_t> y2(*planeReader, "y2");
  TTreeReaderValue<Double_t> z1(*planeReader, "z1");
  TTreeReaderValue<Double_t> z2(*planeReader, "z2");

  int l = 3;

  while(planeReader->Next()){
    planeLoc[l][0] = *x1;
    planeLoc[l][1] = *x2;
    planeLoc[l][2] = *y1;
    planeLoc[l][3] = *y2;
    planeLoc[l][4] = *z1;
    planeLoc[l][5] = *z2;
    planeLoc[4+l][0] = *x1;
    planeLoc[4+l][1] = *x2;
    planeLoc[4+l][2] = *y1;
    planeLoc[4+l][3] = *y2;
    planeLoc[4+l][4] = *z1;
    planeLoc[4+l][5] = *z2;
    l--;
  }

  double zCenter = planeLoc[0][5];
  double yCenter = (planeLoc[0][2] + planeLoc[0][3])/2;


  for (int i = 0; i <8; i++)
    {
     //create containers
     TTreeReaderValue<Double_t> x(*myReaders[i], "x");
     TTreeReaderValue<Double_t> y(*myReaders[i], "y");
     TTreeReaderValue<Double_t> z(*myReaders[i], "z");

     //a bunch of strings to be used in the histogram naming
     char cxyz[100];
     char xyzn[100];
     std::strcpy(cxyz, names[i]);
     std::strcpy(xyzn, names[i]);

     char cn[100];
     char nn[100];
     std::strcpy(cn, names[i]);
     std::strcpy(nn, names[i]);

     char ce[100];
     char en[100];
     std::strcpy(ce, names[i]);
     std::strcpy(en, names[i]);

     char cs[100];
     char sn[100];
     std::strcpy(cs, names[i]);
     std::strcpy(sn, names[i]);

     char cw[100];
     char wn[100];
     std::strcpy(cw, names[i]);
     std::strcpy(wn, names[i]);

     char ct[100];
     char tn[100];
     std::strcpy(ct, names[i]);
     std::strcpy(tn, names[i]);
     
     //create histograms
     xyz[i] = new TH3F(std::strcat(xyzn, " xyz"), std::strcat(cxyz," in 3D [#muSv/h]"), 100, -5, 5, 100, -5, 5, 100, -5, 5);
     n[i] = new TH2F(std::strcat(nn, " north"), std::strcat(cn, " North Face [#muSv/h]"), 100, -5, 5, 100, -3, 7);
     e[i] = new TH2F(std::strcat(en, " east"), std::strcat(ce, " East Face [#muSv/h]"), 100, -5, 5, 100, -3, 7);
     s[i] = new TH2F(std::strcat(sn, " south"), std::strcat(cs, " South Face [#muSv/h]"), 100, -5, 5, 100, -3, 7);
     w[i] = new TH2F(std::strcat(wn, " west"), std::strcat(cw, " West Face [#muSv/h]"), 100, -5, 5, 100, -3, 7);
     t[i] = new TH2F(std::strcat(tn, " top"), std::strcat(ct, " Top Face [#muSv/h]"), 100, -5, 5, 100, -5, 5);

     while(myReaders[i]->Next()){
       //Get the data from the current TTree entry
       //by getting the value from the connected reader

       //plot all of the data in the 3D histogram
       xyz[i]->Fill(*x,*y-yCenter,*z-zCenter);
       //use the plane location array to cut the data and plot the values
       //at each face
       if( *y == planeLoc[i][2]){
	 n[i]->Fill(*x, *z-zCenter);
       }
       if(*y == planeLoc[i][3]){
	 s[i]->Fill(*x, *z-zCenter);
       }
       if(*x == planeLoc[i][0]){
	 e[i]->Fill(*y-yCenter, *z-zCenter);
       }
       if(*x == planeLoc[i][1]){
	 w[i]->Fill(*y-yCenter, *z-zCenter);
       }
       if(*z == planeLoc[i][4]){
	 t[i]->Fill(*x, *y-yCenter);
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

  double peak; //variable to hold the peak dose rate

  //loop through all of the canvases and plot the respective
  //histograms

  printf("Test volumes are numbered according to their distance from the system (Test Plane 1 = 1 m, Test Plane 2 = 2 m, etc.)\n");
  
  for (int i = 0; i<8; i++){
    
    if(i >3){               //indexes of the gamma histograms
      scale = gammaScale; 
    }else{                          //otherwise use neutron scaling factor
      scale = neutronScale;
    }

    c[i]->Divide(3,3);		   
    c[i]->cd(2);
    n[i]->Scale(scale);
    n[i]->Draw("colz");
    n[i]->GetXaxis()->SetTitle("x [m]");
    n[i]->GetYaxis()->SetTitle("z [m]");
    c[i]->cd(4);
    w[i]->Scale(scale);
    w[i]->Draw("colz");
    w[i]->GetXaxis()->SetTitle("y [m]");
    w[i]->GetYaxis()->SetTitle("z [m]");
    c[i]->cd(5);
    t[i]->Scale(scale);
    t[i]->Draw("colz");
    t[i]->GetXaxis()->SetTitle("x [m]");
    t[i]->GetYaxis()->SetTitle("y [m]");
    c[i]->cd(6);
    e[i]->Scale(scale);
    e[i]->Draw("colz");
    e[i]->GetXaxis()->SetTitle("y [m]");
    e[i]->GetYaxis()->SetTitle("z [m]");
    c[i]->cd(8);
    s[i]->Scale(scale);
    s[i]->Draw("colz");
    s[i]->GetXaxis()->SetTitle("x [m]");
    s[i]->GetYaxis()->SetTitle("z [m]");

    xyz[i]->Scale(scale);

    peak = xyz[i]->GetMaximum();

    printf("Peak %s: %f \n", names[i], peak);
 
 }

}
