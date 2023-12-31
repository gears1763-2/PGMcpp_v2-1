/*
 *  Nondispatchable <-- Solar class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Solar.h"


Solar :: Solar(
    structNondispatchable struct_nondisp,
    structSolar struct_solar,
    int n_timesteps
) : Nondispatchable(struct_nondisp, n_timesteps) {
    /*
     *  Solar class constructor
     */
    
    // input bounds checking
    if (struct_solar.derating < 0 or struct_solar.derating > 1) {
        std::string error_str = "\nERROR  Solar::Solar()";
        error_str += "  structSolar::derating must be in ";
        error_str += "the closed interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_solar.capital_cost_per_kW < 0) {
        std::string error_str = "\nERROR  Solar::Solar()";
        error_str += "  structSolar::capital_cost_per_kW must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    
    // set attributes
    this->nondisp_type = SOLAR;
    this->nondisp_type_str = "SOLAR";
    this->struct_solar = struct_solar;
    
    //  init economic attributes
    if (this->struct_nondisp.capital_cost < 0) {
        // Canadian dollars
        this->struct_nondisp.capital_cost =
            this->struct_solar.capital_cost_per_kW * 
            this->struct_nondisp.cap_kW;
    }
    
    else {
        // Canadian dollars
        this->struct_solar.capital_cost_per_kW = 
            this->struct_nondisp.capital_cost / 
            this->struct_nondisp.cap_kW;
    }
    
    if (this->struct_nondisp.op_maint_cost_per_kWh < 0) {
        // Canadian dollars
        this->struct_nondisp.op_maint_cost_per_kWh = 0.01;
    }
    
    if (not this->struct_nondisp.is_sunk) {
        this->real_capital_cost_vec[0] =
            this->struct_nondisp.capital_cost;
        
        this->net_present_cost +=
            this->struct_nondisp.capital_cost;
    }
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tSolar object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Solar :: _writeSummary(std::string _write_path, int asset_idx) {
    /*
     *  Helper method to write Solar-level summary
     */
    
    // construct filename 
    std::string filename = "Nondispatchable/" +
        std::to_string(int(this->struct_nondisp.cap_kW)) +
        "kW_" + this->nondisp_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_nondisp.cap_kW)) +
        "kW_" + this->nondisp_type_str +
        "_" + std::to_string(asset_idx) +
        "_summary.txt";
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + filename);
    
    // write attributes
    ofs << this->struct_nondisp.cap_kW << " kW Solar Summary\n\n";
    ofs << "Attributes:\n\n";
    
    ofs << "\tsunk cost: " << std::boolalpha <<
        this->struct_nondisp.is_sunk << std::noboolalpha << "\n";
    ofs << "\t1D resource key: " << this->struct_solar.resource_key
        << "\n";
    ofs << "\treplacement running hours: " << this->struct_nondisp.replace_running_hrs
        << " hrs\n";
    ofs << "\tderating: " << this->struct_solar.derating << "\n";
    ofs << "\tcapital cost: " << this->struct_nondisp.capital_cost <<
        "\n";
    ofs << "\toperation and maintenance cost (per kWh produced): " <<
        this->struct_nondisp.op_maint_cost_per_kWh << "\n";
    ofs << "\tnominal inflation rate (annual): " <<
        this->struct_nondisp.nominal_inflation_rate_annual << "\n";
    ofs << "\tnominal discount rate (annual): " <<
        this->struct_nondisp.nominal_discount_rate_annual << "\n";
    ofs << "\treal discount rate (annual): " <<
        this->real_discount_rate_annual << "\n";
    
    // write results
    ofs << "\nResults:\n\n";
    
    ofs << "\tproject life: " << this->project_life_yrs << " yrs\n";
    ofs << "\trunning hours: " << this->running_hrs
        << " hrs\n";
    ofs << "\tnumber of replacements: " << this->n_replacements
        << "\n";
    ofs << "\ttotal dispatch (over project life): " <<
        this->total_dispatch_kWh << " kWh\n";
    ofs << "\tnet present cost: " <<
        this->net_present_cost << "\n";
    ofs << "\tlevellized cost of energy (per kWh dispatched): " <<
        this->levellized_cost_of_energy_per_kWh << "\n";
    
    ofs.close();
    
    return;
}


double Solar :: getProductionkW(double solar_resource_kWm2) {
    /*
     *  Method to compute and return production under given solar
     *  resource. Temperature effects are not modelled.
     * 
     *  ref: https://www.homerenergy.com/products/pro/docs/3.11/how_homer_calculates_the_pv_array_power_output.html
     * 
     *  P = d * (r / 1 kW/m2) * P_rated
     */
     
    // first, check if no resource 
    if (solar_resource_kWm2 <= 0) {
        return 0;
    }
    
    double production_kW = this->struct_solar.derating *
        solar_resource_kWm2 *
        this->struct_nondisp.cap_kW;
        
    return production_kW;
}


void Solar :: writeResults(
    std::string _write_path,
    int asset_idx
) {
    /*
     *  Method to write Solar-level results
     */
    
    Nondispatchable::_writeTimeSeriesResults(_write_path, asset_idx);
    this->_writeSummary(_write_path, asset_idx);
    
    return;
}


Solar :: ~Solar() {
    /*
     *  Solar class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tSolar object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
