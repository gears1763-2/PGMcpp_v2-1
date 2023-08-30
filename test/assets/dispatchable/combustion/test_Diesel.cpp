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
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tDispatchable <-- Combustion <-- Diesel Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tDispatchable <-- Combustion <-- Diesel Tests:  PASS");
std::cout << std::endl;
