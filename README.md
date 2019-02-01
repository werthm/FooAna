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
```

