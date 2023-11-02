/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Python 3 bindings file for PGMcpp.
 *
 *  ref: https://pybind11.readthedocs.io/en/stable/
 * 
 * 
 *  Model::addHydro() has not been bound as Dispatchable <-- Hydro is not yet
 *  implemented and tested.
 *
 */


#include "../header/includes.h"
#include "../header/Model_includes.h"
#include "../header/Model.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


PYBIND11_MODULE(PGMcpp, m) {
    
    pybind11::enum_<NondispatchableType>(m, "NondispatchableType")
        .value("SOLAR", NondispatchableType::SOLAR)
        .value("TIDAL", NondispatchableType::TIDAL)
        .value("WAVE", NondispatchableType::WAVE)
        .value("WIND", NondispatchableType::WIND);
    
    
    pybind11::class_<structNondispatchable>(m, "structNondispatchable")
        .def_readwrite("is_sunk", &structNondispatchable::is_sunk)
        .def_readwrite("print_flag", &structNondispatchable::print_flag)
        .def_readwrite("test_flag", &structNondispatchable::test_flag)
        .def_readwrite("cap_kW", &structNondispatchable::cap_kW)
        .def_readwrite(
            "replace_running_hrs",
            &structNondispatchable::replace_running_hrs
        )
        .def_readwrite(
            "nominal_inflation_rate_annual",
            &structNondispatchable::nominal_inflation_rate_annual
        )
        .def_readwrite(
            "nominal_discount_rate_annual",
            &structNondispatchable::nominal_discount_rate_annual
        )
        .def_readwrite("capital_cost", &structNondispatchable::capital_cost)
        .def_readwrite(
            "op_maint_cost_per_kWh",
            &structNondispatchable::op_maint_cost_per_kWh
        )
        
        .def(pybind11::init());
    
    
    pybind11::class_<structSolar>(m, "structSolar")
        .def_readwrite("resource_key", &structSolar::resource_key)
        .def_readwrite("derating", &structSolar::derating)
        .def_readwrite("capital_cost_per_kW", &structSolar::capital_cost_per_kW)
        
        .def(pybind11::init());
    
    
    pybind11::enum_<TidalPowerCurve>(m, "TidalPowerCurve")
        .value("CUBIC", TidalPowerCurve::CUBIC)
        .value("EXPONENTIAL", TidalPowerCurve::EXPONENTIAL);
    
    
    pybind11::class_<structTidal>(m, "structTidal")
        .def_readwrite("resource_key", &structTidal::resource_key)
        .def_readwrite("power_curve", &structTidal::power_curve)
        .def_readwrite("design_speed_ms", &structTidal::design_speed_ms)
        
        .def(pybind11::init());
    
    
    pybind11::enum_<WavePowerMode>(m, "WavePowerMode")
        .value("GAUSSIAN", WavePowerMode::GAUSSIAN)
        .value(
            "NORMALIZED_PERFORMANCE_MATRIX",
            WavePowerMode::NORMALIZED_PERFORMANCE_MATRIX
        )
        .value("PARABOLOID", WavePowerMode::PARABOLOID);
    
    
    pybind11::class_<structWave>(m, "structWave")
        .def_readwrite("resource_key", &structWave::resource_key)
        .def_readwrite("power_mode", &structWave::power_mode)
        .def_readwrite(
            "design_significant_wave_height_m",
            &structWave::design_significant_wave_height_m
        )
        .def_readwrite(
            "design_energy_period_s",
            &structWave::design_energy_period_s
        )
        .def_readwrite(
            "path_2_normalized_performance_matrix",
            &structWave::path_2_normalized_performance_matrix
        )
        
        .def(pybind11::init());
    
    
    pybind11::class_<structWind>(m, "structWind")
        .def_readwrite("resource_key", &structWind::resource_key)
        .def_readwrite("design_speed_ms", &structWind::design_speed_ms)
        
        .def(pybind11::init());
    
    
    pybind11::enum_<DispatchableType>(m, "DispatchableType")
        .value("DIESEL", DispatchableType::DIESEL)
        .value("HYDRO", DispatchableType::HYDRO);
    
    
    pybind11::class_<structDispatchable>(m, "structDispatchable")
        .def_readwrite("is_sunk", &structDispatchable::is_sunk)
        .def_readwrite("print_flag", &structDispatchable::print_flag)
        .def_readwrite("test_flag", &structDispatchable::test_flag)
        .def_readwrite("cap_kW", &structDispatchable::cap_kW)
        .def_readwrite("replace_running_hrs", &structDispatchable::replace_running_hrs)
        .def_readwrite(
            "nominal_inflation_rate_annual",
            &structDispatchable::nominal_inflation_rate_annual
        )
        .def_readwrite(
            "nominal_discount_rate_annual",
            &structDispatchable::nominal_discount_rate_annual
        )
        .def_readwrite("capital_cost", &structDispatchable::capital_cost)
        .def_readwrite(
            "op_maint_cost_per_kWh",
            &structDispatchable::op_maint_cost_per_kWh
        )
        
        .def(pybind11::init());
    
    
    pybind11::enum_<FuelMode>(m, "FuelMode")
        .value("LINEAR", FuelMode::LINEAR)
        .value("LOOKUP", FuelMode::LOOKUP);
    
    
    pybind11::enum_<FuelType>(m, "FuelType")
        .value("FUEL_DIESEL", FuelType::FUEL_DIESEL)
        .value("FUEL_GAS", FuelType::FUEL_GAS);
    
    
    pybind11::class_<structCombustion>(m, "structCombustion")
        .def_readwrite("fuel_mode", &structCombustion::fuel_mode)
        .def_readwrite(
            "cycle_charging_load_ratio",
            &structCombustion::cycle_charging_load_ratio
        )
        .def_readwrite("fuel_cost_L", &structCombustion::fuel_cost_L)
        .def_readwrite(
            "nominal_fuel_escalation_rate_annual",
            &structCombustion::nominal_fuel_escalation_rate_annual
        )
        .def_readwrite(
            "linear_fuel_intercept_LkWh",
            &structCombustion::linear_fuel_intercept_LkWh
        )
        .def_readwrite(
            "linear_fuel_slope_LkWh",
            &structCombustion::linear_fuel_slope_LkWh
        )
        .def_readwrite(
            "path_2_fuel_consumption_data",
            &structCombustion::path_2_fuel_consumption_data
        )
        .def_readwrite("diesel_CO2_kgL", &structCombustion::diesel_CO2_kgL)
        .def_readwrite("diesel_CO_kgL", &structCombustion::diesel_CO_kgL)
        .def_readwrite("diesel_NOx_kgL", &structCombustion::diesel_NOx_kgL)
        .def_readwrite("diesel_SOx_kgL", &structCombustion::diesel_SOx_kgL)
        .def_readwrite("diesel_CH4_kgL", &structCombustion::diesel_CH4_kgL)
        .def_readwrite("diesel_PM_kgL", &structCombustion::diesel_PM_kgL)
        
        .def(pybind11::init());
    
    
    pybind11::class_<structDiesel>(m, "structDiesel")
        .def_readwrite("minimum_load_ratio", &structDiesel::minimum_load_ratio)
        .def_readwrite("minimum_runtime_hrs", &structDiesel::minimum_runtime_hrs)
        
        .def(pybind11::init());
    
    
    pybind11::enum_<StorageType>(m, "StorageType")
        .value("LIION", StorageType::LIION);
    
    
    pybind11::class_<structStorage>(m, "structStorage")
        .def_readwrite("is_sunk", &structStorage::is_sunk)
        .def_readwrite("print_flag", &structStorage::print_flag)
        .def_readwrite("test_flag", &structStorage::test_flag)
        .def_readwrite("cap_kW", &structStorage::cap_kW)
        .def_readwrite("cap_kWh", &structStorage::cap_kWh)
        .def_readwrite(
            "nominal_inflation_rate_annual",
            &structStorage::nominal_inflation_rate_annual
        )
        .def_readwrite(
            "nominal_discount_rate_annual",
            &structStorage::nominal_discount_rate_annual
        )
        .def_readwrite("capital_cost", &structStorage::capital_cost)
        .def_readwrite(
            "op_maint_cost_per_kWh",
            &structStorage::op_maint_cost_per_kWh
        )
        
        .def(pybind11::init());
    
    
    pybind11::class_<structBatteryStorage>(m, "structBatteryStorage")
        .def_readwrite("init_SOC", &structBatteryStorage::init_SOC)
        .def_readwrite("min_SOC", &structBatteryStorage::min_SOC)
        .def_readwrite("max_SOC", &structBatteryStorage::max_SOC)
        .def_readwrite("hysteresis_SOC", &structBatteryStorage::hysteresis_SOC)
        .def_readwrite("charge_efficiency", &structBatteryStorage::charge_efficiency)
        .def_readwrite(
            "discharge_efficiency",
            &structBatteryStorage::discharge_efficiency
        )
        
        .def(pybind11::init());
    
    
    pybind11::class_<structLiIon>(m, "structLiIon")
        .def_readwrite("replace_SOH", &structLiIon::replace_SOH)
        .def_readwrite("degr_alpha", &structLiIon::degr_alpha)
        .def_readwrite("degr_beta)", &structLiIon::degr_beta)
        .def_readwrite("degr_B_hat_cal_0", &structLiIon::degr_B_hat_cal_0)
        .def_readwrite("degr_r_cal", &structLiIon::degr_r_cal)
        .def_readwrite("degr_Ea_cal_0", &structLiIon::degr_Ea_cal_0)
        .def_readwrite("degr_a_cal", &structLiIon::degr_a_cal)
        .def_readwrite("degr_s_cal", &structLiIon::degr_s_cal)
        .def_readwrite("gas_constant_JmolK", &structLiIon::gas_constant_JmolK)
        .def_readwrite("temperature_K", &structLiIon::temperature_K)
        
        .def(pybind11::init());
    
    
    pybind11::enum_<DispatchMode>(m, "DispatchMode")
        .value("LOAD_FOLLOWING_IN_ORDER", DispatchMode::LOAD_FOLLOWING_IN_ORDER)
        .value("CYCLE_CHARGING_IN_ORDER", DispatchMode::CYCLE_CHARGING_IN_ORDER)
        .value(
            "LOAD_FOLLOWING_SMART_COMBUSTION",
            DispatchMode::LOAD_FOLLOWING_SMART_COMBUSTION
        )
        .value(
            "CYCLE_CHARGING_SMART_COMBUSTION",
            DispatchMode::CYCLE_CHARGING_SMART_COMBUSTION
        )
        .value("FORESIGHT", DispatchMode::FORESIGHT);
    
    
    pybind11::class_<structModel>(m, "structModel")
        .def_readwrite("print_flag", &structModel::print_flag)
        .def_readwrite("test_flag", &structModel::test_flag)
        .def_readwrite("dispatch_mode", &structModel::dispatch_mode)
        .def_readwrite(
            "nominal_inflation_rate_annual",
            &structModel::nominal_inflation_rate_annual
        )
        .def_readwrite(
            "nominal_discount_rate_annual",
            &structModel::nominal_discount_rate_annual
        )
        .def_readwrite(
            "path_2_load_data",
            &structModel::path_2_load_data
        )
        
        .def(pybind11::init());
    
    
    pybind11::class_<Model>(m, "Model")
        .def_readwrite("struct_model", &Model::struct_model)
        .def_readwrite("n_timesteps", &Model::n_timesteps)
        .def_readwrite("project_life_yrs", &Model::project_life_yrs)
        .def_readwrite("total_load_served_kWh", &Model::total_load_served_kWh)
        .def_readwrite("total_fuel_consumed_L", &Model::total_fuel_consumed_L)
        .def_readwrite("total_CO2_emitted_kg", &Model::total_CO2_emitted_kg)
        .def_readwrite("total_CO_emitted_kg", &Model::total_CO_emitted_kg)
        .def_readwrite("total_NOx_emitted_kg", &Model::total_NOx_emitted_kg)
        .def_readwrite("total_SOx_emitted_kg", &Model::total_SOx_emitted_kg)
        .def_readwrite("total_CH4_emitted_kg", &Model::total_CH4_emitted_kg)
        .def_readwrite("total_PM_emitted_kg", &Model::total_PM_emitted_kg)
        .def_readwrite("real_discount_rate_annual", &Model::real_discount_rate_annual)
        .def_readwrite("net_present_cost", &Model::net_present_cost)
        .def_readwrite(
            "levellized_cost_of_energy_per_kWh",
            &Model::levellized_cost_of_energy_per_kWh
        )
        .def_readwrite("dt_vec_hr", &Model::dt_vec_hr)
        .def_readwrite("load_vec_kW", &Model::load_vec_kW)
        .def_readwrite("net_load_vec_kW", &Model::net_load_vec_kW)
        .def_readwrite("remaining_load_vec_kW", &Model::remaining_load_vec_kW)
        .def_readwrite("time_vec_hr", &Model::time_vec_hr)
        
        .def(pybind11::init<structModel>())
        .def("add1dRenewableResource", &Model::add1dRenewableResource)
        .def("add2dRenewableResource", &Model::add2dRenewableResource)
        .def("addSolar", &Model::addSolar)
        .def("addTidal", &Model::addTidal)
        .def("addWave", &Model::addWave)
        .def("addWind", &Model::addWind)
        .def("addDiesel", &Model::addDiesel)
        .def("addLiIon", &Model::addLiIon)
        .def("run", &Model::run)
        .def("writeResults", &Model::writeResults)
        .def("clearAssets", &Model::clearAssets)
        .def("reset", &Model::reset);
}
