/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Implementation file for the Hydro class, which is a child class of
 *  Dispatchable.
 * 
 *  This class implements the dynamics of a hydro electric asset, be it
 *  with (i.e., traditional) or without (i.e., run-of-river) fluid
 *  storage.
 * 
 *  *** WORK IN PROGRESS - NOT YET FULLY IMPLEMENTED ***
 */


#include "../../../header/includes.h"
#include "../../../header/assets/dispatchable/Dispatchable.h"
#include "../../../header/assets/dispatchable/Hydro.h"


Hydro :: Hydro(
    structDispatchable struct_disp,
    structHydro struct_Hydro,
    int n_timesteps
) : Dispatchable(struct_disp, n_timesteps) {
    /*
     *  Hydro class constructor
     */
    
    this->disp_type = HYDRO;
    this->disp_type_str = "HYDRO";
    this->struct_hydro = struct_hydro;
    
    this->resource_key = struct_hydro.resource_key;
        
    this->reservoir_capacity_m3 = struct_hydro.reservoir_capacity_m3;
    this->reservoir_init_state = struct_hydro.reservoir_init_state;
    this->reservoir_volume_m3 =
        this->reservoir_capacity_m3 * this->reservoir_init_state;
    this->minimum_discharge_m3hr = struct_hydro.minimum_discharge_m3hr;
    
    this->reservoir_volume_vec_m3.resize(this->n_timesteps, 0);
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tHydro object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Hydro :: _writeSummary(std::string _write_path, int asset_idx) {
    /*
     *  Helper method to write Hydro-level summary
     */
    
    // construct filename 
    std::string filename = "non-Combustion/" +
        std::to_string(int(this->struct_disp.cap_kW)) +
        "kW_" + this->disp_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_disp.cap_kW)) +
        "kW_" + this->disp_type_str +
        "_" + std::to_string(asset_idx) +
        "_summary.txt";
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + filename);
    
    // write attributes
    //...
    
    // write results
    //...
    
    ofs.close();
    
    return;
}


void Hydro :: commitProductionkW(
    double production_kW,
    int timestep
) {
    /*
     *  Method to commit to given production
     */
    
    // call out to base method
    Dispatchable::commitProductionkW(production_kW, timestep);
    
    // handle hydro storage
    //...
    
    return;
}


double Hydro :: requestProductionkW(
    double requested_production_kW,
    double hydro_resource_m3hr,
    int timestep
) {
    /*
     *  Method to handle production requests (subject to active
     *  operating constraints) and return provided production
     */
    
    // check against hydro resource
    /*
    double dt_hrs = this->ptr_2_dt_vec_hr->at(timestep);
    double available_flow_m3hr = hydro_resource_m3hr -
        this->minimum_discharge_m3hr +
        this->reservoir_volume_m3 / dt_hrs;
    */
    
    return 0;
}


void Hydro :: writeResults(
    std::string _write_path,
    int asset_idx
) {
    /*
     *  Method to write Hydro-level results
     */
    
    Dispatchable::_writeTimeSeriesResults(_write_path, asset_idx);
    this->_writeSummary(_write_path, asset_idx);
    
    return;
}


Hydro :: ~Hydro() {
    /*
     *  Hydro class destructor
     */
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tHydro object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
