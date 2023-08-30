/*
 *  Testing for Nondispatchable <-- Wave class
 */

std::cout << std::endl;
printGold("Testing Nondispatchable <-- Wave ... ");
std::cout << std::endl;

try {
    //  construction
    structNondispatchable struct_nondisp;
    struct_nondisp.test_flag = true;
    
    structWave struct_wave;
    
    Wave test_wave(struct_nondisp, struct_wave);
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tNondispatchable <-- Wave Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tNondispatchable <-- Wave Tests:  PASS");
std::cout << std::endl;
