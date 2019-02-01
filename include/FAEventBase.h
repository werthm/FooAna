/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventBase                                                          //
//                                                                      //
// This class is the base for all FooAna events.                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventBase
#define FooAna_FAEventBase

#include "Rtypes.h"

class FAEventBase
{

public:
    Double_t eventID;           // event ID
    Int_t timestamp;            // UTC timestamp
    Double32_t weight;          // event weight
    Double32_t flux;            // flux

    FAEventBase() : eventID(0), timestamp(0),
                    weight(0), flux(0) { }
    virtual ~FAEventBase() { }

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    ClassDef(FAEventBase, 1)  // Base event class
};

#endif

