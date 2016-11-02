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

#include "FNAL_CC1ppip_XSec_1DQ2_nu.h"

// The constructor
FNAL_CC1ppip_XSec_1DQ2_nu::FNAL_CC1ppip_XSec_1DQ2_nu(std::string inputfile, FitWeight *rw, std::string type, std::string fakeDataFile) {
  
  fName = "FNAL_CC1ppip_XSec_1DQ2_nu";
  fPlotTitles = "; Q^{2}_{CC1#pi} (GeV^{2}); d#sigma/dQ^{2} (cm^{2}/GeV^{2}/proton)";
  EnuMin = 10;
  EnuMax = 100;
  fIsDiag = true; // refers to covariance matrix; this measurement has none so only use errors, not covariance
  fNormError = 0.20; // normalisation error on ANL BNL flux
  Measurement1D::SetupMeasurement(inputfile, type, rw, fakeDataFile);

  this->SetDataValues(GeneralUtils::GetTopLevelDir()+"/data/FNAL/CC1pip_on_p/FNAL_cc1ppip_dsigdQ2_W14_edit.txt");
  this->SetupDefaultHist();

  fFullCovar = StatUtils::MakeDiagonalCovarMatrix(fDataHist);
  covar     = StatUtils::GetInvert(fFullCovar);

  this->fScaleFactor = (this->fEventHist->Integral("width")/TotalIntegratedFlux("width"))*double(1E-38)/double(fNEvents)*(16./8.);
};


void FNAL_CC1ppip_XSec_1DQ2_nu::FillEventVariables(FitEvent *event) {
  
  TLorentzVector Pnu  = event->GetNeutrinoIn()->fP;
  TLorentzVector Pp   = event->GetHMFSParticle(2212)->fP;
  TLorentzVector Ppip = event->GetHMFSParticle(211)->fP;
  TLorentzVector Pmu  = event->GetHMFSParticle(13)->fP;

  double hadMass = FitUtils::MpPi(Pp, Ppip);
  double q2CCpip = -1.0;
  
  // FNAL has a M(pi, p) < 1.4 GeV cut imposed only on this channel
  if (hadMass < 1400) q2CCpip = FitUtils::Q2CC1piprec(Pnu, Pmu, Ppip);
  fXVar = q2CCpip;

  return;
};


bool FNAL_CC1ppip_XSec_1DQ2_nu::isSignal(FitEvent *event) {
  return SignalDef::isCC1pi3Prong(event, 14, 211, 2212, EnuMin, EnuMax);
}
