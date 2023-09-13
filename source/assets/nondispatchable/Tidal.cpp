/*
 *  Nondispatchable <-- Tidal class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Tidal.h"


Tidal :: Tidal(
    structNondispatchable struct_nondisp,
    structTidal struct_tidal,
    int n_timesteps
) : Nondispatchable(struct_nondisp, n_timesteps) {
    /*
     *  Tidal class constructor
     */
    
    this->nondisp_type = TIDAL;
    this->nondisp_type_str = "TIDAL";
    this->struct_tidal = struct_tidal;
    
    //  init economic attributes
    /*
     *  ref:    Dr. S.L. MacDougall, Commercial Potential of Marine
     *              Renewables in British Columbia, technical report
     *              submitted to Natural Resources Canada, 
     *              S.L. MacDougall Research & Consulting, 2019
     */
    if (this->struct_nondisp.capital_cost < 0) {
        // Canadian dollars
        this->struct_nondisp.capital_cost =
            this->struct_nondisp.cap_kW * (
            4497 * exp(
                0.0002 * log(0.894596397) *
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
        std::cout << "\tTidal object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Tidal :: _writeSummary(std::string _write_path, int asset_idx) {
    /*
     *  Helper method to write Tidal-level summary
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
    ofs << this->struct_nondisp.cap_kW << " kW Tidal Summary\n\n";
    ofs << "Attributes:\n\n";
    
    ofs << "\t1D resource key: " << this->struct_tidal.resource_key
        << "\n";
    ofs << "\treplacement running hours: " << this->struct_nondisp.replace_running_hrs
        << " hrs\n";
    
    ofs << "\tpower curve: " << this->struct_tidal.power_curve << " ";
    
    switch (this->struct_tidal.power_curve) {
        case (CUBIC): {
            ofs << "(CUBIC)\n";
            
            break;
        }
        
        case (EXPONENTIAL): {
            ofs << "(EXPONENTIAL)\n";
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    ofs << "\tdesign speed: " << this->struct_tidal.design_speed_ms
        << " m/s\n";
    ofs << "\tcapital cost: " << this->struct_nondisp.capital_cost <<
        "\n";
    ofs << "\toperation and maintenance cost (per kWh produced): " <<
        this->struct_nondisp.op_maint_cost_per_kWh << "\n";
    ofs << "\treal discount rate (annual): " <<
        this->real_discount_rate_annual << "\n";
    
    // write results
    ofs << "\nResults:\n\n";
    
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


double Tidal :: getProductionkW(double tidal_resource_ms) {
    /*
     *  Method to compute and return production under given tidal
     *  resource. Implements multiple power curves.
     */
     
    // first, check if no resource
    if (tidal_resource_ms <= 0) {
        return 0;
    }
    
    // otherwise, apply selected power curve
    double production = 0;
    double turbine_speed = 0;
    
    switch(this->struct_tidal.power_curve) {
        case (CUBIC): {
            /*
             *  ref: B. Buckham, C. Crawford, I. Beya Marshall, and
             *       B. Whitby, "Wei Wai Kum Tidal Prefeasibility
             *       Study - Tidal Resource Assessment", PRIMED
             *       technical report, 2023,
             *       P2202E_BRKLYG+WEI WAI KUM_R01_ V20230613v3
             */
            
            if (
                tidal_resource_ms < 0.15 * this->struct_tidal.design_speed_ms ||
                tidal_resource_ms > 1.25 * this->struct_tidal.design_speed_ms
            ){
                production = 0;
            }
            
            else if (
                0.15 * this->struct_tidal.design_speed_ms <= tidal_resource_ms &&
                tidal_resource_ms <= this->struct_tidal.design_speed_ms
            ) {
                production = 
                    (1 / pow(this->struct_tidal.design_speed_ms, 3)) *
                    pow(tidal_resource_ms, 3);
            }
            
            else {
                production = 1;
            }
            
            break;
        }
        
        case (EXPONENTIAL): {
            /*
             *  ref: docs/wind_tidal_wave.pdf
             */
            
            turbine_speed =
                (tidal_resource_ms - this->struct_tidal.design_speed_ms) /
                this->struct_tidal.design_speed_ms;
                
            if (turbine_speed < -0.71 || turbine_speed > 0.65) {
                production = 0;
            }
            
            else if (turbine_speed >= -0.71 && turbine_speed <= 0) {
                production = 1.69215 * exp(1.25909 * turbine_speed) - 0.69215;
            }
            
            else {
                production = 1;
            }
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    double production_kW = this->struct_nondisp.cap_kW * production;
    return production_kW;
}


void Tidal :: writeResults(
    std::string _write_path,
    int asset_idx
) {
    /*
     *  Method to write Tidal-level results
     */
    
    Nondispatchable::_writeTimeSeriesResults(_write_path, asset_idx);
    this->_writeSummary(_write_path, asset_idx);
    
    return;
}


Tidal :: ~Tidal() {
    /*
     *  Tidal class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tTidal object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
