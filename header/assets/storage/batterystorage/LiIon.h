/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Header file for the LiIon class, which is a child class of
 *  BatteryStorage.
 *
 *  This class implements the dynamics of a lithium ion (Li-ion) battery
 *  energy storage system.
 */


#ifndef LiIon_H
#define LiIon_H


#include "../../../includes.h"
#include "../Storage.h"
#include "BatteryStorage.h"


struct structLiIon {
    double replace_SOH = 0.8;
    
    // degradation parameters
    //
    //  ref: [point to battery degradation modelling doc]
    //
    double degr_alpha = 10;             // [ ]
    double degr_beta = 1.1;             // [ ]
    double degr_B_hat_cal_0 = 5.222e6;  // [1/sqrt(hr)]
    double degr_r_cal = 0.350;          // [ ]
    double degr_Ea_cal_0 = 5.279e4;     // [J/mol]
    double degr_a_cal = 108.5;          // [J/mol]
    double degr_s_cal = 1.895;          // [ ]
    double gas_constant_JmolK = 8.31446;
    double temperature_K = 273 + 20;    
};


class LiIon : public BatteryStorage {
    public:
        //  1. attributes
        structLiIon struct_liion;
        
        double SOH = 1;     // SOH = state of health
        
        std::vector<double> SOH_vec;
        
        //  2. methods
        LiIon(structStorage, structBatteryStorage, structLiIon, int);
        
        void _writeTimeSeriesResults(std::string, int);
        void _writeSummary(std::string, int);
        
        double _getdSOHdt(double);
        void _handleDegradation(double, int);
        void _handleReplacement(int);
        
        void commitChargekW(double, int);
        void commitDischargekW(double, int);
        
        void writeResults(std::string, int);
        
        ~LiIon(void);
};


#endif  /* LiIon_H */
