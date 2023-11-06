/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Testing suite for the Dispatchable <-- Combustion <-- Diesel class.
 */

std::cout << std::endl;
printGold("Testing Dispatchable <-- Combustion <-- Diesel ... ");
std::cout << std::endl;

try {
    std::vector<double> time_vec_hr = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    };
    
    std::vector<double> dt_vec_hr = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    
    
    //  construction
    structDispatchable struct_disp;
    struct_disp.test_flag = true;
    
    structCombustion struct_combustion;
    
    structDiesel struct_diesel;
    
    Diesel test_diesel(struct_disp, struct_combustion, struct_diesel, 16);
    
    test_diesel.ptr_2_dt_vec_hr = &(dt_vec_hr);
    test_diesel.ptr_2_time_vec_hr = &(time_vec_hr);
    
    struct_combustion.fuel_mode = LOOKUP;
    
    Diesel test_diesel_no_fuel_data_path(
        struct_disp,
        struct_combustion,
        struct_diesel,
        16
    );
    
    test_diesel_no_fuel_data_path.ptr_2_dt_vec_hr = &(dt_vec_hr);
    test_diesel_no_fuel_data_path.ptr_2_time_vec_hr = &(time_vec_hr);
    
    struct_combustion.path_2_fuel_consumption_data =
        "data/input/test/diesel_fuel_curve.csv";
    
    Diesel test_diesel_fuel_lookup(struct_disp, struct_combustion, struct_diesel, 16);
    
    test_diesel_fuel_lookup.ptr_2_dt_vec_hr = &(dt_vec_hr);
    test_diesel_fuel_lookup.ptr_2_time_vec_hr = &(time_vec_hr);
    
    
    // test input bounds checking
    bool error_flag = true;
    
    try {
        // bad cap_kW
        structDispatchable bad_struct_disp;
        bad_struct_disp.test_flag = true;
        
        bad_struct_disp.cap_kW = 0;
        Diesel bad_test_diesel(bad_struct_disp, struct_combustion, struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad replace_running_hrs
        structDispatchable bad_struct_disp;
        bad_struct_disp.test_flag = true;
        
        bad_struct_disp.replace_running_hrs = 0;
        Diesel bad_test_diesel(bad_struct_disp, struct_combustion, struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad cycle_charging_load_ratio
        structCombustion bad_struct_combustion;
        
        if (rand() % 2 == 0) {
            bad_struct_combustion.cycle_charging_load_ratio = -2;
        }
        
        else {
            bad_struct_combustion.cycle_charging_load_ratio = 2;
        }
        
        Diesel bad_test_diesel(struct_disp, bad_struct_combustion, struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad fuel_cost_L
        structCombustion bad_struct_combustion;
        
        bad_struct_combustion.fuel_cost_L = -1;
        Diesel bad_test_diesel(struct_disp, bad_struct_combustion, struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad diesel_CO2_kgL
        structCombustion bad_struct_combustion;
        
        bad_struct_combustion.diesel_CO2_kgL = -1;
        Diesel bad_test_diesel(struct_disp, bad_struct_combustion, struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad diesel_CO_kgL
        structCombustion bad_struct_combustion;
        
        bad_struct_combustion.diesel_CO_kgL = -1;
        Diesel bad_test_diesel(struct_disp, bad_struct_combustion, struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad diesel_NOx_kgL
        structCombustion bad_struct_combustion;
        
        bad_struct_combustion.diesel_NOx_kgL = -1;
        Diesel bad_test_diesel(struct_disp, bad_struct_combustion, struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad diesel_SOx_kgL
        structCombustion bad_struct_combustion;
        
        bad_struct_combustion.diesel_SOx_kgL = -1;
        Diesel bad_test_diesel(struct_disp, bad_struct_combustion, struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad diesel_CH4_kgL
        structCombustion bad_struct_combustion;
        
        bad_struct_combustion.diesel_CH4_kgL = -1;
        Diesel bad_test_diesel(struct_disp, bad_struct_combustion, struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad diesel_PM_kgL
        structCombustion bad_struct_combustion;
        
        bad_struct_combustion.diesel_PM_kgL = -1;
        Diesel bad_test_diesel(struct_disp, bad_struct_combustion, struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad minimum_load_ratio
        structDiesel bad_struct_diesel;
        
        if (rand() % 2 == 0) {
            bad_struct_diesel.minimum_load_ratio = -2;
        }
        
        else {
            bad_struct_diesel.minimum_load_ratio = -2;
        }
        
        Diesel bad_test_diesel(struct_disp, struct_combustion, bad_struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad minimum_runtime_hrs
        structDiesel bad_struct_diesel;
        
        bad_struct_diesel.minimum_runtime_hrs = -1;
        Diesel bad_test_diesel(struct_disp, struct_combustion, bad_struct_diesel, 16);
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    //  test post-construction attributes
    std::cout << "\tTesting post-construction attributes ..." <<
        std::endl;
    
    testFloatEquals(
        test_diesel.struct_combustion.fuel_mode,
        LINEAR,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_diesel.struct_combustion.linear_fuel_intercept_LkWh,
        0.0940 * pow(struct_disp.cap_kW, -0.2735),
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_diesel.struct_combustion.linear_fuel_slope_LkWh,
        0.4234 * pow(struct_disp.cap_kW, -0.1012),
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_diesel_no_fuel_data_path.struct_combustion.fuel_mode,
        LINEAR,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    std::vector<double> exp_load_ratio_vec = {
        0,
        0.3,
        0.35,
        0.4,
        0.45,
        0.5,
        0.55,
        0.6,
        0.65,
        0.7,
        0.75,
        0.8,
        0.85,
        0.9,
        0.95,
        1
    };
    
    std::vector<double> exp_fuel_consumption_vec_Lhr = {
        4.68079520372916,
        11.1278522361839,
        12.4787834830748,
        13.7808847600209,
        15.0417468303382,
        16.277263,
        17.4612831516442,
        18.6279054806525,
        19.7698039220515,
        20.8893499214868,
        21.955378,
        23.0690535155297,
        24.1323614374927,
        25.1797231192866,
        26.2122451458747,
        27.254952
    };
    
    for (size_t i = 0; i < exp_load_ratio_vec.size(); i++) {
        testFloatEquals(
            test_diesel_fuel_lookup.fuel_interp_load_ratio_vec[i],
            exp_load_ratio_vec[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_diesel_fuel_lookup.fuel_interp_consumption_vec_Lhr[i],
            exp_fuel_consumption_vec_Lhr[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    
    //  test requestProductionkW()
    std::cout << "\tTesting Diesel::requestProductionkW() ..." <<
        std::endl;
    
    std::vector<double> requested_production_vec_kW = {
        0, 5, 10, 15, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120
    };
    
    std::vector<double> expected_production_vec_kW = {
        0, 20, 20, 20, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100, 100, 100
    };
    
    for (size_t i = 0; i < requested_production_vec_kW.size(); i++) {
        double requested_production_kW = requested_production_vec_kW[i];
        
        testFloatEquals(
            test_diesel.requestProductionkW(requested_production_kW, i),
            expected_production_vec_kW[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    
    // test commitProductionkW()
    std::cout << "\tTesting Diesel::commitProductionkW() ..." <<
        std::endl;
    
    requested_production_vec_kW = {
        0, 0, 10, 15, 0, 5, 0, 0, 50, 60, 70, 80, 90, 0, 110, 120
    };
    
    std::vector<int> expected_running_state_vec = {
        0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1
    };
    
    for (size_t i = 0; i < requested_production_vec_kW.size(); i++) {
        double requested_production_kW = requested_production_vec_kW[i];
        
        double production_kW = test_diesel.requestProductionkW(
            requested_production_kW, i
        );
        
        test_diesel.commitProductionkW(production_kW, i);
        
        testFloatEquals(
            int(test_diesel.is_running_vec[i]),
            expected_running_state_vec[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    
    // test _fuelConsumptionLookupL()
    std::cout << "\tTesting Combustion::_fuelConsumptionLookupL() ..." <<
        std::endl;
    
    testFloatEquals(
        test_diesel_fuel_lookup._fuelConsumptionLookupL(0, 1),
        exp_fuel_consumption_vec_Lhr[0],
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_diesel_fuel_lookup._fuelConsumptionLookupL(
            struct_disp.cap_kW, 1),
        exp_fuel_consumption_vec_Lhr[
            exp_fuel_consumption_vec_Lhr.size() - 1
        ],
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    std::vector<double> interp_test_load_ratio_vec = {
        0.037231293258847,
        0.327013791607756,
        0.351112150438547,
        0.415084903383285,
        0.491557533539241,
        0.53546385107856,
        0.596705199987357,
        0.627895862039612,
        0.682745287709229,
        0.720881747105973,
        0.766499597404958,
        0.844186912853003,
        0.891682680088856,
        0.912237993871541,
        0.960397937892863
    };
    
    std::vector<double> interp_test_fuel_consumption_vec_L = {
        5.48090277383528,
        11.8577277397822,
        12.5077461331986,
        14.1612844102286,
        16.0686469235181,
        17.1170612866385,
        18.5510297353652,
        19.2649903083428,
        20.503001039156,
        21.3345604963544,
        22.32288195292,
        24.0087394052034,
        25.0054982758835,
        25.4324430839589,
        26.4290851680678
    };
    
    for (size_t i = 0; i < interp_test_load_ratio_vec.size(); i++) {
        testFloatEquals(
            test_diesel_fuel_lookup._fuelConsumptionLookupL(
                interp_test_load_ratio_vec[i] * struct_disp.cap_kW,
                1
            ),
            interp_test_fuel_consumption_vec_L[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    
    // test ramp rate constraint
    std::cout << "\tTesting ramp rate constraint ..." <<
        std::endl;
    
    Diesel test_diesel_ramping(struct_disp, struct_combustion, struct_diesel, 31);
    
    time_vec_hr = {
        0,
        0.000555555555555556,
        0.00111111111111111,
        0.00166666666666667,
        0.00222222222222222,
        0.00277777777777778,
        0.00333333333333333,
        0.00388888888888889,
        0.00444444444444444,
        0.005,
        0.00555555555555556,
        0.00611111111111111,
        0.00666666666666667,
        0.00722222222222222,
        0.00777777777777778,
        0.00833333333333333,
        0.00888888888888889,
        0.00944444444444444,
        0.01,
        0.0105555555555556,
        0.0111111111111111,
        0.0116666666666667,
        0.0122222222222222,
        0.0127777777777778,
        0.0133333333333333,
        0.0138888888888889,
        0.0144444444444444,
        0.015,
        0.0155555555555556,
        0.0161111111111111,
        0.0166666666666667,
    };
    
    dt_vec_hr = {
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555555,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555555,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555555,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555556,
        0.000555555555555554,
        0.000555555555555556
    };
    
    expected_production_vec_kW = {
        0,
        6.666667,
        13.333333,
        20,
        26.666667,
        33.333333,
        40,
        46.666667,
        53.333333,
        60,
        66.666667,
        73.333333,
        80,
        86.666667,
        93.333333,
        100,
        93.333333,
        86.666667,
        80,
        73.333333,
        66.666667,
        60,
        53.333333,
        46.666667,
        40,
        33.333333,
        26.666667,
        20,
        13.333333,
        6.666667,
        0
    };
    
    test_diesel_ramping.ptr_2_dt_vec_hr = &(dt_vec_hr);
    test_diesel_ramping.ptr_2_time_vec_hr = &(time_vec_hr);
    
    for (size_t i = 0; i < time_vec_hr.size(); i++) {
        double production_kW = 0;
        if (i == 0 or i > 15) {
            production_kW = test_diesel_ramping.requestProductionkW(0, i);
        }
        
        else {
            production_kW = test_diesel_ramping.requestProductionkW(
                test_diesel_ramping.struct_disp.cap_kW, i
            );
        }
        
        test_diesel_ramping.commitProductionkW(production_kW, i);
        
        double expected_production_kW = expected_production_vec_kW[i];
        
        testFloatEquals(
            production_kW,
            expected_production_kW,
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
} catch (...) {
    printRed("\n\t\t\t\tDispatchable <-- Combustion <-- Diesel Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tDispatchable <-- Combustion <-- Diesel Tests:  PASS");
std::cout << std::endl;
