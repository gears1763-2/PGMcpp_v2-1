/*
 *  Header file of various utilities
 */


#ifndef UTILITIES_H
#define UTILITIES_H


inline std::string stringToUpper(std::string input_str) {
    /*
     *  Utility function to convert string to upper case
     */
    
    std::string return_str = "";
    
    for (size_t i = 0; i < input_str.length(); i++) {
        return_str.push_back(std::toupper(input_str[i]));
    }
    
    return return_str;
}


inline double linearInterpolate1d(
    double x,
    double x_0,
    double x_1,
    double y_0, 
    double y_1
) {
    /*
     *  Utility function to perform a 1d linear interpolation
     *
     *  For 1d data of the form y = f(x):
     *  
     *      (x_0, y_0)
     *      (x_1, y_1)
     */
    
    // check for degenerate inputs
    if (x_0 == x_1) {
        return y_0;
    }
    
    // check for coincidence
    if (x == x_0) {
        return y_0;
    }
    
    else if (x == x_1) {
        return y_1;
    }
    
    // interpolate
    double m = (y_1 - y_0) / (x_1 - x_0);
    double y = m * (x - x_0) + y_0;
    
    return y;
}


inline double linearInterpolation2d(
    double x,
    double y,
    double x_0,
    double x_1,
    double y_0,
    double y_1,
    double z_00,
    double z_10,
    double z_01,
    double z_11
) {
    /*
     *  Utility function to perform a 2d linear interpolation
     *
     *  For 2d data of the form z = f(x, y):
     *
     *              x_0     x_1
     *      y_0     z_00    z_10
     *      y_1     z_01    z_11
     */
    
    // perform first horizontal 1d interpolation
    double z_horizontal_0 = linearInterpolate1d(
        x,
        x_0,
        x_1,
        z_00,
        z_10
    );
    
    // perform second horizontal 1d interpolation
    double z_horizontal_1 = linearInterpolate1d(
        x,
        x_0,
        x_1,
        z_01,
        z_11
    );
    
    // perform vertical 1d interpolation over horizontal interpolations
    double z = linearInterpolate1d(
        y,
        y_0,
        y_1,
        z_horizontal_0,
        z_horizontal_1
    );
    
    return z;
}


inline std::vector<std::string> splitString(
    std::string input_string,
    char delimiter
) {
    /*
     *  Utility function to split string with respect to given delimiter
     */
    
    std::vector<std::string> string_split_vec = {};
    
    std::string temp_string = "";
    for (size_t i = 0; i < input_string.size(); i++) {
        if (char(input_string[i]) != delimiter) {
            if (char(input_string[i]) != '\n') {
                temp_string += input_string[i];
            }
            
            continue;
        }
        
        string_split_vec.push_back(temp_string);
        temp_string = "";
    }
    
    while (temp_string[temp_string.length() - 1] == '\n') {
        temp_string.pop_back();
    }
    string_split_vec.push_back(temp_string);
    
    return string_split_vec;
}


#endif  /* UTILITIES_H */
