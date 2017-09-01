#ifndef _AngularErrors_H_
#define _AngularErrors_H_

#include <iostream>
#include "TLorentzVector.h"
#include "TVector3.h"

double deltaPhi (TLorentzVector MC4Vec, TLorentzVector RECO4Vec) {
  return MC4Vec.Phi()-RECO4Vec.Phi();
}

double deltaTheta (TLorentzVector MC4Vec, TLorentzVector RECO4Vec) {
  return MC4Vec.Theta()-RECO4Vec.Theta();
}

double deltaAngle (TLorentzVector MC4Vec, TLorentzVector RECO4Vec) {
  return MC4Vec.Angle(RECO4Vec.Vect());
}

#endif