/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAConfig                                                             //
//                                                                      //
// This namespace contains some often used constants and definitions.   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAConfig
#define FooAna_FAConfig

namespace FAConfig
{
    enum EA2Detector {
        kNoDetector,        // no detector
        kCBDetector,        // Crystal Ball
        kTAPSDetector,      // TAPS
        kTaggerDetector     // photon tagger
    };
}

#endif

