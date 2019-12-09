/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAFooAna                                                             //
//                                                                      //
// Global definitions.                                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAFooAna
#define FooAna_FAFooAna

#include "Math/GenVector/LorentzVector.h"
#include "Rtypes.h"

# define FOOANA_VERSION "0.4.0beta"

// typedef
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<Double32_t>> FAVector4;
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<Double32_t>,
                                         ROOT::Math::DefaultCoordinateSystemTag> FAVector3;

#endif

