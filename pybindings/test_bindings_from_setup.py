"""
    PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1

    Anthony Truelove MASc, P.Eng.
    email:  gears1763@tutanota.com
    github: gears1763-2

    See license terms
 
    Testing script for Python 3 bindings.
    
    ref: https://stackoverflow.com/questions/39473297/how-do-i-print-colored-output-with-python-3
"""


import os
import sys
import PGMcpp


print()
print("## ======== Testing Python Bindings ======== ##")
print()

os.system("color")
end = "\n"

try:
    #   1. structModel
    print("\tTesting structModel ... ", end="", flush=True)

    struct_model = PGMcpp.structModel()
    assert(not struct_model.print_flag)
    assert(not struct_model.test_flag)
    assert(
        struct_model.dispatch_mode ==
        PGMcpp.DispatchMode.LOAD_FOLLOWING_IN_ORDER
    )
    assert(struct_model.nominal_inflation_rate_annual == 0.02)
    assert(struct_model.nominal_discount_rate_annual == 0.04)
    assert(struct_model.path_2_load_data == "")

    sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)


    #   2. DispatchMode
    print("\tTesting DispatchMode ... ", end="", flush=True)

    struct_model.dispatch_mode = PGMcpp.DispatchMode.CYCLE_CHARGING_IN_ORDER
    assert(
        struct_model.dispatch_mode ==
        PGMcpp.DispatchMode.CYCLE_CHARGING_IN_ORDER
    )
    struct_model.dispatch_mode = PGMcpp.DispatchMode.LOAD_FOLLOWING_SMART_COMBUSTION
    assert(
        struct_model.dispatch_mode ==
        PGMcpp.DispatchMode.LOAD_FOLLOWING_SMART_COMBUSTION
    )
    struct_model.dispatch_mode = PGMcpp.DispatchMode.CYCLE_CHARGING_SMART_COMBUSTION
    assert(
        struct_model.dispatch_mode ==
        PGMcpp.DispatchMode.CYCLE_CHARGING_SMART_COMBUSTION
    )
    struct_model.dispatch_mode = PGMcpp.DispatchMode.FORESIGHT
    assert(struct_model.dispatch_mode == PGMcpp.DispatchMode.FORESIGHT)

    sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)


    #   3. Model
    print("\tTesting Model ... ")
    
    
    #   3.1. construct
    print("\t\tModel construction ...", end="", flush=True)
    
    struct_model.path_2_load_data = "../data/input/test/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv"
    struct_model.dispatch_mode = PGMcpp.DispatchMode.LOAD_FOLLOWING_IN_ORDER
    
    test_model = PGMcpp.Model(struct_model)
    
    assert(test_model.struct_model.path_2_load_data == struct_model.path_2_load_data)
    assert(test_model.n_timesteps == 8760)
    assert(abs(test_model.project_life_yrs - 1) < 1e-3)
    assert(test_model.total_load_served_kWh == 0)
    assert(test_model.total_fuel_consumed_L == 0)
    assert(test_model.total_CO2_emitted_kg == 0)
    assert(test_model.total_CO_emitted_kg == 0)
    assert(test_model.total_NOx_emitted_kg == 0)
    assert(test_model.total_SOx_emitted_kg == 0)
    assert(test_model.total_CH4_emitted_kg == 0)
    assert(test_model.total_PM_emitted_kg == 0)
    assert(abs(test_model.real_discount_rate_annual - 0.019607) < 1e-6)
    assert(test_model.net_present_cost == 0)
    assert(test_model.levellized_cost_of_energy_per_kWh == 0)
    
    assert(len(test_model.dt_vec_hr) == test_model.n_timesteps)
    assert(len(test_model.load_vec_kW) == test_model.n_timesteps)
    assert(len(test_model.net_load_vec_kW) == test_model.n_timesteps)
    assert(len(test_model.remaining_load_vec_kW) == test_model.n_timesteps)
    assert(len(test_model.time_vec_hr) == test_model.n_timesteps)
    
    expected_load_kW = [
        360.253836463674,
        355.171277826775,
        353.776453532298,
        353.75405737934,
        346.592867404975,
        340.132411175118,
        337.354867340578,
        340.644115618736,
        363.639028500678,
        378.787797779238,
        372.215798201712,
        395.093925731298,
        402.325427142659,
        386.907725462306,
        380.709170928091,
        372.062070914977
    ]
    
    for i in range(0, 16):
        assert(test_model.dt_vec_hr[i] == 1)
        assert(abs(test_model.load_vec_kW[i] - expected_load_kW[i]) < 1e-6)
        assert(test_model.net_load_vec_kW[i] == 0)
        assert(test_model.remaining_load_vec_kW[i] == 0)
        assert(test_model.time_vec_hr[i] == i)
    
    sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
    
    
    #   3.2. add Solar resource and asset
    print("\t\tadd Solar resource and asset ...", end="", flush=True)
    
    solar_resource_key = 1
    test_model.add1dRenewableResource(
        "solar",
        "../data/input/test/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv",
        solar_resource_key
    )
    
    struct_nondisp = PGMcpp.structNondispatchable()
    
    struct_solar = PGMcpp.structSolar()
    struct_solar.resource_key = solar_resource_key
    
    test_model.addSolar(struct_nondisp, struct_solar)
    
    sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
    
    
    #   3.3. add Tidal resource and asset
    print("\t\tadd Tidal resource and asset ...", end="", flush=True)
    
    tidal_resource_key = 2
    test_model.add1dRenewableResource(
        "tidal",
        "../data/input/test/tidal_speed_peak-3ms_1yr_dt-1hr.csv",
        tidal_resource_key
    )
    
    struct_tidal = PGMcpp.structTidal()
    struct_tidal.resource_key = tidal_resource_key
    
    test_model.addTidal(struct_nondisp, struct_tidal)
    
    sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
    
    
    #   3.4. add Wave resource and asset
    print("\t\tadd Wave resource and asset ...", end="", flush=True)
    
    wave_resource_key = 3
    test_model.add2dRenewableResource(
        "wave",
        "../data/input/test/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv",
        wave_resource_key
    )
    
    struct_wave = PGMcpp.structWave()
    struct_wave.resource_key = wave_resource_key
    
    test_model.addWave(struct_nondisp, struct_wave)
    
    sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
    
    
    #   3.5. add Wind resource and asset
    print("\t\tadd Wind resource and asset ...", end="", flush=True)
    
    wind_resource_key = 4
    test_model.add1dRenewableResource(
        "wind",
        "../data/input/test/wind_speed_peak-25ms_1yr_dt-1hr.csv",
        wind_resource_key
    )
    
    struct_wind = PGMcpp.structWind()
    struct_wind.resource_key = wind_resource_key
    
    test_model.addWind(struct_nondisp, struct_wind)
    
    sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
    
    
    #   3.6. add Diesel asset
    print("\t\tadd Diesel asset ...", end="", flush=True)
    
    struct_disp = PGMcpp.structDispatchable()
    struct_disp.cap_kW = 1.2 * max(test_model.load_vec_kW)
    
    struct_combustion = PGMcpp.structCombustion()
    
    struct_diesel = PGMcpp.structDiesel()
    
    test_model.addDiesel(struct_disp, struct_combustion, struct_diesel)
    
    sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
    
    
    #   3.7. add LiIon asset
    print("\t\tadd LiIon asset ...", end="", flush=True)

    struct_storage = PGMcpp.structStorage()
    
    struct_battery_storage = PGMcpp.structBatteryStorage()
    
    struct_liion = PGMcpp.structLiIon()
    
    test_model.addLiIon(struct_storage, struct_battery_storage, struct_liion)
    
    sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
    
    
    #   3.8. run and write
    print("\t\trun() and writeResults() ...")
    
    test_model.run()
    test_model.writeResults("test_results_from_setup")

    sys.stdout.write("\t\t\x1b[1;32mPASS\x1b[0m" + end)


except:
    sys.stdout.write("\x1B[31mFAIL\x1b[0m" + end)
    raise


finally:
    print()
