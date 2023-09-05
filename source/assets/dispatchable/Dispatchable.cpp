/*
 *  Dispatchable class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/dispatchable/Dispatchable.h"


Dispatchable :: Dispatchable(
    structDispatchable struct_disp
) {
    /*
     *  Dispatchable class constructor
     */
    
    this->struct_disp = struct_disp;
    
    this->is_running_vec.resize(this->struct_disp.n_timesteps, false);
    this->replaced_vec.resize(this->struct_disp.n_timesteps, false);
    
    this->production_vec_kW.resize(this->struct_disp.n_timesteps, 0);
    this->dispatch_vec_kW.resize(this->struct_disp.n_timesteps, 0);
    this->curtailment_vec_kW.resize(this->struct_disp.n_timesteps, 0);
    this->storage_vec_kW.resize(this->struct_disp.n_timesteps, 0);
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tDispatchable object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Dispatchable :: _handleReplacement(int timestep) {
    /*
     *  Helper method to handle running hours induced replacement
     */
    
    // reset attributes (replace with fresh Dispatchable system)
    this->struct_disp.is_running = false;
    
    // incur capital cost
    //...
    
    // record replacements
    this->struct_disp.n_replacements++;
    this->replaced_vec[timestep] = true;
    
    return;
}


void Dispatchable :: commitProductionkW(
    double production_kW,
    double dt_hrs,
    int timestep
) {
    /*
     *  Method to commit to given production
     */
    
    // track and record running state
    if (production_kW > 0) {
        this->struct_disp.is_running = true;
        this->is_running_vec[timestep] = true;
    }
    
    else {
        this->struct_disp.is_running = false;
    }
    
    // record production
    this->production_vec_kW[timestep] = production_kW;
    
    // incremet running hours
    if (this->struct_disp.is_running) {
        this->struct_disp.running_hrs += dt_hrs;
    }
    
    // trigger replacement, if necessary
    if (
        this->struct_disp.running_hrs >=
        (this->struct_disp.n_replacements + 1) *
        this->struct_disp.replace_running_hrs
    ) {
        this->_handleReplacement(timestep);
    }
    
    return;
}


double Dispatchable :: getDispatchkW(
    double load_kW,
    double production_kW
) {
    /*
     *  Method to compute and return dispatch
     */
    
    // check for zero load
    if (load_kW <= 0) {
        return 0;
    }
    
    // check for zero production
    if (production_kW <= 0) {
        return 0;
    }
    
    double dispatch_kW = load_kW;
    
    // check against production
    if (dispatch_kW > production_kW) {
        dispatch_kW = production_kW;
    }
    
    return dispatch_kW;
}


Dispatchable :: ~Dispatchable() {
    /*
     *  Dispatchable class destructor
     */
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tDispatchable object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
