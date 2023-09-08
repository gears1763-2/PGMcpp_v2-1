/*
 *  Nondispatchable <-- Wind class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Wind.h"


Wind :: Wind(
    structNondispatchable struct_nondisp,
    structWind struct_wind
) : Nondispatchable(struct_nondisp) {
    /*
     *  Wind class constructor
     */
    
    this->struct_nondisp.nondisp_type = WIND;
    this->struct_nondisp.nondisp_type_str = "WIND";
    this->struct_wind = struct_wind;
    
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWind object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Wind :: _writeTimeSeriesResults(
    std::string _write_path,
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Helper method to write Wind-level time series results
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
    //...
    
    // write file body
    for (int i = 0; i < this->struct_nondisp.n_timesteps; i++) {
        //...
    }
    
    ofs.close();
    
    return;
}


void Wind :: _writeSummary(std::string _write_path, int asset_idx) {
    /*
     *  Helper method to write Wind-level summary
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
    //...
    
    // write results
    //...
    
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
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Method to write Wind-level results
     */
    
    this->_writeTimeSeriesResults(
        _write_path, ptr_2_time_vec_hr, asset_idx
    );
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
