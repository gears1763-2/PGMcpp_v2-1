/*
 *  Nondispatchable <-- Solar class header file
 */


#ifndef SOLAR_H
#define SOLAR_H


#include "../../includes.h"
#include "Nondispatchable.h"


struct structSolar {
    int resource_key = 0;
};


class Solar : public Nondispatchable {
    public:
        //  1. attributes
        structSolar struct_solar;
        
        
        //  2. methods
        Solar(structNondispatchable, structSolar);
        
        //...
        
        ~Solar(void);
};


#endif  /* SOLAR_H */
