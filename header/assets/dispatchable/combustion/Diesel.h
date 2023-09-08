/*
 *  Dispatchable <-- Combustion <-- Diesel class header file
 */


#ifndef DIESEL_H
#define DIESEL_H


#include "../../../includes.h"
#include "../Dispatchable.h"
#include "Combustion.h"


struct structDiesel {
    double minimum_load_ratio = 0.2;
    
    double minimum_runtime_hrs = 5;
    double time_since_last_start_hrs = 0;
};


class Diesel : public Combustion {
    public:
        //  1. attributes
        structDiesel struct_diesel;
        
        std::vector<double> fuel_vec_L;
        
        
        //  2. methods
        Diesel(structDispatchable, structCombustion, structDiesel);
        
        void _writeTimeSeriesResults(std::string, std::vector<double>*, int);
        void _writeSummary(std::string, int);
        
        void commitProductionkW(double, double, int);
        double requestProductionkW(double);
        double getFuelConsumptionL(double, double);
        
        void writeResults(std::string, std::vector<double>*, int);
        
        ~Diesel(void);
};


#endif  /* DIESEL_H */
