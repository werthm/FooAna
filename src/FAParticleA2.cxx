/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleA2                                                         //
//                                                                      //
// Base class for presorted A2 analysis particles.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMath.h"
#include "TLorentzVector.h"

#include "FAParticleA2.h"

ClassImp(FAParticleA2)

//______________________________________________________________________________
void FAParticleA2::Calculate4Vector(TLorentzVector& p4, Double_t mass) const
{
    // Calculate the 4-vector of this particle using 'mass' as the particle's
    // mass and the detected energy and angle information. The components of
    // the 4-vector will be stored in 'p4'.

    Calculate4Vector(theta, phi, energy, mass, p4);
}

//______________________________________________________________________________
void FAParticleA2::Calculate4Vector(Double_t theta, Double_t phi, Double_t t, Double_t mass,
                                    TLorentzVector& p4)
{
    // Calculate the 4-vector of a particle using the polar angle 'th', the azimuthal
    // angle 'ph', the kinetic energy 't', and the mass 'mass'.
    // The components of the 4-vector will be stored in 'p4'.

    Double_t tot_e = t + mass;
    p4.SetE(tot_e);
    Double_t p = TMath::Sqrt(tot_e*tot_e - mass*mass);
    TVector3 v;
    v.SetMagThetaPhi(p, theta, phi);
    p4.SetVect(v);
}

//______________________________________________________________________________
void FAParticleA2::Print(Option_t* option) const
{
    // Print the content of this class.

    FAParticle::Print(option);
    printf("Theta [deg]            : %f\n", theta*TMath::RadToDeg());
    printf("Phi [deg]              : %f\n", phi*TMath::RadToDeg());
    printf("Energy                 : %f\n", energy);
    printf("DeltaE                 : %f\n", deltaE);
    printf("time-of-flight         : %f\n", tof);
    printf("PSA angle              : %f\n", psa_a);
    printf("PSA radius             : %f\n", psa_r);
}

//______________________________________________________________________________
void FAParticleA2::Clear(Option_t* option)
{
    // Clear the content of this class.

    FAParticle::Clear(option);
    theta = 0;
    phi = 0;
    energy = 0;
    deltaE = 0;
    tof = 0;
    psa_a = 0;
    psa_r = 0;
}

