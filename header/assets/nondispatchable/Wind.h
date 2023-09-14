/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for the Wind class, which is a child class of
 *  Dispatchable.
 *
 *  This class implements the dynamics of a wind turbine.
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
        Wind(structNondispatchable, structWind, int);
        
        void _writeSummary(std::string, int);
        
        double getProductionkW(double);
        
        void writeResults(std::string, int);
        
        ~Wind(void);
};


#endif  /* WIND_H */
