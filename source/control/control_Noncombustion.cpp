/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Implementation file for various functions for controlling the non-Combustion assets.
 */


double Model :: _controlNoncombustion(
    int timestep,
    double _load_kW
) {
    /*
     *  Helper method to handle the control of non-Combustion assets.
     *
     *  Returns updated load value.
     */
    
    double load_kW = _load_kW;
    
    // for all non-Combustion assets, request production and commit,
    // record production, dispatch, and curtailment, update load
    for (size_t i = 0; i < this->noncombustion_ptr_vec.size(); i++) {
        // request production and commit
        Dispatchable* noncombustion_ptr = this->noncombustion_ptr_vec[i];
        
        double production_kW = 0;
        switch (noncombustion_ptr->disp_type) {
            case (HYDRO): {
                int resource_key =
                    ((Hydro*)noncombustion_ptr)->resource_key;
            
                double hydro_resource_m3hr = this->resource_map_1D[
                    resource_key
                ][timestep];
                
                production_kW =
                    noncombustion_ptr->requestProductionkW(
                        load_kW,
                        hydro_resource_m3hr,
                        timestep
                    );
                
                break;
            }
            
            default: {
                production_kW =
                    noncombustion_ptr->requestProductionkW(load_kW);
                
                break;
            }
        }
        
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
     
    return load_kW;
}
