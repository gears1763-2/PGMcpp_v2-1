/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for the Diesel class, which is a child class of
 *  Combustion.
 * 
 *  This class implements the dynamics of a standard, reciprocating
 *  diesel generator unit.
 */


#ifndef DIESEL_H
#define DIESEL_H


#include "../../../includes.h"
#include "../Dispatchable.h"
#include "Combustion.h"


struct structDiesel {
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    double minimum_load_ratio = 0.2;
    double minimum_runtime_hrs = 5;
};


class Diesel : public Combustion {
    public:
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        structDiesel struct_diesel;
        
        double time_since_last_start_hrs = 0;
        
        
        //  methods
        Diesel(structDispatchable, structCombustion, structDiesel, int);

        void _writeSummary(std::string, int);
        
        void commitProductionkW(double, int);
        double requestProductionkW(double, int);
        
        void writeResults(std::string, int);
        
        ~Diesel(void);
};


#endif  /* DIESEL_H */
