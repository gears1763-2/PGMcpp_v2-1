/*
 *  Header file of various utilities
 */


#ifndef UTILITIES_H
#define UTILITIES_H


inline std::string to_upper(std::string input_str) {
    /*
     *  Helper function to convert string to upper case
     */
    
    std::string return_str = "";
    
    for (size_t i = 0; i < input_str.length(); i++) {
        return_str.push_back(std::toupper(input_str[i]));
    }
    
    return return_str;
}


#endif  /* UTILITIES_H */
