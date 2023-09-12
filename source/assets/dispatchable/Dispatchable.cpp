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
    
    this->real_capital_cost_vec.resize(this->struct_disp.n_timesteps, 0);
    this->real_op_maint_cost_vec.resize(this->struct_disp.n_timesteps, 0);
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tDispatchable object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Dispatchable :: _handleReplacement(int timestep, double t_hrs) {
    /*
     *  Helper method to handle running hours induced replacement
     */
    
    // reset attributes (replace with fresh Dispatchable system)
    this->struct_disp.is_running = false;
    
    // incur capital cost
    /*
     *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
     *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
     */
    double real_discount_scalar = 1.0 / pow(
        1 + this->struct_disp.real_discount_rate_annual,
        t_hrs / 8760
    );
    
    this->real_capital_cost_vec[timestep] = real_discount_scalar *
        this->struct_disp.capital_cost;
    
    // record replacements
    this->struct_disp.n_replacements++;
    this->replaced_vec[timestep] = true;
    
    return;
}


void Dispatchable :: _writeTimeSeriesResults(
    std::string _write_path,
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Helper method to write Dispatchable-level time series results
     */
    
    // construct filename 
    std::string filename = "non-Combustion/" +
        std::to_string(int(this->struct_disp.cap_kW)) +
        "kW_" + this->struct_disp.disp_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_disp.cap_kW)) +
        "kW_" + this->struct_disp.disp_type_str +
        "_" + std::to_string(asset_idx) +
        "_results.csv";
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + filename);
    
    // write file header
    ofs << "Time [hrs],"
        << "Production (over time step) [kW],"
        << "Dispatch (over time step) [kW],"
        << "Curtailment (over time step) [kW],"
        << "Storage (over time step) [kW],"
        << "Is Running [T/F],"
        << "Real Op & Maint Cost (over time step),"
        << "Is Replaced [T/F],"
        << "Real Capital Cost (incurred during time step),"
        //<< ","
        << "\n";
    
    // write file body
    for (int i = 0; i < this->struct_disp.n_timesteps; i++) {
        ofs << std::to_string(ptr_2_time_vec_hr->at(i)) << ","
            << std::to_string(this->production_vec_kW[i]) << ","
            << std::to_string(this->dispatch_vec_kW[i]) << ","
            << std::to_string(this->curtailment_vec_kW[i]) << ","
            << std::to_string(this->storage_vec_kW[i]) << ","
            << std::to_string(this->is_running_vec[i]) << ","
            << std::to_string(this->real_op_maint_cost_vec[i]) << ","
            << std::to_string(this->replaced_vec[i]) << ","
            << std::to_string(this->real_capital_cost_vec[i]) << ","
            //<< [...] << ","
            << "\n";
    }
    
    ofs.close();
    
    return;
}


void Dispatchable :: commitProductionkW(
    double production_kW,
    double dt_hrs,
    double t_hrs,
    int timestep
) {
    /*
     *  Method to commit to given production
     */

    // record production
    this->production_vec_kW[timestep] = production_kW;
    
    // increment running hours (if running)
    if (this->struct_disp.is_running) {
        this->is_running_vec[timestep] = true;
        this->struct_disp.running_hrs += dt_hrs;
    }
    
    // incur operation and maintenance cost
    if (this->struct_disp.is_running) {
        /*
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
         */
        double real_discount_scalar = 1.0 / pow(
            1 + this->struct_disp.real_discount_rate_annual,
            t_hrs / 8760
        );
        
        double op_maint_cost = real_discount_scalar * 
            this->struct_disp.op_maint_cost_per_kWh * 
            production_kW * dt_hrs;
        
        this->real_op_maint_cost_vec[timestep] = op_maint_cost;
    }
    
    // trigger replacement, if necessary
    if (
        this->struct_disp.running_hrs >=
        (this->struct_disp.n_replacements + 1) *
        this->struct_disp.replace_running_hrs
    ) {
        this->_handleReplacement(timestep, t_hrs);
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
