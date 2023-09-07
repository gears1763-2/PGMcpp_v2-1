/*
 *  Dispatchable <-- Combustion <-- Diesel class implementation file
 */


#include "../../../../header/includes.h"
#include "../../../../header/assets/dispatchable/Dispatchable.h"
#include "../../../../header/assets/dispatchable/combustion/Combustion.h"
#include "../../../../header/assets/dispatchable/combustion/Diesel.h"


Diesel :: Diesel(
    structDispatchable struct_disp,
    structCombustion struct_combustion,
    structDiesel struct_diesel
) : Combustion(struct_disp, struct_combustion) {
    /*
     *  Diesel class constructor
     */
    
    this->struct_disp.disp_type = DIESEL;
    this->struct_disp.disp_type_str = "DIESEL";
    this->struct_diesel = struct_diesel;
    
    this->struct_combustion.fuel_type = FUEL_DIESEL;
    
    this->fuel_vec_L.resize(this->struct_disp.n_timesteps, 0);
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tDiesel object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Diesel :: commitProductionkW(
    double production_kW,
    double dt_hrs,
    int timestep
) {
    /*
     *  Method to commit to given production
     */
    
    // enforce minimum runtime, handle stopping
    if (
        this->struct_disp.is_running &&
        production_kW <= 0 &&
        this->struct_diesel.time_since_last_start_hrs >=
            this->struct_diesel.minimum_runtime_hrs
    ) {
        // stop, reset time since last start
        this->struct_disp.is_running = false;
        this->struct_diesel.time_since_last_start_hrs = 0;
    }
    
    // handle starting, track time since last start
    if (
        not(this->struct_disp.is_running) &&
        production_kW > 0
    ) {
        this->struct_disp.is_running = true;
    }
    
    if (this->struct_disp.is_running) {
        this->struct_diesel.time_since_last_start_hrs += dt_hrs;
    }
    
    Dispatchable::commitProductionkW(production_kW, dt_hrs, timestep);
    
    return;
}


double Diesel :: requestProductionkW(double requested_production_kW) {
    /*
     *  Method to handle production requests (subject to active
     *  operating constraints) and return provided production
     */
    
    double production_kW = requested_production_kW;
    
    // enforce minimum load ratio
    if (
        production_kW > 0 &&
        production_kW < this->struct_diesel.minimum_load_ratio * 
            this->struct_disp.cap_kW
    ) {
        production_kW = this->struct_diesel.minimum_load_ratio * 
            this->struct_disp.cap_kW;
    }
    
    // check against capacity
    if (production_kW > this->struct_disp.cap_kW) {
        production_kW = this->struct_disp.cap_kW;
    }
    
    //...
    
    return production_kW;
}


double Diesel :: getFuelConsumptionL(
    double production_kW,
    double dt_hrs
) {
    /*
     *  Method to compute and return fuel consumption under given
     *  production and time step
     */
    
    //...
    
    return 0;
}


Diesel :: ~Diesel() {
    /*
     *  Diesel class destructor
     */
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tDiesel object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
