#ifndef BIActionInitialization_h
#define BIActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"


class BIActionInitialization : public G4VUserActionInitialization
{
public:
   BIActionInitialization(G4String);
   virtual ~BIActionInitialization();

   virtual void BuildForMaster() const;
   virtual void Build() const;

private:
   G4String fOutName;
};

#endif
