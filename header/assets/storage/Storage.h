/*
 *  Storage class header file.
 */


#ifndef STORAGE_H
#define STORAGE_H


#include "../../includes.h"


enum StorageType {
    STORAGE_HYDRO,
    STORAGE_LIION
};


struct structStorage {
    bool print_flag = false;
    bool test_flag = false;
    
    StorageType storage_type = STORAGE_LIION;
    
    int n_timesteps = 8760;
    
    double cap_kW = 100;
    double cap_kWh = 1000;
    
    double charge_kWh = 1000;
    double min_charge_kWh = 0;
    double max_charge_kWh = 1000;
    
    double acceptable_kW = 0;
    double charging_kW = 0;
};


class Storage {
    public:
        // attributes
        structStorage struct_storage;
        
        std::vector<double> charge_vec_kWh;
        std::vector<double> charging_vec_kW;
        std::vector<double> discharging_vec_kW;
        
        
        // methods
        Storage(structStorage);
        
        virtual double getAvailablekW(double) {return 0;}
        virtual double getAcceptablekW(double) {return 0;}
        
        virtual void commitChargekW(double, double, int) {return;}
        virtual void commitDischargekW(double, double, int) {return;}
        
        virtual ~Storage(void);
};


#endif  /* STORAGE_H */
