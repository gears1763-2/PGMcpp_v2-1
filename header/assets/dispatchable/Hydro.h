/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for the Hydro class, which is a child class of
 *  Dispatchable.
 * 
 *  This class implements the dynamics of a hydro electric asset, be it
 *  with (i.e., traditional) or without (i.e., run-of-river) fluid
 *  storage.
 * 
 *  WORK IN PROGRESS - NOT YET FULLY IMPLEMENTED
 */


#ifndef HYDRO_H
#define HYDRO_H


#include "../../includes.h"
#include "Dispatchable.h"


struct structHydro {
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    int resource_key = 0;
};


class Hydro : public Dispatchable {
    public:
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        structHydro struct_hydro;
        
        
        //  methods
        Hydro(structDispatchable, structHydro, int);
        
        void _writeSummary(std::string, int);
        
        double requestProductionkW(double);
        
        void writeResults(std::string, int);
        
        ~Hydro(void);
};


#endif  /* HYDRO_H */
