/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for Model class.
 * 
 *  This is the central container class for all other classes that make
 *  up part of PGMcpp (see header/Model_includes.h).
 */


#ifndef MODEL_H
#define MODEL_H


#include "includes.h"
#include "Model_includes.h"


enum DispatchMode {
    LOAD_FOLLOWING_IN_ORDER,
    CYCLE_CHARGING_IN_ORDER,
    LOAD_FOLLOWING_SMART_COMBUSTION,
    CYCLE_CHARGING_SMART_COMBUSTION,
    FORESIGHT
};


struct structModel {
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    bool print_flag = false;
    bool test_flag = false;
    
    DispatchMode dispatch_mode = LOAD_FOLLOWING_IN_ORDER;
    
    double nominal_inflation_rate_annual = 0.02;
    double nominal_discount_rate_annual = 0.04;
    
    std::string path_2_load_data = "";
};


class Model {
    public:
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        structModel struct_model;
        
        int n_timesteps = 8760;
        
        double project_life_yrs = 0;
        double total_load_served_kWh = 0;
        double total_fuel_consumed_L = 0;

        double total_CO2_emitted_kg = 0;
        double total_CO_emitted_kg = 0;
        double total_NOx_emitted_kg = 0;
        double total_SOx_emitted_kg = 0;
        double total_CH4_emitted_kg = 0;
        double total_PM_emitted_kg = 0;
        
        double real_discount_rate_annual = 0;
        double net_present_cost = 0;
        double levellized_cost_of_energy_per_kWh = 0;
        
        std::vector<double> dt_vec_hr;
        std::vector<double> load_vec_kW;
        std::vector<double> net_load_vec_kW;
        std::vector<double> remaining_load_vec_kW;
        std::vector<double> time_vec_hr;
        
        std::map<int, std::vector<double>> resource_map_1D;
        std::map<int, std::string> resource_path_map_1D;
        
        std::map<int, std::vector<std::vector<double>>> resource_map_2D;
        std::map<int, std::string> resource_path_map_2D;
        
        std::vector<Nondispatchable*> nondisp_ptr_vec;
        std::vector<Combustion*> combustion_ptr_vec;
        std::vector<Dispatchable*> noncombustion_ptr_vec;
        std::vector<Storage*> storage_ptr_vec;
        
        
        //  methods
        void _readInLoadData(void);
        void _populateDeltaVecHr(void);
        
        void _readIn1dRenewableResource(
            std::string,
            std::vector<std::string>,
            int
        );
        void _readIn2dRenewableResource(
            std::string,
            std::vector<std::string>,
            int
        );
        
        void _addNondispatchable(Nondispatchable*);
        void _addNonCombustion(Dispatchable*);
        void _addCombustion(Combustion*);
        void _addStorage(Storage*);
        
        double _getRenewableProductionkW(Nondispatchable*, int);
        void _generateNetLoadVector(void);
        
        double _controlCombustion(int, double, bool);
        double _controlNoncombustion(int, double);
        
        std::vector<Storage*> _getDepletedStorage(void);
        std::vector<Storage*> _getNondepletedStorage(void);
        
        double _dischargeStorage(int, double, std::vector<Storage*>);
        
        void _chargeStorageFromCombustion(int, std::vector<Storage*>);
        void _chargeStorageFromNoncombustion(int, std::vector<Storage*>);
        void _chargeStorageFromNondispatchable(int, std::vector<Storage*>);
        void _chargeStorage(int, std::vector<Storage*>);
        void _toggleReserve(bool);
        
        void _dispatchLoadFollowingInOrderCharging(int);
        void _dispatchLoadFollowingInOrderDischarging(int);
        
        void _dispatchCycleChargingInOrderCharging(int);
        void _dispatchCycleChargingInOrderDischarging(int);
        
        bool _sufficientProductionStorage(double, int);
        void _handleDispatch(void);
        
        void _computeFuelEmissions(void);
        void _computeEconomics(void);
        
        void _writeDispatchResults(std::string);
        void _writeLoadResults(std::string);
        void _writeSummary(std::string);
        
        
        Model(structModel);
        
        void add1dRenewableResource(std::string, std::string, int);
        void add2dRenewableResource(std::string, std::string, int);
        
        void addSolar(structNondispatchable, structSolar);
        void addTidal(structNondispatchable, structTidal);
        void addWave(structNondispatchable, structWave);
        void addWind(structNondispatchable, structWind);
        
        void addDiesel(structDispatchable, structCombustion, structDiesel);
        void addHydro(structDispatchable, structHydro);
        
        void addLiIon(structStorage, structBatteryStorage, structLiIon);
        
        void run();
        void writeResults(std::string);
        
        void clearAssets(void);
        void reset(void);
        virtual ~Model(void);
};


#endif  /* MODEL_H */

