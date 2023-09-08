/*
 *  Testing for Dispatchable <-- Combustion <-- Diesel class
 */

std::cout << std::endl;
printGold("Testing Dispatchable <-- Combustion <-- Diesel ... ");
std::cout << std::endl;

try {
    //  construction
    structDispatchable struct_disp;
    struct_disp.test_flag = true;
    
    structCombustion struct_combustion;
    
    structDiesel struct_diesel;
    
    Diesel test_diesel(struct_disp, struct_combustion, struct_diesel);
    
    
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
    
    double dt_hrs = 1;
    
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
            dt_hrs,
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
    
} catch (...) {
    printRed("\n\t\t\t\tDispatchable <-- Combustion <-- Diesel Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tDispatchable <-- Combustion <-- Diesel Tests:  PASS");
std::cout << std::endl;
