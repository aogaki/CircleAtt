#include <iostream>

#include <TFile.h>
#include <TH1.h>
#include <TString.h>
#include <TGraph.h>


const Int_t kData = 3750;
TGraph *gr;

void MakeGraph()
{
   TFile *file = new TFile("distance050.root", "OPEN");

   Double_t x[kData];
   Double_t y[kData];
   
   for (Int_t i = 0; i < kData; i++) {
      TString hisName = Form("HisDose%04d", i + 1);
      TH1D *His = (TH1D *)file->Get(hisName);
      y[i] = His->GetMean();
      x[i] = i + 1;
   }

   gr = new TGraph(kData, x, y);
   gr->Draw("AL");
}

