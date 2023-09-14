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
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    bool is_sunk = false;
    bool print_flag = false;
    bool test_flag = false;
    
    double cap_kW = 100;
    double replace_running_hrs = 90000;
    
    double nominal_inflation_rate_annual = 0.02;
    double nominal_discount_rate_annual = 0.04;
    
    double capital_cost = -1;
    double op_maint_cost_per_kWh = -1;
};


class Nondispatchable {
    public:
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        NondispatchableType nondisp_type = SOLAR;
        structNondispatchable struct_nondisp;
        
        bool is_running = false;
        
        int n_timesteps = 0;
        int n_replacements = 0;
        
        double project_life_yrs = 0;
        double running_hrs = 0;
        double total_dispatch_kWh = 0;
        
        double real_discount_rate_annual = 0;
    
        double net_present_cost = 0;
        double levellized_cost_of_energy_per_kWh = 0;
        
        std::string nondisp_type_str = "";
        
        std::vector<bool> is_running_vec;
        std::vector<bool> replaced_vec;
        
        std::vector<double> production_vec_kW;
        std::vector<double> dispatch_vec_kW;
        std::vector<double> curtailment_vec_kW;
        std::vector<double> storage_vec_kW;
        
        std::vector<double> real_capital_cost_vec;
        std::vector<double> real_op_maint_cost_vec;
        
        std::vector<double>* ptr_2_dt_vec_hr;
        std::vector<double>* ptr_2_time_vec_hr;
        
        // methods
        Nondispatchable(structNondispatchable, int);
        
        void _handleReplacement(int);
        void _writeTimeSeriesResults(std::string, int);
        
        void commitProductionkW(double, int);
        double getDispatchkW(double, double);
        
        virtual double getProductionkW(double) {return 0;}
        virtual double getProductionkW(double, double) {return 0;}
        
        void computeLevellizedCostOfEnergy(void);
        
        virtual void writeResults(std::string, int) {return;}
        
        virtual ~Nondispatchable(void);
};


#endif  /* NONDISPATCHABLE_H */
