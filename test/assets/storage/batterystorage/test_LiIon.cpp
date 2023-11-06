/*
 *  Testing for Storage <-- BatteryStorage <-- LiIon class
 */

std::cout << std::endl;
printGold("Testing Storage <-- BatteryStorage <-- LiIon ... ");
std::cout << std::endl;

try {
    //  construction
    structStorage struct_storage;
    struct_storage.test_flag = true;
    
    structBatteryStorage struct_battery_storage;
    
    structLiIon struct_liion;
    
    LiIon test_liion(struct_storage, struct_battery_storage, struct_liion, 32);
    
    
    // test input bounds checking
    bool error_flag = true;
    
    try {
        // bad cap_kW
        structStorage bad_struct_storage;
        bad_struct_storage.test_flag = true;
        
        bad_struct_storage.cap_kW = 0;
        LiIon bad_test_liion(
            bad_struct_storage,
            struct_battery_storage,
            struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad cap_kWh
        structStorage bad_struct_storage;
        bad_struct_storage.test_flag = true;
        
        bad_struct_storage.cap_kWh = 0;
        LiIon bad_test_liion(
            bad_struct_storage,
            struct_battery_storage,
            struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad init_SOC
        structBatteryStorage bad_struct_battery_storage;
        
        if (rand() % 2 == 0) {
            bad_struct_battery_storage.init_SOC = -2;
        }
        
        else {
            bad_struct_battery_storage.init_SOC = 2;
        }
        
        LiIon bad_test_liion(
            struct_storage,
            bad_struct_battery_storage,
            struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad min_SOC
        structBatteryStorage bad_struct_battery_storage;
        
        if (rand() % 2 == 0) {
            bad_struct_battery_storage.min_SOC = -2;
        }
        
        else {
            bad_struct_battery_storage.min_SOC = 2;
        }
        
        LiIon bad_test_liion(
            struct_storage,
            bad_struct_battery_storage,
            struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad max_SOC
        structBatteryStorage bad_struct_battery_storage;
        
        if (rand() % 2 == 0) {
            bad_struct_battery_storage.max_SOC = -2;
        }
        
        else {
            bad_struct_battery_storage.max_SOC = 2;
        }
        
        LiIon bad_test_liion(
            struct_storage,
            bad_struct_battery_storage,
            struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad hysteresis_SOC
        structBatteryStorage bad_struct_battery_storage;
        
        if (rand() % 2 == 0) {
            bad_struct_battery_storage.hysteresis_SOC = -2;
        }
        
        else {
            bad_struct_battery_storage.hysteresis_SOC = 2;
        }
        
        LiIon bad_test_liion(
            struct_storage,
            bad_struct_battery_storage,
            struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad reserve_SOC
        structBatteryStorage bad_struct_battery_storage;
        
        if (rand() % 2 == 0) {
            bad_struct_battery_storage.reserve_SOC = -2;
        }
        
        else {
            bad_struct_battery_storage.reserve_SOC = 2;
        }
        
        LiIon bad_test_liion(
            struct_storage,
            bad_struct_battery_storage,
            struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad charge_efficiency
        structBatteryStorage bad_struct_battery_storage;
        
        if (rand() % 2 == 0) {
            bad_struct_battery_storage.charge_efficiency = 0;
        }
        
        else {
            bad_struct_battery_storage.charge_efficiency = 2;
        }
        
        LiIon bad_test_liion(
            struct_storage,
            bad_struct_battery_storage,
            struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad discharge_efficiency
        structBatteryStorage bad_struct_battery_storage;
        
        if (rand() % 2 == 0) {
            bad_struct_battery_storage.discharge_efficiency = 0;
        }
        
        else {
            bad_struct_battery_storage.discharge_efficiency = 2;
        }
        
        LiIon bad_test_liion(
            struct_storage,
            bad_struct_battery_storage,
            struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad replace_SOH
        structLiIon bad_struct_liion;
        
        if (rand() % 2 == 0) {
            bad_struct_liion.replace_SOH = -2;
        }
        
        else {
            bad_struct_liion.replace_SOH = 2;
        }
        
        LiIon bad_test_liion(
            struct_storage,
            struct_battery_storage,
            bad_struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad degr_alpha
        structLiIon bad_struct_liion;
        
        bad_struct_liion.degr_alpha = -1;
        LiIon bad_test_liion(
            struct_storage,
            struct_battery_storage,
            bad_struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad degr_beta
        structLiIon bad_struct_liion;
        
        bad_struct_liion.degr_beta = -1;
        LiIon bad_test_liion(
            struct_storage,
            struct_battery_storage,
            bad_struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad degr_B_hat_cal_0
        structLiIon bad_struct_liion;
        
        bad_struct_liion.degr_B_hat_cal_0 = -1;
        LiIon bad_test_liion(
            struct_storage,
            struct_battery_storage,
            bad_struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad degr_r_cal
        structLiIon bad_struct_liion;
        
        bad_struct_liion.degr_r_cal = -1;
        LiIon bad_test_liion(
            struct_storage,
            struct_battery_storage,
            bad_struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad degr_Ea_cal_0
        structLiIon bad_struct_liion;
        
        bad_struct_liion.degr_Ea_cal_0 = -1;
        LiIon bad_test_liion(
            struct_storage,
            struct_battery_storage,
            bad_struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad degr_a_cal
        structLiIon bad_struct_liion;
        
        bad_struct_liion.degr_a_cal = -1;
        LiIon bad_test_liion(
            struct_storage,
            struct_battery_storage,
            bad_struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad degr_s_cal
        structLiIon bad_struct_liion;
        
        bad_struct_liion.degr_s_cal = -1;
        LiIon bad_test_liion(
            struct_storage,
            struct_battery_storage,
            bad_struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad gas_constant_JmolK
        structLiIon bad_struct_liion;
        
        bad_struct_liion.gas_constant_JmolK = -1;
        LiIon bad_test_liion(
            struct_storage,
            struct_battery_storage,
            bad_struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad temperature_K
        structLiIon bad_struct_liion;
        
        bad_struct_liion.temperature_K = -1;
        LiIon bad_test_liion(
            struct_storage,
            struct_battery_storage,
            bad_struct_liion,
            32
        );
        
        error_flag = false;
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    //  test energy reserve
    std::cout << "\tTesting energy reserve logic ..." <<
        std::endl;
    
    std::vector<double> time_vec_hr = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    };
    
    std::vector<double> dt_vec_hr (32, 1);
    
    std::vector<double> expected_charge_vec_kWh = {
        433.4,
        411.1556,
        403.7259704,
        401.2444741136,
        400.4156543539424,
        400.1388285542167,
        400.0463687371084,
        400.0154871581942,
        400.0051727108369,
        400.0017276854195,
        400.0005770469301,
        400.0001927336747,
        400.0000643730473,
        400.0000215005978,
        400.0000071811997,
        400.0000023985207,
        326.0000023985207,
        252.0000023985207,
        217.3680008011059,
        205.8009122675694,
        201.9375046973682,
        200.647126568921,
        200.2161402740196,
        200.0721908515225,
        200.0241117444085,
        200.0080533226325,
        200.0026898097592,
        200.0008983964596,
        200.0003000644175,
        200.0001002215155,
        200.0000334739862,
        200.0000111803114
    };
    
    test_liion.ptr_2_time_vec_hr = &time_vec_hr;
    test_liion.ptr_2_dt_vec_hr = &dt_vec_hr;
    
    for (size_t i = 0; i < 16; i++) {
        double avail_kW = test_liion.getAvailablekW(dt_vec_hr[i]);

        test_liion.commitDischargekW(0.666 * avail_kW, i);
        
        double charge_kWh = test_liion.charge_kWh;
        double expected_charge_kWh = expected_charge_vec_kWh[i];
        
        testFloatEquals(
            charge_kWh,
            expected_charge_kWh,
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    test_liion.toggleReserve(true);
    
    for (size_t i = 16; i < time_vec_hr.size(); i++) {
        double avail_kW = test_liion.getAvailablekW(dt_vec_hr[i]);

        test_liion.commitDischargekW(0.666 * avail_kW, i);
        
        double charge_kWh = test_liion.charge_kWh;
        double expected_charge_kWh = expected_charge_vec_kWh[i];
        
        testFloatEquals(
            charge_kWh,
            expected_charge_kWh,
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
} catch (...) {
    printRed("\n\t\t\t\tStorage <-- BatteryStorage <-- LiIon Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tStorage <-- BatteryStorage <-- LiIon Tests:  PASS");
std::cout << std::endl;
