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
    
    //...
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tSolar object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Solar :: _writeTimeSeriesResults(
    std::string _write_path,
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Helper method to write Solar-level time series results
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
    //...
    
    // write results
    //...
    
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
    
    this->_writeTimeSeriesResults(
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
