#include <iostream>

#include <TFile.h>
#include <TH2.h>

#include "TCylinder.hpp"


TCylinder::TCylinder(TString dir, Int_t index)
   : fDir(dir),
     fIndex(index),
     fHisPlate(nullptr),
     fHis(nullptr)
{
   fHis = new TH2D("HisDose", "test", 1300, 0., 1300., 3000, 0., 30.);
}

TCylinder::~TCylinder()
{
   TFile *file = new TFile(Form("dose%04d.root", fIndex), "RECREATE");
   fHis->Write();
   file->Close();
   delete fHis;
}

void TCylinder::GetData()
{
   TFile *file = new TFile(Form("%s/result%04d.root", fDir.Data(), fIndex), "OPEN");
   fHisPlate = (TH2D *)file->Get("HisPlate");
   cout << fIndex << "\t" << fHisPlate->GetEntries() << endl;
   FillHists();
   delete fHisPlate;
   delete file;
}

void TCylinder::FillHists()
{
   TAxis *xAxis = fHisPlate->GetXaxis();
   TAxis *yAxis = fHisPlate->GetYaxis();
   const Int_t nX = xAxis->GetNbins();
   const Int_t nY = yAxis->GetNbins();
   Int_t xCenter = 0.5 + nX / 2.;
   Int_t yCenter = 0.5 + nY / 2.;

   for (Int_t x = 1; x <= nX; x++) {
      for (Int_t y = 1; y <= nY; y++) {
         Double_t distance = GetDistance(x, y, xCenter, yCenter);
         Double_t dose = GetDose(fHisPlate->GetBinContent(x, y));
         if (dose > 0.) fHis->Fill(distance, dose);
      }
   }

}

Double_t TCylinder::GetDistance(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
   Double_t x = fabs(x1 - x2);
   Double_t y = fabs(y1 - y2);
   Double_t distance = sqrt(x * x + y * y);
   return distance;
}

Double_t TCylinder::GetDose(Double_t depEne)
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
