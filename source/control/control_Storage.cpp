/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Implementation file for various functions for controlling the Storage assets.
 */


std::vector<Storage*> Model :: _getDepletedStorage() {
    /*
     *  Helper method to return a vector of pointers to all depleted Storage assets.
     */
    
    std::vector<Storage*> depleted_storage_ptr_vec;
    
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        Storage* storage_ptr = this->storage_ptr_vec[i];
        if (storage_ptr->depleted_flag) {
            depleted_storage_ptr_vec.push_back(storage_ptr);
        }
    }
    
    return depleted_storage_ptr_vec;
}


std::vector<Storage*> Model :: _getNondepletedStorage() {
    /*
     *  Helper method to return a vector of pointers to all non-depleted Storage assets.
     */
    
    std::vector<Storage*> nondepleted_storage_ptr_vec;
    
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        Storage* storage_ptr = this->storage_ptr_vec[i];
        if (not storage_ptr->depleted_flag) {
            nondepleted_storage_ptr_vec.push_back(storage_ptr);
        }
    }
    
    return nondepleted_storage_ptr_vec;
}


double Model :: _dischargeStorage(
    int timestep,
    double _load_kW,
    double dt_hrs,
    std::vector<Storage*> storage_ptr_vec
) {
    /*
     *  Helper method to handle the discharging of all given Storage assets.
     *
     *  Returns updated load value.
     */
    
    double load_kW = _load_kW;
    
    for (size_t i = 0; i < storage_ptr_vec.size(); i++) {
        // get available power
        Storage* storage_ptr = this->storage_ptr_vec[i];
        double discharging_kW = storage_ptr->getAvailablekW(dt_hrs);
        
        // discharge up to load
        if (discharging_kW >= load_kW) {
            discharging_kW = load_kW;
        }
        
        // commit
        storage_ptr->commitDischargekW(discharging_kW, timestep);
        
        // update load
        load_kW -= discharging_kW;
    }
    
    return load_kW;
}


void Model :: _chargeStorageFromCombustion(
    int timestep,
    double dt_hrs,
    std::vector<Storage*> storage_ptr_vec
) {
    /*
     *  Helper method to handle the charging of all given Storage assets using any
     *  available overproduction from the Combustion assets.
     */
    
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        Combustion* combustion_ptr = this->combustion_ptr_vec[i];
        
        // if no curtailment, no energy to charge with!
        if (combustion_ptr->curtailment_vec_kW[timestep] <= 0) {
            continue;
        }
        
        // otherwise, for all given Storage assets, update Storage charging,
        // combustion curtailment and storage
        for (size_t j = 0; j < storage_ptr_vec.size(); j++) {
            Storage* storage_ptr = storage_ptr_vec[j];
            
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
    
    return;
}


void Model :: _chargeStorageFromNoncombustion(
    int timestep,
    double dt_hrs,
    std::vector<Storage*> storage_ptr_vec
) {
    /*
     *  Helper method to handle the charging of all given Storage assets using any
     *  available overproduction from the non-Combustion assets.
     */
    
    for (size_t i = 0; i < this->noncombustion_ptr_vec.size(); i++) {
        Dispatchable* noncombustion_ptr = this->noncombustion_ptr_vec[i];
        
        // if no curtailment, no energy to charge with!
        if (noncombustion_ptr->curtailment_vec_kW[timestep] <= 0) {
            continue;
        }
        
        // otherwise, for all storage assets, update storage charging,
        // noncombustion curtailment and storage
        for (size_t j = 0; j < storage_ptr_vec.size(); j++) {
            Storage* storage_ptr = storage_ptr_vec[j];
            
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
    
    return;
}


void Model :: _chargeStorageFromNondispatchable(
    int timestep,
    double dt_hrs,
    std::vector<Storage*> storage_ptr_vec
) {
    /*
     *  Helper method to handle the charging of all given Storage assets using any
     *  available overproduction from the Nondispatchable assets.
     */
    
    for (size_t i = 0; i < this->nondisp_ptr_vec.size(); i++) {
        Nondispatchable* nondisp_ptr = this->nondisp_ptr_vec[i];
        
        // if no curtailment, no energy to charge with!
        if (nondisp_ptr->curtailment_vec_kW[timestep] <= 0) {
            continue;
        }
        
        // otherwise, for all storage assets, update storage charging,
        // nondispatchable curtailment and storage
        for (size_t j = 0; j < storage_ptr_vec.size(); j++) {
            Storage* storage_ptr = storage_ptr_vec[j];
            
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
    
    return;
}


void Model :: _chargeStorage(
    int timestep,
    double dt_hrs,
    std::vector<Storage*> storage_ptr_vec
) {
    /*
     *  Helper method to handle the charging of all given Storage assets using any
     *  available overproduction.
     */
    
    // for all given Storage assets, compute and record acceptable power,
    // reset charging
    for (size_t i = 0; i < storage_ptr_vec.size(); i++) {
        Storage* storage_ptr = storage_ptr_vec[i];
        
        storage_ptr->acceptable_kW =
            storage_ptr->getAcceptablekW(dt_hrs);
        
        storage_ptr->charging_kW = 0;
    }
    
    // charge from Combustion overproduction
    this->_chargeStorageFromCombustion(timestep, dt_hrs, storage_ptr_vec);
    
    // charge from non-Combustion overproduction
    this->_chargeStorageFromNoncombustion(timestep, dt_hrs, storage_ptr_vec);
    
    // chrage from Nondispatchable overproduction
    this->_chargeStorageFromNondispatchable(timestep, dt_hrs, storage_ptr_vec);
    
    // commit charge
    for (size_t i = 0; i < storage_ptr_vec.size(); i++) {
        Storage* storage_ptr = storage_ptr_vec[i];
        storage_ptr->commitChargekW(storage_ptr->charging_kW, timestep);
    }
    
    return;
}
