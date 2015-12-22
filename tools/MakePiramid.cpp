#include <fstream>

#include <TString.h>

void MakePiramid(Double_t factor = 1.)
{
  Int_t i = 0;
  while(1){
    TString fileName = TString::Format("att%03d.dat", i);
    ofstream fout(fileName);
    const Int_t max = i * 100 * factor;
    for(Int_t iPlate = 0; iPlate < 75; iPlate++){
      Int_t t = max - (74 - iPlate) * 100 * factor;
      if(t < 0) t = 0;
      else if(t > 5000) t = 5000;
      fout << t << endl;
    }
    fout.close();
    if(max - 7400*factor >= 5000) break;
    i++;
  }
}
