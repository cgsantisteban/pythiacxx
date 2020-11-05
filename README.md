# cxxpythia
A basic example using PYTHIA MonteCarlo and Cxx.jl

[PYTHIA](http://home.thep.lu.se/~torbjorn/pythia83html/Welcome.html) is a general-purpose event generator for high-energy particle collisions.

# Prerequisites

You must have installed PYTHIA. View [PYTHIA Worksheet](http://home.thep.lu.se/~torbjorn/pdfdoc/worksheet8200.pdf)

# Installation

* Edit the Makefile.inc file and change PATH_TO_PYTHIA variable with your configuration.

* Move to cxxpythia folder and in the terminal type 

```
make Pythiaexperiment
```
to compile the code and build the Pythiaexperiment.so library.

# How it works

View the PythiaLab notebook. It is a simple implementation of the main02 PYTHIA example.