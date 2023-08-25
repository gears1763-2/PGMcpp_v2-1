/*
 *  Nondispatchable <-- Tidal class header file
 */


#ifndef TIDAL_H
#define TIDAL_H


#include "../../includes.h"
#include "Nondispatchable.h"


struct structTidal {
    int resource_key = 0;
};


class Tidal : public Nondispatchable {
    public:
        //  1. attributes
        structTidal struct_tidal;
        
        
        //  2. methods
        Tidal(structNondispatchable, structTidal);
        
        //...
        
        ~Tidal(void);
};


#endif  /* TIDAL_H */
