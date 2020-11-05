#include <iostream>
#include<vector>
#include<sstream> 

#include"Pythia8/Pythia.h"

#include"Pythiaexperiment.h"
#include"Histogram.cpp"

using namespace Pythia8;

PythiaExperiment::PythiaExperiment(std::string ocsv, bool is_f, bool is_c) {
    is_final = is_f;
    is_charged = is_c;
    outCSVFile = ocsv;
}

PythiaExperiment::~PythiaExperiment()
{}

//get data
double getValue(std::string dataType,Particle particle){
	double data = 0;

	if(dataType == "px") data = particle.px();
	if(dataType == "py") data = particle.py();
	if(dataType == "pz") data = particle.pz();
	if(dataType == "e") data = particle.e();
	if(dataType == "xProd") data = particle.xProd();
	if(dataType == "yProd") data = particle.yProd();
	if(dataType == "zProd") data = particle.zProd();
	if(dataType == "tProd") data = particle.tProd();
	if(dataType == "pT2") data = particle.pT2();
	if(dataType == "eT2") data = particle.eT2();
	if(dataType == "pT") data = particle.pT();
	if(dataType == "eT") data = particle.eT();
	if(dataType == "y") data = particle.y();
	if(dataType == "eta") data = particle.eta();
	if(dataType == "theta") data = particle.theta();
	if(dataType == "phi") data = particle.phi();
	if(dataType == "mT") data = particle.mT();
	if(dataType == "mT2") data = particle.mT2();

	return data;
}

//PYTHIA Event Listing (csv format)
std::vector<std::string> writeOutEventCSV(Event event){
    std::vector<std::string> outCSV;
    
	int precision = 4;
	for(int i=0;i<event.size();i++){
        std::stringstream linestream;
        linestream<< i << "," << event[i].id() << "," + event[i].nameWithStatus(18) << ","
				<< event[i].status() << ","<< event[i].mother1() << ","
				<< event[i].mother2() << "," << event[i].daughter1() << ","
				<< event[i].daughter2() << "," << event[i].col() << "," << event[i].acol() << ","
				<< setprecision(precision)
				<< event[i].px() << "," << event[i].py() << "," << event[i].pz() << ","
				<< event[i].e() << "," << event[i].m();
        std::string line;
        linestream >> line;
        outCSV.push_back(line);
	}

    return outCSV;
}

//add data histogram
void PythiaExperiment::addHist(std::string dataName, int particle) {
    std::vector<double> mydata{0.0};
    Histogram myhist(dataName,particle,mydata);
    histogramList.push_back(myhist);
    std::cout<<"Added histogram: "<<"Data "<<myhist.dataName
            <<", particle "<<myhist.pdg<<std::endl;
}

//pythia simulation parameters
void PythiaExperiment::readString(std::string m) {
    std::cout<<"Added parameter: "<<m<<std::endl;
    mstring.push_back(m);
}

//histogram list or parameters (pythia configuration)
void PythiaExperiment::listElement(std::string typeList) {
    
    int count = 1;
    if (typeList == "histogram") {
        if(histogramList.size()>0){
            std::cout<<"Histogram list"<<std::endl;
            for ( auto&& h : histogramList) {
                std::cout <<"["<<count<<"] "<<"data: " <<h.dataName 
                        << " pdg: "<<to_string(h.pdg)<<std::endl;
                count++;
            }
        }
        else
        {
             std::cout<<"Empty histogram list"<<std::endl;
        }
        
    }
    else if (typeList == "macro") {
        if(mstring.size() > 0) {
            std::cout<<"Macro parameters"<<std::endl;
            for ( auto&& m : mstring) {
                std::cout <<"["<<count<<"] "<< m << "\n";
                count++;
            }
        }
        else {
            std::cout<<"No parameters configured"<<std::endl;
        }
    }
    else {
        std::cout<<"Select macro or histogram"<<std::endl;
    }

}

//delete parameters (pythia configuration) or histograms
void PythiaExperiment::deleteElement(std::string typeList, size_t pos){
    size_t sizeList = 0;
    if(typeList == "histogram")
        sizeList = histogramList.size();
    else 
        sizeList = mstring.size();
    
    if(pos>0 && pos<=sizeList){
        std::string msg = "Deleted: ";
        bool is_error = false;

        if (typeList == "histogram") {
            msg += "Histogram-> data:" + histogramList[pos-1].dataName
                    + ", pdg: " + to_string(histogramList[pos-1].pdg);
            histogramList.erase(histogramList.begin() + pos - 1);
        }
        else if (typeList == "macro") {
            msg += mstring[pos-1];
            mstring.erase(mstring.begin() + pos - 1);
        }
        else{
            msg = "Error: type a macro instruction (macro) or histogram";
            is_error = true;
        }
        std::cout<<msg<<std::endl;
        
        if(!is_error)
            PythiaExperiment::listElement(typeList);
    }
    else
    {
        std::cout<<"Error: type a number between"<<std::endl;
        std::cout<<"1 and "<<sizeList<<std::endl;
    }
}

//run pythia
std::vector<std::vector<double>> PythiaExperiment::runExperiment() {
    
    std::vector<std::string> outCSV;
    std::vector<std::vector<double>> outdata;
    
    Pythia pythia;
    for (auto&& m : mstring){
        pythia.readString(m);
    }
    
    pythia.init();
    
    int nEvt = pythia.mode("Main:numberOfEvents");
    for (int iEvent = 0; iEvent < nEvt; ++iEvent) {
		
		if (!pythia.next()) continue;
		
		//saving data
		for ( int i = 0; i < pythia.event.size(); ++i){
            bool is_final_event = pythia.event[i].isFinal(); 
            if (!is_final) is_final_event = true; //no final
            bool is_charged_event = pythia.event[i].isCharged(); //no charged
            if(!is_charged) is_charged_event = true;

			if (is_final_event){
                if(is_charged_event){
                    Particle particle = pythia.event[i];
                    for(size_t ih=0;ih<histogramList.size();ih++){
                        int pdg = histogramList[ih].pdg;
                        if (particle.id() == pdg){
                            double value = getValue(histogramList[ih].dataName,particle);
                            if(value != 0) histogramList[ih].data.push_back(value);
                        }
                    }
                }
			   
			}
		}
        //Only last event
        if(iEvent == (nEvt - 1)) outCSV = writeOutEventCSV(pythia.event);
	}

    //CSV Event file
    std::ofstream outEvent;
	if( outEvent ) {
		remove( outCSVFile.c_str() );
	}
	outEvent.open(outCSVFile.c_str(),ios::app);
	outEvent<<"event,id,particle,status,mother1,mother2,daughter1,daughter2,colour,acolour,"<<
			"px,py,pz,e,m"<<"\n";

    for(auto&& o : outCSV){
        outEvent<<o<<"\n";
    }
    outEvent.close();    
    

    //Saving histograms
    for(auto&& h : histogramList){
        outdata.push_back(h.data);
    }
    
    return outdata;
    
}



