#ifndef PYTHIAEXPERIMENT_H
#define PYTHIAEXPERIMENT_H

#include <iostream>
#include<vector>

#include"Histogram.cpp"

class PythiaExperiment
{
    private:
        std::vector<std::string> mstring;
        std::vector<Histogram> histogramList; 
        std::string outCSVFile;
        bool is_final;
        bool is_charged;
    public:
        PythiaExperiment(std::string ocsv = "event_list.csv", bool is_f = false, bool is_c = false);
        ~PythiaExperiment();
        std::vector<std::vector<double>> runExperiment();
        void readString(std::string);
        void listElement(std::string);
        void deleteElement(std::string, size_t);
        void addHist(std::string, int);

        
};

#endif