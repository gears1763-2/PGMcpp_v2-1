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
 */


#include "../header/includes.h"
#include "../header/Model_includes.h"
#include "../header/Model.h"

#include <pybind11/pybind11.h>


PYBIND11_MODULE(PGMcpp, m) {
    
    pybind11::enum_<DispatchMode>(m, "DispatchMode")
        .value("LOAD_FOLLOWING_IN_ORDER", DispatchMode::LOAD_FOLLOWING_IN_ORDER)
        .value("CYCLE_CHARGING_IN_ORDER", DispatchMode::CYCLE_CHARGING_IN_ORDER)
        .value("LOAD_FOLLOWING_SMART_COMBUSTION", DispatchMode::LOAD_FOLLOWING_SMART_COMBUSTION)
        .value("CYCLE_CHARGING_SMART_COMBUSTION", DispatchMode::CYCLE_CHARGING_SMART_COMBUSTION)
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
        
        .def(pybind11::init<structModel>());
}

/*
double total_CO2_emitted_kg = 0;
double total_CO_emitted_kg = 0;
double total_NOx_emitted_kg = 0;
double total_SOx_emitted_kg = 0;
double total_CH4_emitted_kg = 0;
double total_PM_emitted_kg = 0;

double real_discount_rate_annual = 0;
double net_present_cost = 0;
double levellized_cost_of_energy_per_kWh = 0;

std::vector<double> dt_vec_hr;
std::vector<double> load_vec_kW;
std::vector<double> net_load_vec_kW;
std::vector<double> remaining_load_vec_kW;
std::vector<double> time_vec_hr;
 */
