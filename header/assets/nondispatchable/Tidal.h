/*
 *  Nondispatchable <-- Tidal class header file
 */


#ifndef TIDAL_H
#define TIDAL_H


#include "../../includes.h"
#include "Nondispatchable.h"


struct structTidal {
    int resource_key = 0;
    int power_curve = 0;
    
    double design_speed_ms = 2;
};


class Tidal : public Nondispatchable {
    public:
        //  1. attributes
        structTidal struct_tidal;
        
        
        //  2. methods
        Tidal(structNondispatchable, structTidal);
        
        double getProductionkW(double);
        
        ~Tidal(void);
};


#endif  /* TIDAL_H */
