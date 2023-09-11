/*
 *  Nondispatchable <-- Wave class implementation file
 */


#include "../../../header/includes.h"
#include "../../../header/assets/nondispatchable/Nondispatchable.h"
#include "../../../header/assets/nondispatchable/Wave.h"


Wave :: Wave(
    structNondispatchable struct_nondisp,
    structWave struct_wave
) : Nondispatchable(struct_nondisp) {
    /*
     *  Wave class constructor
     */
    
    this->struct_nondisp.nondisp_type = WAVE;
    this->struct_nondisp.nondisp_type_str = "WAVE";
    this->struct_wave = struct_wave;
    
    if (
        not this->struct_wave.path_2_normalized_performance_matrix.empty()
    ) {
        this->struct_wave.power_mode = NORMALIZED_PERFORMANCE_MATRIX;
        this->_readInNormalizedPerformanceMatrix();
    }
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWave object constructed at " << this
            << std::endl;
    }
    
    return;
}


void Wave :: _readInNormalizedPerformanceMatrix() {
    /*
     *  Helper method to read in normalized performance matrix data
     */
    
    std::ifstream ifs;
    ifs.open(this->struct_wave.path_2_normalized_performance_matrix);
    
    int line_idx = 0;
    std::string file_line;
    while (std::getline(ifs, file_line)) {
        std::vector<std::string> file_line_split_vec = 
            splitString(file_line, ',');
        
        if (line_idx <= 0) {
            for (size_t i = 1; i < file_line_split_vec.size(); i++) {
                this->interp_sig_wave_height_vec_m.push_back(
                    std::stod(file_line_split_vec[i])
                );
            }
            
            this->min_interp_sig_wave_height_m = 
                this->interp_sig_wave_height_vec_m[0];
            
            this->max_interp_sig_wave_height_m = 
                this->interp_sig_wave_height_vec_m[
                    this->interp_sig_wave_height_vec_m.size() - 1
                ];
        }
        
        else {
            this->interp_energy_period_vec_s.push_back(
                std::stod(file_line_split_vec[0])
            );
            
            std::vector<double> normalized_performance_matrix_row = {};
            
            for (size_t i = 1; i < file_line_split_vec.size(); i++) {
                normalized_performance_matrix_row.push_back(
                    std::stod(file_line_split_vec[i])
                );
            }
            
            this->interp_normalized_performance_matrix.push_back(
                normalized_performance_matrix_row
            );
        }
        
        line_idx++;
    }
    
    this->min_interp_energy_period_s = 
        this->interp_energy_period_vec_s[0];
    
    this->max_interp_energy_period_s = 
        this->interp_energy_period_vec_s[
            this->interp_energy_period_vec_s.size() - 1
        ];
    
    ifs.close();
    
    return;
}


void Wave :: _writeSummary(std::string _write_path, int asset_idx) {
    /*
     *  Helper method to write Wave-level summary
     */
    
    // construct filename 
    std::string filename = "Nondispatchable/" +
        std::to_string(int(this->struct_nondisp.cap_kW)) +
        "kW_" + this->struct_nondisp.nondisp_type_str +
        "_" + std::to_string(asset_idx) + "/" +
        std::to_string(int(this->struct_nondisp.cap_kW)) +
        "kW_" + this->struct_nondisp.nondisp_type_str +
        "_" + std::to_string(asset_idx) +
        "_summary.txt";
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + filename);
    
    // write attributes
    ofs << this->struct_nondisp.cap_kW << "kW Wave Summary\n\n";
    ofs << "Attributes:\n\n";
    
    ofs << "\t2D resource key: " << this->struct_wave.resource_key
        << "\n";
    ofs << "\treplacement running hours: " << this->struct_nondisp.replace_running_hrs
        << " hrs\n";
    
    ofs << "\tpower mode: " << this->struct_wave.power_mode << " ";
    
    switch (this->struct_wave.power_mode) {
        case (GAUSSIAN): {
            ofs << "(GAUSSIAN)\n";
            ofs << "\tdesign significant wave height: " <<
                this->struct_wave.design_significant_wave_height_m <<
                " m\n";
            ofs << "\tdesign energy period: " << 
                this->struct_wave.design_energy_period_s << " s\n";
            
            break;
        }
        
        case (NORMALIZED_PERFORMANCE_MATRIX): {
            ofs << "(NORMALIZED_PERFORMANCE_MATRIX)\n";
            ofs << "\tpath to normalized performance matrix: " <<
                this->struct_wave.path_2_normalized_performance_matrix
                << "\n";
            
            break;
        }
        
        case (PARABOLOID): {
            ofs << "(PARABOLOID)\n";
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    ofs << "\tcapital cost: " << this->struct_nondisp.capital_cost <<
        "\n";
    ofs << "\toperation and maintenance cost (per kWh produced): " <<
        this->struct_nondisp.op_maint_cost_per_kWh << "\n";
    ofs << "\treal discount rate (annual): " <<
        this->struct_nondisp.real_discount_rate_annual << "\n";
    
    // write results
    ofs << "\nResults:\n\n";
    
    ofs << "\trunning hours: " << this->struct_nondisp.running_hrs
        << " hrs\n";
    ofs << "\tnumber of replacements: " << this->struct_nondisp.n_replacements
        << "\n";
    
    ofs.close();
    
    return;
}


double Wave :: _productionLookupkW(
    double significant_wave_height_m,
    double energy_period_s
) {
    /*
     *  Helper method to lookup and return wave energy converter
     *  production
     */
    
    if (
        significant_wave_height_m <= this->min_interp_sig_wave_height_m ||
        significant_wave_height_m >= this->max_interp_sig_wave_height_m
    ) {
        return 0;
    }
    
    else if (
        energy_period_s <= this->min_interp_energy_period_s ||
        energy_period_s >= this->max_interp_energy_period_s
    ) {
        return 0;
    }
    
    int idx_horizontal = 0;
    int idx_vertical = 0;
    int max_idx_horizontal = this->interp_sig_wave_height_vec_m.size() - 1;
    int max_idx_vertical = this->interp_energy_period_vec_s.size() - 1;
    double normalized_production = 0;
    
    while (
        this->interp_sig_wave_height_vec_m[idx_horizontal + 1] <
        significant_wave_height_m
    ) {
        idx_horizontal++;
        
        if (idx_horizontal >= max_idx_horizontal - 1) {
            idx_horizontal = max_idx_horizontal - 1;
            break;
        }
    }
    
    while (
        this->interp_energy_period_vec_s[idx_vertical + 1] <
        energy_period_s
    ) {
        idx_vertical++;
        
        if (idx_vertical >= max_idx_vertical - 1) {
            idx_vertical = max_idx_vertical - 1;
            break;
        }
    }
    
    normalized_production = linearInterpolation2d(
        significant_wave_height_m,                              // x
        energy_period_s,                                        // y
        this->interp_sig_wave_height_vec_m[idx_horizontal],     // x_0
        this->interp_sig_wave_height_vec_m[idx_horizontal + 1], // x_1
        this->interp_energy_period_vec_s[idx_vertical],         // y_0
        this->interp_energy_period_vec_s[idx_vertical + 1],     // y_1
        this->interp_normalized_performance_matrix[
            idx_vertical
        ][
            idx_horizontal
        ],                                                      // z_00
        this->interp_normalized_performance_matrix[
            idx_vertical
        ][
            idx_horizontal + 1
        ],                                                      // z_10
        this->interp_normalized_performance_matrix[
            idx_vertical + 1
        ][
            idx_horizontal
        ],                                                      // z_01
        this->interp_normalized_performance_matrix[
            idx_vertical + 1
        ][
            idx_horizontal + 1
        ]                                                       // z_11
    );
    
    return normalized_production * this->struct_nondisp.cap_kW;
}


double Wave :: getProductionkW(
    double significant_wave_height_m,
    double energy_period_s
) {
    /*
     *  Method to compute and return production under given wave
     *  resource statistics, namely significant wave height (H_s) and
     *  energy period (T_e). Implements multiple power modes.
     */
     
    // first, check if no resource
    if (
        significant_wave_height_m <= 0 ||
        energy_period_s <= 0
    ) {
        return 0;
    }
    
    // otherwise, apply selected power mode
    double production = 0;
    double H_s_nondim = 0;
    double T_e_nondim = 0;
    
    switch (this->struct_wave.power_mode) {
        case (NORMALIZED_PERFORMANCE_MATRIX): {
            /*
             *  Interpolate over given normalized performance matrix
             */
             
            //...
            
            break;
        }
        
        case (GAUSSIAN): {
            /*
             *  ref: docs/wind_tidal_wave.pdf
             */
            
            H_s_nondim = 
                (
                    significant_wave_height_m - 
                    this->struct_wave.design_significant_wave_height_m
                ) / 
                this->struct_wave.design_significant_wave_height_m;
                
            T_e_nondim =
                (
                    energy_period_s -
                    this->struct_wave.design_energy_period_s
                ) / 
                this->struct_wave.design_energy_period_s;
                
            production = exp(
                -2.25119 * pow(T_e_nondim, 2) +
                3.44570 * T_e_nondim * H_s_nondim -
                4.01508 * pow(H_s_nondim, 2)
            );
            
            break;
        }
        
        case (PARABOLOID): {
            /*
             *  ref: B. Robertson, H. Bailey, M. Leary, and B. Buckham,
             *       “A methodology for architecture agnostic and
             *       time flexible representations of wave energy
             *       converter performance”,
             *       Applied Energy, vol. 287, p. 116588, 2021,
             *       doi:10.1016/j.apenergy.2021.116588
             */
             
            // first, check for idealized wave breaking (deep water)
            if (
                significant_wave_height_m >= 0.2184 * pow(energy_period_s, 2)
            ) {
                return 0;
            }
            
            // otherwise, apply generic quadratic performance model
            // (with outputs bounded to [0, 1])
            production = 
                0.289 * significant_wave_height_m - 
                0.00111 * pow(significant_wave_height_m, 2) * energy_period_s - 
                0.0169 * energy_period_s;
            
            if (production < 0) {
                production = 0;
            } 
            
            else if (production > 1) {
                production = 1;
            }
            
            break;
        }
        
        default : {
            // do nothing!
            
            break;
        }
    }
    
    double production_kW = this->struct_nondisp.cap_kW * production;
    return production_kW;
}


void Wave :: writeResults(
    std::string _write_path,
    std::vector<double>* ptr_2_time_vec_hr,
    int asset_idx
) {
    /*
     *  Method to write Wave-level results
     */
    
    Nondispatchable::_writeTimeSeriesResults(
        _write_path, ptr_2_time_vec_hr, asset_idx
    );
    this->_writeSummary(_write_path, asset_idx);
    
    return;
}


Wave :: ~Wave() {
    /*
     *  Wave class destructor
     */
    
    if (this->struct_nondisp.test_flag) {
        std::cout << "\tWave object at " << this
            << " destroyed" << std::endl;
    }
    
    return;
}
