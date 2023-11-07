/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Implementation file for the CYCLE_CHARGING_IN_ORDER dispatch control
 *  mode.
 */

void Model :: _dispatchCycleChargingInOrderCharging(int timestep) {
    /*
     *  Helper method to handle cycle charging in order dispatch, under
     *  storage charging, for a single timestep
     *
     *  In this mode, net load <= 0.
     */
    
    // partition Storage assets into depleted and non-depleted
    std::vector<Storage*> depleted_storage_ptr_vec = this->_getDepletedStorage();
    std::vector<Storage*> nondepleted_storage_ptr_vec = this->_getNondepletedStorage();
    
    // if no depleted Storage assets, default to LOAD_FOLLOWING_IN_ORDER
    if (depleted_storage_ptr_vec.empty()) {
        this->_dispatchLoadFollowingInOrderCharging(timestep);
        return;
    }

    // rescind energy reserve access
    this->_toggleReserve(false);
    
    // request zero production from all non-Combustion assets
    this->_controlNoncombustion(timestep, 0);
    
    // request zero production from all Combustion assets, cycle_charging = true
    this->_controlCombustion(timestep, 0, true);
    
    // attempt to charge all Storage assets
    this->_chargeStorage(timestep, this->storage_ptr_vec);
    
    return;
}


void Model :: _dispatchCycleChargingInOrderDischarging(int timestep) {
    /*
     *  Helper method to handle cycle charging in order dispatch, under 
     *  storage discharging, for a single timestep
     *
     *  In this mode, net load > 0.
     */
    
    double load_kW = this->net_load_vec_kW[timestep];
    
    // partition Storage assets into depleted and non-depleted
    std::vector<Storage*> depleted_storage_ptr_vec = this->_getDepletedStorage();
    std::vector<Storage*> nondepleted_storage_ptr_vec = this->_getNondepletedStorage();
    
    // if no depleted Storage assets, default to LOAD_FOLLOWING_IN_ORDER
    if (depleted_storage_ptr_vec.empty()) {
        this->_dispatchLoadFollowingInOrderDischarging(timestep);
        return;
    }
    
    // check if sufficient production and storage
    // if not, permit energy reserve access
    if (not this->_sufficientProductionStorage(load_kW, timestep)) {
        this->_toggleReserve(true);
    }
    
    // discharge all non-depleted Storage assets, update load
    load_kW = this->_dischargeStorage(timestep, load_kW, nondepleted_storage_ptr_vec);
    
    // request sufficient production from all non-Combustion assets, update load
    load_kW = this->_controlNoncombustion(timestep, load_kW);
    
    // request sufficient production from all Combustion assets, update load
    // cycle_charging = true
    load_kW = this->_controlCombustion(timestep, load_kW, true);
    
    // attempt to charge all depleted Storage assets
    this->_chargeStorage(timestep, depleted_storage_ptr_vec);
    
    // track unmet load
    if (load_kW > 0) {
        this->remaining_load_vec_kW[timestep] = load_kW;
    }
    
    return;
}
