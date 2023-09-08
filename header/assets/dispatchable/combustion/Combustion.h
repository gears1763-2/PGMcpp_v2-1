/*
 *  Dispatchable <-- Combustion class header file
 */


#ifndef COMBUSTION_H
#define COMBUSTION_H


#include "../../../includes.h"
#include "../Dispatchable.h"


enum FuelType {
    FUEL_DIESEL,
    FUEL_GAS
};


struct structCombustion {
    FuelType fuel_type = FUEL_DIESEL;
    
    /*
     *  Diesel emissions coefficients
     *  ref: docs/diesel_emissions_ref_1.pdf
     *  ref: docs/diesel_emissions_ref_2.pdf
     */
    double diesel_CO2_kgL = 2.7;
    double diesel_CO_kgL = 0.0178;
    double diesel_NOx_kgL = 0.0014;
    double diesel_SOx_kgL = 0.0042;
    double diesel_CH4_kgL = 0.0007;
    double diesel_PM_kgL = 0.0001;
    
    //...
};


struct structEmissions {
    double CO2_kg = 0;
    double CO_kg = 0;
    double NOx_kg = 0;
    double SOx_kg = 0;
    double CH4_kg = 0;
    double PM_kg = 0;
};


class Combustion : public Dispatchable {
    public:
        //  1. attributes
        structCombustion struct_combustion;
        
        std::vector<double> CO2_vec_kg;
        std::vector<double> CO_vec_kg;
        std::vector<double> NOx_vec_kg;
        std::vector<double> SOx_vec_kg;
        std::vector<double> CH4_vec_kg;
        std::vector<double> PM_vec_kg;
        
        
        //  2. methods
        Combustion(structDispatchable, structCombustion);
        
        structEmissions getEmissions(double);
        
        virtual void commitProductionkW(double, double, int) {return;}
        virtual double requestProductionkW(double) {return 0;}
        virtual double getFuelConsumptionL(double) {return 0;}
        
        virtual void writeResults(
            std::string,
            std::vector<double>*,
            int
        ) {return;}
        
        ~Combustion(void);
};


#endif  /* COMBUSTION_H */
