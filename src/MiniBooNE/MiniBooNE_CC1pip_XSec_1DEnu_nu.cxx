// Copyright 2016 L. Pickering, P Stowell, R. Terri, C. Wilkinson, C. Wret

/*******************************************************************************
*    This file is part of NUISANCE.
*
*    NUISANCE is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    NUISANCE is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with NUISANCE.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#include "MiniBooNE_CC1pip_XSec_1DEnu_nu.h"


//********************************************************************
MiniBooNE_CC1pip_XSec_1DEnu_nu::MiniBooNE_CC1pip_XSec_1DEnu_nu(nuiskey samplekey) {
//********************************************************************

  // Sample overview ---------------------------------------------------
  std::string descrip = "MiniBooNE_CC1pip_XSec_1DEnu_nu sample. \n" \
                        "Target: CH \n" \
                        "Flux: MiniBooNE Forward Horn Current nue + nuebar \n" \
                        "Signal: Any event with 1 muon, any nucleons, and no other FS particles \n";

  // Setup common settings
  fSettings = LoadSampleSettings(samplekey);
  fSettings.SetDescription(descrip);
  fSettings.SetXTitle("E_{#nu} (MeV)");
  fSettings.SetYTitle("#sigma(E_{#nu}) (cm^{2}/CH_{2})");
  fSettings.SetAllowedTypes("FIX,FREE,SHAPE/DIAG/NORM/MASK", "FIX");
  fSettings.SetEnuRange(0.0, 100.0);
  fSettings.DefineAllowedTargets("C,H");

  // CCQELike plot information
  fSettings.SetTitle("MiniBooNE_CC1pip_XSec_1DEnu_nu");
  fSettings.SetDataInput(  FitPar::GetDataBase() + "MiniBooNE/CC1pip/ccpipXSec_enu.txt" );
  fSettings.DefineAllowedSpecies("numu");

  FinaliseSampleSettings();

  // Scaling Setup ---------------------------------------------------
  // ScaleFactor automatically setup for DiffXSec/cm2/Nucleon
  fScaleFactor = GetEventHistogram()->Integral("width")*double(1E-38)/double(fNEvents)*(14.08);

  // Plot Setup -------------------------------------------------------
  SetDataFromTextFile( fSettings.GetDataInput() );
  SetCovarFromDiagonal();

  // Final setup  ---------------------------------------------------
  FinaliseMeasurement();

};



//********************************************************************
void MiniBooNE_CC1pip_XSec_1DEnu_nu::FillEventVariables(FitEvent* event) {
//********************************************************************

  if (event->NumFSParticle(211) == 0 ||
      event->NumFSParticle(13) == 0)
    return;

  TLorentzVector Pnu  = event->GetNeutrinoIn()->fP;
  TLorentzVector Ppip = event->GetHMFSParticle(211)->fP;
  TLorentzVector Pmu  = event->GetHMFSParticle(13)->fP;

  // No W cut on MiniBooNE data from publication
  // WARNING: DRAGONS LAY HERE! Mike Wilking's thesis might not have this. Beware that the publication says W < 1.35 GeV, but this is "efficiency corrected", i.e. FILLED WITH MONTE-CARLO!!!!!!!! AAAAAH
  //double hadMass = FitUtils::Wrec(Pnu, Pmu, Ppip);

  double Enu = FitUtils::EnuCC1piprec(Pnu, Pmu, Ppip);
  //  if (isRat) Enu = Enu/1000.; //CCpi+/CCQE ratio paper puts in GeV, CCpi+ paper in MeV

  fXVar = Enu;

  return;
};

// *********************************************
// MiniBooNE CC1pi+ signal definition
// Warning: This one is a little scary because there's a W = 1.35 GeV cut for
// signal in the selection
//          Although this is unfolded over and is filled up with NUANCE
//          So there is actually no W cut applied, but everything above W = 1.35
//          GeV is NUANCE...
//
// The signal definition is:
//                            Exactly one negative muon
//                            Exactly one positive pion
//                            No other mesons
//                            No requirements on photons, nucleons and
//                            multi-nucleons
//                            Doesn't mention other leptons
//
// Additionally, it asks for 2 Michel e- from decay of muon and pion
// So there is good purity and we can be fairly sure that the positive pion is a
// positive pion
//
bool MiniBooNE_CC1pip_XSec_1DEnu_nu::isSignal(FitEvent *event) {
  return SignalDef::isCC1pi(event, 14, 211, EnuMin, EnuMax);
}
