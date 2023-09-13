/*
 *  Testing for Model class
 */

std::cout << std::endl;
printGold("Testing Model ... ");
std::cout << std::endl;

try {
    //  construction
    structModel struct_model;
    
    struct_model.print_flag = true;
    struct_model.test_flag = true;
    
    struct_model.path_2_load_data =
        "data/input/test/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";
    
    Model test_model(struct_model);
    
    
    //  test _readInLoadData()
    std::cout << "\tTesting Model::_readInLoadData() ..." <<
        std::endl;
    
    testFloatEquals(
        test_model.n_timesteps,
        8760,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    std::vector<double> exp_load_vec_kW = {
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
    };
    
    for (int i = 0; i < exp_load_vec_kW.size(); i++) {
        testFloatEquals(
            test_model.time_vec_hr[i],
            i,
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_model.load_vec_kW[i],
            exp_load_vec_kW[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    testFloatEquals(
        test_model.project_life_yrs,
        8759.0 / 8760,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    
    // test _populateDeltaVecHr()
    std::cout << "\tTesting Model::_populateDeltaVecHr() ..." <<
        std::endl;
    
    for (int i = 0; i < 16; i++) {
        testFloatEquals(
            test_model.dt_vec_hr[i],
            1,
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    testFloatEquals(
        test_model.dt_vec_hr[test_model.n_timesteps - 1],
        1,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    
    // test post-construction economic parameters
    testFloatEquals(
        test_model.real_discount_rate_annual,
        (0.04 - 0.02) / (1 + 0.02),
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    
    // test add1dRenewableResource()
    std::cout << "\tTesting Model::add1dRenewableResource() ..." <<
        std::endl;
        
    int hydro_key = 0;

    try {
        test_model.add1dRenewableResource("balls", "", hydro_key);
        
        std::string error_str = "Model::add1dRenewableResource() fails ";
        error_str += "to throw expected runtime error at line ";
        error_str += std::to_string(__LINE__);
        throw std::runtime_error(error_str);
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    try {
        test_model.add1dRenewableResource(
            "Hydro",
            "data/input/test/hydro_flow_peak-3810m3hr_1yr_dt-1hr_bad_length.csv",
            hydro_key
        );
        
        std::string error_str = "Model::add1dRenewableResource() fails ";
        error_str += "to throw expected runtime error at line ";
        error_str += std::to_string(__LINE__);
        throw std::runtime_error(error_str);
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    try {
        test_model.add1dRenewableResource(
            "Hydro",
            "data/input/test/hydro_flow_peak-3810m3hr_1yr_dt-1hr_bad_times.csv",
            hydro_key
        );
        
        std::string error_str = "Model::add1dRenewableResource() fails ";
        error_str += "to throw expected runtime error at line ";
        error_str += std::to_string(__LINE__);
        throw std::runtime_error(error_str);
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    test_model.add1dRenewableResource(
        "Hydro",
        "data/input/test/hydro_flow_peak-3810m3hr_1yr_dt-1hr.csv",
        hydro_key
    );
    
    std::vector<double> exp_hydro_resource_vec_m3hr = {
        1779,
        1748,
        1720,
        1697,
        1678,
        1664,
        1653,
        1647,
        1645,
        1647,
        1653,
        1664,
        1678,
        1697,
        1720,
        1748
    };
    
    for (int i = 0; i < exp_hydro_resource_vec_m3hr.size(); i++) {
        testFloatEquals(
            test_model.resource_map_1D[hydro_key][i],
            exp_hydro_resource_vec_m3hr[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    int solar_key = 1;
     
    test_model.add1dRenewableResource(
        "Solar",
        "data/input/test/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv",
        solar_key
    );
    
    std::vector<double> exp_solar_resource_vec_kWm2 = {
        0,
        0,
        0,
        0,
        0,
        0,
        8.51702662684015E-05,
        0.000348341567045,
        0.00213793728593,
        0.004099863613322,
        0.000997135230553,
        0.009534527624657,
        0.022927996790616,
        0.0136071715294,
        0.002535134127751,
        0.005206897515821
    };
    
    for (int i = 0; i < exp_solar_resource_vec_kWm2.size(); i++) {
        testFloatEquals(
            test_model.resource_map_1D[solar_key][i],
            exp_solar_resource_vec_kWm2[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    int tidal_key = 2;
     
    test_model.add1dRenewableResource(
        "Tidal",
        "data/input/test/tidal_speed_peak-3ms_1yr_dt-1hr.csv",
        tidal_key
    );
    
    std::vector<double> exp_tidal_resource_vec_ms = {
        0.347439913040533,
        0.770545522195602,
        0.731352084836198,
        0.293389814389542,
        0.209959110813115,
        0.610609623896497,
        1.78067162013604,
        2.53522775118089,
        2.75966627832024,
        2.52101111143895,
        2.05389330201031,
        1.3461515862445,
        0.28909254878384,
        0.897754086048563,
        1.71406453837407,
        1.85047408742869
    };
    
    for (int i = 0; i < exp_tidal_resource_vec_ms.size(); i++) {
        testFloatEquals(
            test_model.resource_map_1D[tidal_key][i],
            exp_tidal_resource_vec_ms[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    int wind_key = 3;
     
    test_model.add1dRenewableResource(
        "Wind",
        "data/input/test/wind_speed_peak-25ms_1yr_dt-1hr.csv",
        wind_key
    );
    
    std::vector<double> exp_wind_resource_vec_ms = {
        6.88566688469997,
        5.02177105466549,
        3.74211715899568,
        5.67169579985362,
        4.90670669971858,
        4.29586955031368,
        7.41155377205065,
        10.2243290476943,
        13.1258696725555,
        13.7016198628274,
        16.2481482330233,
        16.5096744355418,
        13.4354482206162,
        14.0129230731609,
        14.5554549260515,
        13.4454539065912
    };
    
    for (int i = 0; i < exp_wind_resource_vec_ms.size(); i++) {
        testFloatEquals(
            test_model.resource_map_1D[wind_key][i],
            exp_wind_resource_vec_ms[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    
    // test add2dRenewableResource()
    std::cout << "\tTesting Model::add2dRenewableResource() ..." <<
        std::endl;
        
    int wave_key = 4;

    try {
        test_model.add2dRenewableResource("balls", "", wave_key);
        
        std::string error_str = "Model::add2dRenewableResource() fails ";
        error_str += "to throw expected runtime error at line ";
        error_str += std::to_string(__LINE__);
        throw std::runtime_error(error_str);
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    try {
        test_model.add2dRenewableResource(
            "Wave",
            "data/input/test/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr_bad_length.csv",
            wave_key
        );
        
        std::string error_str = "Model::add2dRenewableResource() fails ";
        error_str += "to throw expected runtime error at line ";
        error_str += std::to_string(__LINE__);
        throw std::runtime_error(error_str);
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    try {
        test_model.add2dRenewableResource(
            "Wave",
            "data/input/test/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr_bad_times.csv",
            wave_key
        );
        
        std::string error_str = "Model::add2dRenewableResource() fails ";
        error_str += "to throw expected runtime error at line ";
        error_str += std::to_string(__LINE__);
        throw std::runtime_error(error_str);
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    test_model.add2dRenewableResource(
        "Wave",
        "data/input/test/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv",
        wave_key
    );
    
    std::vector<double> exp_significant_wave_height_vec_m = {
        4.26175222125028,
        4.25020976167872,
        4.25656524330349,
        4.27193854786718,
        4.28744955711233,
        4.29421815278154,
        4.2839937266082,
        4.25716982457976,
        4.22419391611483,
        4.19588925217606,
        4.17338788587412,
        4.14672746914214,
        4.10560041173665,
        4.05074966447193,
        3.9953696962433,
        3.95316976150866
    };
    
    std::vector<double> exp_energy_period_vec_s = {
        10.4456008226821,
        10.4614151137651,
        10.4462827795433,
        10.4127692097884,
        10.3734397942723,
        10.3408599227669,
        10.32637292093,
        10.3245412676322,
        10.310409818185,
        10.2589529840966,
        10.1728100603103,
        10.0862908658929,
        10.03480243813,
        10.023673635806,
        10.0243418565116,
        10.0063487117653
    };
    
    for (int i = 0; i < exp_energy_period_vec_s.size(); i++) {
        testFloatEquals(
            test_model.resource_map_2D[wave_key][i][0],
            exp_significant_wave_height_vec_m[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_model.resource_map_2D[wave_key][i][1],
            exp_energy_period_vec_s[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    
    // test addSolar()
    std::cout << "\tTesting Model::addSolar() ..." <<
        std::endl;
        
    structNondispatchable struct_nondisp;
    struct_nondisp.print_flag = true;
    struct_nondisp.test_flag = true;
    
    structSolar struct_solar;
    struct_solar.resource_key = solar_key;
    
    test_model.addSolar(struct_nondisp, struct_solar);
    
    testFloatEquals(
        test_model.nondisp_ptr_vec.size(),
        1,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->n_timesteps,
        test_model.n_timesteps,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->nondisp_type,
        SOLAR,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->project_life_yrs,
        test_model.project_life_yrs,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->struct_nondisp.nominal_inflation_rate_annual,
        test_model.struct_model.nominal_inflation_rate_annual,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->struct_nondisp.nominal_discount_rate_annual,
        test_model.struct_model.nominal_discount_rate_annual,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->real_discount_rate_annual,
        test_model.real_discount_rate_annual,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testTruth(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->ptr_2_dt_vec_hr ==
        &(test_model.dt_vec_hr),
        __FILE__,
        __LINE__
    );
    
    testTruth(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->ptr_2_time_vec_hr ==
        &(test_model.time_vec_hr),
        __FILE__,
        __LINE__
    );
    
    
    // test addTidal()
    std::cout << "\tTesting Model::addTidal() ..." <<
        std::endl;
    
    structTidal struct_tidal;
    struct_tidal.resource_key = tidal_key;
    
    test_model.addTidal(struct_nondisp, struct_tidal);
    
    testFloatEquals(
        test_model.nondisp_ptr_vec.size(),
        2,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->n_timesteps,
        test_model.n_timesteps,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->nondisp_type,
        TIDAL,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    
    // test addWave()
    std::cout << "\tTesting Model::addWave() ..." <<
        std::endl;
    
    structWave struct_wave;
    struct_wave.resource_key = wave_key;
    
    test_model.addWave(struct_nondisp, struct_wave);
    
    testFloatEquals(
        test_model.nondisp_ptr_vec.size(),
        3,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->n_timesteps,
        test_model.n_timesteps,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->nondisp_type,
        WAVE,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    
    // test addWind()
    std::cout << "\tTesting Model::addWind() ..." <<
        std::endl;
    
    structWind struct_wind;
    struct_wind.resource_key = wind_key;
    
    test_model.addWind(struct_nondisp, struct_wind);
    
    testFloatEquals(
        test_model.nondisp_ptr_vec.size(),
        4,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->n_timesteps,
        test_model.n_timesteps,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.nondisp_ptr_vec[
            test_model.nondisp_ptr_vec.size() - 1
        ]->nondisp_type,
        WIND,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    
    // test addDiesel()
    std::cout << "\tTesting Model::addDiesel() ..." <<
        std::endl;
    
    structDispatchable struct_disp;
    struct_disp.print_flag = true;
    struct_disp.test_flag = true;
    struct_disp.cap_kW = 600;
    
    structCombustion struct_combustion;
    
    structDiesel struct_diesel;
    
    test_model.addDiesel(struct_disp, struct_combustion, struct_diesel);
    
    testFloatEquals(
        test_model.combustion_ptr_vec.size(),
        1,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.combustion_ptr_vec[
            test_model.combustion_ptr_vec.size() - 1
        ]->n_timesteps,
        test_model.n_timesteps,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.combustion_ptr_vec[
            test_model.combustion_ptr_vec.size() - 1
        ]->disp_type,
        DIESEL,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.combustion_ptr_vec[
            test_model.combustion_ptr_vec.size() - 1
        ]->project_life_yrs,
        test_model.project_life_yrs,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.combustion_ptr_vec[
            test_model.combustion_ptr_vec.size() - 1
        ]->struct_disp.nominal_inflation_rate_annual,
        test_model.struct_model.nominal_inflation_rate_annual,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.combustion_ptr_vec[
            test_model.combustion_ptr_vec.size() - 1
        ]->struct_disp.nominal_discount_rate_annual,
        test_model.struct_model.nominal_discount_rate_annual,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.combustion_ptr_vec[
            test_model.combustion_ptr_vec.size() - 1
        ]->real_discount_rate_annual,
        test_model.real_discount_rate_annual,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testTruth(
        test_model.combustion_ptr_vec[
            test_model.combustion_ptr_vec.size() - 1
        ]->ptr_2_dt_vec_hr ==
        &(test_model.dt_vec_hr),
        __FILE__,
        __LINE__
    );
    
    testTruth(
        test_model.combustion_ptr_vec[
            test_model.combustion_ptr_vec.size() - 1
        ]->ptr_2_time_vec_hr ==
        &(test_model.time_vec_hr),
        __FILE__,
        __LINE__
    );
    
    
    // test addHydro()
    std::cout << "\tTesting Model::addHydro() ..." <<
        std::endl;
    
    structHydro struct_hydro;
    
    test_model.addHydro(struct_disp, struct_hydro);
    
    testFloatEquals(
        test_model.noncombustion_ptr_vec.size(),
        1,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.noncombustion_ptr_vec[
            test_model.noncombustion_ptr_vec.size() - 1
        ]->n_timesteps,
        test_model.n_timesteps,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.noncombustion_ptr_vec[
            test_model.noncombustion_ptr_vec.size() - 1
        ]->disp_type,
        HYDRO,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.noncombustion_ptr_vec[
            test_model.noncombustion_ptr_vec.size() - 1
        ]->project_life_yrs,
        test_model.project_life_yrs,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.noncombustion_ptr_vec[
            test_model.noncombustion_ptr_vec.size() - 1
        ]->struct_disp.nominal_inflation_rate_annual,
        test_model.struct_model.nominal_inflation_rate_annual,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.noncombustion_ptr_vec[
            test_model.noncombustion_ptr_vec.size() - 1
        ]->struct_disp.nominal_discount_rate_annual,
        test_model.struct_model.nominal_discount_rate_annual,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.noncombustion_ptr_vec[
            test_model.noncombustion_ptr_vec.size() - 1
        ]->real_discount_rate_annual,
        test_model.real_discount_rate_annual,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testTruth(
        test_model.noncombustion_ptr_vec[
            test_model.noncombustion_ptr_vec.size() - 1
        ]->ptr_2_dt_vec_hr ==
        &(test_model.dt_vec_hr),
        __FILE__,
        __LINE__
    );
    
    testTruth(
        test_model.noncombustion_ptr_vec[
            test_model.noncombustion_ptr_vec.size() - 1
        ]->ptr_2_time_vec_hr ==
        &(test_model.time_vec_hr),
        __FILE__,
        __LINE__
    );
    
    
    // test addLiIon()
    std::cout << "\tTesting Model::addLiIon() ..." <<
        std::endl;
    
    structStorage struct_storage;
    struct_storage.print_flag = true;
    struct_storage.test_flag = true;
    
    
    structBatteryStorage struct_battery_storage;
    
    structLiIon struct_liion;
    
    test_model.addLiIon(
        struct_storage,
        struct_battery_storage,
        struct_liion
    );
    
    testFloatEquals(
        test_model.storage_ptr_vec.size(),
        1,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    LiIon* liion_ptr = (LiIon*)(test_model.storage_ptr_vec[
        test_model.storage_ptr_vec.size() - 1
    ]);
    
    testFloatEquals(
        liion_ptr->n_timesteps,
        test_model.n_timesteps,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        liion_ptr->storage_type,
        LIION,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    
    // test run()
    std::cout << "\tTesting Model::run() ..." <<
        std::endl;
    
    test_model.run();
    
    
    // test post-run attributes
    std::cout << "\tTesting post Model::run() attributes ..." <<
        std::endl;
    
    for (int i = 0; i < test_model.n_timesteps; i++) {
        double load_remaining_kW = test_model.load_vec_kW[i];
        
        for (size_t j = 0; j < test_model.combustion_ptr_vec.size(); j++) {
            Combustion* combustion_ptr = test_model.combustion_ptr_vec[j];
            
            double difference_kW = combustion_ptr->production_vec_kW[i];
            difference_kW -= combustion_ptr->dispatch_vec_kW[i];
            difference_kW -= combustion_ptr->curtailment_vec_kW[i];
            difference_kW -= combustion_ptr->storage_vec_kW[i];
            
            load_remaining_kW -= combustion_ptr->dispatch_vec_kW[i];
            
            testFloatEquals(
                difference_kW,
                0,
                FLOAT_TOLERANCE,
                __FILE__,
                __LINE__
            );
        }
        
        for (size_t j = 0; j < test_model.noncombustion_ptr_vec.size(); j++) {
            Dispatchable* noncombustion_ptr = test_model.noncombustion_ptr_vec[j];
            
            double difference_kW = noncombustion_ptr->production_vec_kW[i];
            difference_kW -= noncombustion_ptr->dispatch_vec_kW[i];
            difference_kW -= noncombustion_ptr->curtailment_vec_kW[i];
            difference_kW -= noncombustion_ptr->storage_vec_kW[i];
            
            load_remaining_kW -= noncombustion_ptr->dispatch_vec_kW[i];
            
            testFloatEquals(
                difference_kW,
                0,
                FLOAT_TOLERANCE,
                __FILE__,
                __LINE__
            );
        }
        
        for (size_t j = 0; j < test_model.nondisp_ptr_vec.size(); j++) {
            Nondispatchable* nondisp_ptr = test_model.nondisp_ptr_vec[j];
            
            double difference_kW = nondisp_ptr->production_vec_kW[i];
            difference_kW -= nondisp_ptr->dispatch_vec_kW[i];
            difference_kW -= nondisp_ptr->curtailment_vec_kW[i];
            difference_kW -= nondisp_ptr->storage_vec_kW[i];
            
            load_remaining_kW -= nondisp_ptr->dispatch_vec_kW[i];
            
            testFloatEquals(
                difference_kW,
                0,
                FLOAT_TOLERANCE,
                __FILE__,
                __LINE__
            );
        }
        
        if (test_model.net_load_vec_kW[i] > 0) {
            for (size_t j = 0; j < test_model.storage_ptr_vec.size(); j++) {
                Storage* storage_ptr = test_model.storage_ptr_vec[j];
                
                testGreaterThanOrEqualTo(
                    storage_ptr->discharging_vec_kW[i],
                    0,
                    __FILE__,
                    __LINE__
                );
                
                testFloatEquals(
                    storage_ptr->charging_vec_kW[i],
                    0,
                    FLOAT_TOLERANCE,
                    __FILE__,
                    __LINE__
                );
                
                load_remaining_kW -= storage_ptr->discharging_vec_kW[i];
            }
            
            for (size_t j = 0; j < test_model.combustion_ptr_vec.size(); j++) {
                Combustion* combustion_ptr = test_model.combustion_ptr_vec[j];
                
                testFloatEquals(
                    combustion_ptr->storage_vec_kW[i],
                    0,
                    FLOAT_TOLERANCE,
                    __FILE__,
                    __LINE__
                );
            }
            
            for (size_t j = 0; j < test_model.noncombustion_ptr_vec.size(); j++) {
                Dispatchable* noncombustion_ptr = test_model.noncombustion_ptr_vec[j];
                
                testFloatEquals(
                    noncombustion_ptr->storage_vec_kW[i],
                    0,
                    FLOAT_TOLERANCE,
                    __FILE__,
                    __LINE__
                );
            }
            
            for (size_t j = 0; j < test_model.nondisp_ptr_vec.size(); j++) {
                Nondispatchable* nondisp_ptr = test_model.nondisp_ptr_vec[j];
                
                testFloatEquals(
                    nondisp_ptr->storage_vec_kW[i],
                    0,
                    FLOAT_TOLERANCE,
                    __FILE__,
                    __LINE__
                );
            }
        }
        
        else {
            double charged_kW = 0;
            double charging_kW = 0;
            
            for (size_t j = 0; j < test_model.storage_ptr_vec.size(); j++) {
                Storage* storage_ptr = test_model.storage_ptr_vec[j];
                
                charged_kW += storage_ptr->charging_vec_kW[i];
                
                testGreaterThanOrEqualTo(
                    storage_ptr->charging_vec_kW[i],
                    0,
                    __FILE__,
                    __LINE__
                );
                
                testFloatEquals(
                    storage_ptr->discharging_vec_kW[i],
                    0,
                    FLOAT_TOLERANCE,
                    __FILE__,
                    __LINE__
                );
            }
            
            for (size_t j = 0; j < test_model.combustion_ptr_vec.size(); j++) {
                Combustion* combustion_ptr = test_model.combustion_ptr_vec[j];
                
                charging_kW += combustion_ptr->storage_vec_kW[i];
            }
            
            for (size_t j = 0; j < test_model.noncombustion_ptr_vec.size(); j++) {
                Dispatchable* noncombustion_ptr = test_model.noncombustion_ptr_vec[j];
                
                charging_kW += noncombustion_ptr->storage_vec_kW[i];
            }
            
            for (size_t j = 0; j < test_model.nondisp_ptr_vec.size(); j++) {
                Nondispatchable* nondisp_ptr = test_model.nondisp_ptr_vec[j];
                
                charging_kW += nondisp_ptr->storage_vec_kW[i];
            }
            
            testFloatEquals(
                charged_kW,
                charging_kW,
                FLOAT_TOLERANCE,
                __FILE__,
                __LINE__
            );
        }
        
        testFloatEquals(
            load_remaining_kW,
            0,
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    
    // test writeResults()
    std::cout << "\tTesting Model::writeResults() ..." <<
        std::endl;
    
    test_model.writeResults("test");
    
} catch (...) {
    printRed("\n\t\t\t\tModel Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tModel Tests:  PASS");
std::cout << std::endl;
