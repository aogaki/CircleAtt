#include <fstream>

#include <TString.h>


const Int_t kAtt = 75;

void MakeSingleAtt(Int_t plate, Int_t T)
{
   TString fileName = "att" + to_string(plate) + "T" + to_string(T) + ".dat";
   std::ofstream fout(fileName);
   for(Int_t i = 0; i < kAtt; i++){
      if(i == plate) fout << T << endl;
      else fout << 0 << endl;
   }
   fout.close();
}
