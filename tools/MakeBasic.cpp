#include <fstream>

#include <TString.h>

void MakeBasic()
{
  Int_t attNo = 1;

  for(Int_t iPlate = 0; iPlate < 75; iPlate++){
    for(Int_t iT = 0; iT <= 5000; iT += 1000){
      TString fileName = TString::Format("att%03d.dat", attNo++);
      ofstream fout(fileName);
      
      for(Int_t i = 0; i < 75; i++){
	Int_t position = 74 - iPlate; // Think more smart way!
	if(i == position) {
           if(iT != 0) fout << iT << endl;
           else fout << 100 << endl;
        }
	else if(i < position) fout << 0 << endl;
	else if(i > position) fout << 5000 << endl;
      }
      
      fout.close();
    }
  }

}
