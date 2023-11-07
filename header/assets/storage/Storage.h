/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for the Storage class, which is the parent class to
 *  BatteryStorage.
 */


#ifndef STORAGE_H
#define STORAGE_H


#include "../../includes.h"


enum StorageType {
    LIION
};


struct structStorage {
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    bool is_sunk = false;
    bool print_flag = false;
    bool test_flag = false;
    
    double cap_kW = 100;
    double cap_kWh = 1000;  // this is "nominal capacity", and is static
    
    double nominal_inflation_rate_annual = 0.02;
    double nominal_discount_rate_annual = 0.04;
    
    double capital_cost = -1;
    double op_maint_cost_per_kWh = -1;
};


class Storage {
    public:
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        bool depleted_flag;
        bool reserve_flag;
        
        StorageType storage_type = LIION;
        structStorage struct_storage;
        
        int n_timesteps = 8760;
        int n_replacements = 0;
        
        double cap_kWh = 0; // this is "dynamic capacity", and is subject to degradation
        
        double charge_kWh = 0;
        double min_charge_kWh = 0;
        double max_charge_kWh = 1000;
        
        double project_life_yrs = 0;
        double total_throughput_kWh = 0;
        
        double acceptable_kW = 0;
        double charging_kW = 0;
        
        double real_discount_rate_annual = -1;
        
        double net_present_cost = 0;
        double levellized_cost_of_energy_per_kWh = 0;
    
        std::string storage_type_str = "";
        
        std::vector<bool> replaced_vec;
        
        std::vector<double> charge_vec_kWh;
        std::vector<double> charging_vec_kW;
        std::vector<double> discharging_vec_kW;
        
        std::vector<double> real_capital_cost_vec;
        std::vector<double> real_op_maint_cost_vec;
        
        std::vector<double>* ptr_2_dt_vec_hr;
        std::vector<double>* ptr_2_time_vec_hr;
        
        
        //  methods
        Storage(structStorage, int);
        
        virtual double getAvailablekW(int) {return 0;}
        virtual double getAcceptablekW(int) {return 0;}
        
        virtual void commitChargekW(double, int) {return;}
        virtual void commitDischargekW(double, int) {return;}
        
        void computeLevellizedCostOfEnergy(void);
        
        virtual void writeResults(std::string, int) {return;}
        virtual void toggleReserve(bool) {return;}
        
        virtual ~Storage(void);
};


#endif  /* STORAGE_H */
