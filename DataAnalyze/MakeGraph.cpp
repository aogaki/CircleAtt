#include <iostream>

#include <TFile.h>
#include <TH1.h>
#include <TString.h>
#include <TGraph.h>


const Int_t kData = 3750;


void MakeGraph(Int_t distance = 100)
{
   TString fileName = Form("Data/distance%03d.root", distance);
   TFile *file = new TFile(fileName, "OPEN");

   Double_t x[kData];
   Double_t y[kData];
   
   for (Int_t i = 0; i < kData; i++) {
      TString hisName = Form("HisDose%04d", i + 1);
      TH1D *His = (TH1D *)file->Get(hisName);
      y[i] = His->GetMean();
      x[i] = (i / 50.);
   }

   TString outName = Form("graph%03d.root", distance);
   TFile *output = new TFile(outName, "RECREATE");
   TGraph *gr = new TGraph(kData, x, y);
   gr->SetNameTitle("all", "Dose");
   gr->Write();
   delete gr;
   
   Double_t smallX[50];
   for(Int_t i = 0; i < 50; i++) smallX[i] = i * 100 + 100;
   for(Int_t i = 0; i < 75; i++){
      TGraph *gr = new TGraph(50, smallX, &y[i * 50]);
      gr->SetNameTitle(Form("distance%02d", i), "Dose");
      gr->Write();
      delete gr;
   }
   output->Close();
}

