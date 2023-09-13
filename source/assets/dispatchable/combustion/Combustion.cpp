/*
 *  Dispatchable <-- Combustion class implementation file
 */


#include "../../../../header/includes.h"
#include "../../../../header/assets/dispatchable/Dispatchable.h"
#include "../../../../header/assets/dispatchable/combustion/Combustion.h"


Combustion :: Combustion(
    structDispatchable struct_disp,
    structCombustion struct_combustion,
    int n_timesteps
) : Dispatchable(struct_disp, n_timesteps) {
    /*
     *  Combustion class constructor
     */
    
    this->struct_combustion = struct_combustion;
    
    this->fuel_vec_L.resize(this->n_timesteps, 0);
    this->real_fuel_cost_vec.resize(this->n_timesteps, 0);
    
    this->CO2_vec_kg.resize(this->n_timesteps, 0);
    this->CO_vec_kg.resize(this->n_timesteps, 0);
    this->NOx_vec_kg.resize(this->n_timesteps, 0);
    this->SOx_vec_kg.resize(this->n_timesteps, 0);
    this->CH4_vec_kg.resize(this->n_timesteps, 0);
    this->PM_vec_kg.resize(this->n_timesteps, 0);
    
    if (this->struct_combustion.fuel_mode == LOOKUP) {
        if (this->struct_combustion.path_2_fuel_consumption_data.empty()) {
            std::string warning_str = "WARNING:  Combustion()  path to ";
            warning_str += "fuel consumption data is empty, defaulting to ";
            warning_str += "linear fuel consumption model.";
            std::cout << warning_str << std::endl;
            
            this->struct_combustion.fuel_mode = LINEAR;
        }
        
        else {
            this->_readInFuelConsumptionData();
        }
    }
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tCombustion object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Combustion :: _readInFuelConsumptionData() {
    /*
     *  Helper method to read in fuel consumption data for use as
     *  a lookup table
     */
    
    // init CSVReader
    io::CSVReader<2> in(
        this->struct_combustion.path_2_fuel_consumption_data
    );
    
    // define expected .csv structure
    in.read_header(
        io::ignore_extra_column,
        "Load Ratio [ ]",
        "Fuel Consumption [L/hr]"
    );
    
    // read in data
    double load_ratio;
    double fuel_consumption_rate_Lhr;
    while (
        in.read_row(
            load_ratio,
            fuel_consumption_rate_Lhr
        )
    ) {
        this->fuel_interp_load_ratio_vec.push_back(load_ratio);
        this->fuel_interp_consumption_vec_Lhr.push_back(
            fuel_consumption_rate_Lhr
        );
    }
    
    return;
}


double Combustion :: _fuelConsumptionLookupL(
    double production_kW,
    double dt_hrs
) {
    /*
     *  Helper method to get fuel consumption from given fuel
     *  consumption data (1d linear interpolation) and return
     */
    
    double load_ratio = production_kW / this->struct_disp.cap_kW;
    
    if (load_ratio <= 0) {
        return this->fuel_interp_consumption_vec_Lhr[0] * dt_hrs;
    }
    
    else if (load_ratio >= 1) {
        return this->fuel_interp_consumption_vec_Lhr[
            this->fuel_interp_consumption_vec_Lhr.size() - 1
        ] * dt_hrs;
    }
    
    int interp_idx = 0;
    int max_idx = int(this->fuel_interp_load_ratio_vec.size() - 1);
    double fuel_consumption_L = 0;
    
    while (
        this->fuel_interp_load_ratio_vec[interp_idx + 1] < load_ratio
    ) {
        interp_idx++;
        
        if (interp_idx >= max_idx - 1) {
            interp_idx = max_idx - 1;
            break;
        }
    }
    
    fuel_consumption_L = linearInterpolate1d(
        load_ratio,
        this->fuel_interp_load_ratio_vec[interp_idx],
        this->fuel_interp_load_ratio_vec[interp_idx + 1],
        this->fuel_interp_consumption_vec_Lhr[interp_idx], 
        this->fuel_interp_consumption_vec_Lhr[interp_idx + 1]
    ) * dt_hrs;
    
    return fuel_consumption_L;
}


void Combustion :: _writeTimeSeriesResults(
    std::string _write_path,
    int asset_idx
) {
    /*
     *  Helper method to write Combustion-level time series results
     */
    
    // construct filename 
    std::string filename = "Combustion/" +
        std::to_string(int(this->struct_disp.cap_kW)) +
        "kW_" + this->disp_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_disp.cap_kW)) +
        "kW_" + this->disp_type_str +
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
        << "Fuel Consumption (over time step) [L],"
        << "Real Fuel Cost (over time step),"
        << "CO2 Emissions (over time step) [kg],"
        << "CO Emissions (over time step) [kg],"
        << "NOx Emissions (over time step) [kg],"
        << "SOx Emissions (over time step) [kg],"
        << "CH4 Emissions (over time step) [kg],"
        << "PM Emissions (over time step) [kg],"
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
            << std::to_string(this->fuel_vec_L[i]) << ","
            << std::to_string(this->real_fuel_cost_vec[i]) << ","
            << std::to_string(this->CO2_vec_kg[i]) << ","
            << std::to_string(this->CO_vec_kg[i]) << ","
            << std::to_string(this->NOx_vec_kg[i]) << ","
            << std::to_string(this->SOx_vec_kg[i]) << ","
            << std::to_string(this->CH4_vec_kg[i]) << ","
            << std::to_string(this->PM_vec_kg[i]) << ","
            << std::to_string(this->replaced_vec[i]) << ","
            << std::to_string(this->real_capital_cost_vec[i]) << ","
            //<< [...] << ","
            << "\n";
    }
    
    ofs.close();
    
    return;
}


double Combustion :: getFuelConsumptionL(
    double production_kW,
    double dt_hrs
) {
    /*
     *  Method to compute and return fuel consumption under given
     *  production and time step
     */
    
    // check running state
    if (not this->is_running) {
        return 0;
    }
    
    double fuel_consumption_L = 0;
    
    switch (this->struct_combustion.fuel_mode) {
        case (LOOKUP): {
            /*
             *  Interpolated over given fuel consumption data
             */
            
            fuel_consumption_L = this->_fuelConsumptionLookupL(
                production_kW,
                dt_hrs
            );
            
            break;
        }
        
        default: {  // default to LINEAR
            /*
             *  Fuel consumption over time step based on linearized fuel
             *  consumption curve (i.e. HOMER-like consumption model)
             *
             *  ref: https://www.homerenergy.com/products/pro/docs/latest/fuel_curve.html
             *  ref: https://www.homerenergy.com/products/pro/docs/latest/generator_fuel_curve_intercept_coefficient.html
             *  ref: https://www.homerenergy.com/products/pro/docs/latest/generator_fuel_curve_slope.html
             */
            
            fuel_consumption_L =
                (this->struct_combustion.linear_fuel_intercept_LkWh *
                this->struct_disp.cap_kW +
                this->struct_combustion.linear_fuel_slope_LkWh *
                production_kW) * dt_hrs;
            
            break;
        }
    }
    
    return fuel_consumption_L;
}


structEmissions Combustion :: getEmissions(double fuel_consumption_L) {
    /*
     *  Method to compute and return emissions under given fuel
     *  consumption
     */
    
    structEmissions struct_emissions;
    
    switch (this->fuel_type) {
        case (FUEL_DIESEL): {
            struct_emissions.CO2_kg =
                this->struct_combustion.diesel_CO2_kgL * fuel_consumption_L;
            
            struct_emissions.CO_kg =
                this->struct_combustion.diesel_CO_kgL * fuel_consumption_L;
            
            struct_emissions.NOx_kg =
                this->struct_combustion.diesel_NOx_kgL * fuel_consumption_L;
            
            struct_emissions.SOx_kg =
                this->struct_combustion.diesel_SOx_kgL * fuel_consumption_L;
            
            struct_emissions.CH4_kg =
                this->struct_combustion.diesel_CH4_kgL * fuel_consumption_L;
                
            struct_emissions.PM_kg =
                this->struct_combustion.diesel_PM_kgL * fuel_consumption_L;
            
            break;
        }
        
        case (FUEL_GAS): {
            //...
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    return struct_emissions;
}


void Combustion :: recordEmissions(
    structEmissions struct_emissions,
    int timestep
) {
    /*
     *  Method to record emissions over time step
     */
    
    this->CO2_vec_kg[timestep] = struct_emissions.CO2_kg;
    this->CO_vec_kg[timestep] = struct_emissions.CO_kg;
    this->NOx_vec_kg[timestep] = struct_emissions.NOx_kg;
    this->SOx_vec_kg[timestep] = struct_emissions.SOx_kg;
    this->CH4_vec_kg[timestep] = struct_emissions.CH4_kg;
    this->PM_vec_kg[timestep] = struct_emissions.PM_kg;
    
    total_CO2_emitted_kg += struct_emissions.CO2_kg;
    total_CO_emitted_kg += struct_emissions.CO_kg;
    total_NOx_emitted_kg += struct_emissions.NOx_kg;
    total_SOx_emitted_kg += struct_emissions.SOx_kg;
    total_CH4_emitted_kg += struct_emissions.CH4_kg;
    total_PM_emitted_kg += struct_emissions.PM_kg;
    
    return;
}


void Combustion :: computeLevellizedCostOfEnergy() {
    /*
     *  Method to compute levellized cost of energy
     * 
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/levelized_cost_of_energy.html
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/total_annualized_cost.html
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/capital_recovery_factor.html
     */
    
    Dispatchable::computeLevellizedCostOfEnergy();
    
    return;
}


Combustion :: ~Combustion() {
    /*
     *  Combustion class destructor
     */
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tCombustion object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
