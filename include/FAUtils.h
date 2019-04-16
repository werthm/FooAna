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


#ifndef FooAna_FAUtils
#define FooAna_FAUtils

#include "TDataType.h"
#include "TFile.h"
#include "TH1.h"
#include "TError.h"

class TLorentzVector;
class TAxis;

namespace FAUtils
{
    Char_t TreeDataTypeToChar(EDataType datatype);
    Int_t ShowBanner();
    Int_t LaunchProgressServer();

    TString FormatTimeSec(Double_t seconds);
    TString ExtractFileName(const Char_t* s);
    TString ExpandPath(const Char_t* p);
    Bool_t FileExists(const Char_t* f);

    TAxis CreateVariableAxis(const Char_t* binning);
    Int_t CalcBinOverlapWeights(TAxis* axis, Double_t x, Double_t x_width,
                                std::vector<std::pair<Int_t, Double_t>>& out);

    void Calculate4Vector(Double_t theta, Double_t phi, Double_t t, Double_t mass,
                          TLorentzVector& p4);

    template <class T>
    Bool_t LoadObject(const Char_t* file, const Char_t* name, T*& out)
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
}

#endif

