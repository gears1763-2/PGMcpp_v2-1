/*
 *  Dispatchable <-- Combustion <-- Diesel class implementation file
 */


#include "../../../../header/includes.h"
#include "../../../../header/assets/dispatchable/Dispatchable.h"
#include "../../../../header/assets/dispatchable/combustion/Combustion.h"
#include "../../../../header/assets/dispatchable/combustion/Diesel.h"


Diesel :: Diesel(
    structDispatchable struct_disp,
    structCombustion struct_combustion,
    structDiesel struct_diesel
) : Combustion(struct_disp, struct_combustion) {
    /*
     *  Diesel class constructor
     */
    
    this->struct_disp.disp_type = DIESEL;
    this->struct_disp.disp_type_str = "DIESEL";
    this->struct_diesel = struct_diesel;
    
    this->struct_combustion.fuel_type = FUEL_DIESEL;
    
    // init linear fuel consumption parameters if sentinel value detected
    if (this->struct_combustion.linear_fuel_intercept_LkWh < 0) {
        this->struct_combustion.linear_fuel_intercept_LkWh =
            0.0940 * pow(this->struct_disp.cap_kW, -0.2735);
    }
    
    if (this->struct_combustion.linear_fuel_slope_LkWh < 0) {
        this->struct_combustion.linear_fuel_slope_LkWh =
            0.3062 * pow(this->struct_disp.cap_kW, -0.0370);
    }
    
    this->fuel_vec_L.resize(this->struct_disp.n_timesteps, 0);
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tDiesel object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Diesel :: _writeTimeSeriesResults(
    std::string _write_path,
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Helper method to write Diesel-level time series results
     */
    
    // construct filename 
    std::string filename = "Combustion/" +
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
        << "Fuel Consumption (over time step) [L],"
        << "CO2 Emissions (over time step) [kg],"
        << "CO Emissions (over time step) [kg],"
        << "NOx Emissions (over time step) [kg],"
        << "SOx Emissions (over time step) [kg],"
        << "CH4 Emissions (over time step) [kg],"
        << "PM Emissions (over time step) [kg],"
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
            << std::to_string(this->fuel_vec_L[i]) << ","
            << std::to_string(this->CO2_vec_kg[i]) << ","
            << std::to_string(this->CO_vec_kg[i]) << ","
            << std::to_string(this->NOx_vec_kg[i]) << ","
            << std::to_string(this->SOx_vec_kg[i]) << ","
            << std::to_string(this->CH4_vec_kg[i]) << ","
            << std::to_string(this->PM_vec_kg[i]) << ","
            //<< [...] << ","
            << "\n";
    }
    
    ofs.close();
    
    return;
}


void Diesel :: _writeSummary(std::string _write_path, int asset_idx) {
    /*
     *  Helper method to write Diesel-level summary
     */
    
    // construct filename 
    std::string filename = "Combustion/" +
        std::to_string(int(this->struct_disp.cap_kW)) +
        "kW_" + this->struct_disp.disp_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_disp.cap_kW)) +
        "kW_" + this->struct_disp.disp_type_str +
        "_" + std::to_string(asset_idx) +
        "_summary.txt";
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + filename);
    
    // write attributes
    ofs << this->struct_disp.cap_kW << "kW Diesel Summary\n\n";
    ofs << "Attributes:\n\n";
    
    ofs << "\treplacement running hours: " << this->struct_disp.replace_running_hrs
        << " hrs\n";
    ofs << "\tminimum load ratio: " << this->struct_diesel.minimum_load_ratio
        << "\n";
    ofs << "\tminimum runtime: " << this->struct_diesel.minimum_runtime_hrs
        << " hrs\n";
    ofs << "\tfuel mode: " << this->struct_combustion.fuel_mode;
    
    switch (this->struct_combustion.fuel_mode) {
        case (LINEAR): {
            ofs << " (LINEAR)\n";
            
            break;
        }
        
        case (LOOKUP): {
            ofs << " (LOOKUP)\n";
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    // write results
    ofs << "\nResults:\n\n";
    
    ofs << "\trunning hours: " << this->struct_disp.running_hrs
        << " hrs\n";
    ofs << "\tnumber of starts: " << this->struct_disp.n_starts
        << "\n";
    ofs << "\tnumber of replacements: " << this->struct_disp.n_replacements
        << "\n";
    
    ofs.close();
    
    return;
}


void Diesel :: commitProductionkW(
    double production_kW,
    double dt_hrs,
    int timestep
) {
    /*
     *  Method to commit to given production
     */
    
    // enforce minimum runtime, handle stopping
    if (
        this->struct_disp.is_running &&
        production_kW <= 0 &&
        this->struct_diesel.time_since_last_start_hrs >=
            this->struct_diesel.minimum_runtime_hrs
    ) {
        // stop, reset time since last start
        this->struct_disp.is_running = false;
        this->struct_diesel.time_since_last_start_hrs = 0;
    }
    
    // handle starting, track time since last start
    if (
        not(this->struct_disp.is_running) &&
        production_kW > 0
    ) {
        this->struct_disp.is_running = true;
        this->struct_disp.n_starts++;
    }
    
    if (this->struct_disp.is_running) {
        this->struct_diesel.time_since_last_start_hrs += dt_hrs;
    }
    
    // callback
    Dispatchable::commitProductionkW(production_kW, dt_hrs, timestep);
    
    // handle fuel consumption
    double fuel_consumption_L =
        Combustion::getFuelConsumptionL(production_kW, dt_hrs);
        
    this->fuel_vec_L[timestep] = fuel_consumption_L;
    
    // handle emissions
    structEmissions struct_emissions =
        Combustion::getEmissions(fuel_consumption_L);
    
    Combustion::recordEmissions(struct_emissions, timestep);
    
    return;
}


double Diesel :: requestProductionkW(double requested_production_kW) {
    /*
     *  Method to handle production requests (subject to active
     *  operating constraints) and return provided production
     */
    
    double production_kW = requested_production_kW;
    
    // enforce minimum load ratio
    if (
        production_kW > 0 &&
        production_kW < this->struct_diesel.minimum_load_ratio * 
            this->struct_disp.cap_kW
    ) {
        production_kW = this->struct_diesel.minimum_load_ratio * 
            this->struct_disp.cap_kW;
    }
    
    // check against capacity
    if (production_kW > this->struct_disp.cap_kW) {
        production_kW = this->struct_disp.cap_kW;
    }
    
    //...
    
    return production_kW;
}


void Diesel :: writeResults(
    std::string _write_path,
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Method to write Diesel-level results
     */
    
    this->_writeTimeSeriesResults(
        _write_path, ptr_2_time_vec_hr, asset_idx
    );
    this->_writeSummary(_write_path, asset_idx);
    
    return;
}


Diesel :: ~Diesel() {
    /*
     *  Diesel class destructor
     */
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tDiesel object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
