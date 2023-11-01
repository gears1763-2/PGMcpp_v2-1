"""
    PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1

    Anthony Truelove MASc, P.Eng.
    email:  gears1763@tutanota.com
    github: gears1763-2

    See license terms
 
    Testing script for Python 3 bindings.
"""


import PGMcpp


try:
    #   1. structModel
    print("Testing structModel ... ", end="", flush=True)

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

    print("PASS")


    #   2. DispatchMode
    print("Testing DispatchMode ... ", end="", flush=True)

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

    print("PASS")


    #   3. Model
    print("Testing Model ... ", end="", flush=True)
    
    struct_model.path_2_load_data = "data/input/test/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv"
    test_model = PGMcpp.Model(struct_model)
    
    assert(test_model.struct_model.path_2_load_data == struct_model.path_2_load_data)
    assert(test_model.n_timesteps == 8760)
    assert(abs(test_model.project_life_yrs - 1) < 1e-3)
    assert(test_model.total_load_served_kWh == 0)
    assert(test_model.total_fuel_consumed_L == 0)
    
    print("PASS")


except:
    print("FAIL\n")
    raise
