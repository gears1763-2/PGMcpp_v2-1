/*
 *  Dispatchable <-- Hydro class header file
 */


#ifndef HYDRO_H
#define HYDRO_H


#include "../../includes.h"
#include "Dispatchable.h"


struct structHydro {
    int resource_key = 0;
};


class Hydro : public Dispatchable {
    public:
        //  1. attributes
        structHydro struct_hydro;
        
        
        //  2. methods
        Hydro(structDispatchable, structHydro);
        
        double requestProduction(double);
        
        ~Hydro(void);
};


#endif  /* HYDRO_H */
