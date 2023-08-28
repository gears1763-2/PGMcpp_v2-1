/*
 *  Dispatchable <-- Combustion <-- Diesel class header file
 */


#ifndef DIESEL_H
#define DIESEL_H


#include "../../../includes.h"
#include "../Dispatchable.h"
#include "Combustion.h"


struct structDiesel {
    //...
};


class Diesel : public Combustion {
    public:
        //  1. attributes
        structDiesel struct_Combustion;
        
        
        //  2. methods
        Diesel(structDispatchable, structCombustion, structDiesel);
        
        double requestProduction(double);
        
        ~Diesel(void);
};


#endif  /* DIESEL_H */
