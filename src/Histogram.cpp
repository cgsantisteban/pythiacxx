#ifndef HISTOGRAM
#define HISTOGRAM

#include<vector>
#include <iostream>

class Histogram
{
           
    public:
        Histogram(std::string dName, int p, std::vector<double> d);
        ~Histogram();
        std::string dataName;
        int pdg;
        std::vector<double> data;
   
};


Histogram::Histogram(std::string dName, int p, std::vector<double> d) {
    Histogram::dataName = dName;
    Histogram::pdg = p;
    Histogram::data = d;
}

Histogram::~Histogram()
{}

#endif
