#include "TCylinder.cpp"


void MakeDose(Int_t i = 5)
{
   TCylinder *test = new TCylinder("Pipe", i, 5e+9);
   test->GetData();
   delete test;
}
