#include <fstream>

#include <TString.h>
#include <TF1.h>

void MakeGaus()
{
  Int_t i = 1;
  while(1){
    TString fileName = TString::Format("att%04d.dat", i);
    ofstream fout(fileName);

    TF1 *f1 = new TF1("f1", "gaus");
    f1->SetParameters(5500, 0, i / 10.);
    
    for(Int_t iPlate = 0; iPlate < 75; iPlate++){
      Int_t t = f1->Eval(74 - iPlate);
      if(t < 0) t = 0;
      else if(t > 5000) t = 5000;
      fout << t << endl;
    }
    fout.close();
    if(f1->Eval(74) > 5000) break;
    i++;
  }
}
