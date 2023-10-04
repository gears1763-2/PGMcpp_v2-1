/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for the Solar class, which is a child class of
 *  Dispatchable.
 *
 *  This class implements the dynamics of a solar photovoltaic (PV)
 *  array.
 */


#ifndef SOLAR_H
#define SOLAR_H


#include "../../includes.h"
#include "Nondispatchable.h"


struct structSolar {
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    int resource_key = 0;
    
    double derating = 0.8;
    double DC2AC_conversion_ratio = 0.9;    // a.k.a. performance ratio
    
    /*
     *  ref: survey of Solar models in HOMER Pro 3.12.3. (Canadian dollars)
     *  ref: discussion with Hakai Energy Solutions
     */
    double capital_cost_per_kW = 3000;
};


class Solar : public Nondispatchable {
    public:
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        structSolar struct_solar;
        
        
        //  methods
        Solar(structNondispatchable, structSolar, int);
        
        void _writeSummary(std::string, int);
        
        double getProductionkW(double);
        
        void writeResults(std::string, int);
        
        ~Solar(void);
};


#endif  /* SOLAR_H */
