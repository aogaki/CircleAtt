#ifndef BICommonHit_h
#define BICommonHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"


class BICommonHit : public G4VHit
{
public:
   BICommonHit();
   virtual ~BICommonHit();
   BICommonHit(const BICommonHit &right);
   const BICommonHit &operator=(const BICommonHit &right);
   int operator==(const BICommonHit &right) const;

   inline void *operator new(size_t);
   inline void  operator delete(void *);

   // add setter/getter methods
   void SetDepositEnergy(G4double ene) {fDepositEnergy = ene;};
   G4double GetDepositEnergy() {return fDepositEnergy;};
   
   void SetPosition(G4ThreeVector pos) {fPosition = pos;};
   G4ThreeVector GetPosition() {return fPosition;};

private:
   G4double fDepositEnergy;
   G4ThreeVector fPosition;
      
};

typedef G4THitsCollection<BICommonHit> BICommonHitsCollection;

extern G4ThreadLocal G4Allocator<BICommonHit> *BICommonHitAllocator;

inline void *BICommonHit::operator new(size_t)
{
   if (!BICommonHitAllocator)
      BICommonHitAllocator = new G4Allocator<BICommonHit>;
   return (void *)BICommonHitAllocator->MallocSingle();
}

inline void BICommonHit::operator delete(void *hit)
{
   BICommonHitAllocator->FreeSingle((BICommonHit *) hit);
}

#endif
