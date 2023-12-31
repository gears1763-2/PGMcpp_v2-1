/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for the Combustion class, which is a child class of
 *  Dispatchable and the parent class of Diesel.
 * 
 *  This class implements the general dynamics of combustion assets (
 *  i.e., assets that burn some kind of fuel in order to dispatch
 *  energy to the grid).
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
    
    double cycle_charging_load_ratio = 0.85;
    
    double ramp_rate_constraint_kWperhr = -1;   // sentinel
    
    double fuel_cost_L = 1.50;
    double nominal_fuel_escalation_rate_annual = 0.05;
    
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
     *  ref: docs/refs/diesel_emissions_ref_1.pdf
     *  ref: docs/refs/diesel_emissions_ref_2.pdf
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
        
        double real_fuel_discount_rate_annual = 0;
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
        double _fuelConsumptionLookupL(double, int);
        void _writeTimeSeriesResults(std::string, int);
        
        double getFuelConsumptionL(double, int);
        structEmissions getEmissions(double);
        void recordEmissions(structEmissions, int);
        
        virtual void commitProductionkW(double, int) {return;}
        virtual double requestProductionkW(double, int) {return 0;}
        
        void computeLevellizedCostOfEnergy(void);
        
        virtual void writeResults(std::string, int) {return;}
        
        ~Combustion(void);
};


#endif  /* COMBUSTION_H */
