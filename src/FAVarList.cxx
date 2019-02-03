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


#include "FAVarList.h"

ClassImp(FAVarList)

//______________________________________________________________________________
FAVarList::FAVarList(const Char_t* name, const Char_t* title)
    : TNamed(name, title)
{
    // Constructor.

}

//______________________________________________________________________________
Int_t FAVarList::GetNVar() const
{
    // Return the number of variables.

    return fVar.size();
}

//______________________________________________________________________________
FAVarAbs* FAVarList::GetVariable(Int_t i)
{
    // Return the variable with index 'i'.

    return fVar[i];
}

//______________________________________________________________________________
void FAVarList::AddVariable(FAVarAbs* var)
{
    // Add the variable 'var'.

    fVar.push_back(var);
}

