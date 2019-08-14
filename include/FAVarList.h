/*************************************************************************
 * Author: Dominik Werthmueller, 2018-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarList                                                            //
//                                                                      //
// Class for a list of analysis variables.                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAVarList
#define FooAna_FAVarList

#include <vector>

#include "TNamed.h"

class FAVarAbs;

class FAVarList : public TNamed
{

protected:
    std::vector<FAVarAbs*> fVar;        // list of analysis variables (elements not owned)

public:
    FAVarList() : TNamed() { }
    FAVarList(const Char_t* name, const Char_t* title);
    virtual ~FAVarList() { }

    Int_t GetNVar() const;
    FAVarAbs* GetVariable(Int_t i);

    void AddVariable(FAVarAbs* var);

    void EnableFillBinned();
    void DisableFillBinned();
    void EnableStorageBinned();
    void DisableStorageBinned();

    ClassDef(FAVarList, 1)  // list of analysis variables
};

#endif

