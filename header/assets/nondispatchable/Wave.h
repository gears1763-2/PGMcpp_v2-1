/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for the Wave class, which is a child class of
 *  Dispatchable.
 *
 *  This class implements the dynamics of a wave energy converter (WEC).
 */


#ifndef WAVE_H
#define WAVE_H


#include "../../includes.h"
#include "Nondispatchable.h"


enum WavePowerMode {
    GAUSSIAN,
    NORMALIZED_PERFORMANCE_MATRIX,
    PARABOLOID
};


struct structWave {
    //  input attributes (structured)
    //  these are the only attributes the user should interact with
    int resource_key = 0;
    
    WavePowerMode power_mode = PARABOLOID;
    
    double design_significant_wave_height_m = 2;
    double design_energy_period_s = 10;
    
    std::string path_2_normalized_performance_matrix = "";
};


class Wave : public Nondispatchable {
    public:
        //  modelling and output attributes (unstructured)
        //  the user should not interact with these attributes
        structWave struct_wave;
        
        double min_interp_sig_wave_height_m = 0;
        double max_interp_sig_wave_height_m = 0;
        double min_interp_energy_period_s = 0;
        double max_interp_energy_period_s = 0;
        
        std::vector<double> interp_sig_wave_height_vec_m;
        std::vector<double> interp_energy_period_vec_s;
        std::vector<std::vector<double>> interp_normalized_performance_matrix;
        
        
        //  methods
        Wave(structNondispatchable, structWave, int);
        
        void _readInNormalizedPerformanceMatrix(void);
        void _writeSummary(std::string, int);
        double _productionLookupkW(double, double);
        
        double getProductionkW(double, double);
        
        void writeResults(std::string, int);
        
        ~Wave(void);
};


#endif  /* WAVE_H */
