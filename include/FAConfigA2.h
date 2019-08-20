/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAConfigA2                                                           //
//                                                                      //
// This namespace contains some often used constants and definitions.   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAConfigA2
#define FooAna_FAConfigA2

#include "Rtypes.h"

namespace FAConfigA2
{
    // define A2 detectors
    enum FADetectorA2_t
    {
        kEmpty   = 0,
        kTagger  = 1 << 0,
        kMicro   = 1 << 1,
        kCB      = 1 << 2,
        kPID     = 1 << 3,
        kMWPC_1  = 1 << 4,
        kMWPC_2  = 1 << 5,
        kTAPS    = 1 << 6,
        kVeto    = 1 << 7,
        kPizza   = 1 << 8,
        kTOFWall = 1 << 9
    };

    // define binary operators for FADetectorA2_t
    inline FADetectorA2_t operator|(const FADetectorA2_t a, const FADetectorA2_t b)
    {
        return (FADetectorA2_t) ((Int_t)a | (Int_t)b);
    }
    inline FADetectorA2_t& operator|=(FADetectorA2_t& a, const FADetectorA2_t b)
    {
        return (FADetectorA2_t&) ((Int_t&)a |= (Int_t)b);
    }
}

#endif

