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
    int resource_key = 0;
    
    TidalPowerCurve power_curve = CUBIC;
    
    double design_speed_ms = 2;
};


class Tidal : public Nondispatchable {
    public:
        //  1. attributes
        structTidal struct_tidal;
        
        
        //  2. methods
        Tidal(structNondispatchable, structTidal);
        
        void commitProduction(double, int);
        
        double getProductionkW(double);
        
        ~Tidal(void);
};


#endif  /* TIDAL_H */
