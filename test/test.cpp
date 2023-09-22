/*
 *  PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.1
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 *  Testing suite for entire program.
 */

#include "../header/includes.h"
#include "../header/Model_includes.h"
#include "../header/Model.h"


#ifdef _WIN32
#include <windows.h>

void activateVirtualTerminal(void)
{
    /*
     *  Helper function to activate virtual terminal, so that Windows can
     *  attempt to pretend to be almost as awesome as Linux ...
     */

    HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(handleOut , &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;
    SetConsoleMode(handleOut , consoleMode);
}

#endif  /* _WIN32 */


void testFloatEquals(
    double x,
    double y,
    double tol,
    std::string file,
    int line
) {
    /*
     *  Helper function to handle floating point == tests
     */

    if (fabs(x - y) <= tol) {
        return;
    }

    std::string error_str = "ERROR: testFloatEquals():\t in ";
    error_str += file;
    error_str += "\tline ";
    error_str += std::to_string(line);
    error_str += ":\t\n";
    error_str += std::to_string(x);
    error_str += " and ";
    error_str += std::to_string(y);
    error_str += " are not equal to within +/- ";
    error_str += std::to_string(tol);
    error_str += "\n";

    #ifdef _WIN32
        std::cout << error_str << std::endl;
    #endif

    throw std::runtime_error(error_str);
    return;
}


void testGreaterThan(
    double x,
    double y,
    std::string file,
    int line
) {
    /*
     *  Helper function to handle > tests
     */

    if (x > y) {
        return;
    }

    std::string error_str = "ERROR: testGreaterThan():\t in ";
    error_str += file;
    error_str += "\tline ";
    error_str += std::to_string(line);
    error_str += ":\t\n";
    error_str += std::to_string(x);
    error_str += " is not greater than ";
    error_str += std::to_string(y);
    error_str += "\n";

    #ifdef _WIN32
        std::cout << error_str << std::endl;
    #endif

    throw std::runtime_error(error_str);
}


void testGreaterThanOrEqualTo(
    double x,
    double y,
    std::string file,
    int line
) {
    /*
     *  Helper function to handle >= tests
     */

    if (x >= y) {
        return;
    }

    std::string error_str = "ERROR: testGreaterThanOrEqualTo():\t in ";
    error_str += file;
    error_str += "\tline ";
    error_str += std::to_string(line);
    error_str += ":\t\n";
    error_str += std::to_string(x);
    error_str += " is not greater than or equal to ";
    error_str += std::to_string(y);
    error_str += "\n";

    #ifdef _WIN32
        std::cout << error_str << std::endl;
    #endif

    throw std::runtime_error(error_str);
}


void testLessThan(
    double x,
    double y,
    std::string file,
    int line
) {
    /*
     *  Helper function to handle < tests
     */

    if (x < y) {
        return;
    }

    std::string error_str = "ERROR: testLessThan():\t in ";
    error_str += file;
    error_str += "\tline ";
    error_str += std::to_string(line);
    error_str += ":\t\n";
    error_str += std::to_string(x);
    error_str += " is not less than ";
    error_str += std::to_string(y);
    error_str += "\n";

    #ifdef _WIN32
        std::cout << error_str << std::endl;
    #endif

    throw std::runtime_error(error_str);
}


void testLessThanOrEqualTo(
    double x,
    double y,
    std::string file,
    int line
) {
    /*
     *  Helper function to handle <= tests
     */

    if (x <= y) {
        return;
    }

    std::string error_str = "ERROR: testLessThanOrEqualTo():\t in ";
    error_str += file;
    error_str += "\tline ";
    error_str += std::to_string(line);
    error_str += ":\t\n";
    error_str += std::to_string(x);
    error_str += " is not less than or equal to ";
    error_str += std::to_string(y);
    error_str += "\n";

    #ifdef _WIN32
        std::cout << error_str << std::endl;
    #endif

    throw std::runtime_error(error_str);
}


void testTruth(
    bool truth_value,
    std::string file,
    int line
) {
    /*
     *  Helper function to handle truth value tests
     */

    if (truth_value) {
        return;
    }

    std::string error_str = "ERROR: testTruth():\t in ";
    error_str += file;
    error_str += "\tline ";
    error_str += std::to_string(line);
    error_str += ":\t\n";
    error_str += "Given statement is not true";

    #ifdef _WIN32
        std::cout << error_str << std::endl;
    #endif

    throw std::runtime_error(error_str);
    return;
}


void printGreen(std::string input_str) {
    /*
     *  Helper function to stream a green string to std::cout
     */
    
    std::cout << "\x1B[32m" << input_str << "\033[0m";
    
    return;
}


void printRed(std::string input_str) {
    /*
     *  Helper function to stream a red string to std::cout
     */
    
    std::cout << "\x1B[31m" << input_str << "\033[0m";
    
    return;
}


void printGold(std::string input_str) {
    /*
     *  Helper function to stream a red string to std::cout
     */
    
    std::cout << "\x1B[33m" << input_str << "\033[0m";
    
    return;
}


int main(int argc, char** argv) {
    
    //  colour streaming
    std::cout << std::endl;
    std::cout << "TEST:  Streaming in ";
    printGreen("GREEN");
    std::cout << std::endl;
    
    std::cout << "TEST:  Streaming in ";
    printRed("RED");
    std::cout << std::endl;
    
    std::cout << "TEST:  Streaming in ";
    printGold("GOLD");
    std::cout << std::endl << std::endl;
    
    std::cout << "Running Tests ..." << std::endl;
    
    #include "test_utilities.cpp"

    #include "assets/nondispatchable/test_Solar.cpp"
    #include "assets/nondispatchable/test_Tidal.cpp"
    #include "assets/nondispatchable/test_Wave.cpp"
    #include "assets/nondispatchable/test_Wind.cpp"
    
    #include "assets/dispatchable/test_Hydro.cpp"
    #include "assets/dispatchable/combustion/test_Diesel.cpp"
    
    #include "assets/storage/batterystorage/test_LiIon.cpp"

    #include "test_Model.cpp"
    
    
    //  all passed
    std::cout << std::endl << std::endl;
    printGreen("\t\t************************\n");
    printGreen("\t\t**  ALL TESTS PASSED  **\n");
    printGreen("\t\t************************\n");
    std::cout << std::endl << std::endl;
    return 0;
}
