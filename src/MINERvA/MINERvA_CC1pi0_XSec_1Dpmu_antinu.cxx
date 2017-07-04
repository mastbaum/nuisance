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

#include "MINERvA_SignalDef.h"

#include "MINERvA_CC1pi0_XSec_1Dpmu_antinu.h"


//********************************************************************
MINERvA_CC1pi0_XSec_1Dpmu_antinu::MINERvA_CC1pi0_XSec_1Dpmu_antinu(nuiskey samplekey) {
//********************************************************************

  // Sample overview ---------------------------------------------------
  std::string descrip = "MINERvA_CC1pi0_XSec_1Dpmu_antinu sample. \n" \
                        "Target: CH \n" \
                        "Flux: MINERvA Forward Horn Current numubarr \n" \
                        "Signal: Any event with 1 muon, 1 pion, no other tracks \n";

  // Setup common settings
  fSettings = LoadSampleSettings(samplekey);
  fSettings.SetDescription(descrip);
  fSettings.SetXTitle("p_{#mu} (GeV)");
  fSettings.SetYTitle("d#sigma/dp_{#mu} (cm^{2}/GeV/nucleon)");
  fSettings.SetAllowedTypes("FIX,FREE,SHAPE/DIAG/NORM/MASK", "FIX/DIAG");
  fSettings.SetEnuRange(1.5, 10.0);
  fSettings.DefineAllowedTargets("C,H");

  // CCQELike plot information
  fSettings.SetTitle("MINERvA_CC1pi0_XSec_1Dpmu_antinu");
  fSettings.DefineAllowedSpecies("numu");
  FinaliseSampleSettings();

  // Scaling Setup ---------------------------------------------------
  // ScaleFactor automatically setup for DiffXSec/cm2/Nucleon
  fScaleFactor =  GetEventHistogram()->Integral("width")*double(1E-38)/double(fNEvents)/TotalIntegratedFlux("width");

  // Plot Setup -------------------------------------------------------
  SetDataFromTextFile(GeneralUtils::GetTopLevelDir() + "/data/MINERvA/CC1pi0/2016/anu-cc1pi0-xsec-muon-momentum.csv");
  for (int i = 0; i < fDataHist->GetNbinsX()+1; i++) {
    fDataHist->SetBinError(i+1, fDataHist->GetBinContent(i+1)*fDataHist->GetBinError(i+1)/100.);
  }

  SetCorrelationFromTextFile(GeneralUtils::GetTopLevelDir()+"/data/MINERvA/CC1pi0/2016/anu-cc1pi0-correlation-muon-momentum.csv");

  // Final setup  ---------------------------------------------------
  FinaliseMeasurement();

};


void MINERvA_CC1pi0_XSec_1Dpmu_antinu::FillEventVariables(FitEvent *event) {

  if (event->NumFSParticle(-13) == 0) return;

  TLorentzVector Pnu  = event->GetNeutrinoIn()->fP;
  TLorentzVector Pmu  = event->GetHMFSParticle(-13)->fP;

  double hadMass = FitUtils::Wrec(Pnu, Pmu);
  double pmu     = -999;

  if (hadMass < 1800)
    pmu = FitUtils::p(Pmu);

  fXVar = pmu;

  return;
};

//********************************************************************
bool MINERvA_CC1pi0_XSec_1Dpmu_antinu::isSignal(FitEvent *event) {
//********************************************************************
  return SignalDef::isCC1pi0_MINERvA_2016(event, EnuMin, EnuMax);
}
