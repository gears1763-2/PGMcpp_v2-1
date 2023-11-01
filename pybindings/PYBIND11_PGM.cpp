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
