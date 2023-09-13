/*
 *  Dispatchable <-- Hydro class header file
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
