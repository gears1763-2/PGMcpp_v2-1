/*
 *  Storage <-- BatteryStorage class implementation file
 */


#include "../../../../header/includes.h"
#include "../../../../header/assets/storage/Storage.h"
#include "../../../../header/assets/storage/batterystorage/BatteryStorage.h"


BatteryStorage :: BatteryStorage(
    structStorage struct_storage,
    structBatteryStorage struct_battery_storage
) : Storage(struct_storage) {
    /*
     *  BatteryStorage class constructor
     */
    
    this->struct_battery_storage = struct_battery_storage;

    this->struct_storage.charge_kWh =
        this->struct_battery_storage.init_SOC *
        this->struct_storage.cap_kWh;
        
    this->struct_storage.min_charge_kWh =
        this->struct_battery_storage.min_SOC *
        this->struct_storage.cap_kWh;
        
    this->struct_storage.max_charge_kWh =
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
    
    // compute available energy
    double available_kWh = this->struct_storage.charge_kWh -
        this->struct_storage.min_charge_kWh;
    
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
    double acceptable_kWh = this->struct_storage.max_charge_kWh - 
        this->struct_storage.charge_kWh;
    
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
    double dt_hrs,
    int timestep
) {
    /*
     *  Method to commit and record charging over timestep
     */
    
    // compute accepted energy
    double accepted_kWh = 
        this->struct_battery_storage.charge_efficiency *
        charging_kW * dt_hrs;
    
    // update charge and record
    this->struct_storage.charge_kWh += accepted_kWh;
    this->charge_vec_kWh[timestep] = this->struct_storage.charge_kWh;
    this->charging_vec_kW[timestep] = charging_kW;
    
    return;
}


void BatteryStorage :: commitDischargekW(
    double discharging_kW,
    double dt_hrs,
    int timestep
) {
    /*
     *  Method to commit and record discharging over timestep
     */
    
    // compute discharged energy
    double discharged_kWh = (discharging_kW * dt_hrs) / 
        this->struct_battery_storage.discharge_efficiency;
    
    // update charge and record 
    this->struct_storage.charge_kWh -= discharged_kWh;
    this->charge_vec_kWh[timestep] = this->struct_storage.charge_kWh;
    this->discharging_vec_kW[timestep] = discharging_kW;
    
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
