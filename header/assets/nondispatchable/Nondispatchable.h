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
    int n_replacements = 0;
    
    double cap_kW = 100;
    double running_hrs = 0;
    double replace_running_hrs = 30000;
    
    std::string nondisp_type_str = "";
};


class Nondispatchable {
    public:
        // attributes
        structNondispatchable struct_nondisp;
        
        std::vector<bool> is_running_vec;
        std::vector<bool> replaced_vec;
        
        std::vector<double> production_vec_kW;
        std::vector<double> dispatch_vec_kW;
        std::vector<double> curtailment_vec_kW;
        std::vector<double> storage_vec_kW;
        
        
        // methods
        Nondispatchable(structNondispatchable);
        
        void _handleReplacement(int);
        
        void commitProductionkW(double, double, int);
        double getDispatchkW(double, double);
        
        virtual double getProductionkW(double) {return 0;}
        virtual double getProductionkW(double, double) {return 0;}
        
        virtual ~Nondispatchable(void);
};


#endif  /* NONDISPATCHABLE_H */
