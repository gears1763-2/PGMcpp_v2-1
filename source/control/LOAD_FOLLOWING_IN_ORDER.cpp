/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Implementation file for the LOAD_FOLLOWING_IN_ORDER dispatch control
 *  mode.
 */

void Model :: _dispatchLoadFollowingInOrderCharging(int timestep) {
    /*
     *  Helper method to handle load following in order dispatch, under
     *  storage charging, for a single timestep
     *
     *  In this mode, net load <= 0.
     */
    
    double dt_hrs = this->dt_vec_hr[timestep];
    
    // request zero production from all non-Combustion assets
    this->_controlNoncombustion(timestep, 0, dt_hrs);
    
    // request zero production from all Combustion assets
    this->_controlCombustion(timestep, 0, false);
    
    // attempt to charge all Storage assets
    this->_chargeStorage(
        timestep,
        dt_hrs,
        this->storage_ptr_vec
    );
    
    return;
}


void Model :: _dispatchLoadFollowingInOrderDischarging(int timestep) {
    /*
     *  Helper method to handle load following in order dispatch, under 
     *  storage discharging, for a single timestep
     *
     *  In this mode, net load > 0.
     */
    
    double dt_hrs = this->dt_vec_hr[timestep];
    double load_kW = this->net_load_vec_kW[timestep];
    
    // partition Storage assets into depleted and non-depleted
    std::vector<Storage*> depleted_storage_ptr_vec = this->_getDepletedStorage();
    std::vector<Storage*> nondepleted_storage_ptr_vec = this->_getNondepletedStorage();
    
    // discharge all non-depleted Storage assets, update load
    load_kW = this->_dischargeStorage(
        timestep,
        load_kW,
        dt_hrs,
        nondepleted_storage_ptr_vec
    );
    
    // request sufficient production from all non-Combustion assets, update load
    load_kW = this->_controlNoncombustion(timestep, load_kW, dt_hrs);
    
    // request sufficient production from all Combustion assets, update load
    load_kW = this->_controlCombustion(timestep, load_kW, false);
    
    // attempt to charge all depleted Storage assets
    this->_chargeStorage(
        timestep,
        dt_hrs,
        depleted_storage_ptr_vec
    );
    
    // track unmet load
    if (load_kW > 0) {
        this->remaining_load_vec_kW[timestep] = load_kW;
    }
    
    return;
}
