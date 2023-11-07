/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for the Dispatchable class, which is the parent class to
 *  Hydro and Combustion.
 */


#ifndef DISPATCHABLE_H
#define DISPATCHABLE_H


#include "../../includes.h"


enum DispatchableType {
    DIESEL,
    HYDRO
};


struct structDispatchable {
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    bool is_sunk = false;
    bool print_flag = false;
    bool test_flag = false;
    
    double cap_kW = 100;
    double replace_running_hrs = 30000;
    
    double nominal_inflation_rate_annual = 0.02;
    double nominal_discount_rate_annual = 0.04;
    
    double capital_cost = -1;
    double op_maint_cost_per_kWh = -1;
};


class Dispatchable {
    public:
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        DispatchableType disp_type = DIESEL;
        structDispatchable struct_disp;
        
        bool is_running = false;
        
        int n_timesteps = 0;
        int n_replacements = 0;
        int n_starts = 0;
        
        double project_life_yrs = 0;
        double running_hrs = 0;
        double total_dispatch_kWh = 0;
        
        double real_discount_rate_annual = 0;
        double net_present_cost = 0;
        double levellized_cost_of_energy_per_kWh = 0;
        
        std::string disp_type_str = "";
        
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
        
        
        //  methods
        Dispatchable(structDispatchable, int);
        
        void _handleReplacement(int);
        void _writeTimeSeriesResults(std::string, int);
        
        virtual void commitProductionkW(double, int);
        double getDispatchkW(double, double);
        
        virtual double requestProductionkW(double) {return 0;}
        virtual double requestProductionkW(double, int) {return 0;}
        virtual double requestProductionkW(double, double, int) {return 0;}
        virtual double getFuelConsumptionL(double, int) {return 0;}
        
        void computeLevellizedCostOfEnergy(void);
        
        virtual void writeResults(std::string, int) {return;}
        
        virtual ~Dispatchable(void);
};


#endif  /* DISPATCHABLE_H */
