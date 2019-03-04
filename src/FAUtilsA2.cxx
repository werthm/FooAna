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

