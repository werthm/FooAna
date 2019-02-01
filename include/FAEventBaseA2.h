/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventBaseA2                                                        //
//                                                                      //
// Base class for A2 events.                                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventBaseA2
#define FooAna_FAEventBaseA2

#include "Rtypes.h"

class FAEventBaseA2
{

public:
    Short_t taggCh;                 // tagger channel
    Double32_t coincTime;           // coincidence time
    Double32_t cbSum;               // CB energy sum
    Short_t trigMultCB;             // CB trigger multiplicity
    Short_t trigMultTAPSLED1;       // TAPS LED1 trigger multiplicity
    Short_t trigMultTAPSLED2;       // TAPS LED2 trigger multiplicity

    FAEventBaseA2() : taggCh(-1), coincTime(0),
                      cbSum(0), trigMultCB(0),
                      trigMultTAPSLED1(0), trigMultTAPSLED2(0) { }
    virtual ~FAEventBaseA2() { }

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    ClassDef(FAEventBaseA2, 1)  // Base class for A2 events
};

#endif

