#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include "g4root.hh"

#include "BIRunAction.hpp"


BIRunAction::BIRunAction(G4String name)
   : G4UserRunAction()
{
   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->SetVerboseLevel(1);
    
   G4double cellR = 6.39 / 2.;
   G4double cellPitch = 9.;
   G4double cellL = cellR * 2 + cellPitch * 11;
   G4double cellW = cellR * 2 + cellPitch * 7;
   //G4int nStep = G4int(10 * sqrt(cellL*cellL + cellW*cellW) / 2) + 1;

   anaMan->CreateH2("HisPlate", "Deposited Energy",
                    G4int(cellL * 50), -cellL / 2., cellL / 2,
                    G4int(cellW * 50), -cellW / 2., cellW / 2);

   fOutName = name;
}

BIRunAction::~BIRunAction()
{
   delete G4AnalysisManager::Instance();
}

void BIRunAction::BeginOfRunAction(const G4Run *)
{
    G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
    anaMan->OpenFile(fOutName);
}

void BIRunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
    anaMan->Write();
    anaMan->CloseFile();
}

