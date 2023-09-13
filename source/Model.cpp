/*
 *  Model class implementation file
 */


#include "../header/includes.h"
#include "../header/Model_includes.h"
#include "../header/Model.h"


// -------- HELPER METHODS -------- //


void Model :: _readInLoadData() {
    /*
     *  Helper method to read in electrical load time series
     * 
     *  ref: https://github.com/ben-strasser/fast-cpp-csv-parser
     */

    // init CSVReader
    io::CSVReader<2> in(this->struct_model.path_2_load_data);
    
    // define expected .csv structure
    in.read_header(
        io::ignore_extra_column,
        "Time (since start of data) [hr]",
        "Electrical Load [kW]"
    );
    
    if (this->struct_model.print_flag) {
        std::cout << "Reading electrical load data ... ";
    }
    
    // populate time_vec_hr and load_vec_kW
    int periods = 0;
    double time_hr;
    double load_kW;
    while (in.read_row(time_hr, load_kW)) {
        if (this->struct_model.print_flag) {
            for (int i = 0; i < periods; i++) {
                std::cout << ".";
            }
        }
        
        this->time_vec_hr.push_back(time_hr);
        this->load_vec_kW.push_back(load_kW);
        
        if (this->struct_model.print_flag) {
            for (int i = 0; i < periods; i++) {
                std::cout << "\b";
            }
            periods = (periods + 1) % 3;
        }
    }
    
    if (this->struct_model.print_flag) {
        std::cout << "DONE" << std::endl;
    }
    
    return;
}


void Model :: _populateDeltaVecHr() {
    /*
     *  Helper method to populate dt_vec_hr
     */
    
    // initialize
    this->dt_vec_hr.resize(this->n_timesteps, 0);
    
    // populate dt_vec_hr
    for (int i = 0; i < this->n_timesteps; i++) {
        double dt_hr = 0;
        
        // handle running off end of time_vec_hr
        if (i == this->n_timesteps - 1) {
            dt_hr = this->time_vec_hr[i] - this->time_vec_hr[i - 1];
        }
        
        else {
            dt_hr = this->time_vec_hr[i + 1] - this->time_vec_hr[i];
        }
        
        this->dt_vec_hr[i] = dt_hr;
    }
    
    return;
}


void Model :: _readIn1dRenewableResource(
    std::string path_2_resource_data,
    std::vector<std::string> column_header_vec,
    int map_key
) {
    /*
     *  Helper method to read in 1d renewable resource time series
     * 
     *  ref: https://github.com/ben-strasser/fast-cpp-csv-parser
     */
     
    // init CSVReader
    io::CSVReader<2> in(path_2_resource_data);
    
    // define expected .csv structure
    in.read_header(
        io::ignore_extra_column,
        column_header_vec[0],
        column_header_vec[1]
    );
    
    // init resource_map_1D element,
    // handle duplicate key by way of overwrite
    if (this->resource_map_1D.count(map_key) > 0) {
        std::string warning_str = "\nWARNING:  Model::_readIn1dRenewableResource()";
        warning_str += "  map key ";
        warning_str += std::to_string(map_key);
        warning_str += " is already in use, previously read in data";
        warning_str += " will be replaced";
        std::cout << warning_str << std::endl;
        
        this->resource_map_1D.erase(map_key);
    }
    this->resource_map_1D.insert(
        std::pair<int, std::vector<double>> (map_key, {})
    );
    
    int idx = 0;
    int periods = 0;
    double time_hr;
    double resource;
    while (in.read_row(time_hr, resource)) {
        if (this->struct_model.print_flag) {
            for (int i = 0; i < periods; i++) {
                std::cout << ".";
            }
        }
                
        if (fabs(time_hr - this->time_vec_hr[idx]) > FLOAT_TOLERANCE) {
            std::cout << std::endl;
            
            std::string error_str = "\nERROR  Model::_readIn1dRenewableResource()";
            error_str += " 1d renewable resource data at ";
            error_str += path_2_resource_data;
            error_str += " does not correspond to the same points in time";
            error_str += " that the given electrical load data at ";
            error_str += this->struct_model.path_2_load_data;
            error_str += " does";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif
            
            this->clearAssets();
            throw std::runtime_error(error_str);
        }
        
        this->resource_map_1D[map_key].push_back(resource);
        
        if (this->struct_model.print_flag) {
            for (int i = 0; i < periods; i++) {
                std::cout << "\b";
            }
            periods = (periods + 1) % 3;
        }
        
        idx++;
    }
    
    if (
        this->resource_map_1D[map_key].size() !=
        (size_t)(this->n_timesteps)
    ) {
        std::cout << std::endl;
        
        std::string error_str = "\nERROR  Model::_readIn1dRenewableResource()";
        error_str += " 1d renewable resource data at ";
        error_str += path_2_resource_data;
        error_str += " is not the same size (i.e., number of points)";
        error_str += " as the given electrical load data at ";
        error_str += this->struct_model.path_2_load_data;
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        this->clearAssets();
        throw std::runtime_error(error_str);
    }
    
    // register path
    this->resource_path_map_1D.insert(
        std::pair<int, std::string> (map_key, path_2_resource_data)
    );
    
    return;
}


void Model :: _readIn2dRenewableResource(
    std::string path_2_resource_data,
    std::vector<std::string> column_header_vec,
    int map_key
) {
    /*
     *  Helper method to read in 2d renewable resource time series
     * 
     *  ref: https://github.com/ben-strasser/fast-cpp-csv-parser
     */
     
    // init CSVReader
    io::CSVReader<3> in(path_2_resource_data);
    
    // define expected .csv structure
    in.read_header(
        io::ignore_extra_column,
        column_header_vec[0],
        column_header_vec[1],
        column_header_vec[2]
    );
    
    // init resource_map_2D element,
    // handle duplicate key by way of overwrite
    if (this->resource_map_2D.count(map_key) > 0) {
        std::string warning_str = "\nWARNING:  Model::_readIn2dRenewableResource()";
        warning_str += "  map key ";
        warning_str += std::to_string(map_key);
        warning_str += " is already in use, previously read in data";
        warning_str += " will be replaced";
        std::cout << warning_str << std::endl;
        
        this->resource_map_2D.erase(map_key);
    }
    this->resource_map_2D.insert(
        std::pair<int, std::vector<std::vector<double>>> (map_key, {})
    );
    
    int idx = 0;
    int periods = 0;
    double time_hr;
    double resource_0;
    double resource_1;
    while (in.read_row(time_hr, resource_0, resource_1)) {
        if (this->struct_model.print_flag) {
            for (int i = 0; i < periods; i++) {
                std::cout << ".";
            }
        }
                
        if (fabs(time_hr - this->time_vec_hr[idx]) > FLOAT_TOLERANCE) {
            std::cout << std::endl;
            
            std::string error_str = "\nERROR  Model::_readIn2dRenewableResource()";
            error_str += " 2d renewable resource data at ";
            error_str += path_2_resource_data;
            error_str += " does not correspond to the same points in time";
            error_str += " that the given electrical load data at ";
            error_str += this->struct_model.path_2_load_data;
            error_str += " does";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif
            
            this->clearAssets();
            throw std::runtime_error(error_str);
        }
        
        std::vector<double> resource_vec = {resource_0, resource_1};
        this->resource_map_2D[map_key].push_back(resource_vec);
        
        if (this->struct_model.print_flag) {
            for (int i = 0; i < periods; i++) {
                std::cout << "\b";
            }
            periods = (periods + 1) % 3;
        }
        
        idx++;
    }
    
    if (
        this->resource_map_2D[map_key].size() !=
        (size_t)(this->n_timesteps)
    ) {
        std::cout << std::endl;
        
        std::string error_str = "\nERROR  Model::_readIn2dRenewableResource()";
        error_str += " 2d renewable resource data at ";
        error_str += path_2_resource_data;
        error_str += " is not the same size (i.e., number of points)";
        error_str += " as the given electrical load data at ";
        error_str += this->struct_model.path_2_load_data;
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        this->clearAssets();
        throw std::runtime_error(error_str);
    }
    
    // register path
    this->resource_path_map_2D.insert(
        std::pair<int, std::string> (map_key, path_2_resource_data)
    );
    
    return;
}


void Model :: _addNondispatchable(Nondispatchable* nondisp_ptr) {
    /*
     *  Helper method to add Nondispatchable asset to the Model
     */
    
    // set project life attribute
    nondisp_ptr->project_life_yrs = this->project_life_yrs;
    
    //  set asset real discount rate
    if (
        nondisp_ptr->struct_nondisp.nominal_inflation_rate_annual !=
        this->struct_model.nominal_inflation_rate_annual ||
        nondisp_ptr->struct_nondisp.nominal_discount_rate_annual !=
        this->struct_model.nominal_discount_rate_annual
    ) {
        /*
         *
         *  ref: https://www.homerenergy.com/products/pro/docs/3.11/real_discount_rate.html
         */
        nondisp_ptr->real_discount_rate_annual =
            (
                nondisp_ptr->struct_nondisp.nominal_discount_rate_annual - 
                nondisp_ptr->struct_nondisp.nominal_inflation_rate_annual
            ) / 
            (1 + nondisp_ptr->struct_nondisp.nominal_inflation_rate_annual);
    }
    
    else {
        nondisp_ptr->real_discount_rate_annual =
            this->real_discount_rate_annual;
    }
    
    // set pointers to Model vectors
    nondisp_ptr->ptr_2_dt_vec_hr = &(this->dt_vec_hr);
    nondisp_ptr->ptr_2_time_vec_hr = &(this->time_vec_hr);
    
    // push back
    this->nondisp_ptr_vec.push_back(nondisp_ptr);
    
    return;
}


void Model :: _addNonCombustion(Dispatchable* noncombustion_ptr) {
    /*
     *  Helper method to add non-Combustion asset to the Model
     */
    
    // set project life attribute
    noncombustion_ptr->project_life_yrs = this->project_life_yrs;
    
    //  set asset real discount rate
    if (
        noncombustion_ptr->struct_disp.nominal_inflation_rate_annual !=
        this->struct_model.nominal_inflation_rate_annual ||
        noncombustion_ptr->struct_disp.nominal_discount_rate_annual !=
        this->struct_model.nominal_discount_rate_annual
    ) {
        /*
         *
         *  ref: https://www.homerenergy.com/products/pro/docs/3.11/real_discount_rate.html
         */
        noncombustion_ptr->real_discount_rate_annual =
            (
                noncombustion_ptr->struct_disp.nominal_discount_rate_annual - 
                noncombustion_ptr->struct_disp.nominal_inflation_rate_annual
            ) / 
            (1 + noncombustion_ptr->struct_disp.nominal_inflation_rate_annual);
    }
    
    else {
        noncombustion_ptr->real_discount_rate_annual =
            this->real_discount_rate_annual;
    }
    
    // set pointers to Model vectors
    noncombustion_ptr->ptr_2_dt_vec_hr = &(this->dt_vec_hr);
    noncombustion_ptr->ptr_2_time_vec_hr = &(this->time_vec_hr);
    
    // push back
    this->noncombustion_ptr_vec.push_back(noncombustion_ptr);
    
    return;
}


void Model :: _addCombustion(Combustion* combustion_ptr) {
    /*
     *  Helper method to add Combustion asset to the Model
     */
    
    // set project life attribute
    combustion_ptr->project_life_yrs = this->project_life_yrs;
    
    //  set asset real discount rate
    if (
        combustion_ptr->struct_disp.nominal_inflation_rate_annual !=
        this->struct_model.nominal_inflation_rate_annual ||
        combustion_ptr->struct_disp.nominal_discount_rate_annual !=
        this->struct_model.nominal_discount_rate_annual
    ) {
        /*
         *
         *  ref: https://www.homerenergy.com/products/pro/docs/3.11/real_discount_rate.html
         */
        combustion_ptr->real_discount_rate_annual =
            (
                combustion_ptr->struct_disp.nominal_discount_rate_annual - 
                combustion_ptr->struct_disp.nominal_inflation_rate_annual
            ) / 
            (1 + combustion_ptr->struct_disp.nominal_inflation_rate_annual);
    }
    
    else {
        combustion_ptr->real_discount_rate_annual =
            this->real_discount_rate_annual;
    }
    
    // set pointers to Model vectors
    combustion_ptr->ptr_2_dt_vec_hr = &(this->dt_vec_hr);
    combustion_ptr->ptr_2_time_vec_hr = &(this->time_vec_hr);
    
    // push back
    this->combustion_ptr_vec.push_back(combustion_ptr);
    
    return;
}



void Model :: _addStorage(Storage* storage_ptr) {
    /*
     *  Helper method to add Storage asset to the Model
     */
    
    //...
    
    return;
}



double Model :: _getRenewableProductionkW(
    Nondispatchable* nondisp_ptr,
    int timestep
) {
    /*
     *  Helper method to compute and return production from renewable 
     *  asset in given time step
     */
    
    double production_kW = 0;
    
    switch (nondisp_ptr->nondisp_type) {
        case (SOLAR): {
            int resource_key =
                ((Solar*)nondisp_ptr)->struct_solar.resource_key;
            
            double solar_resource_kWm2 = this->resource_map_1D[
                resource_key
            ][timestep];
            
            production_kW = nondisp_ptr->getProductionkW(solar_resource_kWm2);
            
            break;
        }
        
        case (TIDAL): {
            int resource_key =
                ((Tidal*)nondisp_ptr)->struct_tidal.resource_key;
            
            double tidal_resource_ms = this->resource_map_1D[
                resource_key
            ][timestep];
            
            production_kW = nondisp_ptr->getProductionkW(tidal_resource_ms);
            
            break;
        }
        
        case (WAVE): {
            int resource_key =
                ((Wave*)nondisp_ptr)->struct_wave.resource_key;
                
            std::vector<double> wave_resource = this->resource_map_2D[
                resource_key
            ][timestep];
            
            double significant_wave_height_m = wave_resource[0];
            double energy_period_s = wave_resource[1];
            
            production_kW = nondisp_ptr->getProductionkW(
                significant_wave_height_m,
                energy_period_s
            );
            
            break;
        }
        
        case (WIND): {
            int resource_key =
                ((Wind*)nondisp_ptr)->struct_wind.resource_key;
            
            double wind_resource_ms = this->resource_map_1D[
                resource_key
            ][timestep];
            
            production_kW = nondisp_ptr->getProductionkW(wind_resource_ms);
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    return production_kW;
}


void Model :: _generateNetLoadVector() {
    /*
     *  Helper method to populate net load vector, as well as compute
     *  and commit renewable production, dispatch, and curtailment
     */
    
    for (int i = 0; i < this->n_timesteps; i++) {
        double load_kW = this->load_vec_kW[i];
        double net_load_kW = this->load_vec_kW[i];
        
        for (size_t j = 0; j < this->nondisp_ptr_vec.size(); j++) {
            // compute and commit renewable production
            Nondispatchable* nondisp_ptr = this->nondisp_ptr_vec[j];
            
            double production_kW = this->_getRenewableProductionkW(
                nondisp_ptr,
                i
            );
            
            nondisp_ptr->commitProductionkW(production_kW, i);
            
            // compute and record renewable dispatch
            double dispatch_kW = nondisp_ptr->getDispatchkW(
                load_kW,
                production_kW
            );
            
            nondisp_ptr->dispatch_vec_kW[i] = dispatch_kW;
            
            // compute and record renewable curtailment
            double curtailment_kW = production_kW - dispatch_kW;
            nondisp_ptr->curtailment_vec_kW[i] = curtailment_kW;
            
            // update load and net load
            load_kW -= dispatch_kW;
            net_load_kW -= production_kW;
        }
        
        this->net_load_vec_kW[i] = net_load_kW;
    }
    
    return;
}


#include "LoadFollowingInOrder.cpp"


void Model :: _handleDispatch() {
    /*
     *  Helper method to handle dispatch under choice of dispatch mode
     */
    
    switch (this->struct_model.dispatch_mode) {
        case (LOAD_FOLLOWING_IN_ORDER): {
            for (int i = 0; i < this->n_timesteps; i++) {
                if (this->net_load_vec_kW[i] <= 0) {
                    this->_dispatchLoadFollowingInOrderCharging(i);
                }
                
                else {
                    this->_dispatchLoadFollowingInOrderDischarging(i);
                }
                
                /*
                for (size_t j = 0; j < this->storage_ptr_vec.size(); j++) {
                    Storage* storage_ptr = this->storage_ptr_vec[j];

                    if (
                        storage_ptr->struct_storage.charge_kWh > 
                        storage_ptr->struct_storage.min_charge_kWh
                    ) {
                        std::cout << "Net " << this->net_load_vec_kW[i]
                            << " kW  C " << storage_ptr->charging_vec_kW[i]
                            << " kW  D " << storage_ptr->discharging_vec_kW[i]
                            << " kW  Q " << storage_ptr->charge_vec_kWh[i]
                            << " kWh" << std::endl << std::endl;
                    }
                    
                    std::cout << ((LiIon*)storage_ptr)->struct_liion.SOH
                        << std::endl;
                }
                */
            }
            
            break;
        }
        
        case (LOAD_FOLLOWING_SMART_COMBUSTION): {
            //...
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    return;
}


void Model :: _computeFuelEmissions(void) {
    /*
     *  Helper method to compute fuel consumption and emissions totals
     *  of Model run
     */
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        Combustion* combustion_ptr = this->combustion_ptr_vec[i];
    
        this->total_fuel_consumed_L +=
            combustion_ptr->total_fuel_consumed_L;
        
        this->total_CO2_emitted_kg +=
            combustion_ptr->total_CO2_emitted_kg;
        
        this->total_CO_emitted_kg +=
            combustion_ptr->total_CO_emitted_kg;
        
        this->total_NOx_emitted_kg +=
            combustion_ptr->total_NOx_emitted_kg;
        
        this->total_SOx_emitted_kg +=
            combustion_ptr->total_SOx_emitted_kg;
        
        this->total_CH4_emitted_kg +=
            combustion_ptr->total_CH4_emitted_kg;
        
        this->total_PM_emitted_kg +=
            combustion_ptr->total_PM_emitted_kg;
    }
    
    return;
}


void Model :: _computeEconomics(void) {
    /*
     *  Helper method to compute economics of Model run
     */
    
    // compute net present cost
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        Combustion* combustion_ptr = this->combustion_ptr_vec[i];
        
        this->net_present_cost +=
            combustion_ptr->net_present_cost;
    }
    
    for (size_t i = 0; i < this->noncombustion_ptr_vec.size(); i++) {
        Dispatchable* noncombustion_ptr = this->noncombustion_ptr_vec[i];
        
        this->net_present_cost +=
            noncombustion_ptr->net_present_cost;
    }
    
    for (size_t i = 0; i < this->nondisp_ptr_vec.size(); i++) {
        Nondispatchable* nondisp_ptr = this->nondisp_ptr_vec[i];
        
        this->net_present_cost +=
            nondisp_ptr->net_present_cost;
    }
    
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        Storage* storage_ptr = this->storage_ptr_vec[i];
        
        this->net_present_cost +=
            storage_ptr->struct_storage.net_present_cost;
    }
    
    
    // compute levellized cost of energy (for each component)
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        Combustion* combustion_ptr = this->combustion_ptr_vec[i];
        
        combustion_ptr->computeLevellizedCostOfEnergy();
    }
    
    for (size_t i = 0; i < this->noncombustion_ptr_vec.size(); i++) {
        Dispatchable* noncombustion_ptr = this->noncombustion_ptr_vec[i];
        
        noncombustion_ptr->computeLevellizedCostOfEnergy();
    }
    
    for (size_t i = 0; i < this->nondisp_ptr_vec.size(); i++) {
        Nondispatchable* nondisp_ptr = this->nondisp_ptr_vec[i];
        
        nondisp_ptr->computeLevellizedCostOfEnergy();
    }
    
    // compute levellized cost of energy (for entire system)
    /*
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/levelized_cost_of_energy.html
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/total_annualized_cost.html
     *  ref: https://www.homerenergy.com/products/pro/docs/3.12/capital_recovery_factor.html
     */
    double total_load_served_kWh = 0;
    
    for (size_t i = 0; i < this->load_vec_kW.size(); i++) {
        total_load_served_kWh += (
            this->load_vec_kW[i] - 
            this->remaining_load_vec_kW[i]
        ) * this->dt_vec_hr[i];
    }
    
    if (total_load_served_kWh <= 0) {
        return;
    }
    this->total_load_served_kWh = total_load_served_kWh;
    
    double capital_recovery_factor = 
        (
            this->real_discount_rate_annual *
            pow(
                1 + this->real_discount_rate_annual,
                this->project_life_yrs
            )
        ) / 
        (
            pow(
                1 + this->real_discount_rate_annual,
                this->project_life_yrs
            ) -
            1
        );
        
    double total_annualized_cost = capital_recovery_factor *
        this->net_present_cost;
    
    this->levellized_cost_of_energy_per_kWh =
        total_annualized_cost / total_load_served_kWh;
    
    return;
}


void Model :: _writeDispatchResults(std::string _write_path) {
    /*
     *  Helper method to write Model-level dispatch results
     */
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + "Model/Model_dispatch_results.csv");
    
    // write file header
    ofs << "Time [hrs]," <<
        "Load (at beginning of time step a.k.a. demand) [kW],";
    
    for (size_t i = 0; i < this->nondisp_ptr_vec.size(); i++) {
        Nondispatchable* nondisp_ptr = this->nondisp_ptr_vec[i];
        
        std::string type_str =
            nondisp_ptr->nondisp_type_str;
            
        double cap_kW = nondisp_ptr->struct_nondisp.cap_kW;
        
        ofs << cap_kW << " kW " << type_str
            << " Dispatch (over time step) [kW],";
    }
    
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        Storage* storage_ptr = this->storage_ptr_vec[i];
        
        std::string type_str =
            storage_ptr->struct_storage.storage_type_str;
            
        double cap_kW = storage_ptr->struct_storage.cap_kW;
        double cap_kWh = 0;
        
        switch (storage_ptr->struct_storage.storage_type) {
            case (LIION): {
                cap_kWh =
                    ((LiIon*)storage_ptr)->struct_liion.init_cap_kWh;
                    
                break;
            }
            
            default: {
                cap_kWh = storage_ptr->struct_storage.cap_kWh;
                
                break;
            }
        }
        
        ofs << "(" << cap_kW << " kW " << cap_kWh << " kWh) " <<
            type_str << " Discharge (over time step) [kW],";
    }
    
    for (size_t i = 0; i < this->noncombustion_ptr_vec.size(); i++) {
        Dispatchable* noncombustion_ptr = this->noncombustion_ptr_vec[i];
        
        std::string type_str =
            noncombustion_ptr->disp_type_str;
            
        double cap_kW = noncombustion_ptr->struct_disp.cap_kW;
        
        ofs << cap_kW << " kW " << type_str
            << " Dispatch (over time step) [kW],";
    }
    
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        Combustion* combustion_ptr = this->combustion_ptr_vec[i];
        
        std::string type_str =
            combustion_ptr->disp_type_str;
            
        double cap_kW = combustion_ptr->struct_disp.cap_kW;
        
        ofs << cap_kW << " kW " << type_str
            << " Dispatch (over time step) [kW],";
    }
    
    ofs << "\n";
    
    // write file body
    for (int i = 0; i < this->n_timesteps; i++) {
        ofs << std::to_string(this->time_vec_hr[i]) << ","
            << std::to_string(this->load_vec_kW[i]) << ",";
        
        for (size_t j = 0; j < this->nondisp_ptr_vec.size(); j++) {
            Nondispatchable* nondisp_ptr = this->nondisp_ptr_vec[j];
            
            double dispatch_kW = nondisp_ptr->dispatch_vec_kW[i];
            
            ofs << std::to_string(dispatch_kW) << ",";
        }
        
        for (size_t j = 0; j < this->storage_ptr_vec.size(); j++) {
            Storage* storage_ptr = this->storage_ptr_vec[j];
            
            double discharging_kW = storage_ptr->discharging_vec_kW[i];
            
            ofs << std::to_string(discharging_kW) << ",";
        }
        
        for (size_t j = 0; j < this->noncombustion_ptr_vec.size(); j++) {
            Dispatchable* noncombustion_ptr =
                this->noncombustion_ptr_vec[j];
            
            double dispatch_kW = noncombustion_ptr->dispatch_vec_kW[i];
            
            ofs << std::to_string(dispatch_kW) << ",";
        }
        
        for (size_t j = 0; j < this->combustion_ptr_vec.size(); j++) {
            Combustion* combustion_ptr =
                this->combustion_ptr_vec[j];
            
            double dispatch_kW = combustion_ptr->dispatch_vec_kW[i];
            
            ofs << std::to_string(dispatch_kW) << ",";
        }
        
        ofs << "\n";
    }
    
    ofs.close();
    
    return;
}


void Model :: _writeLoadResults(std::string _write_path) {
    /*
     *  Helper method to write Model-level load results
     */
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + "Model/Model_load_results.csv");
    
    // write file header
    ofs << "Time [hrs]," <<
        "Load (at beginning of time step a.k.a. demand) [kW]," <<
        "Net Load (load minus all renewable production over time step) [kW]," <<
        "Load Remaining (at end of time step after all dispatch) [kW]\n";
    
    // write file body
    for (int i = 0; i < this->n_timesteps; i++) {
        double time_hrs = this->time_vec_hr[i];
        double load_kW = this->load_vec_kW[i];
        double net_load_kW = this->net_load_vec_kW[i];
        double remaining_load_kW = this->remaining_load_vec_kW[i];
        
        ofs << std::to_string(time_hrs) << ","
            << std::to_string(load_kW) << ","
            << std::to_string(net_load_kW) << ","
            << std::to_string(remaining_load_kW) << "\n";
    }
    
    ofs.close();
    
    return;
}


void Model :: _writeSummary(std::string _write_path) {
    /*
     *  Helper method to write Model-level summary
     */
    
    // init output file stream
    std::ofstream ofs;
    ofs.open(_write_path + "Model/Model_summary.txt");
    
    // write attributes
    ofs << "Model Summary\n\n";
    ofs << "Attributes:\n\n";
    
    ofs << "\tpath to load data: " <<
        this->struct_model.path_2_load_data << "\n";
    ofs << "\ttimesteps: " << this->n_timesteps << "\n";
    ofs << "\tproject life: " << this->project_life_yrs
        << " yrs\n";
    ofs << "\treal discount rate (annual): " <<
        this->real_discount_rate_annual << "\n";
    ofs << "\tdispatch mode: " << this->struct_model.dispatch_mode
        << " ";
    
    switch (this->struct_model.dispatch_mode) {
        case (LOAD_FOLLOWING_IN_ORDER): {
            ofs << "(LOAD_FOLLOWING_IN_ORDER)\n";
            
            break;
        }
        
        case (LOAD_FOLLOWING_SMART_COMBUSTION): {
            ofs << "(LOAD_FOLLOWING_SMART_COMBUSTION)\n";
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    // write resource map
    ofs << "\nResource Mapping:\n\n";
    
    ofs << "1D Resources:\n\n";
    if (not this->resource_path_map_1D.empty()) {
        std::map<int, std::string>::iterator itr;
        
        for (
            itr = this->resource_path_map_1D.begin();
            itr != this->resource_path_map_1D.end();
            itr++
        ) {
            ofs << "\tresource key: " << itr->first << "  path: "
                << itr->second << std::endl;
        }
    }
    
    ofs << "\n2D Resources:\n\n";
    if (not this->resource_path_map_2D.empty()) {
        std::map<int, std::string>::iterator itr;
        
        for (
            itr = this->resource_path_map_2D.begin();
            itr != this->resource_path_map_2D.end();
            itr++
        ) {
            ofs << "\tresource key: " << itr->first << "  path: "
                << itr->second << std::endl;
        }
    }
    
    // write results
    ofs << "\nResults:\n\n";
    
    ofs << "\ttotal load served (over project life): " <<
        this->total_load_served_kWh << " kWh\n";
    ofs << "\ttotal fuel consumed (over project life): " <<
        this->total_fuel_consumed_L << " L\n";
    ofs << "\ttotal CO2 emissions (over project life): " <<
        this->total_CO2_emitted_kg << " kg\n";
    ofs << "\ttotal CO emissions (over project life): " <<
        this->total_CO_emitted_kg << " kg\n";
    ofs << "\ttotal NOx emissions (over project life): " <<
        this->total_NOx_emitted_kg << " kg\n";
    ofs << "\ttotal SOx emissions (over project life): " <<
        this->total_SOx_emitted_kg << " kg\n";
    ofs << "\ttotal CH4 emissions (over project life): " <<
        this->total_CH4_emitted_kg << " kg\n";
    ofs << "\ttotal particulate matter emissions (over project life): " <<
        this->total_PM_emitted_kg << " kg\n";
    ofs << "\tnet present cost: " <<
        this->net_present_cost << "\n";
    ofs << "\tlevellized cost of energy (per kWh served): " <<
        this->levellized_cost_of_energy_per_kWh << "\n";
    
    ofs.close();
    
    return;
}


// -------- USER INTERFACE METHODS -------- //


Model :: Model(structModel struct_model) {
    /*
     *  Model class constructor
     */
    
    this->struct_model = struct_model;
    
    // read in load data, set n_timesteps attribute, compute and set
    // project_life_yrs
    this->_readInLoadData();
    this->n_timesteps = this->time_vec_hr.size();
    this->project_life_yrs =
        this->time_vec_hr[this->time_vec_hr.size() - 1] / 8760;
    
    // populate dt_vec_hrs
    this->_populateDeltaVecHr();
    
    // size net and remaining load vectors
    this->net_load_vec_kW.resize(this->n_timesteps, 0);
    this->remaining_load_vec_kW.resize(this->n_timesteps, 0);
    
    if (this->struct_model.test_flag) {
        std::cout << "Model object constructed at " << this <<
            std::endl;
    }
    
    /*  compute real discount rate
     *
     *  ref: https://www.homerenergy.com/products/pro/docs/3.11/real_discount_rate.html
     */
    this->real_discount_rate_annual =
        (
            this->struct_model.nominal_discount_rate_annual - 
            this->struct_model.nominal_inflation_rate_annual
        ) / 
        (1 + this->struct_model.nominal_inflation_rate_annual);

    
    return;
}


void Model :: add1dRenewableResource(
    std::string type_str,
    std::string path_2_resource_data,
    int map_key
) {
    /*
     *  Method to register 1d renewable resource with the Model
     */
    
    std::string _type_str = stringToUpper(type_str);
    
    // handle by renewable type
    if (_type_str.compare("HYDRO") == 0) {
        std::vector<std::string> column_header_vec = {
            "Time (since start of data) [hr]",
            "Hydro Flow (into reservoir) [m3/hr]"
        };
        
        if (this->struct_model.print_flag) {
            std::cout << "Reading Hydro resource data ... ";
        }
    
        this->_readIn1dRenewableResource(
            path_2_resource_data,
            column_header_vec,
            map_key
        );
    }
    
    else if (_type_str.compare("SOLAR") == 0) {
        std::vector<std::string> column_header_vec = {
            "Time (since start of data) [hr]",
            "Solar GHI [kW/m2]"
        };
        
        if (this->struct_model.print_flag) {
            std::cout << "Reading Solar resource data ... ";
        }
    
        this->_readIn1dRenewableResource(
            path_2_resource_data,
            column_header_vec,
            map_key
        );
    }
    
    else if (_type_str.compare("TIDAL") == 0) {
        std::vector<std::string> column_header_vec = {
            "Time (since start of data) [hr]",
            "Tidal Speed (hub depth) [m/s]"
        };
        
        if (this->struct_model.print_flag) {
            std::cout << "Reading Tidal resource data ... ";
        }
    
        this->_readIn1dRenewableResource(
            path_2_resource_data,
            column_header_vec,
            map_key
        );
    }
    
    else if (_type_str.compare("WIND") == 0) {
        std::vector<std::string> column_header_vec = {
            "Time (since start of data) [hr]",
            "Wind Speed (hub height) [m/s]"
        };
        
        if (this->struct_model.print_flag) {
            std::cout << "Reading Wind resource data ... ";
        }
    
        this->_readIn1dRenewableResource(
            path_2_resource_data,
            column_header_vec,
            map_key
        );
    }
    
    //...
    
    else {
        std::string error_str = "\nERROR  Model::add1dRenewableResource()";
        error_str += "  resource type ";
        error_str += type_str;
        error_str += " not a recognized 1d renewable resource.";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        this->clearAssets();
        throw std::runtime_error(error_str);
    }
    
    if (this->struct_model.print_flag) {
        std::cout << "DONE" << std::endl;
    }
    
    return;
}


void Model :: add2dRenewableResource(
    std::string type_str,
    std::string path_2_resource_data,
    int map_key
) {
    /*
     *  Method to register 2d renewable resource with the Model
     */
    
    std::string _type_str = stringToUpper(type_str);
    
    // handle by renewable type
    if (_type_str.compare("WAVE") == 0) {
        std::vector<std::string> column_header_vec = {
            "Time (since start of data) [hr]",
            "Sig Wave Height [m]", 
            "Energy Period [s]"
        };
        
        if (this->struct_model.print_flag) {
            std::cout << "Reading Wave resource data ... ";
        }
    
        this->_readIn2dRenewableResource(
            path_2_resource_data,
            column_header_vec,
            map_key
        );
    }
    
    //...
    
    else {
        std::string error_str = "\nERROR  Model::add2dRenewableResource()";
        error_str += "  resource type ";
        error_str += type_str;
        error_str += " not a recognized 2d renewable resource.";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif
        
        this->clearAssets();
        throw std::runtime_error(error_str);
    }
    
    if (this->struct_model.print_flag) {
        std::cout << "DONE" << std::endl;
    }
    
    return;
}

void Model :: addSolar(
    structNondispatchable struct_nondisp,
    structSolar struct_solar
) {
    /*
     *  Method to add Nondispatchable <-- Solar asset to the Model
     */
    
    Nondispatchable* nondisp_ptr = new Solar(
        struct_nondisp,
        struct_solar,
        this->n_timesteps
    );
    
    this->_addNondispatchable(nondisp_ptr);
    
    return;
}


void Model :: addTidal(
    structNondispatchable struct_nondisp,
    structTidal struct_tidal
) {
    /*
     *  Method to add Nondispatchable <-- Tidal asset to the Model
     */
    
    Nondispatchable* nondisp_ptr = new Tidal(
        struct_nondisp,
        struct_tidal,
        this->n_timesteps
    );
    
    this->_addNondispatchable(nondisp_ptr);
    
    return;
}


void Model :: addWave(
    structNondispatchable struct_nondisp,
    structWave struct_wave
) {
    /*
     *  Method to add Nondispatchable <-- Wave asset to the Model
     */
    
    Nondispatchable* nondisp_ptr = new Wave(
        struct_nondisp,
        struct_wave,
        this->n_timesteps
    );
    
    this->_addNondispatchable(nondisp_ptr);
    
    return;
}


void Model :: addWind(
    structNondispatchable struct_nondisp,
    structWind struct_wind
) {
    /*
     *  Method to add Nondispatchable <-- Wind asset to the Model
     */
    
    Nondispatchable* nondisp_ptr = new Wind(
        struct_nondisp,
        struct_wind,
        this->n_timesteps
    );
    
    this->_addNondispatchable(nondisp_ptr);
    
    return;
}


void Model :: addDiesel(
    structDispatchable struct_disp,
    structCombustion struct_combustion,
    structDiesel struct_diesel
) {
    /*
     *  Method to add Dispatchable <-- Combustion <-- Diesel asset to
     *  the Model
     */
    
    Combustion* combustion_ptr = new Diesel(
        struct_disp,
        struct_combustion,
        struct_diesel,
        this->n_timesteps
    );
    
    this->_addCombustion(combustion_ptr);
    
    return;
}


void Model :: addHydro(
    structDispatchable struct_disp,
    structHydro struct_hydro
) {
    /*
     *  Method to add Dispatchable <-- Hydro asset to the Model
     */
    Dispatchable* noncombustion_ptr = new Hydro(
        struct_disp,
        struct_hydro,
        this->n_timesteps
    );
    
    this->_addNonCombustion(noncombustion_ptr);
    
    return;
}


void Model :: addLiIon(
    structStorage struct_storage,
    structBatteryStorage struct_battery_storage,
    structLiIon struct_liion
) {
    /*
     *  Method to add Storage <-- BatteryStorage <-- LiIon asset to the
     *  Model
     */
    
    Storage* storage_ptr = new LiIon(
        struct_storage,
        struct_battery_storage,
        struct_liion
    );
    
    //storage_ptr->n_timesteps = this->n_timesteps;
    
    //...
    
    this->storage_ptr_vec.push_back(storage_ptr);
    
    return;
}


void Model :: run() {
    /*
     *  Method to run the Model
     */
    
    try {
        // generate net load vector
        this->_generateNetLoadVector();
    
        // handle dispatch control
        this->_handleDispatch();
        
        // compute fuel and emissions
        this->_computeFuelEmissions();
        
        // compute economics
        this->_computeEconomics();
        
    } 
    
    catch (...) {
        this->clearAssets();
        throw;
    }
    
    return;
}


void Model :: writeResults(std::string write_path) {
    /*
     *  Method to write modelling results to given path
     */
    
    // expand write_path 
    std::string _write_path = "data/output/";
    _write_path += write_path;
    _write_path += "/";
    
    
    // create directories (overwrite if already exists)
    if (std::filesystem::is_directory(_write_path)) {
        std::string warning_str = "WARNING:  Model::writeResults():  ";
        warning_str += _write_path;
        warning_str += " already exists, contents will be overwritten!";
        
        std::cout << warning_str << std::endl;
        
        std::filesystem::remove_all(_write_path);
    }
    
    std::filesystem::create_directory(_write_path);
    
    
    // write Model-level results
    std::filesystem::create_directory(_write_path + "Model/");
    
    this->_writeDispatchResults(_write_path);
    this->_writeLoadResults(_write_path);
    this->_writeSummary(_write_path);
    
    
    // write Combustion-level results
    if (not this->combustion_ptr_vec.empty()) {
        std::filesystem::create_directory(_write_path + "Combustion/");
        
        for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
            Combustion* combustion_ptr = this->combustion_ptr_vec[i];
            
            std::filesystem::create_directory(
                _write_path + "Combustion/" +
                std::to_string(int(combustion_ptr->struct_disp.cap_kW)) +
                "kW_" + combustion_ptr->disp_type_str +
                "_" + std::to_string(i) + "/"
            );
            
            combustion_ptr->writeResults(_write_path, i);
        }
        
    }
    
    
    // write non-Combustion-level results
    if (not this->noncombustion_ptr_vec.empty()) {
        std::filesystem::create_directory(_write_path + "non-Combustion/");
        
        for (size_t i = 0; i < this->noncombustion_ptr_vec.size(); i++) {
            Dispatchable* noncombustion_ptr = this->noncombustion_ptr_vec[i];
            
            std::filesystem::create_directory(
                _write_path + "non-Combustion/" +
                std::to_string(int(noncombustion_ptr->struct_disp.cap_kW)) +
                "kW_" + noncombustion_ptr->disp_type_str +
                "_" + std::to_string(i) + "/"
            );
            
            noncombustion_ptr->writeResults(_write_path, i);
        }
    }
    
    
    // write Nondispatchable-level results
    if (not this->nondisp_ptr_vec.empty()) {
        std::filesystem::create_directory(_write_path + "Nondispatchable/");
        
        for (size_t i = 0; i < this->nondisp_ptr_vec.size(); i++) {
            Nondispatchable* nondisp_ptr = this->nondisp_ptr_vec[i];
            
            std::filesystem::create_directory(
                _write_path + "Nondispatchable/" +
                std::to_string(int(nondisp_ptr->struct_nondisp.cap_kW)) +
                "kW_" + nondisp_ptr->nondisp_type_str +
                "_" + std::to_string(i) + "/"
            );
            
            nondisp_ptr->writeResults(_write_path, i);
        }
    }
    
    
    // write Storage-level results
    if (not this->storage_ptr_vec.empty()) {
        std::filesystem::create_directory(_write_path + "Storage/");
        
        for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
            Storage* storage_ptr = this->storage_ptr_vec[i];
            
            std::string file_path = _write_path + "Storage/" +
                std::to_string(int(storage_ptr->struct_storage.cap_kW)) +
                "kW_";
            
            switch (storage_ptr->struct_storage.storage_type) {
                case (LIION): {
                    file_path +=
                        std::to_string(
                            int(
                                ((LiIon*)storage_ptr)->struct_liion.init_cap_kWh
                            )
                        ) +
                        "kWh_";
                    
                    break;
                }
                
                default: {
                    file_path +=
                        std::to_string(
                            int(storage_ptr->struct_storage.cap_kWh)
                        ) +
                        "kWh_";
                    
                    break;
                }
            }
            
            file_path += storage_ptr->struct_storage.storage_type_str +
                "_" + std::to_string(i) + "/";
            
            std::filesystem::create_directory(file_path);
            
            storage_ptr->writeResults(
                _write_path, &(this->time_vec_hr), i
            );
        }
    }
    
    return;
}


void Model :: clearAssets() {
    /*
     *  Method to clear pointer vector attributes (i.e., assets)
     */
    
    // clear renewable assets
    for (size_t i = 0; i < this->nondisp_ptr_vec.size(); i++) {
        delete this->nondisp_ptr_vec[i];
    }
    this->nondisp_ptr_vec.clear();
    
    // clear combustion dispatchable assets
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        delete this->combustion_ptr_vec[i];
    }
    this->combustion_ptr_vec.clear();
    
    // clear noncombustion dispatchable assets
    for (size_t i = 0; i < this->noncombustion_ptr_vec.size(); i++) {
        delete this->noncombustion_ptr_vec[i];
    }
    this->noncombustion_ptr_vec.clear();
    
    // clear storage assets
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        delete this->storage_ptr_vec[i];
    }
    this->storage_ptr_vec.clear();
    
    return;
}


void Model :: reset() {
    /*
     *  Method to reset Model, to be used to change the modelling 
     *  parameters prior to calling run() again
     */
    
    this->clearAssets();
    
    // clear and size net and remaining load vectors
    this->net_load_vec_kW.clear();
    this->remaining_load_vec_kW.clear();
    
    this->net_load_vec_kW.resize(this->n_timesteps, 0);
    this->remaining_load_vec_kW.resize(this->n_timesteps, 0);
    
    return;
}


Model :: ~Model() {
    /*
     *  Model class destructor
     */
    
    this->clearAssets();
    
    if (this->struct_model.test_flag) {
        std::cout << "Model object at " << this << " destroyed" <<
            std::endl;
    }
    
    return;
}
