/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAUtils                                                              //
//                                                                      //
// This namespace contains some often used utility functions.           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TRandom3.h"
#include "TSystem.h"
#include "TLorentzVector.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TAxis.h"

#include "FAUtils.h"
#include "FAVersion.h"

// init static members
static Int_t dummy = FAUtils::ShowBanner();

//______________________________________________________________________________
Char_t FAUtils::TreeDataTypeToChar(EDataType datatype)
{
   // Return the leaflist 'char' for a given datatype.
   // Copied from TTree.cxx

    switch (datatype)
    {
        case kChar_t:     return 'B';
        case kUChar_t:    return 'b';
        case kBool_t:     return 'O';
        case kShort_t:    return 'S';
        case kUShort_t:   return 's';
        case kCounter:
        case kInt_t:      return 'I';
        case kUInt_t:     return 'i';
        case kDouble_t:
        case kDouble32_t: return 'D';
        case kFloat_t:
        case kFloat16_t:  return 'F';
        case kLong_t:     return 0; // unsupported
        case kULong_t:    return 0; // unsupported?
        case kchar:       return 0; // unsupported
        case kLong64_t:   return 'L';
        case kULong64_t:  return 'l';

        case kCharStar:   return 'C';
        case kBits:       return 0; //unsupported

        case kOther_t:
        case kNoType_t:
        default:
            return 0;
    }

    return 0;
}

//______________________________________________________________________________
Int_t FAUtils::ShowBanner()
{
    // Show the FooAna banner.

    printf("\n");
    printf("FooAna %s by Dominik Werthmueller, University of York (2017-2019)\n", FOOANA_VERSION);
    printf("https://github.com/werthm/FooAna\n");
    printf("\n");

    return 0;
}

//______________________________________________________________________________
TString FAUtils::FormatTimeSec(Double_t seconds)
{
    // Return a time string in the format HH:MM:SS of the seconds 'seconds'.

    // convert seconds
    Int_t hours = Int_t(seconds / 3600);
    seconds -= hours * 3600;
    Int_t min = Int_t(seconds / 60);
    seconds -= min * 60;
    Int_t sec = Int_t(seconds);

    // format string
    return TString::Format("%02d:%02d:%02d", hours, min, sec);
}

//______________________________________________________________________________
Int_t FAUtils::LaunchProgressServer()
{
    // Launch a progress server instance on a random port on localhost in an
    // external ROOT session.
    // Return the port number.

    // choose port
    TRandom3 rnd(0);
    Int_t port = rnd.Uniform(5000, 10000);

    // launch server
    gSystem->Exec(TString::Format("root -b -l -e 'FAProgressServer server(%d); server.Listen()' &", port).Data());
    gSystem->Sleep(1000);

    return port;
}

//______________________________________________________________________________
TAxis* FAUtils::CreateVariableAxis(const Char_t* binning)
{
    // Create an axis having variable bin sizes using the low edge string list
    // from 'binning'.

    // tokenize string
    TString st(binning);
    TObjArray* list = st.Tokenize(" ");
    Int_t n = list->GetEntries();

    // create low edge array
    Double_t edg[n];

    // loop over token
    for (Int_t i = 0; i < n; i++)
    {
        // get string
        TObjString* s = (TObjString*) list->At(i);

        // convert to double and save to edge array
        edg[i] = atof(s->GetString().Data());
    }

    // clean-up
    delete list;

    // return axis
    return new TAxis(n-1, edg);
}

//______________________________________________________________________________
void FAUtils::Calculate4Vector(Double_t theta, Double_t phi, Double_t t, Double_t mass,
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

