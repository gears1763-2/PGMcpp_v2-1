/*
 *  Nondispatchable <-- Solar class header file
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
