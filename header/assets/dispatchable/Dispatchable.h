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
        
        
        // methods
        Dispatchable(structDispatchable);
        
        double getDispatchkW(double, double);
        
        virtual double requestProduction(double);
        
        virtual ~Dispatchable(void);
};


#endif  /* DISPATCHABLE_H */
