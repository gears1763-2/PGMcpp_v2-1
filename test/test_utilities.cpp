/*
 *  Testing for various utility functions
 */

std::cout << std::endl;
printGold("Testing Utility Functions ... ");
std::cout << std::endl;

try {
    //  test stringToUpper()
    std::cout << "\tTesting stringToUpper() ..." <<
        std::endl;
        
    testTruth(
        stringToUpper("test").compare("TEST") == 0,
        __FILE__,
        __LINE__ 
    );
    
    
    //  test linearInterpolate1d()
    std::cout << "\tTesting linearInterpolate1d() ..." <<
        std::endl;
    
    testFloatEquals(
        linearInterpolate1d(
            -1,  // x
            0,  // x_0
            0,  // x_1
            1,  // y_0
            6   // y_1
        ),
        1,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        linearInterpolate1d(
            0,  // x
            0,  // x_0
            3,  // x_1
            1,  // y_0
            6   // y_1
        ),
        1,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        linearInterpolate1d(
            1,  // x
            0,  // x_0
            3,  // x_1
            1,  // y_0
            6   // y_1
        ),
        2.0 + (2.0 / 3.0),
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        linearInterpolate1d(
            3,  // x
            0,  // x_0
            3,  // x_1
            1,  // y_0
            6   // y_1
        ),
        6,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    
    //  test linearInterpolate2d()
    std::cout << "\tTesting linearInterpolate2d() ..." <<
        std::endl;
    
    testFloatEquals(
        linearInterpolation2d(
            5,      // x,
            3.14,   // y
            4,      // x_0
            8,      // x_1
            2,      // y_0
            9,      // y_1
            -1,     // z_00
            7,      // z_10
            2,      // z_01
            -4      // z_11
        ),
        0.918571428571429,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        linearInterpolation2d(
            5,      // x,
            3.14,   // y
            4,      // x_0
            4,      // x_1
            2,      // y_0
            9,      // y_1
            -1,     // z_00
            7,      // z_10
            2,      // z_01
            -4      // z_11
        ),
        -0.511428571428571,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        linearInterpolation2d(
            5,      // x,
            3.14,   // y
            4,      // x_0
            8,      // x_1
            2,      // y_0
            2,      // y_1
            -1,     // z_00
            7,      // z_10
            2,      // z_01
            -4      // z_11
        ),
        1,
        FLOAT_TOLERANCE,
        __FILE__,
        __LINE__
    );
    
    
} catch (...) {
    printRed("\n\t\t\t\t Utility Function Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tUtility Function Tests:  PASS");
std::cout << std::endl;
