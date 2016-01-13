#include <iostream>

#include <TFile.h>
#include <TH2.h>
#include <TString.h>


const TString kDir = "Plate/Dose";
const Int_t kFiles = 3750;

void sortDose(Int_t iPosition)
{
   cout << iPosition << endl;
   TString outName = Form("distance%03d.root", iPosition);
   TFile *outputFile = new TFile(outName, "RECREATE");

   for (Int_t iFile = 1; iFile <= kFiles; iFile++) {
      TString fileName = Form("dose%04d.root", iFile);
      TFile *inputFile = new TFile(kDir + "/" + fileName, "OPEN");
      TH2D *HisDose = (TH2D *)inputFile->Get("HisDose");
      Int_t bin = 1 + iPosition * 2;
      TH1D *HisProjection = HisDose->ProjectionY(Form("HisDose%04d", iFile), bin, bin + 1);
      outputFile->cd();
      HisProjection->Write();
      delete HisProjection;
      delete HisDose;
      inputFile->Close();
   }

   outputFile->Close();
}
/*
  void sortDose()
  {

  for(Int_t iPosition = 0; iPosition < 650; iPosition++){
    cout << iPosition << endl;
    TString outName = Form("distance%03d.root", iPosition);
    TFile *outputFile = new TFile(outName, "RECREATE");

    for(Int_t iFile = 1; iFile <= kFiles; iFile++){
      TString fileName = Form("dose%04d.root", iFile);
      TFile *inputFile = new TFile(kDir + "/" + fileName, "OPEN");
      TH2D *HisDose = (TH2D*)inputFile->Get("HisDose");
      Int_t bin = 1 + iPosition * 2;
      TH1D *HisProjection = HisDose->ProjectionY(Form("HisDose%04d", iFile), bin, bin + 1);
      outputFile->cd();
      HisProjection->Write();
      delete HisProjection;
      delete HisDose;
      inputFile->Close();
    }

    outputFile->Close();
  }

}
*/
