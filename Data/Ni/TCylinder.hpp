#ifndef TCylinder_hpp
#define TCylinder_hpp 1

#include <TH2.h>


class TCylinder {
public:
   TCylinder(TString dir, Int_t index);
   virtual ~TCylinder();

   void GetData();

private:
   TString fDir;
   Int_t fIndex;
   TH2D *fHisPlate;

   TH2D *fHis;

   void FillHists();
   Double_t GetDistance(Double_t x1, Double_t y1, Double_t x2, Double_t y2);
   Double_t GetDose(Double_t depEne);
};

#endif
