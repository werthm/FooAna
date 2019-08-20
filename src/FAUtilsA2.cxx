/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAUtilsA2                                                            //
//                                                                      //
// This namespace contains some often used utility functions related to //
// the A2 experiment.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TSystem.h"
#include "TError.h"

#include "FAUtilsA2.h"

//______________________________________________________________________________
Int_t FAUtilsA2::LoadTaggerCalibration(const Char_t* fileName, Int_t nChannel,
                                       Double_t* taggEnergy, Double_t* taggEnergyWidth,
                                       Bool_t quiet)
{
    // Load the photon tagger energy calibration file produced by ugcal (short format)
    // for 'nChannel' channels and save it into the arrays 'taggEnergy' and 'taggEnergyWidth'.
    // Return the number of loaded tagger channels.

    Int_t nRead = 0;

    // expand the path
    TString fs(fileName);
    gSystem->ExpandPathName(fs);

    // try to open the file
    FILE* f = fopen(fs.Data(), "r");
    if (!f)
    {
        Error("FAUtilsA2::LoadTaggerCalibration", "Could not open the file '%s'!", fs.Data());
        return nRead;
    }

    // read the file
    Char_t buffer[256];
    Double_t ch, e, err;
    while (fgets(buffer, 255, f))
    {
        // parse the file
        if (sscanf(buffer, "%lf%lf%lf", &ch, &e, &err) != 3)
        {
            // bad format
            TString out(buffer);
            out.ReplaceAll('\n', '\0');
            out.ReplaceAll('\r', '\0');
            Error("FAUtilsA2::LoadTaggerCalibration", "Could not parse the line '%s'", out.Data());
            continue;
        }
        else
        {
            // store channel
            taggEnergy[nRead] = e;
            if (taggEnergyWidth)
                taggEnergyWidth[nRead] = err;
            nRead++;
        }

        // check number of read channels
        if (nRead == nChannel)
            break;
    }

    // close the file
    fclose(f);

    // user info
    if (!quiet)
        Info("FAUtilsA2::LoadTaggerCalibration", "Loaded calibration for %d channels from '%s'", nRead, fs.Data());

    return nRead;
}

//______________________________________________________________________________
void FAUtilsA2::SetDetFillFlags(Int_t det, FAVarAbs& v_cb, FAVarAbs& v_taps)
{
    // Set the filling flags of the variables 'v_cb' and 'v_taps' according to
    // the detector 'det' so events are filled into the correct histogram.

    // check detector
    if (det & FAConfigA2::kCB)
    {
        v_cb.ResetBit(FAVarAbs::kNoFill);
        v_taps.SetBit(FAVarAbs::kNoFill);
    }
    else if (det & FAConfigA2::kTAPS)
    {
        v_cb.SetBit(FAVarAbs::kNoFill);
        v_taps.ResetBit(FAVarAbs::kNoFill);
    }
    else
    {
        v_cb.SetBit(FAVarAbs::kNoFill);
        v_taps.SetBit(FAVarAbs::kNoFill);
    }
}

//______________________________________________________________________________
TString FAUtilsA2::DetectorsAsString(FAConfigA2::FADetectorA2_t d)
{
    // Return a string of all detectors bit-marked in 'd'.

    TString out;

    // check detectors
    if (d == 0)
        out += "Empty ";
    if (d & FAConfigA2::kTagger)
        out += "Tagger ";
    if (d & FAConfigA2::kMicro)
        out += "Micro ";
    if (d & FAConfigA2::kCB)
        out += "CB ";
    if (d & FAConfigA2::kPID)
        out += "PID ";
    if (d & FAConfigA2::kMWPC_1)
        out += "MWPC_1 ";
    if (d & FAConfigA2::kMWPC_2)
        out += "MWPC_2 ";
    if (d & FAConfigA2::kTAPS)
        out += "TAPS ";
    if (d & FAConfigA2::kVeto)
        out += "Veto ";
    if (d & FAConfigA2::kPizza)
        out += "Pizza ";
    if (d & FAConfigA2::kTOFWall)
        out += "TOFWall ";

    // format
    out.Remove(TString::kTrailing, ' ');
    out.ReplaceAll(" ", ", ");

    return out;
}

