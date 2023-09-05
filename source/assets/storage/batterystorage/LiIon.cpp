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
    
    this->struct_storage.storage_type = STORAGE_LIION;
    this->struct_liion = struct_liion;
    
    this->struct_liion.init_cap_kWh = this->struct_storage.cap_kWh;
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tLiIon object constructed at " << this
            << std::endl;
    }
    
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
        this->_handleReplacement(timestep);
    }
    
    return;
}


void LiIon :: _handleReplacement(int timestep) {
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
    //..
    
    // record replacements
    this->struct_storage.n_replacements++;
    this->replaced_vec[timestep] = true;
    
    return;
}


void LiIon :: commitChargekW(
    double charging_kW,
    double dt_hrs,
    int timestep
) {
    /*
     *  Method to commit and record charging over timestep
     */
    
    // call out to BatteryStorage :: commitChargekW()
    BatteryStorage::commitChargekW(charging_kW, dt_hrs, timestep);
    
    // handle degradation
    this->_handleDegradation(charging_kW, dt_hrs, timestep);
    
    return;
}


void LiIon :: commitDischargekW(
    double discharging_kW,
    double dt_hrs,
    int timestep
) {
    /*
     *  Method to commit and record discharging over timestep
     */
    
    // call out to BatteryStorage :: commitDischargekW()
    BatteryStorage::commitDischargekW(discharging_kW, dt_hrs, timestep);
    
    // handle degradation
    this->_handleDegradation(discharging_kW, dt_hrs, timestep);
    
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
