const Int_t kAtt = 75;

void MakeAtt(Int_t plate = 0, Int_t T = 0)
{
   ofstream fout("att.dat");
   for(Int_t i = 0; i < kAtt - plate; i++)
      fout << 0 << endl;
   for(Int_t i = 0; i < plate; i++)
      fout << T << endl;
   fout.close();
}
