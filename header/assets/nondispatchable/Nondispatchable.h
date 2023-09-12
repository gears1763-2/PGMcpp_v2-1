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
    bool is_sunk = false;
    bool print_flag = false;
    bool test_flag = false;
    
    NondispatchableType nondisp_type = SOLAR;
    
    int n_timesteps = 8760;
    int n_replacements = 0;
    
    double cap_kW = 100;
    double running_hrs = 0;
    double replace_running_hrs = 90000;
    
    double capital_cost = -1;
    double op_maint_cost_per_kWh = -1;
    
    double real_discount_rate_annual = -1;
    
    double net_present_cost = 0;
    double levellized_cost_of_energy_per_kWh = 0;
    
    std::string nondisp_type_str = "";
};


class Nondispatchable {
    public:
        // attributes
        structNondispatchable struct_nondisp;
        
        double total_dispatch_kWh = 0;
        
        std::vector<bool> is_running_vec;
        std::vector<bool> replaced_vec;
        
        std::vector<double> production_vec_kW;
        std::vector<double> dispatch_vec_kW;
        std::vector<double> curtailment_vec_kW;
        std::vector<double> storage_vec_kW;
        
        std::vector<double> real_capital_cost_vec;
        std::vector<double> real_op_maint_cost_vec;
        
        
        // methods
        Nondispatchable(structNondispatchable);
        
        void _handleReplacement(int, double);
        void _writeTimeSeriesResults(
            std::string, std::vector<double>* ptr_2_time_vec_hr, int
        );
        
        void commitProductionkW(double, double, double, int);
        double getDispatchkW(double, double);
        
        virtual double getProductionkW(double) {return 0;}
        virtual double getProductionkW(double, double) {return 0;}
        
        void computeLevellizedCostOfEnergy(double, std::vector<double>*);
        
        virtual void writeResults(
            std::string,
            std::vector<double>*,
            int
        ) {return;}
        
        virtual ~Nondispatchable(void);
};


#endif  /* NONDISPATCHABLE_H */
