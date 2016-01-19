#include <iostream>
#include <limits>

#include <TFile.h>
#include <TGraph.h>
#include <TString.h>
#include <TH1.h>
#include <TMath.h>


const TString kDir = "Plate/Dose/Graph";

class TMeanGraph
{
public:
   TMeanGraph(Int_t index);
   virtual ~TMeanGraph();

   TGraph *GetGraph(Int_t grNo);

private:
   TGraph *fGraph1;
   TGraph *fGraph2;
   TGraph *fGraph3;
   void MakeGraph(Int_t index);
};

TMeanGraph::TMeanGraph(Int_t index)
   :fGraph1(nullptr),
    fGraph2(nullptr),
    fGraph3(nullptr)
{
   MakeGraph(index);
}

TMeanGraph::~TMeanGraph()
{
   delete fGraph1;
   delete fGraph2;
   delete fGraph3;
}

TGraph *TMeanGraph::GetGraph(Int_t index)
{
   if(index == 0) return fGraph1;
   else if(index == 1) return fGraph2;
   else if(index == 2) return fGraph3;
   else{
      cout << "error in GetGraph" << endl;
      exit(1);
   }
}

void TMeanGraph::MakeGraph(Int_t index)// Too much index* use other name
{
   Double_t y1[50] = {0};
   Double_t y2[50] = {0};
   Double_t y3[50] = {0};
   Double_t x[50] = {0};
   for(Int_t i = 0; i < 50; i++) x[i] = 100 + i * 100;
   
   TString fileName = Form("graph%03d.root", index);
   TFile *file = new TFile(kDir + "/" + fileName, "OPEN");
   Int_t index1 = (index / 10) - 1;
   Int_t index2 = index1 + 1;
   Int_t index3 = index2 + 1;
   if(index1 < 0) index1 = 0;
   
   TGraph *gr1 = (TGraph*)file->Get(Form("distance%02d", index1));
   TGraph *gr2 = (TGraph*)file->Get(Form("distance%02d", index2));
   TGraph *gr3 = (TGraph*)file->Get(Form("distance%02d", index3));
   
   for(Int_t iData = 0; iData < 50; iData++){
      y1[iData] = gr1->GetY()[iData];
      y2[iData] = gr2->GetY()[iData];
      y3[iData] = gr3->GetY()[iData];
   }
      
   delete gr3;
   delete gr2;
   delete gr1;
   
   file->Close();
   
   //fGraph1 = new TGraph(50, x, y1);
   //fGraph2 = new TGraph(50, x, y2);
   
   fGraph1 = new TGraph(50, y1, x);
   fGraph2 = new TGraph(50, y2, x);
   fGraph3 = new TGraph(50, y3, x);
}


TMeanGraph *meanGraph[650];

void test()
{
   Int_t att[75][2] = {{0}};
   att[74][1] = 5000;// OK?
   
   //for(Int_t i = 1; i < 65; i++){
   for(Int_t i = 10; i < 650; i++){
      Int_t T = 0;
      meanGraph[i] = new TMeanGraph(i);
      TGraph *gr[3];
      gr[0] = meanGraph[i]->GetGraph(0);
      gr[1] = meanGraph[i]->GetGraph(1);
      gr[2] = meanGraph[i]->GetGraph(2);
      
      Int_t useGraph = 0;
      if(TMath::MinElement(gr[0]->GetN(), gr[0]->GetX()) > 0.1){
         useGraph = 1;
         if(TMath::MinElement(gr[0]->GetN(), gr[1]->GetX()) > 0.1){
            useGraph = 2;
         }
      }
      
      T = (Int_t)gr[useGraph]->Eval(0.1);
      if(T > 5000) T = 5000;
      else if(T < 0) T = 0;

      cout << i <<"\t"
           << useGraph <<"\t"
           << gr[0]->Eval(0.1) <<"\t"
           << gr[1]->Eval(0.1) <<"\t"
           << gr[2]->Eval(0.1) << endl;
      if(useGraph == 1){
         Int_t iAtt = i / 10;
         cout << iAtt << endl;
         att[74 - iAtt][1] += T;
         att[74 - iAtt][0]++;
      }
   }

   for(Int_t i = 0; i < 75; i++){
      if(att[i][0] > 0)
         cout << att[i][1] / att[i][0] << endl;
      else
         cout << att[i][1] << endl;
   }
}

