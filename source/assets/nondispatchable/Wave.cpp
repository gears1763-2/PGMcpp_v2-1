/*
 *  Nondispatchable <-- Wave class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Wave.h"


Wave :: Wave(
    structNondispatchable struct_nondisp,
    structWave struct_wave
) : Nondispatchable(struct_nondisp) {
    /*
     *  Wave class constructor
     */
    
    this->struct_wave = struct_wave;
    
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWave object constructed at " << this
            << std::endl;
    }
    
    return;
}


double Wave :: getProductionkW(
    double significant_wave_height_m,
    double energy_period_s
) {
    /*
     *  Method to compute and return production under given wave
     *  resource statistics, namely significant wave height (H_s) and
     *  energy period (T_e). Implements multiple power modes.
     */
     
    // first, check if no resource
    if (
        significant_wave_height_m <= 0 ||
        energy_period_s <= 0
    ) {
        return 0;
    }
    
    // otherwise, apply selected power mode
    double production = 0;
    double H_s_nondim = 0;
    double T_e_nondim = 0;
    
    switch (this->struct_wave.power_mode) {
        case (-1): {
            /*
             *  Interpolate over given normalized performance matrix
             */
             
            //...
            
            break;
        }
        
        
        case (1): {
            /*
             *  ref: docs/wind_tidal_wave.pdf
             */
            
            H_s_nondim = 
                (
                    significant_wave_height_m - 
                    this->struct_wave.design_significant_wave_height_m
                ) / 
                this->struct_wave.design_significant_wave_height_m;
                
            T_e_nondim =
                (
                    energy_period_s -
                    this->struct_wave.design_energy_period_s
                ) / 
                this->struct_wave.design_energy_period_s;
                
            production = exp(
                -2.25119 * pow(T_e_nondim, 2) +
                3.44570 * T_e_nondim * H_s_nondim -
                4.01508 * pow(H_s_nondim, 2)
            );
            
            break;
        }
        
        
        default: {
            /*
             *  ref: B. Robertson, H. Bailey, M. Leary, and B. Buckham,
             *       “A methodology for architecture agnostic and
             *       time flexible representations of wave energy
             *       converter performance”,
             *       Applied Energy, vol. 287, p. 116588, 2021,
             *       doi:10.1016/j.apenergy.2021.116588
             */
             
            // first, check for idealized wave breaking (deep water)
            if (
                significant_wave_height_m >= 0.2184 * pow(energy_period_s, 2)
            ) {
                return 0;
            }
            
            // otherwise, apply generic quadratic performance model
            // (with outputs bounded to [0, 1])
            production = 
                0.289 * significant_wave_height_m - 
                0.00111 * pow(significant_wave_height_m, 2) * energy_period_s - 
                0.0169 * energy_period_s;
            
            if (production < 0) {
                production = 0;
            } 
            
            else if (production > 1) {
                production = 1;
            }
            
            break;
        }
    }
    
    double production_kW = this->struct_nondisp.cap_kW * production;
    return production_kW;
}


//...


Wave :: ~Wave() {
    /*
     *  Wave class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWave object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
