#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"
#include "g4root.hh"

#include "BIEventAction.hpp"
#include "BICommonHit.hpp"


BIEventAction::BIEventAction()
   : G4UserEventAction(),
     fHitsCollectionID(-1)
{
   // set printing per each event
   //G4RunManager::GetRunManager()->SetPrintProgress(1);
}

BIEventAction::~BIEventAction()
{}

void BIEventAction::BeginOfEventAction(const G4Event *)
{
    if (fHitsCollectionID == -1) {
       G4SDManager *manager = G4SDManager::GetSDMpointer();
       fHitsCollectionID = manager->GetCollectionID("Common/CommonHitsCollection");
       if (fHitsCollectionID == -1) {
          G4cout << "Common/CommonHitsCollection not found" << G4endl;
          exit(0);
       }
    }
}

void BIEventAction::EndOfEventAction(const G4Event *event)
{
   G4HCofThisEvent *hce = event->GetHCofThisEvent();
   if (!hce) {
      G4cout << "HCE not found: EndOfEventAction@BIEventAction" << G4endl;
      exit(0);
   }
   BICommonHitsCollection *hitsCollection =
      static_cast<BICommonHitsCollection *>(hce->GetHC(fHitsCollectionID));
   if (!hitsCollection) {
      G4cout << "HC not found: EndOfEventAction@BIEventAction" << G4endl;
      exit(0);
   }

   //G4int eventID = event->GetEventID();

   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();

   G4double cellPitch = 9.*mm;
   
   const G4int kHit = hitsCollection->entries();
   for (G4int iHit = 0; iHit < kHit; iHit++) {
      BICommonHit *newHit = (*hitsCollection)[iHit];

      G4double depositEnergy = newHit->GetDepositEnergy();

      G4ThreeVector position = newHit->GetPosition();

      G4int xPos = G4int((position.x() + 6*cellPitch) / cellPitch);
      G4int yPos = G4int((position.y() + 4*cellPitch) / cellPitch);
      
      anaMan->FillH2(0, xPos, yPos, depositEnergy);
      
   }

}

