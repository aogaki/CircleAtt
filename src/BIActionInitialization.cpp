#include "BIActionInitialization.hpp"
#include "BIPrimaryGeneratorAction.hpp"
#include "BIRunAction.hpp"
#include "BIEventAction.hpp"


BIActionInitialization::BIActionInitialization(G4String name)
   : G4VUserActionInitialization()
{
   fOutName = name;
}

BIActionInitialization::~BIActionInitialization()
{}

void BIActionInitialization::BuildForMaster() const
{
   SetUserAction(new BIRunAction(fOutName));
}

void BIActionInitialization::Build() const
{
   SetUserAction(new BIPrimaryGeneratorAction());
   SetUserAction(new BIRunAction(fOutName));
   SetUserAction(new BIEventAction());
}
