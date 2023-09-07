/*
 *  Nondispatchable <-- Tidal class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Tidal.h"


Tidal :: Tidal(
    structNondispatchable struct_nondisp,
    structTidal struct_tidal
) : Nondispatchable(struct_nondisp) {
    /*
     *  Tidal class constructor
     */
    
    this->struct_nondisp.nondisp_type = TIDAL;
    this->struct_nondisp.nondisp_type_str = "TIDAL";
    this->struct_tidal = struct_tidal;
    
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tTidal object constructed at " << this
            << std::endl;
    }
    
    return;
}


double Tidal :: getProductionkW(double tidal_resource_ms) {
    /*
     *  Method to compute and return production under given tidal
     *  resource. Implements multiple power curves.
     */
     
    // first, check if no resource
    if (tidal_resource_ms <= 0) {
        return 0;
    }
    
    // otherwise, apply selected power curve
    double production = 0;
    double turbine_speed = 0;
    
    switch(this->struct_tidal.power_curve) {
        case (CUBIC): {
            /*
             *  ref: B. Buckham, C. Crawford, I. Beya Marshall, and
             *       B. Whitby, "Wei Wai Kum Tidal Prefeasibility
             *       Study - Tidal Resource Assessment", PRIMED
             *       technical report, 2023,
             *       P2202E_BRKLYG+WEI WAI KUM_R01_ V20230613v3
             */
            
            if (
                tidal_resource_ms < 0.15 * this->struct_tidal.design_speed_ms ||
                tidal_resource_ms > 1.25 * this->struct_tidal.design_speed_ms
            ){
                production = 0;
            }
            
            else if (
                0.15 * this->struct_tidal.design_speed_ms <= tidal_resource_ms &&
                tidal_resource_ms <= this->struct_tidal.design_speed_ms
            ) {
                production = 
                    (1 / pow(this->struct_tidal.design_speed_ms, 3)) *
                    pow(tidal_resource_ms, 3);
            }
            
            else {
                production = 1;
            }
            
            break;
        }
        
        case (EXPONENTIAL): {
            /*
             *  ref: docs/wind_tidal_wave.pdf
             */
            
            turbine_speed =
                (tidal_resource_ms - this->struct_tidal.design_speed_ms) /
                this->struct_tidal.design_speed_ms;
                
            if (turbine_speed < -0.71 || turbine_speed > 0.65) {
                production = 0;
            }
            
            else if (turbine_speed >= -0.71 && turbine_speed <= 0) {
                production = 1.69215 * exp(1.25909 * turbine_speed) - 0.69215;
            }
            
            else {
                production = 1;
            }
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    double production_kW = this->struct_nondisp.cap_kW * production;
    return production_kW;
}


//...


Tidal :: ~Tidal() {
    /*
     *  Tidal class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tTidal object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
