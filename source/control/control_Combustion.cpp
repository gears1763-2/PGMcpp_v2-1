/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Implementation file for various functions for controlling the Combustion assets.
 */


double Model :: _controlCombustion(
    int timestep,
    double _load_kW,
    bool cycle_charging
) {
    /*
     *  Helper method to handle the control of Combustion assets.
     *  Returns updated load value.
     */
    
    double load_kW = _load_kW;
    
    double dt_hrs = 0;
    if (timestep > 0) {
        // previous dt_hrs for checking ramp rate constraints
        dt_hrs = this->dt_vec_hr[timestep - 1];
    }
     
    // for all Combustion assets, request production and commit,
    // record production, dispatch, and curtailment, update load
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        // request production and commit
        Combustion* combustion_ptr = this->combustion_ptr_vec[i];
        
        double production_kW = 0;
        if (cycle_charging) {
            double request_kW =
                combustion_ptr->struct_combustion.cycle_charging_load_ratio *
                combustion_ptr->struct_disp.cap_kW;
            
            if (load_kW <= 0) {
                request_kW = 0;
            }
            else if (request_kW < load_kW) {
                request_kW = load_kW;
            }
            
            production_kW = combustion_ptr->requestProductionkW(request_kW, timestep);
        }
        
        else {
            production_kW = combustion_ptr->requestProductionkW(load_kW, timestep);
        }
        
        combustion_ptr->commitProductionkW(production_kW, timestep);
        
        // record dispatch
        double dispatch_kW = combustion_ptr->getDispatchkW(load_kW, production_kW);
        combustion_ptr->dispatch_vec_kW[timestep] = dispatch_kW;
        
        // record curtailment
        double curtailment_kW = production_kW - dispatch_kW;
        combustion_ptr->curtailment_vec_kW[timestep] = curtailment_kW;
        
        // update load 
        load_kW -= dispatch_kW;
    }
    
    return load_kW;
}
