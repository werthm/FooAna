/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAConfigCLAS12                                                       //
//                                                                      //
// This namespace contains some often used constants and definitions    //
// for the CLAS12 experiment.                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAConfigCLAS12
#define FooAna_FAConfigCLAS12

#include "Rtypes.h"

namespace FAConfigCLAS12
{
    // define CLAS12 detectors
    enum FADetectorCLAS12_t
    {
        kNoDet = 0,
        kCD    = 1 << 0,
        kFD    = 1 << 1,
        kFT    = 1 << 2,
    };

    // define binary operators for FADetectorCLAS12_t
    inline FADetectorCLAS12_t operator|(const FADetectorCLAS12_t a, const FADetectorCLAS12_t b)
    {
        return (FADetectorCLAS12_t) ((Int_t)a | (Int_t)b);
    }
    inline FADetectorCLAS12_t& operator|=(FADetectorCLAS12_t& a, const FADetectorCLAS12_t b)
    {
        return (FADetectorCLAS12_t&) ((Int_t&)a |= (Int_t)b);
    }
}

#endif

