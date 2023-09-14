/*
 *  Nondispatchable <-- Wind class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Wind.h"


Wind :: Wind(
    structNondispatchable struct_nondisp,
    structWind struct_wind,
    int n_timesteps
) : Nondispatchable(struct_nondisp, n_timesteps) {
    /*
     *  Wind class constructor
     */
    
    this->nondisp_type = WIND;
    this->nondisp_type_str = "WIND";
    this->struct_wind = struct_wind;
    
    //  init ecomonomic attributes
    /*
     *  These capital and operational cost formulae are derived from a
     *  survey of data for commercially available wind turbine
     *  technologies [Canadian dollars]
     */
    if (this->struct_nondisp.capital_cost < 0) {
        this->struct_nondisp.capital_cost =
            2 * this->struct_nondisp.cap_kW * (
            4525 * exp(
                0.333333333 * log(0.8839779) *
                this->struct_nondisp.cap_kW
            ) + 2000
        );
    }
    
    if (this->struct_nondisp.op_maint_cost_per_kWh < 0) {
        // Canadian dollars
        this->struct_nondisp.op_maint_cost_per_kWh = 0.05;
    }
    
    if (not this->struct_nondisp.is_sunk) {
        this->real_capital_cost_vec[0] =
            this->struct_nondisp.capital_cost;
            
        this->net_present_cost +=
            this->struct_nondisp.capital_cost;
    }
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWind object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Wind :: _writeSummary(std::string _write_path, int asset_idx) {
    /*
     *  Helper method to write Wind-level summary
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
    ofs << this->struct_nondisp.cap_kW << " kW Wind Summary\n\n";
    ofs << "Attributes:\n\n";
    
    ofs << "\tsunk cost: " << std::boolalpha <<
        this->struct_nondisp.is_sunk << std::noboolalpha << "\n";
    ofs << "\t1D resource key: " << this->struct_wind.resource_key
        << "\n";
    ofs << "\treplacement running hours: " << this->struct_nondisp.replace_running_hrs
        << " hrs\n";
    ofs << "\tdesign speed: " << this->struct_wind.design_speed_ms <<
        " m/s\n";
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


double Wind :: getProductionkW(double wind_resource_ms) {
    /*
     *  Method to compute and return production under given wind
     *  resource.
     */
    
    // first, check if no resource
    if (wind_resource_ms <= 0) {
        return 0;
    }
    
    // otherwise, apply power curve
    /*
     *  ref: docs/wind_tidal_wave.pdf
     */
    double production = 0;
    double turbine_speed = 0;
    
    turbine_speed = (wind_resource_ms - this->struct_wind.design_speed_ms) /
        this->struct_wind.design_speed_ms;
        
    if (turbine_speed < -0.76 || turbine_speed > 0.68) {
        production = 0;
    }
    
    else if (turbine_speed >= -0.76 && turbine_speed <= 0) {
        production = 1.03273 * exp(-5.97588 * pow(turbine_speed, 2)) -
            0.03273;
    }
    
    else {
        production = 0.16154 * exp(-9.30254 * pow(turbine_speed, 2)) +
            0.83846;
    }
    
    double production_kW = this->struct_nondisp.cap_kW * production;
    return production_kW;
}


void Wind :: writeResults(
    std::string _write_path,
    int asset_idx
) {
    /*
     *  Method to write Wind-level results
     */
    
    Nondispatchable::_writeTimeSeriesResults(_write_path, asset_idx);
    this->_writeSummary(_write_path, asset_idx);
    
    return;
}


Wind :: ~Wind() {
    /*
     *  Wind class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWind object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
