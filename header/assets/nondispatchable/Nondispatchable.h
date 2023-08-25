/*
 *  Nondispatchable class header file.
 */


#ifndef NONDISPATCHABLE_H
#define NONDISPATCHABLE_H


#include "../../includes.h"


enum NondispatchableType {
    SOLAR,
    TIDAL,
    WAVE,
    WIND
};


struct structNondispatchable {
    bool print_flag = false;
    bool test_flag = false;
    
    NondispatchableType nondisp_type;
    
    int n_timesteps;
    
    double cap_kW = 100;
};


class Nondispatchable {
    public:
        // attributes
        structNondispatchable struct_nondisp;
        
        
        // methods
        Nondispatchable(structNondispatchable);
        
        //...
        
        virtual ~Nondispatchable(void);
};


#endif  /* NONDISPATCHABLE_H */
