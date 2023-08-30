/*
 *  Dispatchable class header file.
 */


#ifndef DISPATCHABLE_H
#define DISPATCHABLE_H


#include "../../includes.h"


enum DispatchableType {
    DIESEL,
    HYDRO
};


struct structDispatchable {
    bool is_running = false;
    bool print_flag = false;
    bool test_flag = false;
    
    DispatchableType disp_type;
    
    int n_timesteps;
    
    double cap_kW = 100;
};


class Dispatchable {
    public:
        // attributes
        structDispatchable struct_disp;
        
        std::vector<double> production_vec_kW;
        std::vector<double> dispatch_vec_kW;
        std::vector<double> curtailment_vec_kW;
        std::vector<double> storage_vec_kW;
        
        std::vector<bool> is_running_vec;
        
        
        // methods
        Dispatchable(structDispatchable);
        
        double getDispatchkW(double, double);
        
        virtual void commitProduction(double, int) {return;}
        
        virtual double requestProduction(double) {return 0;}
        virtual double getFuelConsumption(double) {return 0;}
        
        virtual ~Dispatchable(void);
};


#endif  /* DISPATCHABLE_H */
