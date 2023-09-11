/*
 *  Nondispatchable <-- Wind class header file
 */


#ifndef WIND_H
#define WIND_H


#include "../../includes.h"
#include "Nondispatchable.h"


struct structWind {
    int resource_key = 0;
    
    double design_speed_ms = 8;
};


class Wind : public Nondispatchable {
    public:
        //  1. attributes
        structWind struct_wind;
        
        
        //  2. methods
        Wind(structNondispatchable, structWind);
        
        void _writeSummary(std::string, int);
        
        double getProductionkW(double);
        
        void writeResults(std::string, std::vector<double>*, int);
        
        ~Wind(void);
};


#endif  /* WIND_H */
