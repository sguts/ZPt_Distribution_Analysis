#ifndef _CollinsSopperAngle_H_
#define _CollinsSopperAngle_H_

#include <iostream>
#include "TLorentzVector.h"
#include "TVector3.h"

double CollinsSopperAngle (TLorentzVector electron4Vec, TLorentzVector positron4Vec) {
  
  TLorentzVector Q4Vec = electron4Vec + positron4Vec;
  TLorentzVector Delta4Vec = electron4Vec - positron4Vec;
  TLorentzVector QT4Vec (Q4Vec.Px(),Q4Vec.Py(),0,0);
  TLorentzVector DeltaT4Vec (Delta4Vec.Px(),Delta4Vec.Py(),0,0);
  TLorentzVector protonPlus4Vec (0,0,6500,6500);
  //TVector3 QT3Vec = QT4Vec.Vect();

  //make a /0 exception
  if (protonPlus4Vec.Vect().Cross(Q4Vec.Vect()).Mag()!=0 && QT4Vec.Vect().Mag() != 0) {
    
    TVector3 RHat3Vector = protonPlus4Vec.Vect().Cross(Q4Vec.Vect())*(1/protonPlus4Vec.Vect().Cross(Q4Vec.Vect()).Mag());
    
    TVector3 QTHat3Vector = QT4Vec.Vect()*(1/QT4Vec.Vect().Mag());
    
    return atan2 (sqrt(Q4Vec.Mag2()-QT4Vec.Mag2())*(DeltaT4Vec.Vect()*RHat3Vector),Q4Vec.Mag()*(DeltaT4Vec.Vect()*QTHat3Vector));
    }
  return 4*M_PI;
}

double CollinsSopperAngleAlt (TLorentzVector electron4Vec, TLorentzVector positron4Vec) {
  TLorentzVector Z4Vec = electron4Vec + positron4Vec;
  TLorentzVector protonPlus4Vec (0,0,6500,6500);
  TLorentzVector protonMinus4Vec (0,0,-6500,6500);
  electron4Vec.Boost(-Z4Vec.BoostVector());
  positron4Vec.Boost(-Z4Vec.BoostVector());
  protonPlus4Vec.Boost(-Z4Vec.BoostVector());
  protonMinus4Vec.Boost(-Z4Vec.BoostVector());
  
  TVector3 protonNormPlus3Vec = protonPlus4Vec.Vect()*(1/(protonPlus4Vec.Vect().Mag()));
  TVector3 protonNormMinus3Vec = protonMinus4Vec.Vect()*(1/(protonMinus4Vec.Vect().Mag()));
  
  TVector3 normXAxis = (protonPlus4Vec+protonMinus4Vec).Vect()*(1/((protonPlus4Vec+protonMinus4Vec).Vect().Mag()));
  TVector3 normZAxis = (protonPlus4Vec-protonMinus4Vec).Vect()*(1/((protonPlus4Vec-protonMinus4Vec).Vect().Mag()));
  TVector3 normYAxis = normZAxis.Cross(normXAxis);
  
  TVector3 normElectron = electron4Vec.Vect()*(1/(electron4Vec.Vect().Mag()));
  
  return atan2(normElectron*normYAxis,normElectron*normXAxis);
  
  //return 0;
}


#endif