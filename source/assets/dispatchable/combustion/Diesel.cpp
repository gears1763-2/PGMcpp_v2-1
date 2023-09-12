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
    
    //  init ecomonomic attributes
    /*
     *  These capital and operational cost formulae are derived from a survey
     *  of data for commercially available diesel generators
     *  [Canadian dollars]
     */
    if (this->struct_disp.capital_cost < 0) {
        // Canadian dollars
        this->struct_disp.capital_cost =
            this->struct_disp.cap_kW * (
            150 * exp(
                0.0002 * log(0.14666666) *
                this->struct_disp.cap_kW
            ) + 400
        );
    }
    
    if (this->struct_disp.op_maint_cost_per_kWh < 0) {
        // Canadian dollars
        this->struct_disp.op_maint_cost_per_kWh = 0.03 * exp(
            0.0002 * log(0.03333333) *
            this->struct_disp.cap_kW
        ) + 0.01;
    }
    
    if (not this->struct_disp.is_sunk) {
        this->real_capital_cost_vec[0] =
            this->struct_disp.capital_cost;
        
        this->struct_disp.net_present_cost +=
            this->struct_disp.capital_cost;
    }
    
    if (this->struct_disp.test_flag) {
        std::cout << "\tDiesel object constructed at " << this
            << std::endl;
    }
    
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
    ofs << this->struct_disp.cap_kW << " kW Diesel Summary\n\n";
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
            ofs << "\tfuel intercept: " <<
                this->struct_combustion.linear_fuel_intercept_LkWh
                << " L/kWh\n";
            ofs << "\tfuel slope: " <<
                this->struct_combustion.linear_fuel_slope_LkWh
                << " L/kWh\n";
            
            break;
        }
        
        case (LOOKUP): {
            ofs << " (LOOKUP)\n";
            ofs << "\tpath to fuel consumption data: " <<
                this->struct_combustion.path_2_fuel_consumption_data
                << "\n";
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    ofs << "\tcapital cost: " << this->struct_disp.capital_cost <<
        "\n";
    ofs << "\toperation and maintenance cost (per kWh produced): " <<
        this->struct_disp.op_maint_cost_per_kWh << "\n";
    ofs << "\tfuel cost (per L): " <<
        this->struct_combustion.fuel_cost_L << "\n";
    ofs << "\treal discount rate (annual): " <<
        this->struct_disp.real_discount_rate_annual << "\n";
    ofs << "\treal fuel discount rate (annual): " <<
        this->struct_combustion.real_fuel_discount_rate_annual << "\n";
    
    // write results
    ofs << "\nResults:\n\n";
    
    ofs << "\trunning hours: " << this->struct_disp.running_hrs
        << " hrs\n";
    ofs << "\tnumber of starts: " << this->struct_disp.n_starts
        << "\n";
    ofs << "\tnumber of replacements: " << this->struct_disp.n_replacements
        << "\n";
    ofs << "\ttotal dispatch (over project life): " << 
        this->total_dispatch_kWh << " kWh\n";
    ofs << "\ttotal fuel consumed (over project life): " <<
        this->total_fuel_consumed_L << " L\n";
    ofs << "\ttotal CO2 emissions (over project life): " <<
        this->total_CO2_emitted_kg << " kg\n";
    ofs << "\ttotal CO emissions (over project life): " <<
        this->total_CO_emitted_kg << " kg\n";
    ofs << "\ttotal NOx emissions (over project life): " <<
        this->total_NOx_emitted_kg << " kg\n";
    ofs << "\ttotal SOx emissions (over project life): " <<
        this->total_SOx_emitted_kg << " kg\n";
    ofs << "\ttotal CH4 emissions (over project life): " <<
        this->total_CH4_emitted_kg << " kg\n";
    ofs << "\ttotal particulate matter emissions (over project life): " <<
        this->total_PM_emitted_kg << " kg\n";
    ofs << "\tnet present cost: " <<
        this->struct_disp.net_present_cost << "\n";
    ofs << "\tlevellized cost of energy (per kWh dispatched): " <<
        this->struct_disp.levellized_cost_of_energy_per_kWh << "\n";
    
    ofs.close();
    
    return;
}


void Diesel :: commitProductionkW(
    double production_kW,
    double dt_hrs,
    double t_hrs,
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
    Dispatchable::commitProductionkW(production_kW, dt_hrs, t_hrs, timestep);
    
    
    if (this->struct_disp.is_running) {
        // handle fuel consumption
        double fuel_consumption_L =
            Combustion::getFuelConsumptionL(production_kW, dt_hrs);
            
        this->fuel_vec_L[timestep] = fuel_consumption_L;
        
        this->total_fuel_consumed_L += fuel_consumption_L;
        
        // incur fuel cost
        /*
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
         */
        double real_discount_scalar = 1.0 / pow(
            1 + this->struct_combustion.real_fuel_discount_rate_annual,
            t_hrs / 8760
        );

        double fuel_cost = real_discount_scalar * 
            this->struct_combustion.fuel_cost_L * 
            fuel_consumption_L;

        this->real_fuel_cost_vec[timestep] = fuel_cost;

        this->struct_disp.net_present_cost +=
            this->real_fuel_cost_vec[timestep];
        
        // handle emissions
        structEmissions struct_emissions =
            Combustion::getEmissions(fuel_consumption_L);
        
        Combustion::recordEmissions(struct_emissions, timestep);
    }
    
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
    
    Combustion::_writeTimeSeriesResults(
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
