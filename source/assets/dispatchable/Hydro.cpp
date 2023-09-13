/*
 *  Dispatchable <-- Hydro class implementation file
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
    
    //...
    
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


double Hydro :: requestProductionkW(double requested_production_kW) {
    /*
     *  Method to handle production requests (subject to active
     *  operating constraints) and return provided production
     */
    
    //...
    
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
