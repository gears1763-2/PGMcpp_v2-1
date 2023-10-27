/*
 *  Storage <-- BatteryStorage class implementation file
 */


#include "../../../../header/includes.h"
#include "../../../../header/assets/storage/Storage.h"
#include "../../../../header/assets/storage/batterystorage/BatteryStorage.h"


BatteryStorage :: BatteryStorage(
    structStorage struct_storage,
    structBatteryStorage struct_battery_storage,
    int n_timesteps
) : Storage(struct_storage, n_timesteps) {
    /*
     *  BatteryStorage class constructor
     */
    
    this->struct_battery_storage = struct_battery_storage;

    this->charge_kWh =
        this->struct_battery_storage.init_SOC *
        this->struct_storage.cap_kWh;
        
    this->min_charge_kWh =
        this->struct_battery_storage.min_SOC *
        this->struct_storage.cap_kWh;
        
    this->max_charge_kWh =
        this->struct_battery_storage.max_SOC *
        this->struct_storage.cap_kWh;
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tBatteryStorage object constructed at " << this
            << std::endl;
    }
    
    return;
}


double BatteryStorage :: getAvailablekW(double dt_hrs) {
    /*
     *  Method to compute and return available power [kW] over timestep
     */
    
    // check if depleted
    if (this->depleted_flag) {
        return 0;
    }
    
    // compute available energy
    double available_kWh = this->charge_kWh -
        this->min_charge_kWh;
    
    if (available_kWh <= 0) {
        return 0;
    }
    
    // translate into power over timestep, check against power capacity
    double available_kW =
        this->struct_battery_storage.discharge_efficiency * (
            available_kWh / dt_hrs
        );
    
    if (available_kW >= this->struct_storage.cap_kW) {
        available_kW = this->struct_storage.cap_kW;
    }
    
    return available_kW;
}


double BatteryStorage :: getAcceptablekW(double dt_hrs) {
    /*
     *  Method to compute and return acceptable power [kW] over timestep
     */
    
    // compute acceptable energy
    double acceptable_kWh = this->max_charge_kWh - 
        this->charge_kWh;
    
    if (acceptable_kWh <= 0) {
        return 0;
    }
    
    // translate into power over timestep, check against power capacity
    double acceptable_kW = (acceptable_kWh / dt_hrs) / 
        this->struct_battery_storage.charge_efficiency;
    
    if (acceptable_kW >= this->struct_storage.cap_kW) {
        acceptable_kW = this->struct_storage.cap_kW;
    }
    
    return acceptable_kW;
}


void BatteryStorage :: commitChargekW(
    double charging_kW,
    int timestep
) {
    /*
     *  Method to commit and record charging over timestep
     */
    
    // compute accepted energy
    double dt_hrs = this->ptr_2_dt_vec_hr->at(timestep);
    double accepted_kWh = 
        this->struct_battery_storage.charge_efficiency *
        charging_kW * dt_hrs;
    
    // incur operation and maintenance cost
    if (charging_kW > 0) {
        /*
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
         */
        double t_hrs = this->ptr_2_time_vec_hr->at(timestep);
        double real_discount_scalar = 1.0 / pow(
            1 + this->real_discount_rate_annual,
            t_hrs / 8760
        );
        
        double op_maint_cost = real_discount_scalar * 
            this->struct_storage.op_maint_cost_per_kWh * 
            charging_kW * dt_hrs;
        
        this->real_op_maint_cost_vec[timestep] = op_maint_cost;
        
        this->net_present_cost +=
            this->real_op_maint_cost_vec[timestep];
    }
    
    // update charge and record
    this->charge_kWh += accepted_kWh;
    this->charge_vec_kWh[timestep] = this->charge_kWh;
    this->charging_vec_kW[timestep] = charging_kW;
    
    // set depleted flag
    if (this->depleted_flag) {
        double SOC = this->charge_kWh / this->struct_storage.cap_kWh;
        if (SOC >= this->struct_battery_storage.hysteresis_SOC) {
            this->depleted_flag = false;
        }
    }
    
    return;
}


void BatteryStorage :: commitDischargekW(
    double discharging_kW,
    int timestep
) {
    /*
     *  Method to commit and record discharging over timestep
     */
    
    // compute discharged energy
    double dt_hrs = this->ptr_2_dt_vec_hr->at(timestep);
    double discharged_kWh = (discharging_kW * dt_hrs) / 
        this->struct_battery_storage.discharge_efficiency;
    this->total_throughput_kWh += discharged_kWh;
    
    // incur operation and maintenance cost
    if (discharging_kW > 0) {
        /*
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/real_discount_rate.html
         *  ref: https://www.homerenergy.com/products/pro/docs/latest/present_value.html
         */
        double t_hrs = this->ptr_2_time_vec_hr->at(timestep);
        double real_discount_scalar = 1.0 / pow(
            1 + this->real_discount_rate_annual,
            t_hrs / 8760
        );
        
        double op_maint_cost = real_discount_scalar * 
            this->struct_storage.op_maint_cost_per_kWh * 
            discharging_kW * dt_hrs;
        
        this->real_op_maint_cost_vec[timestep] = op_maint_cost;
        
        this->net_present_cost +=
            this->real_op_maint_cost_vec[timestep];
    }
    
    // update charge and record 
    this->charge_kWh -= discharged_kWh;
    this->charge_vec_kWh[timestep] = this->charge_kWh;
    this->discharging_vec_kW[timestep] = discharging_kW;
    
    // set depleted flag
    if (not this->depleted_flag) {
        double SOC = this->charge_kWh / this->struct_storage.cap_kWh;
        if (SOC <= this->struct_battery_storage.min_SOC) {
            this->depleted_flag = true;
        }
    }
    
    return;
}


BatteryStorage :: ~BatteryStorage() {
    /*
     *  BatteryStorage class destructor
     */
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tBatteryStorage object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
