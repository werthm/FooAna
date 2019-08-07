/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CustomEvent                                                          //
//                                                                      //
// Example ROOT macro showing how to build, write, and read a custom    //
// event class using FooAna.                                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TSystem.h"
#include "TRandom.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TTreeReader.h"

#include "FAEventT.h"
#include "FAParticleA2_B.h"
#include "FAParticleA2MC_B.h"
#include "FAVarFiller.h"
#include "FAVar.h"

#ifdef __ROOTCLING__
#pragma link C++ class FAEventT<Double32_t, FAParticleA2_B, FAParticleA2MC_B>+;
#pragma link C++ class CustomEvent+;
#endif

class CustomEvent :
    public FAEventT<Double32_t, FAParticleA2_B, FAParticleA2MC_B>
{
public:
    Short_t someIndex;
    Double32_t someVariable;

    CustomEvent() : FAEventT(),
                    someIndex(0), someVariable(0) { }
    virtual void Print(Option_t* option = "") const
    {
        FAEventT::Print(option);
        printf("Some index             : %d\n", someIndex);
        printf("Some variable          : %f\n", someVariable);
    }
    virtual void Clear(Option_t* option = "")
    {
        FAEventT::Clear(option);
        someIndex = -1;
        someVariable = 0;
    }
    using FAEventT::operator=;

    ClassDef(CustomEvent, 1) // my custom event
};

//______________________________________________________________________________
void WriteCustomEvents(Int_t nEvent = 5000)
{
    // Example method showing how to write events.

    // create the event object
    CustomEvent event;

    // open the output file
    TFile* f = new TFile("tree.root", "recreate");

    // create the tree
    TTree* tree = new TTree("events", "An event tree");
    tree->Branch("Event", "CustomEvent", &event);

    // loop over events
    for (Int_t i = 0; i < nEvent; i++)
    {
        // set event variables
        event.someIndex = gRandom->Uniform(0, 10);
        event.someVariable = gRandom->Gaus(10, 1);

        // randomize number of particles
        Int_t npartMC = gRandom->Uniform(0, 5);
        Int_t npart = gRandom->Uniform(0, npartMC);

        // add MC particles to event
        for (Int_t j = 0; j < npartMC; j++)
        {
            FAParticleA2MC_B particle;
            particle.pdg = gRandom->Uniform(0, 20);
            event.AddParticleMC(particle);
        }

        // add particles to event
        for (Int_t j = 0; j < npart; j++)
        {
            FAParticleA2_B particle;
            particle.detector = gRandom->Uniform(0, 3);
            particle.detElem = gRandom->Uniform(0, 1000);
            event.AddParticle(particle);
        }

        // fill the event
        tree->Fill();

        // debug show first 3 events
        if (i < 3)
        {
            event.Print();
            printf("\n");
        }

        // clear the event
        event.Clear();
    }

    // write the tree and close the file
    tree->Write();
    delete f;

    // user info
    printf("Wrote tree to tree.root\n");
}

//______________________________________________________________________________
void ReadCustomEvents()
{
    // Example method showing how to read events.

    // create chain of tree files
    TChain chain("events");
    chain.AddFile("tree.root");

    // create tree reader
    TTreeReader reader(&chain);

    // configure reader
    TTreeReaderValue<CustomEvent> event(reader, "Event");

    // set-up a FooAna analysis variable filler
    FAVarFiller filler("MyFiller", "Example filler");
    FAVar<Short_t> my_index(filler, "index", "My custom index", 0, 10, 0, 10);
    FAVar<Double_t> my_variable(filler, "variable", "My custom variable", "some unit", 100, 0, 20);
    filler.AddHistogram2D(&my_variable, &my_index);

    // configure the filler
    //filler.Init(FAVarFiller::kUnbinned);    // unbinned, i.e. a tree
    filler.Init(FAVarFiller::kBinned);      // binned, i.e. histograms

    // loop over events
    while (reader.Next())
    {
        // read variables
        my_index = event->someIndex;
        my_variable = event->someVariable;

        // fill event
        filler.Fill();
    }

    // write output file
    filler.WriteFile("events.root");
}

