/*
 *  Dispatchable <-- Combustion class implementation file
 */


#include "../../../../header/includes.h"
#include "../../../../header/assets/dispatchable/Dispatchable.h"
#include "../../../../header/assets/dispatchable/combustion/Combustion.h"


Combustion :: Combustion(
    structDispatchable struct_disp,
    structCombustion struct_combustion
) : Dispatchable(struct_disp) {
    /*
     *  Combustion class constructor
     */
    
    this->struct_combustion = struct_combustion;
    
    this->CO2_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    this->CO_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    this->NOx_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    this->SOx_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    this->CH4_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    this->PM_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tCombustion object constructed at " << this
            << std::endl;
    }
    
    return;
}


double Combustion :: getFuelConsumptionL(
    double production_kW,
    double dt_hrs
) {
    /*
     *  Method to compute and return fuel consumption under given
     *  production and time step
     */
    
    // check running state
    if (not this->struct_disp.is_running) {
        return 0;
    }
    
    double fuel_consumption_L = 0;
    
    switch (this->struct_combustion.fuel_mode) {
        case (LOOKUP): {
            //...
            
            break;
        }
        
        default: {  // default to LINEAR
            /*
             *  Fuel consumption over time step based on linearized fuel
             *  consumption curve (i.e. HOMER-like consumption model)
             *
             *  ref: https://www.homerenergy.com/products/pro/docs/latest/fuel_curve.html
             *  ref: https://www.homerenergy.com/products/pro/docs/latest/generator_fuel_curve_intercept_coefficient.html
             *  ref: https://www.homerenergy.com/products/pro/docs/latest/generator_fuel_curve_slope.html
             */
            
            fuel_consumption_L =
                (this->struct_combustion.linear_fuel_intercept_LkWh *
                this->struct_disp.cap_kW +
                this->struct_combustion.linear_fuel_slope_LkWh *
                production_kW) * dt_hrs;
            
            break;
        }
    }
    
    return fuel_consumption_L;
}


structEmissions Combustion :: getEmissions(double fuel_consumption_L) {
    /*
     *  Method to compute and return emissions under given fuel
     *  consumption
     */
    
    structEmissions struct_emissions;
    
    switch (this->struct_combustion.fuel_type) {
        case (FUEL_DIESEL): {
            struct_emissions.CO2_kg =
                this->struct_combustion.diesel_CO2_kgL * fuel_consumption_L;
            
            struct_emissions.CO_kg =
                this->struct_combustion.diesel_CO_kgL * fuel_consumption_L;
            
            struct_emissions.NOx_kg =
                this->struct_combustion.diesel_NOx_kgL * fuel_consumption_L;
            
            struct_emissions.SOx_kg =
                this->struct_combustion.diesel_SOx_kgL * fuel_consumption_L;
            
            struct_emissions.CH4_kg =
                this->struct_combustion.diesel_CH4_kgL * fuel_consumption_L;
                
            struct_emissions.PM_kg =
                this->struct_combustion.diesel_PM_kgL * fuel_consumption_L;
            
            break;
        }
        
        case (FUEL_GAS): {
            //...
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    return struct_emissions;
}


void Combustion :: recordEmissions(
    structEmissions struct_emissions,
    int timestep
) {
    /*
     *  Method to record emissions over time step
     */
    
    this->CO2_vec_kg[timestep] = struct_emissions.CO2_kg;
    this->CO_vec_kg[timestep] = struct_emissions.CO_kg;
    this->NOx_vec_kg[timestep] = struct_emissions.NOx_kg;
    this->SOx_vec_kg[timestep] = struct_emissions.SOx_kg;
    this->CH4_vec_kg[timestep] = struct_emissions.CH4_kg;
    this->PM_vec_kg[timestep] = struct_emissions.PM_kg;
    
    return;
}


Combustion :: ~Combustion() {
    /*
     *  Combustion class destructor
     */
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tCombustion object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
