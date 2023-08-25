/*
 *  Nondispatchable <-- Wave class header file
 */


#ifndef WAVE_H
#define WAVE_H


#include "../../includes.h"
#include "Nondispatchable.h"


struct structWave {
    int resource_key = 0;
};


class Wave : public Nondispatchable {
    public:
        //  1. attributes
        structWave struct_wave;
        
        
        //  2. methods
        Wave(structNondispatchable, structWave);
        
        //...
        
        ~Wave(void);
};


#endif  /* WAVE_H */
