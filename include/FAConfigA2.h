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

namespace FAConfigA2
{
    enum EA2Detector {
        kNoDetector,        // no detector
        kCBDetector,        // Crystal Ball
        kTAPSDetector,      // TAPS
        kTaggerDetector     // photon tagger
    };
}

#endif

