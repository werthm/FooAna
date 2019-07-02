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
#include "TFile.h"
#include "TH2.h"
#include "TGraph.h"
#include "TCutG.h"

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
Int_t FAUtils::LaunchProgressServer()
{
    // Launch a progress server instance on a random port on localhost in an
    // external ROOT session.
    // Return the port number.

    // choose port
    TRandom3 rnd(0);
    Int_t port = rnd.Uniform(5000, 10000);

    // launch server
    gSystem->RedirectOutput("/dev/null");
    gSystem->Exec(TString::Format("root -b -l -e 'FAProgressServer server(%d); server.Listen(); "
                                  "gSystem->Exit(0)' &", port).Data());
    gSystem->RedirectOutput(0);

    // wait for server startup
    gSystem->Sleep(1000);

    return port;
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
TString FAUtils::ExtractFileName(const Char_t* s)
{
    // Extracts the file name of a file given by its full Unix paths in
    // the string s.

    // search last slash
    TString p(s);
    Ssiz_t pos = p.Last('/');

    // return the same string or copy substring
    if (pos == kNPOS)
        return p;
    else
        return TString(p(pos+1, p.Length()-pos));
}

//______________________________________________________________________________
TString FAUtils::ExpandPath(const Char_t* p)
{
    // Expand the path 'p'.

    Char_t* fnt = gSystem->ExpandPathName(p);
    TString out(fnt);
    delete fnt;
    return out;
}

//______________________________________________________________________________
Bool_t FAUtils::FileExists(const Char_t* f)
{
    // Return kTRUE if the file 'f' exists, otherwise return kFALSE.

    // expand filename
    TString fn = ExpandPath(f);

    // check file
    if (!gSystem->AccessPathName(fn.Data()))
        return kTRUE;
    else
        return kFALSE;
}

//______________________________________________________________________________
template <class T>
Bool_t FAUtils::LoadObject(const Char_t* file, const Char_t* name, T*& out)
{
    // Load generic TObjects named 'name' from the file 'file' to 'out'.

    // set output object pointer to 0
    out = 0;

    // try to open the file
    TString fileExp = ExpandPath(file);
    TFile f(fileExp.Data());

    // check if file exists
    if (!f.IsZombie())
    {
        // try to load the object
        f.GetObject(name, out);
        if (out)
        {
            // clone the object
            out = (T*)out->Clone();

            // unset directory for histograms
            if (out->InheritsFrom("TH1"))
            {
                TH1* h = (TH1*) out;
                h->SetDirectory(0);
            }

            return kTRUE;
        }
        else
        {
            Error("FAUtils::LoadObject", "Could not load the object '%s'!", name);
            return kFALSE;
        }
    }
    else
    {
        // file opening error
        Error("FAUtils::LoadObject", "Could not open the file '%s' containing the '%s' object!",
              fileExp.Data(), name);
        return kFALSE;
    }
}

//______________________________________________________________________________
void FAUtils::NormHistogram(TH1* h, TH1* hnorm)
{
    // Normalize the histogram 'h' by 'hnorm'.

    // check histogram type
    Int_t dim = h->GetDimension();
    if (dim == 1)
    {
        // loop over axis 1
        for (Int_t i = 0; i < h->GetNbinsX(); i++)
        {
            Double_t norm = hnorm->GetBinContent(i+1);
            if (norm != 0)
            {
                h->SetBinContent(i+1, h->GetBinContent(i+1) / norm);
                h->SetBinError(i+1, h->GetBinError(i+1) / norm);
            }
        }
    }
    else if (dim == 2)
    {
        // loop over axis 1
        for (Int_t i = 0; i < h->GetNbinsX(); i++)
        {
            // loop over axis 2
            for (Int_t j = 0; j < h->GetNbinsY(); j++)
            {
                Double_t norm = hnorm->GetBinContent(i+1, j+1);
                if (norm != 0)
                {
                    h->SetBinContent(i+1, j+1, h->GetBinContent(i+1, j+1) / norm);
                    h->SetBinError(i+1, j+1, h->GetBinError(i+1, j+1) / norm);
                }
            }
        }
    }
    else
        Error("FAUtils::NormHistogram", "Not implemented for histogram dim=%d", dim);
}

//______________________________________________________________________________
TAxis FAUtils::CreateVariableAxis(const Char_t* binning)
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
    return TAxis(n-1, edg);
}

//______________________________________________________________________________
Int_t FAUtils::CalcBinOverlapWeights(TAxis* axis, Double_t x, Double_t x_width,
                                     std::vector<std::pair<Int_t, Double_t>>& out)
{
    // Calculate bin overlap weights.
    // Return the number of bins to fill
    //
    // Parameters:
    // axis       : binning axis
    // x          : x bin mean value (overlapping bin)
    // x_width    : x bin width
    // out        : output vector of bin indices and weights

    // calculate original min/max bin values
    Double_t origMin = x - x_width / 2.;
    Double_t origMax = x + x_width / 2.;

    // get new min/max bins
    Int_t bMin = axis->FindFixBin(origMin);
    Int_t bMax = axis->FindFixBin(origMax);

    // calculate number of affected new bins
    Int_t nbins = bMax - bMin + 1;

    // loop over new bins and calculate filling weights
    for (Int_t i = 0; i < nbins; i++)
    {
        // set bin number
        Int_t bin = bMin + i;

        // lower border (including correction for underflow bin)
        Double_t low = TMath::Max(axis->GetBinLowEdge(bin), origMin);
        if (bin == 0) low = origMin;

        // upper border (including correction for overflow bin)
        Double_t upp = TMath::Min(axis->GetBinUpEdge(bin), origMax);
        if (bin == axis->GetNbins()+1) upp = origMax;

        // calculate filling weight
        Double_t fweight = (upp - low) / x_width;

        // set output
        out.push_back(std::make_pair(bin, fweight));
    }

    return nbins;
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

//______________________________________________________________________________
void FAUtils::Calculate4VectorTOF(Double_t theta, Double_t phi, Double_t tof, Double_t mass,
                                  TLorentzVector& p4)
{
    // Calculate the 4-vector of a particle using the polar angle 'th', the azimuthal
    // angle 'ph', the normalized time-of-flight 'tof', and the mass 'mass'.
    // The components of the 4-vector will be stored in 'p4'.

    Double_t tot_e = CalculateEkinTOF(tof, mass) + mass;
    p4.SetE(tot_e);
    Double_t p = TMath::Sqrt(tot_e*tot_e - mass*mass);
    TVector3 v;
    v.SetMagThetaPhi(p, theta, phi);
    p4.SetVect(v);
}

//______________________________________________________________________________
Double_t FAUtils::CalculateEkinTOF(Double_t tof, Double_t mass)
{
    // Calculate the kinetic energy of a particle with normalized
    // time-of-flight 'tof' and mass 'mass'.

    // calculate the beta
    Double_t beta = 1. / (tof * 1e-9 * TMath::C());

    // calculate the kinetic energy
    if (beta*beta < 1)
    {
        Double_t gamma = TMath::Sqrt(1. / (1. - beta*beta));
        return  mass * (gamma - 1.);
    }
    else return -1.;
}

// template instantiations
template Bool_t FAUtils::LoadObject(const Char_t*, const Char_t*, TH1*&);
template Bool_t FAUtils::LoadObject(const Char_t*, const Char_t*, TH2*&);
template Bool_t FAUtils::LoadObject(const Char_t*, const Char_t*, TGraph*&);
template Bool_t FAUtils::LoadObject(const Char_t*, const Char_t*, TCutG*&);

