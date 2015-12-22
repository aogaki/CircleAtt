#include <string>
#include <fstream>

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4CutTubs.hh"
#include "G4GenericMessenger.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4UImanager.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

#include "BIDetectorConstruction.hpp"
#include "BICommonSD.hpp"


BIDetectorConstruction::BIDetectorConstruction(G4String name)
   : G4VUserDetectorConstruction(),
     fWorldLV(nullptr),
     fWindowPV(nullptr),
     fCassettePV(nullptr),
     fAirGapPV(nullptr),
     fPlatePV(nullptr),
     fCellPV(nullptr),
     fWaterPV(nullptr),
     fVacuum(nullptr),
     fAir(nullptr),
     fWindowMat(nullptr),
     fCassetteMat(nullptr),
     fPlateMat(nullptr),
     fCellMat(nullptr),
     fAttMat(nullptr),
     fWaterMat(nullptr)
{
   fCheckOverlap = true;
   fInName = name;
   
   ReadAttData();
   for(G4int i = 0; i < kAtt; i++) fAttPV[i] = nullptr;
   
   DefineMaterial();
   DefineGeoPar();
   DefineCommands();
}

BIDetectorConstruction::~BIDetectorConstruction()
{}

void BIDetectorConstruction::DefineGeoPar()
{
   // Geometry parameters
   fPlateL = 125.*mm;
   fPlateW = 82.4*mm;
   fPlateT = 1.*mm;
   
   fWindowL = fPlateL;
   fWindowW = fPlateW;
   fWindowT = 1.*mm;

   fCassetteL = 163.6*mm; // temporary
   fCassetteW = 118.4*mm; // temporary
   fCassetteH = 30.*mm; // temporary
   fCassetteBottomT = 3.*mm; // temporary

   fCellR = 6.39*mm / 2.;
   fCellPitch = 9.*mm;
   fCellL = fCellR * 2 + fCellPitch * 11;
   fCellW = fCellR * 2 + fCellPitch * 7;
   fCellT = 20.*um;

   fWaterL = fCellL;
   fWaterW = fCellW;
   fWaterT = 10.9*mm - fCellT;

   fAirGapL = fPlateL;
   fAirGapW = fPlateW;
   //fAirGapT = 5.*mm;
   fAirGapT = (2 * 5.*mm) + fWaterT + fCellT + fPlateT;
}

void BIDetectorConstruction::DefineMaterial()
{
   G4NistManager *manager = G4NistManager::Instance();

   // NIST database materials
   fVacuum = manager->FindOrBuildMaterial("G4_Galactic");
   fAir = manager->FindOrBuildMaterial("G4_AIR");
   //fWindowMat = manager->FindOrBuildMaterial("G4_Al");
   fCassetteMat = manager->FindOrBuildMaterial("G4_Al");
   fWindowMat = manager->FindOrBuildMaterial("G4_Ni");
   fPlateMat = manager->FindOrBuildMaterial("G4_POLYSTYRENE");
   fCellMat = manager->BuildMaterialWithNewDensity("G4_WATER_MODIFIED","G4_WATER",
                                                   1.1*g/cm/cm/cm);
   fAttMat = fWindowMat;
   fWaterMat = manager->FindOrBuildMaterial("G4_WATER");
}

G4VPhysicalVolume *BIDetectorConstruction::Construct()
{
   // world volume
   G4double worldDx = 2.*m;
   G4double worldDy = 2.*m;
   G4double worldDz = 2.*m;

   G4Box *worldS = new G4Box("World", worldDx / 2., worldDy / 2., worldDz / 2.);
   fWorldLV = new G4LogicalVolume(worldS, fVacuum, "World");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   fWorldLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4VPhysicalVolume *worldPV
      = new G4PVPlacement(nullptr, G4ThreeVector(), fWorldLV, "World", 0,
                          false, 0, fCheckOverlap);


   G4LogicalVolume *windowLV = ConstructWindow();
   G4double windowZ = 0.*mm;
   G4ThreeVector windowPos = G4ThreeVector(0., 0., windowZ);
   fWindowPV = new G4PVPlacement(nullptr, windowPos, windowLV, "Window", fWorldLV,
                                 false, 0, fCheckOverlap);

   G4LogicalVolume *airLV = ConstructAirGap();
   G4double airZ = (fAirGapT + fWindowT) / 2.;
   G4ThreeVector airPos = G4ThreeVector(0., 0., airZ);
   fAirGapPV = new G4PVPlacement(nullptr, airPos, airLV, "Air", fWorldLV,
   false, 0, fCheckOverlap);

   G4LogicalVolume *cassetteLV = ConstructCassette();
   G4double cassetteZ = (fAirGapT + fWindowT / 2.) - fCassetteH / 2. + fCassetteBottomT;
   G4ThreeVector cassettePos = G4ThreeVector(0., 0., cassetteZ);
   fCassettePV = new G4PVPlacement(nullptr, cassettePos, cassetteLV, "Cassette", fWorldLV,
   false, 0, fCheckOverlap);



   G4LogicalVolume *attLV[kAtt];
   G4double attR = 75.*mm;
   G4double attOffset = -fAirGapT / 2.;
   for(G4int i = 0; i < kAtt; i++){
      if(fAttT[i] > 0.){
         attLV[i] = ConstructAtt(attR, fAttT[i]);
         G4double zPos = attOffset + fAttT[i] / 2.;
         G4ThreeVector attPos = G4ThreeVector(0., 0., zPos);
         fAttPV[i] = new G4PVPlacement(nullptr, attPos, attLV[i], "Att", airLV,
                                       false, i, fCheckOverlap);
      }
      attR -= 1.*mm;
   }

   G4LogicalVolume *plateLV = ConstructPlate();
   G4double plateZ = 5.*mm - (fAirGapT - fPlateT) / 2.;
   G4ThreeVector platePos = G4ThreeVector(0., 0., plateZ);
   fPlatePV = new G4PVPlacement(nullptr, platePos, plateLV, "Plate", airLV,
                                false, 0, fCheckOverlap);

   G4LogicalVolume *cellLV = ConstructCell();
   G4double cellZ = plateZ + (fPlateT + fCellT) / 2.;
   G4ThreeVector cellPos = G4ThreeVector(0., 0., cellZ);
   fCellPV = new G4PVPlacement(nullptr, cellPos, cellLV, "Cell", airLV,
                               false, 0, fCheckOverlap);

   G4LogicalVolume *waterLV = ConstructWater();
   G4double waterZ = cellZ + (fCellT + fWaterT) / 2.;
   G4ThreeVector waterPos = G4ThreeVector(0., 0., waterZ);
   fWaterPV = new G4PVPlacement(nullptr, waterPos, waterLV, "Water", airLV,
                                false, 0, fCheckOverlap);

   return worldPV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructCassette()
{
   G4Box *cassetteS = new G4Box("Cassette", fCassetteL / 2., fCassetteW / 2., fCassetteH / 2.);
   G4Box *openS = new G4Box("Open", fPlateL / 2., fPlateW / 2., fCassetteH / 2.);
   G4ThreeVector openPos(0., 0., -3.*mm);
   G4SubtractionSolid *subS = new G4SubtractionSolid("Cassette", cassetteS, openS, nullptr, openPos);

   G4LogicalVolume *cassetteLV = new G4LogicalVolume(subS, fCassetteMat, "Cassette");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Yellow());
   visAttributes->SetVisibility(true);
   cassetteLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return cassetteLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructPlate()
{
   G4Box *plateS = new G4Box("Plate", fPlateL / 2., fPlateW / 2., fPlateT / 2.);
   G4LogicalVolume *plateLV = new G4LogicalVolume(plateS, fPlateMat, "Plate");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Cyan());
   visAttributes->SetVisibility(true);
   plateLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return plateLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructWindow()
{
   G4Box *windowS = new G4Box("Window", fWindowL / 2., fWindowW / 2., fWindowT / 2.);
   G4LogicalVolume *windowLV = new G4LogicalVolume(windowS, fWindowMat, "Window");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Red());
   visAttributes->SetVisibility(true);
   windowLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return windowLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructAirGap()
{
   G4Box *airGapS = new G4Box("AirGap", fAirGapL / 2., fAirGapW / 2., fAirGapT / 2.);
   G4LogicalVolume *airGapLV = new G4LogicalVolume(airGapS, fAir, "AirGap");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(true);
   airGapLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return airGapLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructAtt(G4double R, G4double T)
{
   G4LogicalVolume *attLV;
   
   G4Tubs *attS = new G4Tubs("layer", R - 1.*mm, R, T / 2., 0., 360.*deg);
   
   if(R > fAirGapW / 2.){
      G4Box *airS = new G4Box("Air", fAirGapL / 2., fAirGapW / 2., fAirGapT);
      G4IntersectionSolid *intS = new G4IntersectionSolid("Att", attS, airS);
      attLV = new G4LogicalVolume(intS, fAttMat, "Att");
   }
   else attLV = new G4LogicalVolume(attS, fAttMat, "Att");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Green());
   visAttributes->SetVisibility(true);
   attLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return attLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructCell()
{
   G4Box *cellS = new G4Box("Cell", fCellL / 2., fCellW / 2., fCellT / 2.);
   G4LogicalVolume *cellLV = new G4LogicalVolume(cellS, fCellMat, "Cell");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Magenta());
   visAttributes->SetVisibility(true);
   cellLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return cellLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructWater()
{
   G4Box *cellS = new G4Box("Water", fWaterL / 2., fWaterW / 2., fWaterT / 2.);
   G4LogicalVolume *cellLV = new G4LogicalVolume(cellS, fWaterMat, "Water");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Blue());
   visAttributes->SetVisibility(true);
   cellLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return cellLV;
}

void BIDetectorConstruction::ConstructSDandField()
{
   G4VSensitiveDetector *CommonSD = new BICommonSD("Common",
                                                   "CommonHitsCollection");

   G4LogicalVolumeStore *lvStore = G4LogicalVolumeStore::GetInstance();
   std::vector<G4LogicalVolume*>::const_iterator it;
   for(it = lvStore->begin(); it != lvStore->end(); it++){
      if((*it)->GetName() == "Cell")
         SetSensitiveDetector((*it)->GetName(), CommonSD);
   }
}

void BIDetectorConstruction::DefineCommands()
{
   fMessenger = new G4GenericMessenger(this, "/BI/Geometry/", 
                                       "For geometries");

   
   G4GenericMessenger::Command &windowTCmd
      = fMessenger->DeclareMethodWithUnit("WindowThickness", "um",
                                          &BIDetectorConstruction::SetWindowT, 
                                          "Set the thickness of the window.");
   windowTCmd.SetParameterName("thickness", true);
   windowTCmd.SetRange("thickness>=0. && thickness<=10000.");
   windowTCmd.SetDefaultValue("500.0");

   G4GenericMessenger::Command &windowMatCmd
      = fMessenger->DeclareMethod("WindowMaterial",
                                  &BIDetectorConstruction::SetWindowMat, 
                                  "Set the material of the window.");
   windowMatCmd.SetParameterName("matName", true);

   G4GenericMessenger::Command &attMatCmd
      = fMessenger->DeclareMethod("AttMaterial",
                                  &BIDetectorConstruction::SetAttMat, 
                                  "Set the material of the att.");
   attMatCmd.SetParameterName("matName", true);
}

void BIDetectorConstruction::SetWindowT(G4double t)
{
   G4cout << t << G4endl;
   fWindowT = t;
   G4Box *window = (G4Box*)(fWindowPV->GetLogicalVolume()->GetSolid());
   window->SetZHalfLength(fWindowT / 2.);
   
   G4ThreeVector airPos = G4ThreeVector(0., 0., (fWindowT + fAirGapT) / 2.);
   fAirGapPV->SetTranslation(airPos);
   
   G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void BIDetectorConstruction::SetWindowMat(G4String matName)
{
   G4NistManager *manager = G4NistManager::Instance();
   G4Material *mat;
   mat = manager->FindOrBuildMaterial(matName);
   if(mat == nullptr){
      G4cout << matName << " is not a defined material.\n"
             << "Window material is not changed." << G4endl;
      //exit(0);
   }
   else{
      G4LogicalVolume *windowLV = fWindowPV->GetLogicalVolume();
      G4cout << "The material of window is changed from "
             << windowLV->GetMaterial()->GetName()
             << " to " << mat->GetName() <<". "<< G4endl;
      windowLV->SetMaterial(mat);
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
   }
}

void BIDetectorConstruction::SetAttMat(G4String matName)
{
   G4NistManager *manager = G4NistManager::Instance();
   G4Material *mat;
   mat = manager->FindOrBuildMaterial(matName);
   if(mat == nullptr){
      G4cout << matName << " is not a defined material.\n"
             << "Att material is not changed." << G4endl;
      //exit(0);
   }
   else{
      G4LogicalVolume *attLV[kAtt];
      for(G4int i = 0; i < kAtt; i++){
         attLV[i] = fAttPV[i]->GetLogicalVolume();
         G4cout << "The material of att is changed from "
                << attLV[i]->GetMaterial()->GetName()
                << " to " << mat->GetName() <<". "<< G4endl;
         attLV[i]->SetMaterial(mat);
         G4RunManager::GetRunManager()->GeometryHasBeenModified();
      }
   }
}

#ifdef NOTCPP11
#include <sstream>
G4double stol(std::string buf)
{
   std::istringstream is;
   is.str(buf);
   G4double retVal;
   is >> retVal;
   return retVal;
}
#endif

void BIDetectorConstruction::ReadAttData()
{
   std::ifstream fin(fInName);
   if(!fin.is_open()){
      G4cout << "Attenuator data file not found." << G4endl;
      exit(0);
   }

   fAttH = 0.;
   G4int it = 0;
   std::string buf;
   while(1){
      fin >> buf;
      if(it >= kAtt || fin.eof()) break;
      fAttT[it] = stol(buf)*um;
      G4cout << buf <<"\t"<< fAttT[it] << G4endl;
      if(fAttT[it] > fAttH) fAttH = fAttT[it];
      it++;
   }
   
   fin.close();
}
