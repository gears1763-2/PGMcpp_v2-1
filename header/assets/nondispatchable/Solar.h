/*
 *  Nondispatchable <-- Solar class header file
 */


#ifndef SOLAR_H
#define SOLAR_H


#include "../../includes.h"
#include "Nondispatchable.h"


struct structSolar {
    int resource_key = 0;
    
    double derating = 0.8;
};


class Solar : public Nondispatchable {
    public:
        //  1. attributes
        structSolar struct_solar;
        
        
        //  2. methods
        Solar(structNondispatchable, structSolar);
        
        void _writeTimeSeriesResults(std::string, std::vector<double>*, int);
        void _writeSummary(std::string, int);
        
        double getProductionkW(double);
        
        void writeResults(std::string, std::vector<double>*, int);
        
        ~Solar(void);
};


#endif  /* SOLAR_H */
