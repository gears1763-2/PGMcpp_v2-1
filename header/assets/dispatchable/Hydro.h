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
        
        void _writeSummary(std::string, int);
        
        double requestProductionkW(double);
        
        void writeResults(std::string, std::vector<double>*, int);
        
        ~Hydro(void);
};


#endif  /* HYDRO_H */
