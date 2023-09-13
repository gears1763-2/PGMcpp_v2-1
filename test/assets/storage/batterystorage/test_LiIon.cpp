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
    
    LiIon test_Liion(
        struct_storage,
        struct_battery_storage,
        struct_liion,
        16
    );
    
    
    //...
    
} catch (...) {
    printRed("\n\t\t\t\tStorage <-- BatteryStorage <-- LiIon Tests:  FAIL\n");
    std::cout << std::endl;
    
    throw;
}

printGreen("\n\t\t\t\tStorage <-- BatteryStorage <-- LiIon Tests:  PASS");
std::cout << std::endl;
