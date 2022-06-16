//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("Hadr04")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);     //enable inactivation of histograms
  
  // Define histograms start values
  const G4int kMaxHisto = 16;
  const G4String id[] = {"0","1", "2", "3", "4", "5", "6", "7", "8","9", "10", "11", "12","13","14","15"};
  const G4String title[] = 
                { "dummy",                                           //0
                  "KE of Neutrons Hitting Detector",                      //1
		  "KE of Gammas - Work Area",
		  "KE of Gammas - Front Door",
		  "KE of Gammas - Side Door",
		  "KE of Gammas - Ceiling",
		  "KE of Neutrons - Work Area",
		  "KE of Neutrons - Front Door",
		  "KE of Neutrons - Side Door",
		  "KE of Neutrons - Ceiling",
		  "KE of Gammas - Beam-side",
		  "KE of Neutrons - Beam-side",
		  "KE of Gammas - Side",
		  "KE of Neutrons - Side",
		  "KE of Gammas - Top",
		  "KE of Neutrons - Top"
                 };  

  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 10.;

  for (G4int k=0; k<kMaxHisto; k++) {
    G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, true);
  }

  // ID=0, neutrons leaving the water tank
  analysisManager->CreateNtuple("nFlux_tank", "Neutrons leaving the water tank"); //id = 0
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("KE");
  analysisManager->FinishNtuple();

    // ID=1, gammas leaving the water tank
  analysisManager->CreateNtuple("gFlux_tank", "Gammas leaving the water tank"); //id = 1
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("E");
  analysisManager->FinishNtuple();

  
  // ID=2, neutrons leaving the concret slab
  analysisManager->CreateNtuple("nFlux_slab", "Neutrons leaving the concrete slab"); //id = 0
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("KE");
  analysisManager->FinishNtuple();

  // ID=3, gammas leaving the water tank
  analysisManager->CreateNtuple("gFlux_slab", "Gammas leaving the concrete slab"); //id = 1
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("E");
  analysisManager->FinishNtuple();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
