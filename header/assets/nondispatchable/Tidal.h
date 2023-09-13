/*
 *  Nondispatchable <-- Tidal class header file
 */


#ifndef TIDAL_H
#define TIDAL_H


#include "../../includes.h"
#include "Nondispatchable.h"


enum TidalPowerCurve {
    CUBIC,
    EXPONENTIAL
};


struct structTidal {
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    int resource_key = 0;
    
    TidalPowerCurve power_curve = CUBIC;
    
    double design_speed_ms = 2;
};


class Tidal : public Nondispatchable {
    public:
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        structTidal struct_tidal;
        
        
        //  methods
        Tidal(structNondispatchable, structTidal, int);
        
        void _writeSummary(std::string, int);
        
        double getProductionkW(double);
        
        void writeResults(std::string, int);
        
        ~Tidal(void);
};


#endif  /* TIDAL_H */
