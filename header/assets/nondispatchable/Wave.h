/*
 *  Nondispatchable <-- Wave class header file
 */


#ifndef WAVE_H
#define WAVE_H


#include "../../includes.h"
#include "Nondispatchable.h"


enum WavePowerMode {
    GAUSSIAN,
    NORMALIZED_PERFORMANCE_MATRIX,
    PARABOLOID
};


struct structWave {
    int resource_key = 0;
    
    WavePowerMode power_mode = PARABOLOID;
    
    double design_significant_wave_height_m = 2;
    double design_energy_period_s = 10;
};


class Wave : public Nondispatchable {
    public:
        //  1. attributes
        structWave struct_wave;
        
        
        //  2. methods
        Wave(structNondispatchable, structWave);
        
        double getProductionkW(double, double);
        
        ~Wave(void);
};


#endif  /* WAVE_H */
