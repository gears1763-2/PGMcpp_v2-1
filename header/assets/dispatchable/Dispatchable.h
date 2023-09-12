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
    bool is_sunk = false;
    bool print_flag = false;
    bool test_flag = false;
    
    DispatchableType disp_type = DIESEL;
    
    int n_timesteps = 8760;
    int n_replacements = 0;
    int n_starts = 0;
    
    double cap_kW = 100;
    double running_hrs = 0;
    double replace_running_hrs = 30000;
    
    double capital_cost = -1;
    double op_maint_cost_per_kWh = -1;
    
    double real_discount_rate_annual = -1;
    
    std::string disp_type_str = "";
};


class Dispatchable {
    public:
        // attributes
        structDispatchable struct_disp;
        
        std::vector<bool> is_running_vec;
        std::vector<bool> replaced_vec;
        
        std::vector<double> production_vec_kW;
        std::vector<double> dispatch_vec_kW;
        std::vector<double> curtailment_vec_kW;
        std::vector<double> storage_vec_kW;
        
        std::vector<double> real_capital_cost_vec;
        std::vector<double> real_op_maint_cost_vec;
        
        
        // methods
        Dispatchable(structDispatchable);
        
        void _handleReplacement(int, double);
        void _writeTimeSeriesResults(
            std::string, std::vector<double>* ptr_2_time_vec_hr, int
        );
        
        virtual void commitProductionkW(double, double, double, int);
        double getDispatchkW(double, double);
        
        virtual double requestProductionkW(double) {return 0;}
        virtual double getFuelConsumptionL(double) {return 0;}
        
        virtual void writeResults(
            std::string,
            std::vector<double>*,
            int
        ) {return;}
        
        virtual ~Dispatchable(void);
};


#endif  /* DISPATCHABLE_H */
