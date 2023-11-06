/*
 *  Nondispatchable class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"


Nondispatchable :: Nondispatchable(
    structNondispatchable struct_nondisp,
    int n_timesteps
) {
    /*
     *  Nondispatchable class constructor
     */
    
    // input bounds checking
    if (struct_nondisp.cap_kW <= 0) {
        std::string error_str = "\nERROR  Nondispatchable::Nondispatchable()";
        error_str += "  structNondispatchable::cap_kW must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_nondisp.replace_running_hrs <= 0) {
        std::string error_str = "\nERROR  Nondispatchable::Nondispatchable()";
        error_str += "  structNondispatchable::replace_running_hrs must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    
    // set attributes
    this->struct_nondisp = struct_nondisp;
    this->n_timesteps = n_timesteps;
    
    this->is_running_vec.resize(this->n_timesteps, false);
    this->replaced_vec.resize(this->n_timesteps, false);
    
    this->production_vec_kW.resize(this->n_timesteps, 0);
    this->dispatch_vec_kW.resize(this->n_timesteps, 0);
    this->curtailment_vec_kW.resize(this->n_timesteps, 0);
    this->storage_vec_kW.resize(this->n_timesteps, 0);
    
    this->real_capital_cost_vec.resize(this->n_timesteps, 0);
    this->real_op_maint_cost_vec.resize(this->n_timesteps, 0);
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tNondispatchable object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Nondispatchable :: _handleReplacement(int timestep) {
    /*
     *  Helper method to handle running hours induced replacement
     */
    
    // reset attributes (replace with fresh Nondispatchable system)
    this->is_running = false;
    
    // incur capital cost
    /*
     *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
     *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
     */
    double t_hrs = this->ptr_2_time_vec_hr->at(timestep);
    double real_discount_scalar = 1.0 / pow(
        1 + this->real_discount_rate_annual,
        t_hrs / 8760
    );
    
    this->real_capital_cost_vec[timestep] = real_discount_scalar *
        this->struct_nondisp.capital_cost;
        
    this->net_present_cost +=
        this->real_capital_cost_vec[timestep];
    
    // record replacements
    this->n_replacements++;
    this->replaced_vec[timestep] = true;
    
    return;
}


void Nondispatchable :: _writeTimeSeriesResults(
    std::string _write_path,
    int asset_idx
) {
    /*
     *  Helper method to write Nondispatchable-level time series results
     */
    
    // construct filename 
    std::string filename = "Nondispatchable/" +
        std::to_string(int(this->struct_nondisp.cap_kW)) +
        "kW_" + this->nondisp_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_nondisp.cap_kW)) +
        "kW_" + this->nondisp_type_str +
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
    for (int i = 0; i < this->n_timesteps; i++) {
        ofs << std::to_string(this->ptr_2_time_vec_hr->at(i)) << ","
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
    int timestep
) {
    /*
     *  Method to commit to given production
     */
    
    // track and record running state
    if (production_kW > 0) {
        this->is_running = true;
        this->is_running_vec[timestep] = true;
    }
    
    else {
        this->is_running = false;
    }
    
    // record production
    this->production_vec_kW[timestep] = production_kW;
    
    // increment running hours
    double dt_hrs = this->ptr_2_dt_vec_hr->at(timestep);
    if (this->is_running) {
        this->running_hrs += dt_hrs;
    }
    
    // incur operation and maintenance cost
    if (this->is_running) {
        /*
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
         */
        double t_hrs = this->ptr_2_time_vec_hr->at(timestep);
        double real_discount_scalar = 1.0 / pow(
            1 + this->real_discount_rate_annual,
            t_hrs / 8760
        );
        
        double op_maint_cost = real_discount_scalar * 
            this->struct_nondisp.op_maint_cost_per_kWh * 
            production_kW * dt_hrs;
        
        this->real_op_maint_cost_vec[timestep] = op_maint_cost;
        
        this->net_present_cost +=
            this->real_op_maint_cost_vec[timestep];
    }
    
    // trigger replacement, if necessary
    if (
        this->running_hrs >=
        (this->n_replacements + 1) *
        this->struct_nondisp.replace_running_hrs
    ) {
        this->_handleReplacement(timestep);
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


void Nondispatchable :: computeLevellizedCostOfEnergy() {
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
            this->ptr_2_dt_vec_hr->at(i);
    }
    
    if (total_dispatch_kWh <= 0) {
        return;
    }
    this->total_dispatch_kWh = total_dispatch_kWh;
    
    double capital_recovery_factor = 
        (
            this->real_discount_rate_annual *
            pow(
                1 + this->real_discount_rate_annual,
                this->project_life_yrs
            )
        ) / 
        (
            pow(
                1 + this->real_discount_rate_annual,
                this->project_life_yrs
            ) -
            1
        );
        
    double total_annualized_cost = capital_recovery_factor *
        this->net_present_cost;
    
    this->levellized_cost_of_energy_per_kWh =
        (this->project_life_yrs * total_annualized_cost) /
        total_dispatch_kWh;
    
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
