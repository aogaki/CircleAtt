#include "TCylinder.cpp"


void test(Int_t i)
{
   TCylinder *test = new TCylinder("Plate", i);
   test->GetData();
   delete test;
}
