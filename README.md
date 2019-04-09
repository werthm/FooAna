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
FAEventT              : base event class template
  FAEventA2_B         : A2 event class (basic)
  FAEventA2_BF1       : A2 event class (basic, kinfit type 1)
  FAEventA2_BF2       : A2 event class (basic, kinfit type 2)
  FAEventA2_BF3       : A2 event class (basic, kinfit type 3)
  FAEventA2_BIF3      : A2 event class (basic, event info, kinfit type 3)
  FAEventA2_BIF4      : A2 event class (basic, event info, kinfit type 4)

FAParticleA2_B        : A2 particle class (basic)
FAParticleA2_BF1      : A2 particle class (basic, kinfit type 1)
FAParticleA2MC_B      : A2 MC particle class (basic)

FAVector3             : three-vector class using Double32_t
  FAVector4           : four-vector class using Double32_t

FAVarAbs              : abstract base class for analysis variables
  FAVar               : analysis variable class template
FAVarList             : a list of analysis variables
  FAVarParticleA2     : a list of analysis variables for A2 particles
FAVarHistogram        : a histogram to be filled with analysis variables
FAVarFiller           : the histogram/tree filler class for analysis variables
FAVarPlotter          : the histogram plotter class for analysis variables
FAVarPlotterEntry     : an entry (histogram(s) from file) to be plotted by FAVarPlotter

FAProgressServer      : processing progress monitoring server
FAProgressClient      : processing progress monitoring client

FAUtils               : namespace with utility functions
FAUtilsA2             : namespace with utility functions related to the A2 experiment
FAConfig              : namespace with constants and definitions
FAVersion             : namespace with version information
```

## User Guide

### Writing a custom event class
Custom event classes can be easily created by using the template class `FAEventT`.
Classes created by `FAEventT` contain also vectors for detected and generated (MC) particles
as well as a vector for generic 4-vectors.  The exact types of the classes representing
those particles can be specified when the template class `FAEventT` is instantiated.
Additional event variables can be added to the custom event class.
Example:
```C++
class CustomEvent :
    public FAEventT<FAParticleA2_B, FAParticleA2MC_B>
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
