#ifndef BIDetectorConstruction_h
#define BIDetectorConstruction_h 1

#include <vector>

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4GenericMessenger.hh"


class BIDetectorConstruction : public G4VUserDetectorConstruction
{
public:
   BIDetectorConstruction();
   virtual ~BIDetectorConstruction();

   virtual G4VPhysicalVolume *Construct();
   virtual void ConstructSDandField();

   // UI Commands
   void SetWindowT(G4double t);
   void SetWindowMat(G4String matName);
   void SetAttT(G4double t);
   void SetAttMat(G4String matName);
   
private:
   G4LogicalVolume *fWorldLV;
   G4bool fCheckOverlap;

   // Commands
   void DefineCommands();
   G4GenericMessenger *fMessenger;
   G4VPhysicalVolume *fWindowPV;
   G4VPhysicalVolume *fAirGapPV;
   G4VPhysicalVolume *fPlatePV;
   G4VPhysicalVolume *fCellPV;
   G4VPhysicalVolume *fAttPV;

   // Materials
   void DefineMaterial();
   G4Material *fVacuum;
   G4Material *fAir;
   G4Material *fWindowMat;
   G4Material *fPlateMat;
   G4Material *fCellMat;
   G4Material *fAttMat;
   
   // Geometries
   // Do I really need all members? too much?
   void DefineGeoPar();
   
   G4LogicalVolume *ConstructPlate();
   G4double fPlateT;
   G4double fPlateL;
   G4double fPlateW;
   
   G4LogicalVolume *ConstructWindow();
   G4double fWindowL;
   G4double fWindowW;
   G4double fWindowT;

   G4LogicalVolume *ConstructAirGap();
   G4double fAirGapL;
   G4double fAirGapW;
   G4double fAirGapT;

   G4LogicalVolume *ConstructAtt();
   G4double fAttL;
   G4double fAttW;
   G4double fAttT;

   G4LogicalVolume *ConstructCell();
   G4double fCellL;
   G4double fCellPitch;
   G4double fCellW;
   G4double fCellR;
   G4double fCellT;

   std::vector<G4VisAttributes *> fVisAttributes;
};

#endif
