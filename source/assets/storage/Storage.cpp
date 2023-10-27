/*
 *  Storage class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/storage/Storage.h"


Storage :: Storage(
    structStorage struct_storage,
    int n_timesteps
) {
    /*
     *  Storage class constructor
     */
    
    this->depleted_flag = false;
    
    this->struct_storage = struct_storage;
    this->n_timesteps = n_timesteps;
    this->cap_kWh = this->struct_storage.cap_kWh;
    
    this->replaced_vec.resize(this->n_timesteps, false);
    
    this->charge_vec_kWh.resize(this->n_timesteps, 0);
    this->charging_vec_kW.resize(this->n_timesteps, 0);
    this->discharging_vec_kW.resize(this->n_timesteps, 0);
    
    real_capital_cost_vec.resize(this->n_timesteps, 0);
    real_op_maint_cost_vec.resize(this->n_timesteps, 0);
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tStorage object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Storage :: computeLevellizedCostOfEnergy() {
    /*
     *  Method to compute levellized cost of energy
     * 
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/levelized_cost_of_energy.html
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/total_annualized_cost.html
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/capital_recovery_factor.html
     */
    
    if (this->total_throughput_kWh <= 0) {
        return;
    }
    
    double capital_recovery_factor = 
        (
            this->real_discount_rate_annual *
            pow(
                1 + this->real_discount_rate_annual,
                this->project_life_yrs
            )
        ) / 
        (
            pow(
                1 + this->real_discount_rate_annual,
                this->project_life_yrs
            ) -
            1
        );
        
    double total_annualized_cost = capital_recovery_factor *
        this->net_present_cost;
    
    this->levellized_cost_of_energy_per_kWh =
        (this->project_life_yrs * total_annualized_cost) /
        this->total_throughput_kWh;
    
    return;
}


Storage :: ~Storage() {
    /*
     *  Storage class destructor
     */
    
    if (this->struct_storage.test_flag) {
        std::cout << "\tStorage object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
