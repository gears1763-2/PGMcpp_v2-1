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
    
    Diesel test_diesel(
        struct_disp,
        struct_combustion,
        struct_diesel,
        16
    );
    
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
    
    Diesel test_diesel_fuel_lookup(
        struct_disp,
        struct_combustion,
        struct_diesel,
        16
    );
    
    test_diesel_fuel_lookup.ptr_2_dt_vec_hr = &(dt_vec_hr);
    test_diesel_fuel_lookup.ptr_2_time_vec_hr = &(time_vec_hr);
    
    
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
        0.3062 * pow(struct_disp.cap_kW, -0.0370),
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
    
    for (int i = 0; i < exp_load_ratio_vec.size(); i++) {
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
    
    for (int i = 0; i < requested_production_vec_kW.size(); i++) {
        double requested_production_kW = requested_production_vec_kW[i];
        
        testFloatEquals(
            test_diesel.requestProductionkW(requested_production_kW),
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
    
    for (int i = 0; i < requested_production_vec_kW.size(); i++) {
        double requested_production_kW = requested_production_vec_kW[i];
        
        double production_kW = test_diesel.requestProductionkW(
            requested_production_kW
        );
        
        test_diesel.commitProductionkW(
            production_kW,
            i
        );
        
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
    
    for (int i = 0; i < interp_test_load_ratio_vec.size(); i++) {
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
    
} catch (...) {
    printRed("\n\t\t\t\tDispatchable <-- Combustion <-- Diesel Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tDispatchable <-- Combustion <-- Diesel Tests:  PASS");
std::cout << std::endl;
