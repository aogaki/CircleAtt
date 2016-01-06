#include <fstream>

#include <TString.h>


void MakeSingleAtt()
{
  Int_t attNo = 1;

  for(Int_t iPlate = 0; iPlate < 75; iPlate++){
    for(Int_t iT = 100; iT <= 5000; iT += 100){
      TString fileName = TString::Format("att%04d.dat", attNo++);
      ofstream fout(fileName);

      for(Int_t i = 0; i < 75; i++){
	Int_t position = 74 - iPlate; // Think more smart way!
	if(i == position) fout << iT << endl;
	else fout << 0 << endl;
      }
      
      fout.close();
    }
  }

}
