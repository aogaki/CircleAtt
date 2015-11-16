#include <iomanip>

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "BICommonHit.hpp"


G4ThreadLocal G4Allocator<BICommonHit> *BICommonHitAllocator = 0;


BICommonHit::BICommonHit()
   : G4VHit(),
     fDepositEnergy(0),
     fPosition(0)
{}

BICommonHit::~BICommonHit()
{}

BICommonHit::BICommonHit(const BICommonHit & /*right*/)
   : G4VHit()
{}

const BICommonHit &
BICommonHit::operator=(const BICommonHit & /*right*/)
{
   return *this;
}

int BICommonHit::operator==(const BICommonHit & /*right*/) const
{
   return 0;
}
