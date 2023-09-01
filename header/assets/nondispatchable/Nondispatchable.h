/*
 *  Nondispatchable class header file.
 */


#ifndef NONDISPATCHABLE_H
#define NONDISPATCHABLE_H


#include "../../includes.h"


enum NondispatchableType {
    SOLAR,
    TIDAL,
    WAVE,
    WIND
};


struct structNondispatchable {
    bool is_running = false;
    bool print_flag = false;
    bool test_flag = false;
    
    NondispatchableType nondisp_type = SOLAR;
    
    int n_timesteps = 8760;
    
    double cap_kW = 100;
};


class Nondispatchable {
    public:
        // attributes
        structNondispatchable struct_nondisp;
        
        std::vector<double> production_vec_kW;
        std::vector<double> dispatch_vec_kW;
        std::vector<double> curtailment_vec_kW;
        std::vector<double> storage_vec_kW;
        
        std::vector<bool> is_running_vec;
        
        
        // methods
        Nondispatchable(structNondispatchable);
        
        double getDispatchkW(double, double);
        
        virtual void commitProduction(double, int) {return;}
        
        virtual double getProductionkW(double) {return 0;}
        virtual double getProductionkW(double, double) {return 0;}
        
        virtual ~Nondispatchable(void);
};


#endif  /* NONDISPATCHABLE_H */
