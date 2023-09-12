/*
 *  Nondispatchable <-- Solar class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Solar.h"


Solar :: Solar(
    structNondispatchable struct_nondisp,
    structSolar struct_solar
) : Nondispatchable(struct_nondisp) {
    /*
     *  Solar class constructor
     */
    
    this->struct_nondisp.nondisp_type = SOLAR;
    this->struct_nondisp.nondisp_type_str = "SOLAR";
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
        
        this->struct_nondisp.net_present_cost +=
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
        "kW_" + this->struct_nondisp.nondisp_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_nondisp.cap_kW)) +
        "kW_" + this->struct_nondisp.nondisp_type_str +
        "_" + std::to_string(asset_idx) +
        "_summary.txt";
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + filename);
    
    // write attributes
    ofs << this->struct_nondisp.cap_kW << " kW Solar Summary\n\n";
    ofs << "Attributes:\n\n";
    
    ofs << "\t1D resource key: " << this->struct_solar.resource_key
        << "\n";
    ofs << "\treplacement running hours: " << this->struct_nondisp.replace_running_hrs
        << " hrs\n";
    ofs << "\tderating: " << this->struct_solar.derating << "\n";
    ofs << "\tcapital cost: " << this->struct_nondisp.capital_cost <<
        "\n";
    ofs << "\toperation and maintenance cost (per kWh produced): " <<
        this->struct_nondisp.op_maint_cost_per_kWh << "\n";
    ofs << "\treal discount rate (annual): " <<
        this->struct_nondisp.real_discount_rate_annual << "\n";
    
    // write results
    ofs << "\nResults:\n\n";
    
    ofs << "\trunning hours: " << this->struct_nondisp.running_hrs
        << " hrs\n";
    ofs << "\tnumber of replacements: " << this->struct_nondisp.n_replacements
        << "\n";
    ofs << "\ttotal dispatch (over project life): " <<
        this->total_dispatch_kWh << " kWh\n";
    ofs << "\tnet present cost: " <<
        this->struct_nondisp.net_present_cost << "\n";
    ofs << "\tlevellized cost of energy (per kWh dispatched): " <<
        this->struct_nondisp.levellized_cost_of_energy_per_kWh << "\n";
    
    ofs.close();
    
    return;
}


double Solar :: getProductionkW(double solar_resource_kWm2) {
    /*
     *  Method to compute and return production under given solar
     *  resource. Temperature effects are not modelled.
     * 
     *  ref: https://www.homerenergy.com/products/pro/docs/3.11/how_homer_calculates_the_pv_array_power_output.html
     */
     
    // first, check if no resource 
    if (solar_resource_kWm2 <= 0) {
        return 0;
    } 
    
    // otherwise, simple linear model
    double production_kW = this->struct_solar.derating *
        this->struct_nondisp.cap_kW * solar_resource_kWm2;
    return production_kW;
}


void Solar :: writeResults(
    std::string _write_path,
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Method to write Solar-level results
     */
    
    Nondispatchable::_writeTimeSeriesResults(
        _write_path, ptr_2_time_vec_hr, asset_idx
    );
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
