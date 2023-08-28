/*
 *  Dispatchable <-- Combustion class header file
 */


#ifndef COMBUSTION_H
#define COMBUSTION_H


#include "../../../includes.h"
#include "../Dispatchable.h"


struct structCombustion {
    //...
};


class Combustion : public Dispatchable {
    public:
        //  1. attributes
        structCombustion struct_Combustion;
        
        
        //  2. methods
        Combustion(structDispatchable, structCombustion);
        
        double requestProduction(double);
        
        ~Combustion(void);
};


#endif  /* COMBUSTION_H */
