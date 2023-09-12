/*
 *  Model class header file.
 */


#ifndef MODEL_H
#define MODEL_H


#include "includes.h"
#include "Model_includes.h"


enum DispatchMode {
    LOAD_FOLLOWING_IN_ORDER,
    LOAD_FOLLOWING_SMART_COMBUSTION
};


struct structModel {
    bool print_flag = false;
    bool test_flag = false;
    
    DispatchMode dispatch_mode = LOAD_FOLLOWING_IN_ORDER;
    
    int n_timesteps = 8760;
    
    double project_life_yrs = 0;
    double nominal_inflation_rate_annual = 0.02;
    double nominal_discount_rate_annual = 0.04;
    double real_discount_rate_annual = 0;
    
    double net_present_cost = 0;
    
    double total_dispatch_kWh = 0;
    double levellized_cost_of_energy_per_kWh = 0;
    
    std::string path_2_load_data = "";
};


class Model {
    public:
        //  attributes
        structModel struct_model;
        
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
        
        double _getRenewableProductionkW(Nondispatchable*, int);
        void _generateNetLoadVector(void);
        
        void _dispatchLoadFollowingInOrderCharging(int);
        void _dispatchLoadFollowingInOrderDischarging(int);
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
