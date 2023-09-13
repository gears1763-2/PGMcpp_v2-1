/*
 *  Dispatchable <-- Combustion class header file
 */


#ifndef COMBUSTION_H
#define COMBUSTION_H


#include "../../../includes.h"
#include "../Dispatchable.h"


enum FuelMode {
    LINEAR,
    LOOKUP
};


enum FuelType {
    FUEL_DIESEL,
    FUEL_GAS
};


struct structCombustion {
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    FuelMode fuel_mode = LINEAR;
    
    double fuel_cost_L = 1.50;
    
    /*
     *  LINEAR fuel consumption parameters
     */
    double linear_fuel_intercept_LkWh = -1; // sentinel
    double linear_fuel_slope_LkWh = -1;     // sentinel
    
    /*
     *  LOOKUP fuel consumption parameters
     */
    std::string path_2_fuel_consumption_data = "";
    
    /*
     *  Diesel emissions parameters
     *  ref: docs/diesel_emissions_ref_1.pdf
     *  ref: docs/diesel_emissions_ref_2.pdf
     */
    double diesel_CO2_kgL = 2.7;
    double diesel_CO_kgL = 0.0178;
    double diesel_NOx_kgL = 0.0014;
    double diesel_SOx_kgL = 0.0042;
    double diesel_CH4_kgL = 0.0007;
    double diesel_PM_kgL = 0.0001;
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
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        FuelType fuel_type = FUEL_DIESEL;
        structCombustion struct_combustion;
        
        double real_fuel_discount_rate_annual = -1;
        double total_fuel_consumed_L = 0;
        
        double total_CO2_emitted_kg = 0;
        double total_CO_emitted_kg = 0;
        double total_NOx_emitted_kg = 0;
        double total_SOx_emitted_kg = 0;
        double total_CH4_emitted_kg = 0;
        double total_PM_emitted_kg = 0;
        
        std::vector<double> fuel_interp_load_ratio_vec;
        std::vector<double> fuel_interp_consumption_vec_Lhr;
        
        std::vector<double> fuel_vec_L;
        std::vector<double> real_fuel_cost_vec;
        
        std::vector<double> CO2_vec_kg;
        std::vector<double> CO_vec_kg;
        std::vector<double> NOx_vec_kg;
        std::vector<double> SOx_vec_kg;
        std::vector<double> CH4_vec_kg;
        std::vector<double> PM_vec_kg;
        
        
        //  methods
        Combustion(structDispatchable, structCombustion, int);
        
        void _readInFuelConsumptionData(void);
        double _fuelConsumptionLookupL(double, double);
        void _writeTimeSeriesResults(std::string, int);
        
        double getFuelConsumptionL(double, double);
        structEmissions getEmissions(double);
        void recordEmissions(structEmissions, int);
        
        virtual void commitProductionkW(double, int) {return;}
        virtual double requestProductionkW(double) {return 0;}
        
        void computeLevellizedCostOfEnergy(void);
        
        virtual void writeResults(
            std::string,
            int
        ) {return;}
        
        ~Combustion(void);
};


#endif  /* COMBUSTION_H */
