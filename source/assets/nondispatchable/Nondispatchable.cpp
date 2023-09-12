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
    
    this->real_capital_cost_vec.resize(this->struct_nondisp.n_timesteps, 0);
    this->real_op_maint_cost_vec.resize(this->struct_nondisp.n_timesteps, 0);
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tNondispatchable object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Nondispatchable :: _handleReplacement(int timestep, double t_hrs) {
    /*
     *  Helper method to handle running hours induced replacement
     */
    
    // reset attributes (replace with fresh Nondispatchable system)
    this->struct_nondisp.is_running = false;
    
    // incur capital cost
    /*
     *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
     *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
     */
    double real_discount_scalar = 1.0 / pow(
        1 + this->struct_nondisp.real_discount_rate_annual,
        t_hrs / 8760
    );
    
    this->real_capital_cost_vec[timestep] = real_discount_scalar *
        this->struct_nondisp.capital_cost;
        
    this->struct_nondisp.net_present_cost +=
        this->real_capital_cost_vec[timestep];
    
    // record replacements
    this->struct_nondisp.n_replacements++;
    this->replaced_vec[timestep] = true;
    
    return;
}


void Nondispatchable :: _writeTimeSeriesResults(
    std::string _write_path,
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Helper method to write Nondispatchable-level time series results
     */
    
    // construct filename 
    std::string filename = "Nondispatchable/" +
        std::to_string(int(this->struct_nondisp.cap_kW)) +
        "kW_" + this->struct_nondisp.nondisp_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_nondisp.cap_kW)) +
        "kW_" + this->struct_nondisp.nondisp_type_str +
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
    for (int i = 0; i < this->struct_nondisp.n_timesteps; i++) {
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


void Nondispatchable :: commitProductionkW(
    double production_kW,
    double dt_hrs,
    double t_hrs,
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
    
    // increment running hours
    if (this->struct_nondisp.is_running) {
        this->struct_nondisp.running_hrs += dt_hrs;
    }
    
    // incur operation and maintenance cost
    if (this->struct_nondisp.is_running) {
        /*
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
         */
        double real_discount_scalar = 1.0 / pow(
            1 + this->struct_nondisp.real_discount_rate_annual,
            t_hrs / 8760
        );
        
        double op_maint_cost = real_discount_scalar * 
            this->struct_nondisp.op_maint_cost_per_kWh * 
            production_kW * dt_hrs;
        
        this->real_op_maint_cost_vec[timestep] = op_maint_cost;
        
        this->struct_nondisp.net_present_cost +=
            this->real_op_maint_cost_vec[timestep];
    }
    
    // trigger replacement, if necessary
    if (
        this->struct_nondisp.running_hrs >=
        (this->struct_nondisp.n_replacements + 1) *
        this->struct_nondisp.replace_running_hrs
    ) {
        this->_handleReplacement(timestep, t_hrs);
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


void Nondispatchable :: computeLevellizedCostOfEnergy(
    double project_life_yrs,
    std::vector<double>* ptr_2_dt_vec_hr
) {
    /*
     *  Method to compute levellized cost of energy
     * 
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/levelized_cost_of_energy.html
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/total_annualized_cost.html
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/capital_recovery_factor.html
     */
    
    double total_dispatch_kWh = 0;
    
    for (size_t i = 0; i < this->dispatch_vec_kW.size(); i++) {
        total_dispatch_kWh += this->dispatch_vec_kW[i] *
            ptr_2_dt_vec_hr->at(i);
    }
    
    if (total_dispatch_kWh <= 0) {
        return;
    }
    this->total_dispatch_kWh = total_dispatch_kWh;
    
    double capital_recovery_factor = 
        (
            this->struct_nondisp.real_discount_rate_annual *
            pow(
                1 + this->struct_nondisp.real_discount_rate_annual,
                project_life_yrs
            )
        ) / 
        (
            pow(
                1 + this->struct_nondisp.real_discount_rate_annual,
                project_life_yrs
            ) -
            1
        );
        
    double total_annualized_cost = capital_recovery_factor *
        this->struct_nondisp.net_present_cost;
    
    this->struct_nondisp.levellized_cost_of_energy_per_kWh =
        total_annualized_cost / total_dispatch_kWh;
    
    return;
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
