/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Testing suite for the Nondispatchable <-- Wave class.
 */

std::cout << std::endl;
printGold("Testing Nondispatchable <-- Wave ... ");
std::cout << std::endl;

try {
    //  construction
    structNondispatchable struct_nondisp;
    struct_nondisp.test_flag = true;
    
    structWave struct_wave;
    
    Wave test_wave(struct_nondisp, struct_wave, 16);
    
    struct_wave.path_2_normalized_performance_matrix =
        "data/input/test/normalized_performance_matrix.csv";
    
    Wave test_wave_lookup(struct_nondisp, struct_wave, 16);
    
    
    // test input bounds checking
    bool error_flag = true;
    
    try {
        // bad design_significant_wave_height_m
        structWave bad_struct_wave;
        bad_struct_wave.design_significant_wave_height_m = 0;
        
        Wave bad_test_wave(struct_nondisp, bad_struct_wave, 16);
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    try {
        // bad design_energy_period_s
        structWave bad_struct_wave;
        bad_struct_wave.design_energy_period_s = 0;
        
        Wave bad_test_wave(struct_nondisp, bad_struct_wave, 16);
    }
    
    catch (...) {
        // task failed successfully! =P
    }
    
    if (not error_flag) {
        expectedErrorNotDetected(__LINE__, __FILE__);
    }
    
    
    //  test post-construction attributes
    std::vector<double> exp_interp_significant_wave_height_vec_m = {
        0.25,
        0.75,
        1.25,
        1.75,
        2.25,
        2.75,
        3.25,
        3.75,
        4.25,
        4.75,
        5.25,
        5.75,
        6.25,
        6.75,
        7.25,
        7.75,
    };
    
    for (size_t i = 0; i < exp_interp_significant_wave_height_vec_m.size(); i++) {
        testFloatEquals(
            test_wave_lookup.interp_sig_wave_height_vec_m[i],
            exp_interp_significant_wave_height_vec_m[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    testFloatEquals(
        test_wave_lookup.min_interp_sig_wave_height_m,
        exp_interp_significant_wave_height_vec_m[0],
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_wave_lookup.max_interp_sig_wave_height_m,
        exp_interp_significant_wave_height_vec_m[
            exp_interp_significant_wave_height_vec_m.size() - 1
        ],
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    std::vector<double> exp_interp_energy_period_vec_s = {
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        16,
        17,
        18,
        19,
        20
    };
    
    for (size_t i = 0; i < exp_interp_energy_period_vec_s.size(); i++) {
        testFloatEquals(
            test_wave_lookup.interp_energy_period_vec_s[i],
            exp_interp_energy_period_vec_s[i],
            FLOAT_TOLERANCE,
            __FILE__,
            __LINE__
        );
    }
    
    testFloatEquals(
        test_wave_lookup.min_interp_energy_period_s,
        exp_interp_energy_period_vec_s[0],
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_wave_lookup.max_interp_energy_period_s,
        exp_interp_energy_period_vec_s[
            exp_interp_energy_period_vec_s.size() - 1
        ],
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    std::vector<std::vector<double>> exp_interp_normalized_performance_matrix = {
        {0,0.129128125,0.268078125,0.404253125,0.537653125,0.668278125,0.796128125,0.921203125,1,1,1,0,0,0,0,0},
        {0,0.11160375,0.24944375,0.38395375,0.51513375,0.64298375,0.76750375,0.88869375,1,1,1,1,1,1,1,1},
        {0,0.094079375,0.230809375,0.363654375,0.492614375,0.617689375,0.738879375,0.856184375,0.969604375,1,1,1,1,1,1,1},
        {0,0.076555,0.212175,0.343355,0.470095,0.592395,0.710255,0.823675,0.932655,1,1,1,1,1,1,1},
        {0,0.059030625,0.193540625,0.323055625,0.447575625,0.567100625,0.681630625,0.791165625,0.895705625,0.995250625,1,1,1,1,1,1},
        {0,0.04150625,0.17490625,0.30275625,0.42505625,0.54180625,0.65300625,0.75865625,0.85875625,0.95330625,1,1,1,1,1,1},
        {0,0.023981875,0.156271875,0.282456875,0.402536875,0.516511875,0.624381875,0.726146875,0.821806875,0.911361875,0.994811875,1,1,1,1,1},
        {0,0.0064575,0.1376375,0.2621575,0.3800175,0.4912175,0.5957575,0.6936375,0.7848575,0.8694175,0.9473175,1,1,1,1,1},
        {0,0,0.119003125,0.241858125,0.357498125,0.465923125,0.567133125,0.661128125,0.747908125,0.827473125,0.899823125,0.964958125,1,1,1,1},
        {0,0,0.10036875,0.22155875,0.33497875,0.44062875,0.53850875,0.62861875,0.71095875,0.78552875,0.85232875,0.91135875,0.96261875,1,1,1},
        {0,0,0.081734375,0.201259375,0.312459375,0.415334375,0.509884375,0.596109375,0.674009375,0.743584375,0.804834375,0.857759375,0.902359375,0.938634375,0.966584375,0.986209375},
        {0,0,0.0631,0.18096,0.28994,0.39004,0.48126,0.5636,0.63706,0.70164,0.75734,0.80416,0.8421,0.87116,0.89134,0.90264},
        {0,0,0.044465625,0.160660625,0.267420625,0.364745625,0.452635625,0.531090625,0.600110625,0.659695625,0.709845625,0.750560625,0.781840625,0.803685624999999,0.816095625,0.819070625},
        {0,0,0.02583125,0.14036125,0.24490125,0.33945125,0.42401125,0.49858125,0.56316125,0.61775125,0.66235125,0.69696125,0.72158125,0.73621125,0.74085125,0.73550125},
        {0,0,0.007196875,0.120061875,0.222381875,0.314156875,0.395386875,0.466071875,0.526211875,0.575806875,0.614856875,0.643361875,0.661321875,0.668736875,0.665606875,0.651931875},
        {0,0,0,0.0997625,0.1998625,0.2888625,0.3667625,0.4335625,0.4892625,0.5338625,0.5673625,0.5897625,0.6010625,0.6012625,0.5903625,0.5683625}
    };
    
    for (size_t i = 0; i < exp_interp_normalized_performance_matrix.size(); i++) {
        std::vector<double> exp_interp_normalized_performance_matrix_row = 
            exp_interp_normalized_performance_matrix[i];
            
        for (size_t j = 0; j < exp_interp_normalized_performance_matrix_row.size(); j++) {
            testFloatEquals(
                test_wave_lookup.interp_normalized_performance_matrix[i][j],
                exp_interp_normalized_performance_matrix_row[j],
                FLOAT_TOLERANCE,
                __FILE__,
                __LINE__
            );
        }
    }
    
    
    // test _productionLookup()
    std::cout << "\tTesting Wave::_productionLookup() ..." <<
        std::endl;
    
    testFloatEquals(
        test_wave_lookup._productionLookup(
            test_wave_lookup.min_interp_sig_wave_height_m - 1,
            10
        ),
        0,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_wave_lookup._productionLookup(
            test_wave_lookup.max_interp_sig_wave_height_m + 1,
            10
        ),
        0,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_wave_lookup._productionLookup(
            2,
            test_wave_lookup.min_interp_energy_period_s - 1
        ),
        0,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_wave_lookup._productionLookup(
            2,
            test_wave_lookup.max_interp_energy_period_s + 1
        ),
        0,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    std::vector<double> test_significant_wave_height_vec_m = {
        0.626108375176177,
        1.219263923425,
        1.41495668945739,
        2.00876328967073,
        2.5354130305416,
        2.84770785755802,
        3.55191508658532,
        4.16683461626257,
        4.30316450962525,
        5.06901867812102,
        5.29145762686883,
        6.05597428540738,
        6.64684112928983,
        6.93190158670711,
        7.55955544184743
    };
    
    std::vector<double> test_energy_period_vec_s = {
        5.16598468831058,
        6.46105949821156,
        7.86646285157274,
        8.87558530428279,
        9.53506682088065,
        10.3795472830698,
        11.9262997882564,
        12.1540201398644,
        13.1212698177209,
        14.9877642230648,
        15.5259068620575,
        16.8719100817382,
        17.424985552884,
        18.3011525323712,
        19.6575525923579
    };
    
    std::vector<std::vector<double>> exp_normalized_performance = {
        {0.094944307090449,0.25645487420773,0.309819882099484,0.469731083948595,0.608216481017995,0.688955533292041,0.866511595749267,0.985996124758886,1,1,0.930847408810175,0.16598468831058,0.16598468831058,0.16598468831058,0.16598468831058},
        {0.077872464640007,0.232410332798113,0.284975580365443,0.441953951616644,0.577000752414972,0.6553546757569,0.826402798354056,0.9673100543388,0.987475923639188,1,1,1,1,1,1},
        {0.059346257161893,0.206317416104218,0.258014771073192,0.411810461350574,0.543125728886674,0.618891332099317,0.782877097809526,0.919363768800713,0.944225202454139,1,1,1,1,1,1},
        {0.046043875404358,0.187581906451755,0.238656088076616,0.390166516387879,0.51880242928252,0.592709539735049,0.75162431789807,0.882822570492047,0.910461222127912,0.998494784515757,1,1,1,1,1},
        {0.037350505365605,0.175337879687056,0.226004805184601,0.376021769523089,0.502906671073462,0.575599219684922,0.731200007014337,0.858942173406256,0.886235581420678,0.990157360351165,1,1,1,1,1},
        {0.026218455469529,0.159659136430308,0.209804561576487,0.357909112732422,0.482551806075148,0.553689081765248,0.705046231569699,0.828362804218925,0.854584041210372,0.97607982319603,0.998194132890957,1,1,1,1},
        {0.005828965320777,0.130941911466111,0.180132155277672,0.324733925713973,0.445269784952642,0.513558418909661,0.657142911717612,0.772353555911481,0.796611001802925,0.922472899730022,0.954895796314347,1,1,1,1},
        {0.004109296128752,0.126818803945098,0.17576364074625,0.319849714938707,0.43978094628569,0.507650189490221,0.650090354308893,0.764107604684571,0.788075931794388,0.912114929701426,0.944529618707578,0.99790562648716,1,1,1},
        {0,0.109566873403587,0.157208231695666,0.299103869143196,0.416466923648158,0.482554790847535,0.620134384921538,0.729082655831607,0.751822941414104,0.86811911157335,0.899402789359582,0.981105525405649,0.999064716830095,1,1},
        {0,0.076923977015025,0.121401998853559,0.259070767041513,0.371478030071632,0.434128389434561,0.562328576816171,0.661495276297517,0.681865819410466,0.78322071328537,0.809809989103457,0.885757979111653,0.931898262347458,0.949429112761252,0.978994626911243},
        {0,0.067512465061594,0.111078442255227,0.247528530884585,0.358506953466571,0.420166219830041,0.545662161940313,0.642008665096893,0.661695970444626,0.758743035789987,0.783978834278381,0.854720627979413,0.896447817858214,0.911830829303322,0.936452317386724},
        {0,0.043972376480072,0.085257146165547,0.218659071555696,0.326063676149207,0.385244019135093,0.503976098696133,0.593268722345277,0.611247112314161,0.697519430128919,0.719369908979864,0.777089956244209,0.807630728705878,0.817205287821241,0.828235713727608},
        {0,0.034299706019354,0.07464712288211,0.206796551773733,0.312732667258999,0.370894415567736,0.486847210197296,0.573241380354698,0.590517573505265,0.672362525560543,0.692821969660542,0.745191354727125,0.77113560543242,0.778323459626182,0.783769290334688},
        {0,0.018976526989941,0.057839015241248,0.188004273560891,0.291614048910847,0.348162167492433,0.459712095224814,0.541514614923043,0.557678407149309,0.632509647899449,0.650765452930854,0.694658465819228,0.713321032932433,0.71672793470436,0.713326802997352},
        {0,0.002313049918869,0.036857828651574,0.158911819848354,0.258920172212785,0.312970219639047,0.417704039182849,0.492398193576206,0.506839870365831,0.570813136942389,0.585657473344172,0.616428156813421,0.623817900033061,0.621371483645493,0.604274309252506}
    };
    
    for (size_t i = 0; i < test_significant_wave_height_vec_m.size(); i++) {
        for (size_t j = 0; j < test_energy_period_vec_s.size(); j++) {
            testFloatEquals(
                test_wave_lookup._productionLookup(
                    test_significant_wave_height_vec_m[i],
                    test_energy_period_vec_s[j]
                ),
                exp_normalized_performance[j][i],
                FLOAT_TOLERANCE,
                __FILE__,
                __LINE__
            );
        }
    }
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Wave Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Wave Tests:  PASS");
std::cout << std::endl;
