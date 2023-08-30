/*
 *  Nondispatchable class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"


Nondispatchable :: Nondispatchable(
    structNondispatchable struct_nondisp
) {
    /*
     *  NonDispatchable class constructor
     */
    
    this->struct_nondisp = struct_nondisp;
    
    this->production_vec_kW.resize(this->struct_nondisp.n_timesteps, 0);
    this->dispatch_vec_kW.resize(this->struct_nondisp.n_timesteps, 0);
    this->curtailment_vec_kW.resize(this->struct_nondisp.n_timesteps, 0);
    this->storage_vec_kW.resize(this->struct_nondisp.n_timesteps, 0);
    
    this->is_running_vec.resize(this->struct_nondisp.n_timesteps, false);
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tNondispatchable object constructed at " << this
            << std::endl;
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
