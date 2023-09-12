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
    structLiIon struct_liion
) : BatteryStorage(struct_storage, struct_battery_storage) {
    /*
     *  LiIon class constructor
     */
    
    this->struct_storage.storage_type = LIION;
    this->struct_storage.storage_type_str = "LIION";
    this->struct_liion = struct_liion;
    
    this->struct_liion.init_cap_kWh = this->struct_storage.cap_kWh;
    
    this->SOH_vec.resize(this->struct_storage.n_timesteps, 0);
    this->SOH_vec[0] = this->struct_liion.SOH;
    
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
    }
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tLiIon object constructed at " << this
            << std::endl;
    }
    
    return;
}


void LiIon :: _writeTimeSeriesResults(
    std::string _write_path,
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Helper method to write LiIon-level time series results
     */
    
    // construct filename 
    std::string filename = "Storage/" +
        std::to_string(int(this->struct_storage.cap_kW)) +
        "kW_" +
        std::to_string(int(this->struct_liion.init_cap_kWh)) +
        "kWh_" +
        this->struct_storage.storage_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_storage.cap_kW)) +
        "kW_" +
        std::to_string(int(this->struct_liion.init_cap_kWh)) +
        "kWh_" + this->struct_storage.storage_type_str +
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
    for (int i = 0; i < this->struct_storage.n_timesteps; i++) {
        ofs << std::to_string(ptr_2_time_vec_hr->at(i)) << ","
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
        std::to_string(int(this->struct_liion.init_cap_kWh)) +
        "kWh_" +
        this->struct_storage.storage_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_storage.cap_kW)) +
        "kW_" +
        std::to_string(int(this->struct_liion.init_cap_kWh)) +
        "kWh_" + this->struct_storage.storage_type_str +
        "_" + std::to_string(asset_idx) +
        "_summary.txt";
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + filename);
    
    // write attributes
    ofs << this->struct_storage.cap_kW << " kW, " << 
        this->struct_liion.init_cap_kWh << " kWh LiIon Summary\n\n";
    ofs << "Attributes:\n\n";
    
    ofs << "\tcapital cost: " << this->struct_storage.capital_cost <<
        "\n";
    ofs << "\toperation and maintenance cost (per kWh produced): " <<
        this->struct_storage.op_maint_cost_per_kWh << "\n";
    ofs << "\treal discount rate (annual): " <<
        this->struct_storage.real_discount_rate_annual << "\n";
    
    // write results
    ofs << "\nResults:\n\n";
    
    ofs << "\tnumber of replacements: " <<
        this->struct_storage.n_replacements << "\n";
    
    ofs.close();
    
    return;
}


double LiIon :: _getdSOHdt(double power_kW) {
    /*
     *  Helper method to compute and return dSOH_dt
     */
    
    // compute SOC and C-rate
    double SOC = this->struct_storage.charge_kWh /
        this->struct_liion.init_cap_kWh;
    
    double C_rate = power_kW / this->struct_liion.init_cap_kWh;
    
    // build up dSOH_dt in steps
    double dSOH_dt = this->struct_liion.degr_Ea_cal_0;
    dSOH_dt -= this->struct_liion.degr_a_cal * (
        exp(this->struct_liion.degr_s_cal * SOC) - 1
    );
    dSOH_dt /= this->struct_liion.gas_constant_JmolK * 
        this->struct_liion.temperature_K;
    dSOH_dt = this->struct_liion.degr_r_cal * SOC - dSOH_dt;
    dSOH_dt = this->struct_liion.degr_B_hat_cal_0 * exp(dSOH_dt);
    dSOH_dt *= dSOH_dt;
    dSOH_dt *= 1 + this->struct_liion.degr_alpha * pow(
        C_rate, this->struct_liion.degr_beta
    );
    dSOH_dt /= 2 * this->struct_liion.SOH;
    
    return dSOH_dt;
}


void LiIon :: _handleDegradation(
    double power_kW,
    double dt_hrs,
    double t_hrs,
    int timestep
) {
    /*
     *  Helper method to handle degradation dynamics
     *
     *  ref: [point to battery degradation modelling document]
     */
    
    // update SOH
    double dSOH_dt = this->_getdSOHdt(power_kW);
    this->struct_liion.SOH -= dSOH_dt * dt_hrs;
    this->SOH_vec[timestep] = this->struct_liion.SOH;
    
    // update energy capacity and charge accordingly
    this->struct_storage.cap_kWh = this->struct_liion.SOH * 
        this->struct_liion.init_cap_kWh;
        
    if (
        this->struct_storage.charge_kWh >=
        this->struct_storage.cap_kWh
    ) {
        this->struct_storage.charge_kWh = this->struct_storage.cap_kWh;
    }
    
    // trigger replacement, if necessary
    if (this->struct_liion.SOH <= this->struct_liion.replace_SOH) {
        this->_handleReplacement(timestep, t_hrs);
    }
    
    return;
}


void LiIon :: _handleReplacement(int timestep, double t_hrs) {
    /*
     *  Helper method to handle degradation induced replacement
     */
    
    // reset attributes (replace with fresh LiIon system)
    this->struct_storage.cap_kWh = this->struct_liion.init_cap_kWh;
    
    this->struct_storage.charge_kWh =
        this->struct_battery_storage.init_SOC *
        this->struct_storage.cap_kWh;
        
    this->struct_liion.SOH = 1;
    
    // incur capital cost
    /*
     *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
     *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
     */
    double real_discount_scalar = 1.0 / pow(
        1 + this->struct_storage.real_discount_rate_annual,
        t_hrs / 8760
    );
    
    this->real_capital_cost_vec[timestep] = real_discount_scalar *
        this->struct_storage.capital_cost;
    
    // record replacements
    this->struct_storage.n_replacements++;
    this->replaced_vec[timestep] = true;
    
    return;
}


void LiIon :: commitChargekW(
    double charging_kW,
    double dt_hrs,
    double t_hrs,
    int timestep
) {
    /*
     *  Method to commit and record charging over timestep
     */
    
    // call out to BatteryStorage :: commitChargekW()
    BatteryStorage::commitChargekW(
        charging_kW, dt_hrs, t_hrs, timestep
    );
    
    // handle degradation
    this->_handleDegradation(charging_kW, dt_hrs, t_hrs, timestep);
    
    return;
}


void LiIon :: commitDischargekW(
    double discharging_kW,
    double dt_hrs,
    double t_hrs,
    int timestep
) {
    /*
     *  Method to commit and record discharging over timestep
     */
    
    // call out to BatteryStorage :: commitDischargekW()
    BatteryStorage::commitDischargekW(
        discharging_kW, dt_hrs, t_hrs, timestep
    );
    
    // handle degradation
    this->_handleDegradation(discharging_kW, dt_hrs, t_hrs, timestep);
    
    return;
}


void LiIon :: writeResults(
    std::string _write_path,
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Method to write LiIon-level results
     */
    
    this->_writeTimeSeriesResults(
        _write_path, ptr_2_time_vec_hr, asset_idx
    );
    this->_writeSummary(_write_path, asset_idx);
    
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
