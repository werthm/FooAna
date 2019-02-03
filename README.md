FooAna
======

Yet another analysis framework.

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

## Quick Start
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

## Class Hierarchy
```
FAEventBase           : abstract base event class
  FAEventT            : base event class template
    FAEventA2         : A2 event class (typedef FAEventT<FAParticleA2, FAParticleMCA2>)
    FAEventA2KF       : A2 event class for kinematic fitting (typedef FAEventT<FAParticleA2KF, FAParticleMCA2>)
FAEventBaseA2         : common analysis variables for A2 event classes

FAEventVar            : base event variable class
  FAEventVarA2        : base A2 event variable class
  FAEventVarA2KF      : A2 event variable class for kinematic fitting

FAParticle            : base particle class
  FAParticleA2        : base A2 particle class
  FAParticleA2KF      : A2 particle class for kinematic fitting

FAParticleMC          : base MC particle class
  FAParticleMCA2      : base A2 MC particle class

FAVector3             : three-vector class using Double32_t
  FAVector4           : four-vector class using Double32_t

FAUtils               : namespace with utility functions
```

## User Guide

### Writing a Custom Event Class
Custom event classes can be easily created by using the template class `FAEventT`. This
class derives from `FAEventBase`, which holds the most common event variables (event ID,
timestamp, etc.). Classes created by `FAEventT` contain also two vectors for detected
and generated (MC) particles. The exact types of the classes representing those particles
can be specified when the template class `FAEventT` is instantiated. The base particle
classes are `FAParticle` and `FAParticleMC`, respectively. Additional event variables
can be added to the custom event class.
Example:
```C++
class CustomEvent :
    public FAEventT<FAParticle, FAParticleMC>
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
