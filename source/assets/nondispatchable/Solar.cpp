/*
 *  Nondispatchable <-- Solar class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Solar.h"


Solar :: Solar(
    structNondispatchable struct_nondisp,
    structSolar struct_solar
) : Nondispatchable(struct_nondisp) {
    /*
     *  Solar class constructor
     */
    
    this->struct_nondisp.nondisp_type = SOLAR;
    this->struct_nondisp.nondisp_type_str = "SOLAR";
    this->struct_solar = struct_solar;
    
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tSolar object constructed at " << this
            << std::endl;
    }
    
    return;
}


double Solar :: getProductionkW(double solar_resource_kWm2) {
    /*
     *  Method to compute and return production under given solar
     *  resource. Temperature effects are not modelled.
     * 
     *  ref: https://www.homerenergy.com/products/pro/docs/3.11/how_homer_calculates_the_pv_array_power_output.html
     */
     
    // first, check if no resource 
    if (solar_resource_kWm2 <= 0) {
        return 0;
    } 
    
    // otherwise, simple linear model
    double production_kW = this->struct_solar.derating *
        this->struct_nondisp.cap_kW * solar_resource_kWm2;
    return production_kW;
}


//...


Solar :: ~Solar() {
    /*
     *  Solar class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tSolar object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
