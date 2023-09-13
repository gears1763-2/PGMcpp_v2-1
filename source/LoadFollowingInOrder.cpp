/*
 *  Implementation file for LOAD_FOLLOWING_IN_ORDER dispatch control
 */

void Model :: _dispatchLoadFollowingInOrderCharging(int timestep) {
    /*
     *  Helper method to handle load following in order dispatch, under
     *  storage charging, for a single timestep
     */
    
    double dt_hrs = this->dt_vec_hr[timestep];
    double t_hrs = this->time_vec_hr[timestep];
    
    // for all combustion assets, request zero production and commit,
    // record production, dispatch, and curtailment
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        // request zero production and commit
        Combustion* combustion_ptr = this->combustion_ptr_vec[i];
        
        double production_kW = combustion_ptr->requestProductionkW(0);
        
        combustion_ptr->commitProductionkW(production_kW, timestep);
        
        // record dispatch (remaining load is <= 0, in this case)
        double dispatch_kW = combustion_ptr->getDispatchkW(
            0,
            production_kW
        );
        combustion_ptr->dispatch_vec_kW[timestep] = dispatch_kW;
        
        // record curtailment
        double curtailment_kW = production_kW - dispatch_kW;
        combustion_ptr->curtailment_vec_kW[timestep] = curtailment_kW;
    }
    
    // for all noncombustion assets, request zero production and commit,
    // record production, dispatch, and curtailment
    for (size_t i = 0; i < this->noncombustion_ptr_vec.size(); i++) {
        // request zero production and commit
        Dispatchable* noncombustion_ptr = this->noncombustion_ptr_vec[i];
        
        double production_kW = noncombustion_ptr->requestProductionkW(0);
        
        noncombustion_ptr->commitProductionkW(production_kW, timestep);
        
        // record dispatch (net load is <= 0, in this case)
        double dispatch_kW = noncombustion_ptr->getDispatchkW(
            0,
            production_kW
        );
        noncombustion_ptr->dispatch_vec_kW[timestep] = dispatch_kW;
        
        // record curtailment
        double curtailment_kW = production_kW - dispatch_kW;
        noncombustion_ptr->curtailment_vec_kW[timestep] = curtailment_kW;
    }
    
    // for all storage assets, compute and record acceptable power,
    // reset charging
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        Storage* storage_ptr = this->storage_ptr_vec[i];
        
        storage_ptr->acceptable_kW =
            storage_ptr->getAcceptablekW(dt_hrs);
        
        storage_ptr->charging_kW = 0;
    }
    
    // for all combustion assets, attempt to charge from curtailment,
    // update curtailment, and record storage
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        Combustion* combustion_ptr = this->combustion_ptr_vec[i];
        
        // if no curtailment, no energy to charge with!
        if (combustion_ptr->curtailment_vec_kW[timestep] <= 0) {
            continue;
        }
        
        // otherwise, for all storage assets, update storage charging,
        // combustion curtailment and storage
        for (size_t j = 0; j < this->storage_ptr_vec.size(); j++) {
            Storage* storage_ptr = this->storage_ptr_vec[j];
            
            double curtailment_kW =
                combustion_ptr->curtailment_vec_kW[timestep];
            
            double increment_kW =
                storage_ptr->acceptable_kW - 
                storage_ptr->charging_kW;
            
            if (curtailment_kW <= increment_kW) {
                increment_kW = curtailment_kW;
            }
            
            storage_ptr->charging_kW += increment_kW;
            combustion_ptr->curtailment_vec_kW[timestep] -= increment_kW;
            combustion_ptr->storage_vec_kW[timestep] += increment_kW;
        }
    }
    
    // for all noncombustion assets, attempt to charge from curtailment,
    // update curtailment, and record storage
    for (size_t i = 0; i < this->noncombustion_ptr_vec.size(); i++) {
        Dispatchable* noncombustion_ptr = this->noncombustion_ptr_vec[i];
        
        // if no curtailment, no energy to charge with!
        if (noncombustion_ptr->curtailment_vec_kW[timestep] <= 0) {
            continue;
        }
        
        // otherwise, for all storage assets, update storage charging,
        // noncombustion curtailment and storage
        for (size_t j = 0; j < this->storage_ptr_vec.size(); j++) {
            Storage* storage_ptr = this->storage_ptr_vec[j];
            
            double curtailment_kW =
                noncombustion_ptr->curtailment_vec_kW[timestep];
            
            double increment_kW =
                storage_ptr->acceptable_kW - 
                storage_ptr->charging_kW;
            
            if (curtailment_kW <= increment_kW) {
                increment_kW = curtailment_kW;
            }
            
            storage_ptr->charging_kW += increment_kW;
            noncombustion_ptr->curtailment_vec_kW[timestep] -= increment_kW;
            noncombustion_ptr->storage_vec_kW[timestep] += increment_kW;
        }
    }
    
    // for all nondispatchable assets, attempt to charge from curtailment,
    // update curtailment, and record storage
    for (size_t i = 0; i < this->nondisp_ptr_vec.size(); i++) {
        Nondispatchable* nondisp_ptr = this->nondisp_ptr_vec[i];
        
        // if no curtailment, no energy to charge with!
        if (nondisp_ptr->curtailment_vec_kW[timestep] <= 0) {
            continue;
        }
        
        // otherwise, for all storage assets, update storage charging,
        // nondispatchable curtailment and storage
        for (size_t j = 0; j < this->storage_ptr_vec.size(); j++) {
            Storage* storage_ptr = this->storage_ptr_vec[j];
            
            double curtailment_kW =
                nondisp_ptr->curtailment_vec_kW[timestep];
            
            double increment_kW =
                storage_ptr->acceptable_kW - 
                storage_ptr->charging_kW;
            
            if (curtailment_kW <= increment_kW) {
                increment_kW = curtailment_kW;
            }
            
            storage_ptr->charging_kW += increment_kW;
            nondisp_ptr->curtailment_vec_kW[timestep] -= increment_kW;
            nondisp_ptr->storage_vec_kW[timestep] += increment_kW;
        }
    }
    
    // for all storage assets, commit charge
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        Storage* storage_ptr = this->storage_ptr_vec[i];
        
        double charging_kW = storage_ptr->charging_kW;
        
        storage_ptr->commitChargekW(charging_kW, dt_hrs, t_hrs, timestep);
    }
    
    return;
}


void Model :: _dispatchLoadFollowingInOrderDischarging(int timestep) {
    /*
     *  Helper method to handle load following in order dispatch, under 
     *  storage discharging, for a single timestep
     */
    
    double dt_hrs = this->dt_vec_hr[timestep];
    double t_hrs = this->time_vec_hr[timestep];
    double load_kW = this->net_load_vec_kW[timestep];
    
    // for all storage assets, get available, discharge up to load, 
    // commit, and update load
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        // get available power
        Storage* storage_ptr = this->storage_ptr_vec[i];
        double discharging_kW = storage_ptr->getAvailablekW(dt_hrs);
        
        // discharge up to load
        if (discharging_kW >= load_kW) {
            discharging_kW = load_kW;
        }
        
        if (discharging_kW <= 0) {
            discharging_kW = 0;
        }
        
        // commit
        storage_ptr->commitDischargekW(
            discharging_kW, dt_hrs, t_hrs, timestep
        );
        
        // update load
        load_kW -= discharging_kW;
    }
    
    // for all noncombustion assets, request production and commit,
    // record production, dispatch, and curtailment, and update load
    for (size_t i = 0; i < this->noncombustion_ptr_vec.size(); i++) {
        // request production and commit
        Dispatchable* noncombustion_ptr = this->noncombustion_ptr_vec[i];
        
        double production_kW = noncombustion_ptr->requestProductionkW(load_kW);
        
        noncombustion_ptr->commitProductionkW(production_kW, timestep);
        
        // record dispatch
        double dispatch_kW = noncombustion_ptr->getDispatchkW(
            load_kW,
            production_kW
        );
        noncombustion_ptr->dispatch_vec_kW[timestep] = dispatch_kW;
        
        // record curtailment
        double curtailment_kW = production_kW - dispatch_kW;
        noncombustion_ptr->curtailment_vec_kW[timestep] = curtailment_kW;
        
        // update load 
        load_kW -= dispatch_kW;
    }
    
    // for all combustion assets, request production and commit,
    // record production, dispatch, and curtailment, and update load
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        // request production and commit
        Combustion* combustion_ptr = this->combustion_ptr_vec[i];
        
        double production_kW = combustion_ptr->requestProductionkW(load_kW);
        
        combustion_ptr->commitProductionkW(production_kW, timestep);
        
        // record dispatch
        double dispatch_kW = combustion_ptr->getDispatchkW(
            load_kW,
            production_kW
        );
        combustion_ptr->dispatch_vec_kW[timestep] = dispatch_kW;
        
        // record curtailment
        double curtailment_kW = production_kW - dispatch_kW;
        combustion_ptr->curtailment_vec_kW[timestep] = curtailment_kW;
        
        // update load 
        load_kW -= dispatch_kW;
    }
    
    // if remaining load >= 0, record
    if (load_kW >= 0) {
        this->remaining_load_vec_kW[timestep] = load_kW;
    }
    
    return;
}
