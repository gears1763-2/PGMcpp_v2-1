/*
 *  Nondispatchable <-- Wave class header file
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
    int resource_key = 0;
    
    WavePowerMode power_mode = PARABOLOID;
    
    double design_significant_wave_height_m = 2;
    double design_energy_period_s = 10;
    
    std::string path_2_normalized_performance_matrix = "";
};


class Wave : public Nondispatchable {
    public:
        //  1. attributes
        structWave struct_wave;
        
        double min_interp_sig_wave_height_m = 0;
        double max_interp_sig_wave_height_m = 0;
        double min_interp_energy_period_s = 0;
        double max_interp_energy_period_s = 0;
        
        std::vector<double> interp_sig_wave_height_vec_m;
        std::vector<double> interp_energy_period_vec_s;
        std::vector<std::vector<double>> interp_normalized_performance_matrix;
        
        
        //  2. methods
        Wave(structNondispatchable, structWave);
        
        void _readInNormalizedPerformanceMatrix(void);
        void _writeTimeSeriesResults(std::string, std::vector<double>*, int);
        void _writeSummary(std::string, int);
        double _productionLookupkW(double, double);
        
        double getProductionkW(double, double);
        
        void writeResults(std::string, std::vector<double>*, int);
        
        ~Wave(void);
};


#endif  /* WAVE_H */
