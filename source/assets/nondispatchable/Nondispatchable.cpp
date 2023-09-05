/*
 *  Nondispatchable class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"


Nondispatchable :: Nondispatchable(
    structNondispatchable struct_nondisp
) {
    /*
     *  Nondispatchable class constructor
     */
    
    this->struct_nondisp = struct_nondisp;
    
    this->is_running_vec.resize(this->struct_nondisp.n_timesteps, false);
    this->replaced_vec.resize(this->struct_nondisp.n_timesteps, false);
    
    this->production_vec_kW.resize(this->struct_nondisp.n_timesteps, 0);
    this->dispatch_vec_kW.resize(this->struct_nondisp.n_timesteps, 0);
    this->curtailment_vec_kW.resize(this->struct_nondisp.n_timesteps, 0);
    this->storage_vec_kW.resize(this->struct_nondisp.n_timesteps, 0);
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tNondispatchable object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Nondispatchable :: _handleReplacement(int timestep) {
    /*
     *  Helper method to handle running hours induced replacement
     */
    
    // reset attributes (replace with fresh Nondispatchable system)
    this->struct_nondisp.is_running = false;
    
    // incur capital cost
    //...
    
    // record replacements
    this->struct_nondisp.n_replacements++;
    this->replaced_vec[timestep] = true;
    
    return;
}


void Nondispatchable :: commitProductionkW(
    double production_kW,
    double dt_hrs,
    int timestep
) {
    /*
     *  Method to commit to given production
     */
    
    // track and record running state
    if (production_kW > 0) {
        this->struct_nondisp.is_running = true;
        this->is_running_vec[timestep] = true;
    }
    
    else {
        this->struct_nondisp.is_running = false;
    }
    
    // record production
    this->production_vec_kW[timestep] = production_kW;
    
    // incremet running hours
    if (this->struct_nondisp.is_running) {
        this->struct_nondisp.running_hrs += dt_hrs;
    }
    
    // trigger replacement, if necessary
    if (
        this->struct_nondisp.running_hrs >=
        (this->struct_nondisp.n_replacements + 1) *
        this->struct_nondisp.replace_running_hrs
    ) {
        this->_handleReplacement(timestep);
    }
    
    return;
}


double Nondispatchable :: getDispatchkW(
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


Nondispatchable :: ~Nondispatchable() {
    /*
     *  Nondispatchable class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tNondispatchable object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
