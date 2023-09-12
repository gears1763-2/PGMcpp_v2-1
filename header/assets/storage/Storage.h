/*
 *  Storage class header file.
 */


#ifndef STORAGE_H
#define STORAGE_H


#include "../../includes.h"


enum StorageType {
    LIION
};


struct structStorage {
    bool is_sunk = false;
    bool print_flag = false;
    bool test_flag = false;
    
    StorageType storage_type = LIION;
    
    int n_timesteps = 8760;
    int n_replacements = 0;
    
    double cap_kW = 100;
    double cap_kWh = 1000;
    
    double charge_kWh = 1000;
    double min_charge_kWh = 0;
    double max_charge_kWh = 1000;
    
    double acceptable_kW = 0;
    double charging_kW = 0;
    
    double capital_cost = -1;
    double op_maint_cost_per_kWh = -1;
    
    double real_discount_rate_annual = -1;
    
    double net_present_cost = 0;
    
    std::string storage_type_str = "";
};


class Storage {
    public:
        // attributes
        structStorage struct_storage;
        
        std::vector<bool> replaced_vec;
        
        std::vector<double> charge_vec_kWh;
        std::vector<double> charging_vec_kW;
        std::vector<double> discharging_vec_kW;
        
        std::vector<double> real_capital_cost_vec;
        std::vector<double> real_op_maint_cost_vec;
        
        
        // methods
        Storage(structStorage);
        
        virtual double getAvailablekW(double) {return 0;}
        virtual double getAcceptablekW(double) {return 0;}
        
        virtual void commitChargekW(double, double, double, int) {return;}
        virtual void commitDischargekW(double, double, double, int) {return;}
        
        virtual void writeResults(
            std::string,
            std::vector<double>*,
            int
        ) {return;}
        
        virtual ~Storage(void);
};


#endif  /* STORAGE_H */
