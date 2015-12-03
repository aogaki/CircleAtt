const Int_t kAtt = 75;

void MakeAtt(Int_t plate, Int_t T)
{
   ofstream fout("att.dat");
   for(Int_t i = 0; i < kAtt; i++){
      if(i == plate) fout << T << endl;
      else fout << 0 << endl;
   }
   fout.close();
}
