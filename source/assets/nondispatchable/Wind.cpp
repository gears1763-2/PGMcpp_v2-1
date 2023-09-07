/*
 *  Nondispatchable <-- Wind class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Wind.h"


Wind :: Wind(
    structNondispatchable struct_nondisp,
    structWind struct_wind
) : Nondispatchable(struct_nondisp) {
    /*
     *  Wind class constructor
     */
    
    this->struct_nondisp.nondisp_type = WIND;
    this->struct_nondisp.nondisp_type_str = "WIND";
    this->struct_wind = struct_wind;
    
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWind object constructed at " << this
            << std::endl;
    }
    
    return;
}


double Wind :: getProductionkW(double wind_resource_ms) {
    /*
     *  Method to compute and return production under given wind
     *  resource.
     */
    
    // first, check if no resource
    if (wind_resource_ms <= 0) {
        return 0;
    }
    
    // otherwise, apply power curve
    /*
     *  ref: docs/wind_tidal_wave.pdf
     */
    double production = 0;
    double turbine_speed = 0;
    
    turbine_speed = (wind_resource_ms - this->struct_wind.design_speed_ms) /
        this->struct_wind.design_speed_ms;
        
    if (turbine_speed < -0.76 || turbine_speed > 0.68) {
        production = 0;
    }
    
    else if (turbine_speed >= -0.76 && turbine_speed <= 0) {
        production = 1.03273 * exp(-5.97588 * pow(turbine_speed, 2)) -
            0.03273;
    }
    
    else {
        production = 0.16154 * exp(-9.30254 * pow(turbine_speed, 2)) +
            0.83846;
    }
    
    double production_kW = this->struct_nondisp.cap_kW * production;
    return production_kW;
}


//...


Wind :: ~Wind() {
    /*
     *  Wind class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWind object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
