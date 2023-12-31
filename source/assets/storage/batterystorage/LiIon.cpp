/*
 *  Storage <-- BatteryStorage <-- LiIon class implementation file
 */


#include "../../../../header/includes.h"
#include "../../../../header/assets/storage/Storage.h"
#include "../../../../header/assets/storage/batterystorage/BatteryStorage.h"
#include "../../../../header/assets/storage/batterystorage/LiIon.h"


LiIon :: LiIon(
    structStorage struct_storage,
    structBatteryStorage struct_battery_storage,
    structLiIon struct_liion,
    int n_timesteps
) : BatteryStorage(struct_storage, struct_battery_storage, n_timesteps) {
    /*
     *  LiIon class constructor
     */
    
    // input bounds checking
    if (struct_liion.replace_SOH < 0 or struct_liion.replace_SOH > 1) {
        std::string error_str = "\nERROR  LiIon::LiIon()";
        error_str += "  structLiIon::replace_SOH must be in ";
        error_str += "the closed interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_liion.degr_alpha <= 0) {
        std::string error_str = "\nERROR  LiIon::LiIon()";
        error_str += "  structLiIon::degr_alpha must be > 0, ";
        error_str += "See docs/refs/battery_degradation.pdf";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_liion.degr_beta <= 0) {
        std::string error_str = "\nERROR  LiIon::LiIon()";
        error_str += "  structLiIon::degr_beta must be > 0, ";
        error_str += "See docs/refs/battery_degradation.pdf";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_liion.degr_B_hat_cal_0 <= 0) {
        std::string error_str = "\nERROR  LiIon::LiIon()";
        error_str += "  structLiIon::degr_B_hat_cal_0 must be > 0, ";
        error_str += "See docs/refs/battery_degradation.pdf";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_liion.degr_r_cal < 0) {
        std::string error_str = "\nERROR  LiIon::LiIon()";
        error_str += "  structLiIon::degr_r_cal must be >= 0, ";
        error_str += "See docs/refs/battery_degradation.pdf";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_liion.degr_Ea_cal_0 <= 0) {
        std::string error_str = "\nERROR  LiIon::LiIon()";
        error_str += "  structLiIon::degr_Ea_cal_0 must be > 0, ";
        error_str += "See docs/refs/battery_degradation.pdf";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_liion.degr_a_cal < 0) {
        std::string error_str = "\nERROR  LiIon::LiIon()";
        error_str += "  structLiIon::degr_a_cal must be >= 0, ";
        error_str += "See docs/refs/battery_degradation.pdf";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_liion.degr_s_cal < 0) {
        std::string error_str = "\nERROR  LiIon::LiIon()";
        error_str += "  structLiIon::degr_s_cal must be >= 0, ";
        error_str += "See docs/refs/battery_degradation.pdf";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_liion.gas_constant_JmolK <= 0) {
        std::string error_str = "\nERROR  LiIon::LiIon()";
        error_str += "  structLiIon::gas_constant_JmolK must be > 0, ";
        error_str += "See docs/refs/battery_degradation.pdf";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    else if (struct_liion.temperature_K < 0) {
        std::string error_str = "\nERROR  LiIon::LiIon()";
        error_str += "  structLiIon::temperature_K must be >= 0, ";
        error_str += "See docs/refs/battery_degradation.pdf";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        throw std::invalid_argument(error_str);
    }
    
    
    // set attributes
    this->storage_type = LIION;
    this->storage_type_str = "LIION";
    this->struct_liion = struct_liion;
    
    this->SOH_vec.resize(this->n_timesteps, 0);
    this->SOH_vec[0] = this->SOH;
    
    //  init economic attributes
    /*
     *  These capital and operational cost formulae are derived from a survey
     *  of data for commercially available BESS technologies (mostly Li-ion)
     *  [Canadian dollars]
     */
    if (this->struct_storage.capital_cost < 0) {
        // Canadian dollars
        this->struct_storage.capital_cost = 700 *
            this->struct_storage.cap_kWh;
    }
    
    if (this->struct_storage.op_maint_cost_per_kWh < 0) {
        // Canadian dollars
        this->struct_storage.op_maint_cost_per_kWh = 0.01;
    }
    
    if (not this->struct_storage.is_sunk) {
        this->real_capital_cost_vec[0] =
            this->struct_storage.capital_cost;
        
        this->net_present_cost +=
            this->struct_storage.capital_cost;
    }
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tLiIon object constructed at " << this
            << std::endl;
    }
    
    return;
}


void LiIon :: _writeTimeSeriesResults(
    std::string _write_path,
    int asset_idx
) {
    /*
     *  Helper method to write LiIon-level time series results
     */
    
    // construct filename 
    std::string filename = "Storage/" +
        std::to_string(int(this->struct_storage.cap_kW)) +
        "kW_" +
        std::to_string(int(this->struct_storage.cap_kWh)) +
        "kWh_" +
        this->storage_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_storage.cap_kW)) +
        "kW_" +
        std::to_string(int(this->struct_storage.cap_kWh)) +
        "kWh_" + this->storage_type_str +
        "_" + std::to_string(asset_idx) +
        "_results.csv";
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + filename);
    
    // write file header
    ofs << "Time [hrs],"
        << "Charging (over time step) [kW],"
        << "Discharging (over time step) [kW],"
        << "Charge (at end of time step) [kWh],"
        << "Real Op & Maint Cost (over time step),"
        << "State of Health (at end of time step) [ ],"
        << "Is Replaced [T/F],"
        << "Real Capital Cost (incurred during time step),"
        //<< ","
        << "\n";
    
    // write file body
    for (int i = 0; i < this->n_timesteps; i++) {
        ofs << std::to_string(this->ptr_2_time_vec_hr->at(i)) << ","
            << std::to_string(this->charging_vec_kW[i]) << ","
            << std::to_string(this->discharging_vec_kW[i]) << ","
            << std::to_string(this->charge_vec_kWh[i]) << ","
            << std::to_string(this->real_op_maint_cost_vec[i]) << ","
            << std::to_string(this->SOH_vec[i]) << ","
            << std::to_string(this->replaced_vec[i]) << ","
            << std::to_string(this->real_capital_cost_vec[i]) << ","
            //<< [...] << ","
            << "\n";
    }
    
    ofs.close();
    
    return;
}


void LiIon :: _writeSummary(std::string _write_path, int asset_idx) {
    /*
     *  Helper method to write LiIon-level summary
     */
    
    // construct filename 
    std::string filename = "Storage/" +
        std::to_string(int(this->struct_storage.cap_kW)) +
        "kW_" +
        std::to_string(int(this->struct_storage.cap_kWh)) +
        "kWh_" +
        this->storage_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_storage.cap_kW)) +
        "kW_" +
        std::to_string(int(this->struct_storage.cap_kWh)) +
        "kWh_" + this->storage_type_str +
        "_" + std::to_string(asset_idx) +
        "_summary.txt";
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + filename);
    
    // write attributes
    ofs << this->struct_storage.cap_kW << " kW, " << 
        this->struct_storage.cap_kWh << " kWh LiIon Summary\n\n";
    ofs << "Attributes:\n\n";
    
    ofs << "\tsunk cost: " << std::boolalpha <<
        this->struct_storage.is_sunk << std::noboolalpha << "\n";
    ofs << "\tinitial state of charge: " <<
        this->struct_battery_storage.init_SOC << "\n";
    ofs << "\tminimum state of charge: " <<
        this->init_min_SOC << "\n";
    ofs << "\tmaximum state of charge: " <<
        this->struct_battery_storage.max_SOC << "\n";
    ofs << "\thysteresis state of charge: " <<
        this->struct_battery_storage.hysteresis_SOC << "\n";
    ofs << "\treserve state of charge: " <<
        this->struct_battery_storage.reserve_SOC << "\n";
    ofs << "\tcharging efficiency: " <<
        this->struct_battery_storage.charge_efficiency << "\n";
    ofs << "\tdischarging efficiency: " <<
        this->struct_battery_storage.discharge_efficiency << "\n";
    ofs << "\treplacement state of health: " <<
        this->struct_liion.replace_SOH << "\n";
    ofs << "\tcapital cost: " << this->struct_storage.capital_cost <<
        "\n";
    ofs << "\toperation and maintenance cost (per kWh charged/discharged): "
        << this->struct_storage.op_maint_cost_per_kWh << "\n";
    ofs << "\tnominal inflation rate (annual): " <<
        this->struct_storage.nominal_inflation_rate_annual << "\n";
    ofs << "\tnominal discount rate (annual): " <<
        this->struct_storage.nominal_discount_rate_annual << "\n";
    ofs << "\treal discount rate (annual): " <<
        this->real_discount_rate_annual << "\n";
    
    // write degradation parameters
    ofs << "\nDegradation Parameters:\n\n";
    
    ofs << "\toperating temperature: " <<
        this->struct_liion.temperature_K << " K\n";
    ofs << "\tgas constant: " <<
        this->struct_liion.gas_constant_JmolK << " J/mol.K\n\n";
    
    ofs << "\talpha = " << this->struct_liion.degr_alpha << "\n";
    ofs << "\tbeta = " << this->struct_liion.degr_beta << "\n";
    ofs << "\tB_hat_cal(0) = " <<
        this->struct_liion.degr_B_hat_cal_0 << " 1/sqrt(hr)\n";
    ofs << "\tr_cal = " << this->struct_liion.degr_r_cal << "\n";
    ofs << "\tEa_cal(0) = " <<
        this->struct_liion.degr_Ea_cal_0 << " J/mol\n";
    ofs << "\ta_cal = " << this->struct_liion.degr_a_cal << " J/mol\n";
    ofs << "\ts_cal = " << this->struct_liion.degr_s_cal << "\n";
    
    // write results
    ofs << "\nResults:\n\n";
    
    ofs << "\tproject life: " << this->project_life_yrs << " yrs\n";
    ofs << "\tnumber of replacements: " <<
        this->n_replacements << "\n";
    ofs << "\ttotal throughput (over project life): " <<
        this->total_throughput_kWh << " kWh\n";
    ofs << "\tnet present cost: " << this->net_present_cost << "\n";
    ofs << "\tlevellized cost of energy (per kWh throughput): " <<
        this->levellized_cost_of_energy_per_kWh << "\n";
    
    ofs.close();
    
    return;
}


double LiIon :: _getEacal(void) {
    /*
     *  Helper function to compute Ea_cal(SOC)
     *
     *  ref: docs/refs/battery_degradation.pdf
     */
    
    double SOC = this->charge_kWh / this->struct_storage.cap_kWh;
    
    double Ea_cal = this->struct_liion.degr_Ea_cal_0 -
        this->struct_liion.degr_a_cal * (
            exp(this->struct_liion.degr_s_cal * SOC) - 1
        );
    
    return Ea_cal;
}


double LiIon :: _getBcal(void) {
    /*
     *  Helper function to compute B_cal(SOC)
     *
     *  ref: docs/refs/battery_degradation.pdf
     */
    
    double SOC = this->charge_kWh / this->struct_storage.cap_kWh;
    
    double B_cal = this->struct_liion.degr_B_hat_cal_0 *
        exp(this->struct_liion.degr_r_cal * SOC);
    
    return B_cal;
}


double LiIon :: _getdSOHdtcal(void) {
    /*
     *  Helper function to compute dSOH_dt (calendar)
     *
     *  ref: docs/refs/battery_degradation.pdf
     */
    
    // compute Ea_cal and B_cal
    double Ea_cal = this->_getEacal();
    double B_cal = this->_getBcal();
    
    // compute dSOH_dt (calendar ageing)
    double dSOH_dt_cal = 1 / (2 * this->SOH);
    dSOH_dt_cal *= pow(
        B_cal * exp(
            (-1 * Ea_cal) /
            (
                this->struct_liion.gas_constant_JmolK *
                this->struct_liion.temperature_K
            )
        ),
        2
    );
    
    return dSOH_dt_cal;
}


double LiIon :: _getdSOHdt(double power_kW) {
    /*
     *  Helper method to compute and return dSOH_dt
     *
     *  ref: docs/refs/battery_degradation.pdf
     */
    
    // compute dSOH_dt (calendar)
    double dSOH_dt_cal = this->_getdSOHdtcal();
    
    // compute C-rate
    double C_rate = power_kW / this->struct_storage.cap_kWh;
    
    // compute dSOH_dt (total)
    double dSOH_dt = dSOH_dt_cal;
    dSOH_dt *= 1 + this->struct_liion.degr_alpha *
        pow(C_rate, this->struct_liion.degr_beta);
    
    return dSOH_dt;
}


void LiIon :: _handleDegradation(
    double power_kW,
    int timestep
) {
    /*
     *  Helper method to handle degradation dynamics
     *
     *  ref: docs/refs/battery_degradation.pdf
     */
    
    // update SOH
    double dt_hrs = this->ptr_2_dt_vec_hr->at(timestep);
    double dSOH_dt = this->_getdSOHdt(power_kW);
    this->SOH -= dSOH_dt * dt_hrs;
    this->SOH_vec[timestep] = this->SOH;
    
    // update energy capacity
    this->cap_kWh = this->SOH * this->struct_storage.cap_kWh;
    
    // update charge if now greater than cap_kWh
    if (this->charge_kWh >= this->cap_kWh) {
        this->charge_kWh = this->cap_kWh;
    }
    
    // update max charge if now greater than cap_kWh
    if (this->max_charge_kWh > this->cap_kWh) {
        this->max_charge_kWh = this->cap_kWh;
    }
    
    // update hysteresis SOC if now greater than SOH
    if (this->struct_battery_storage.hysteresis_SOC > this->SOH) {
        this->struct_battery_storage.hysteresis_SOC = this->SOH;
    }
    
    // trigger replacement, if necessary
    if (this->SOH <= this->struct_liion.replace_SOH) {
        this->_handleReplacement(timestep);
    }
    
    return;
}


void LiIon :: _handleReplacement(int timestep) {
    /*
     *  Helper method to handle degradation induced replacement
     */
    
    // reset attributes (replace with fresh LiIon system)
    this->cap_kWh = this->struct_storage.cap_kWh;
    this->SOH = 1;
    
    this->charge_kWh =
        this->struct_battery_storage.init_SOC *
        this->struct_storage.cap_kWh;
        
    this->max_charge_kWh =
        this->struct_battery_storage.max_SOC *
        this->struct_storage.cap_kWh;
    
    // toggle reserve (resets reserve_flag and min_charge_kWh)
    this->toggleReserve(false);
    
    // incur capital cost
    /*
     *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
     *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
     */
    double t_hrs = this->ptr_2_time_vec_hr->at(timestep);
    double real_discount_scalar = 1.0 / pow(
        1 + this->real_discount_rate_annual,
        t_hrs / 8760
    );
    
    this->real_capital_cost_vec[timestep] = real_discount_scalar *
        this->struct_storage.capital_cost;
    
    this->net_present_cost += this->real_capital_cost_vec[timestep];
    
    // record replacements
    this->n_replacements++;
    this->replaced_vec[timestep] = true;
    
    return;
}


void LiIon :: commitChargekW(
    double charging_kW,
    int timestep
) {
    /*
     *  Method to commit and record charging over timestep
     */
    
    // handle degradation
    this->_handleDegradation(charging_kW, timestep);
    
    // call out to BatteryStorage :: commitChargekW()
    BatteryStorage::commitChargekW(charging_kW, timestep);
    
    return;
}


void LiIon :: commitDischargekW(
    double discharging_kW,
    int timestep
) {
    /*
     *  Method to commit and record discharging over timestep
     */
    
    // handle degradation
    this->_handleDegradation(discharging_kW, timestep);
    
    // call out to BatteryStorage :: commitDischargekW()
    BatteryStorage::commitDischargekW(discharging_kW, timestep);
    
    return;
}


void LiIon :: writeResults(
    std::string _write_path,
    int asset_idx
) {
    /*
     *  Method to write LiIon-level results
     */
    
    this->_writeTimeSeriesResults(_write_path, asset_idx);
    this->_writeSummary(_write_path, asset_idx);
    
    return;
}


void LiIon :: toggleReserve(bool reserve_flag) {
    /*
     *  Method to toggle whether or not reserve energy is available for use.
     */
     
     BatteryStorage::toggleReserve(reserve_flag);
     
     return;
}


LiIon :: ~LiIon() {
    /*
     *  LiIon class destructor
     */
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tLiIon object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
