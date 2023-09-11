/*
 *  Dispatchable <-- Combustion class implementation file
 */


#include "../../../../header/includes.h"
#include "../../../../header/assets/dispatchable/Dispatchable.h"
#include "../../../../header/assets/dispatchable/combustion/Combustion.h"


Combustion :: Combustion(
    structDispatchable struct_disp,
    structCombustion struct_combustion
) : Dispatchable(struct_disp) {
    /*
     *  Combustion class constructor
     */
    
    this->struct_combustion = struct_combustion;
    
    this->CO2_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    this->CO_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    this->NOx_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    this->SOx_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    this->CH4_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    this->PM_vec_kg.resize(this->struct_disp.n_timesteps, 0);
    
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


double Combustion :: getFuelConsumptionL(
    double production_kW,
    double dt_hrs
) {
    /*
     *  Method to compute and return fuel consumption under given
     *  production and time step
     */
    
    // check running state
    if (not this->struct_disp.is_running) {
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
    
    switch (this->struct_combustion.fuel_type) {
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
