#include <iostream>
#include <math.h>

#include <TFile.h>
#include <TH2.h>
#include <TBrowser.h>
#include <TROOT.h>
#include <TAxis.h>


TH2D *HisPlate;
TH2D *HisDose;

Double_t GetDistance(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
  Double_t x = fabs(x1 - x2);
  Double_t y = fabs(y1 - y2);
  Double_t distance = sqrt(x*x + y*y);
  return distance;
}

Double_t GetDose(Double_t depEne)
{
   depEne *= 1e6; // MeV -> eV.  Only for easy undestanding
   Double_t eneJ = depEne * 1.60217657e-19; // eV -> J

   const Double_t mesh = 0.005; // cm
   const Double_t t = 0.002;
   Double_t V = t * mesh * mesh; // cm3
   const Double_t density = 1.1; // g / cm3
   Double_t mass = V * density; // g
   mass *= 1e-3; // g -> kg

   Double_t dose = eneJ / mass; // Gy = J / kg

   dose *= 1e+2; // 10^10 -> 10^12
   
   return dose;
}

void CheckData(TString fileName = "./reference.root")
{
  TFile *file = new TFile(fileName, "OPEN");
  HisPlate = (TH2D*)file->Get("HisPlate");

  TAxis *xAxis = HisPlate->GetXaxis();
  TAxis *yAxis = HisPlate->GetYaxis();
  const Int_t nX = xAxis->GetNbins();
  const Int_t nY = yAxis->GetNbins();
  Int_t xCenter = 0.5 + nX / 2.;
  Int_t yCenter = 0.5 + nY / 2.;

  cout << xCenter <<"\t"<< xAxis->GetNbins() <<"\n"
       << yCenter <<"\t"<< yAxis->GetNbins() << endl;
  
  HisDose = new TH2D("HisDose", "test", 650, 0., 650., 100000, 0., 10.);

  for(Int_t x = 1; x <= nX; x++){
    for(Int_t y = 1; y <= nY; y++){
      Double_t distance = GetDistance(x, y, xCenter, yCenter);
      if(distance > 2.5){
	Double_t dose = GetDose(HisPlate->GetBinContent(x, y));
	if(dose > 0.) HisDose->Fill(Int_t(distance / 2.), dose);
      }
    }
  }
  
  if(!gROOT->IsBatch()){
    new TBrowser();
    HisDose->Draw("COLZ");
  }

}
