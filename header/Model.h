/*
 *  Model class header file.
 */


#ifndef MODEL_H
#define MODEL_H


#include "includes.h"
#include "Model_includes.h"


struct structModel {
    bool print_flag = false;
    bool test_flag = false;
    
    int n_timesteps = 0;
    
    std::string path_2_load_data = "";
};


class Model {
    public:
        //  attributes
        structModel struct_model;
        
        std::vector<double> dt_vec_hr;
        std::vector<double> load_vec_kW;
        std::vector<double> net_load_vec_kW;
        std::vector<double> time_vec_hr;
        
        std::map<int, std::vector<double>> resource_map_1D;
        std::map<int, std::vector<std::vector<double>>> resource_map_2D;
        
        std::vector<Nondispatchable*> nondisp_ptr_vec;
        
        
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
        
        double _getRenewableProduction(Nondispatchable*, int);
        void _generateNetLoadVector(void);
        
        Model(structModel);
        
        void add1dRenewableResource(std::string, std::string, int);
        void add2dRenewableResource(std::string, std::string, int);
        
        void addSolar(structNondispatchable, structSolar);
        void addTidal(structNondispatchable, structTidal);
        void addWave(structNondispatchable, structWave);
        void addWind(structNondispatchable, structWind);
        
        void run(void);
        
        void clearAssets(void);
        virtual ~Model(void);
};


#endif  /* MODEL_H */
