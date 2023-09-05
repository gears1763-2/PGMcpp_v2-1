/*
 *  Storage <-- BatteryStorage <-- LiIon class header file
 */


#ifndef LiIon_H
#define LiIon_H


#include "../../../includes.h"
#include "../Storage.h"
#include "BatteryStorage.h"


struct structLiIon {
    double init_cap_kWh = 1000;
    double SOH = 1;     // SOH = state of health
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
        
        
        //  2. methods
        LiIon(structStorage, structBatteryStorage, structLiIon);
        
        double _getdSOHdt(double);
        void _handleDegradation(double, double, int);
        void _handleReplacement(int);
        
        void commitChargekW(double, double, int);
        void commitDischargekW(double, double, int);
        
        ~LiIon(void);
};


#endif  /* LiIon_H */
