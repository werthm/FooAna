FooAna
======

Yet another analysis framework

## Table of contents
* [Release notes](#release-notes)
* [Installation](#installation)
* [Quick start](#quick-start)
* [Class hierarchy](#class-hierarchy)
* [User guide](#user-guide)

## Release notes

### 0.4.0
in beta testing
* new event class template with general-variable array
* new A2 detector bit pattern (not compatible with earlier versions)
* easy enabling/disabling of binned storage

### 0.3.1
August 6, 2019
* added a number of small improvements
* extended the documentation

### 0.3.0
April 23, 2019
* overlap histogram filling
* loading of TH1, TH2, TGraph, and TCutG objects in analyses
* smaller improvements

### 0.2.0
April 12, 2019
* added wrapper for multi-threaded TTree analysis
* new A2 event and particle formats
* added progress server

### 0.1.0
February 10, 2019
* initial release

## Installation

### Dependencies
* ROOT 6
* CMake 2.8

### Installation
```
cd /some/directory
git clone https://github.com/werthm/FooAna.git
cd FooAna
mkdir build
cd build
cmake ..
make
make install
export LD_LIBRARY_PATH="/some/directory/FooAna/build/lib:$LD_LIBRARY_PATH"
export ROOT_INCLUDE_PATH="/some/directory/FooAna/build/include:$ROOT_INCLUDE_PATH"
```
It is recommended to set the `LD_LIBRARY_PATH` and `ROOT_INCLUDE_PATH` variables
in your shell configuration file.

## Quick start
If `LD_LIBRARY_PATH` and `ROOT_INCLUDE_PATH` are set, root should be able to
find the library automatically. If not, there are several ways to load the
library:

ROOT command line interface:
```
root [0] gSystem->Load("libFooAna.so");
```
Interpreted macros:
```
void FooAnaMacro()
{
  gSystem->Load("libFooAna.so");
  ...
}
```
Compiled macros:
```
root [0] .include /some/directory/FooAna/build/include
root [1] .x FooAnaMacro.C++
```

## Class hierarchy
```
FAAnalysis               : base analysis wrapper
  FAAnalysisA2           : A2 analysis wrapper
FAAnalysisResult         : analysis result

FAEventT                 : base event class template
  FAEventA2_B            : A2 event class (basic)
  FAEventA2_BF1          : A2 event class (basic, kinfit type 1)
  FAEventA2_BF2          : A2 event class (basic, kinfit type 2)
  FAEventA2_BF3          : A2 event class (basic, kinfit type 3)
  FAEventA2_BIF3         : A2 event class (basic, event info, kinfit type 3)
  FAEventA2_BIF4         : A2 event class (basic, event info, kinfit type 4)

FAParticleA2_B           : A2 particle class (basic)
FAParticleA2_BF1         : A2 particle class (basic, kinfit type 1)
FAParticleA2MC_B         : A2 MC particle class (basic)

FAVector3                : three-vector class using Double32_t
  FAVector4              : four-vector class using Double32_t

FAWrapPrim               : TObject-wrapper for primitive types

FAVarAbs                 : abstract base class for analysis variables
  FAVar                  : analysis variable class template
FAVarList                : a list of analysis variables
  FAVarParticleA2        : a list of analysis variables for A2 particles
  FAVarParticleA2_CoincT : a list of coincidence time variables for A2 particles
FAVarHistogram           : a histogram to be filled with analysis variables
FAVarFiller              : the histogram/tree filler class for analysis variables
FAVarPlotter             : the histogram plotter class for analysis variables
FAVarPlotterEntry        : an entry (histogram(s) from file) to be plotted by FAVarPlotter

FAProgressServer         : processing progress monitoring server
FAProgressClient         : processing progress monitoring client
FAProgressMessage        : progress message object

FAUtils                  : namespace with utility functions
FAUtilsA2                : namespace with utility functions related to the A2 experiment
FAConfigA2               : namespace with constants and definitions
```

## User Guide

### Writing a custom event class
Custom event classes can be easily created by using the template class `FAEventT`.
Classes created by `FAEventT` contain arrays of general variables, 4-vectors, and
detected and generated (MC) particles. The type of variables and the types of the
classes representing the particles can be specified when the template class `FAEventT`
is instantiated. Additional event variables can be added to the custom event class.
This can make an event class more clear in case a large number of event variables is
needed and keeping track of them in the general variable array is getting difficult.
Example:
```C++
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
```
**NOTE:** The default constructor and the `Clear()` method should be overwritten
in the custom event class to ensure that the class members are properly
initialized and cleared. Before this custom code, the parent constructor and `Clear()`
method should be called.

Tree writing and reading using a custom event class is illustrated in the example
`examples/event`.

### Analyzing a TTree using TTreeReader and FAVarFiller
`FAVarFiller` provides a convenient way to store analysis variables for further
processing. Especially switching from filling a tree (unbinned analysis) or histograms
(binned analysis) requires only changing one flag.
Analysis variables `FAVar<Type>` can be either registered to the filler one-by-one
or by combining them to a set and adding the corresponding list. Name, title, unit,
range, and binning of the variables can all be specified and will be used when creating
histograms or building the output tree. 2D-histograms of two variables can also be easily
defined:
```C++
// create the filler
FAVarFiller filler("MyFiller", "Example filler");

// define some variables
FAVar<Short_t> my_index(filler, "index", "My custom index", 0, 10, 0, 10);
FAVar<Double_t> my_variable(filler, "variable", "My custom variable", "some unit", 100, 0, 20);

// request special histograms
filler.AddHistogram2D(&my_variable, &my_index);

// configure the filling mode
filler.Init(FAVarFiller::kUnbinned);    // unbinned, i.e. a tree
//filler.Init(FAVarFiller::kBinned);      // binned, i.e. histograms

```
If one or two bin axes are defined via `FAVarFiller::SetBins1()` or
`FAVarFiller::SetBins2()`, single trees for all individual bins are created
(unbinned analysis) or the histograms are grouped in `TDirectoryFile` folders
(binned analysis) in the output ROOT file.

Values can be directly assigned to the analysis variables in the event loop. All
variables are filled by calling `FAVarFiller::Fill()`:
```C++
my_index = ...
my_variable = ...
filler.Fill();
```
The final tree or histograms can be written to a ROOT file by calling
```C++
filler.WriteFile("events.root");
```
A complete example can be found in the `ReadCustomEvent()` method in
`examples/event/CustomEvent.C`.

### Analyzing A2 data
`FAAnalysisA2` is an analysis-wrapper for data of the A2 experiment.
It allows the analysis of presorted data with one single ROOT macro
and one configuration file. It is mainly intended to be used as the last
step in the analysis chain to create binned results (histograms) but
it also supports tree output for further unbinned processing.
For this, `FAAnalysisA2` makes use of the `FAVarFiller` class (see above).

Input data is expected to be stored in a `TTree` using a `FAEventT`-based
event class. Depending on the use case, different event classes
are available that also contain different types of particle classes.
This design enables to optimize the size of presorted data sets by only
including the necessary event information.

The configuration file (ROOT config-file format) configures all general settings
of the analysis, sets up the loading of ROOT objects (`TGraph`, `TH1`, etc.)
needed during the analysis, and can be extended by custom configuration keys.
The standard keys will be described in the next section.

Thanks to the extended configuration file, the analysis ROOT macro can be kept
compact and readable. The main part is a C++ lambda function which contains
the code that is executed for every event in the data tree. This function is
then passed to the `FAAnalysisA2` analysis object, which wires up everything
and launches the analysis. Multithreading is fully supported by having multiple
workers analyzing parts of the data. Partial results will be automatically
merged at the end.

#### `FAAnalysisA2` configuration file
The following keys are currently supported:
```
# general keys
FA.Analysis.TreeName:               name of the input TTree
FA.Analysis.NWorker:                number of parallel workers in a multithreaded analysis
FA.Analysis.Progress:               flag for progress indication (0=off, 1=on)
FA.Analysis.Axis1.Binning:          binning for axis 1 (bin edges)
FA.Analysis.Axis1.Name:             name of axis 1
FA.Analysis.Axis2.Binning:          binning for axis 2 (bin edges)
FA.Analysis.Axis2.Name:             name of axis 2
FA.Analysis.Input:                  name of input file(s), wildcards are supported
FA.Analysis.Output:                 name of output file

# object loading (currently supported are TH1, TH2, TGraph, TCutG)
FA.Analysis.Load.TH1.N:             number of TH1 objects to load
FA.Analysis.Load.TH1.0.File:        file of the first TH1 object to load
FA.Analysis.Load.TH1.0.Name:        name of the first TH1 object to load

# A2-specific keys
FA.Analysis.A2.Tagger.Channels:     number of tagger channels
FA.Analysis.A2.Tagger.Calib:        tagger calibration file (ugcal short format)
FA.Analysis.A2.Trig.CB.ESum.Mean:   mean value of energy sum threshold
FA.Analysis.A2.Trig.CB.ESum.Sigma:  Gaussian sigma of energy sum threshold
FA.Analysis.A2.Trig.Mult.Total:     total trigger multiplicity
FA.Analysis.A2.Trig.Mult.UseTAPS:   TAPS contributing to multiplicity (0=no, 1=yes)
```

#### `FAAnalysisA2`-based analysis macro
The following commented ROOT macro illustrates the general structure of an
`FAAnalysisA2`-based analysis macro. More complex macros can be found
in the `examples` folder.
```C++
void Analysis()
{
    // create analysis
    FAAnalysisA2 ana("config.rootrc");

    // print analysis configuration
    ana.Print();

    // event processing lambda function
    auto ProcessEvents = [&](TTreeReader& reader)
    {
        // configure reader
        TTreeReaderValue<FAEventA2_BIF3> event(reader, "Event");

        // define analysis variables
        FAVarFiller filler("my_analysis", "title of my analysis");
        FAVar<Float_t> var_axis1(filler, "var_axis1", "variable of axis 1", "MeV", ana.GetAxis1());
        FAVar<Float_t> some_var(filler, "some_var", "some analysis variable", "MeV", 100, 0, 2000);
        filler.SetBins1(ana.GetAxis1());
        filler.Init(FAVarFiller::kBinned);

        // read events
        while (reader.Next())
        {
            // analyis code
            // ...

            // set analysis variables
            //var_axis1 = 123;
            //some_var = 12.34;

            // event weights via lambda function
            auto weighting = [&] { return 1; };

            // fill analysis variables
            filler.Fill(weighting, var_axis1.GetVar());
        }

        // write and register partial output file
        return FAAnalysis::WritePartialOutput(filler, reader.GetTree()->GetCurrentFile()->GetName());
    };

    // process events
    ana.Process(ProcessEvents);

    // write output file
    ana.WriteOutputFile(out);

    gSystem->Exit(0);
}
```
#### A2 detector codes
The A2 detector bit pattern type `FADetectorA2_t` is defined in the `FAConfigA2`
namespace. The following table shows some common codes as decimal numbers for
a quick lookup.

Code        | Detectors
:-----------|:---------------------
4           | CB
8           | PID
12          | CB, PID
20          | CB, MWPC1
52          | CB, MWPC1, MWPC2
60          | CB, PID, MWPC1, MWPC2
64          | TAPS
128         | Veto
192         | TAPS, Veto

### Plotting and comparing analysis variables
Analysis variables of several analyses can be compared by using `FAVarPlotter`. In the following
minimal example, the analysis variable is defined and two files created by `FAVarFiller` are
added. `FAVarPlotter` provides then multiple ways for comparing the analysis variable in, e.g.,
different bins, using a simple and quick interface.
```C++
FAVarPlotter compare("my_variable");
compare.AddEntry("file_1.root", "old analysis", kBlack);
compare.AddEntry("file_2.root", "new analysis", kRed);
compare.DrawBin(0, 0);
```
