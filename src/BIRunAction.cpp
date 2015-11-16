#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include "g4root.hh"

#include "BIRunAction.hpp"


BIRunAction::BIRunAction()
   : G4UserRunAction()
{
    G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
    anaMan->SetVerboseLevel(1);
    
    anaMan->CreateH2("HisPlate", "Deposited Energy",
                     12, -0.5, 11.5, 8, -0.5, 7.5);
}

BIRunAction::~BIRunAction()
{
   delete G4AnalysisManager::Instance();
}

void BIRunAction::BeginOfRunAction(const G4Run *)
{
    G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
    G4String fileName = "result";
    anaMan->OpenFile(fileName);
}

void BIRunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
    anaMan->Write();
    anaMan->CloseFile();
}

