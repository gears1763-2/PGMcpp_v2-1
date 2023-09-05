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
    this->dt_vec_hr.resize(this->struct_model.n_timesteps, 0);
    
    // populate dt_vec_hr
    for (int i = 0; i < this->struct_model.n_timesteps; i++) {
        double dt_hr = 0;
        
        // handle running off end of time_vec_hr
        if (i == this->struct_model.n_timesteps - 1) {
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
        (size_t)(this->struct_model.n_timesteps)
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
        (size_t)(this->struct_model.n_timesteps)
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
    
    switch (nondisp_ptr->struct_nondisp.nondisp_type) {
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
    
    for (int i = 0; i < this->struct_model.n_timesteps; i++) {
        double dt_hrs = this->dt_vec_hr[i];
        double load_kW = this->load_vec_kW[i];
        double net_load_kW = this->load_vec_kW[i];
        
        for (size_t j = 0; j < this->nondisp_ptr_vec.size(); j++) {
            // compute and commit renewable production
            Nondispatchable* nondisp_ptr = this->nondisp_ptr_vec[j];
            
            double production_kW = this->_getRenewableProductionkW(
                nondisp_ptr,
                i
            );
            
            nondisp_ptr->commitProductionkW(
                production_kW, dt_hrs, i
            );
            
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
            for (int i = 0; i < this->struct_model.n_timesteps; i++) {
                if (this->net_load_vec_kW[i] <= 0) {
                    this->_dispatchLoadFollowingInOrderCharging(i);
                }
                
                else {
                    this->_dispatchLoadFollowingInOrderDischarging(i);
                }
                
                //***
                for (size_t j = 0; j < this->storage_ptr_vec.size(); j++) {
                    Storage* storage_ptr = this->storage_ptr_vec[j];
                    
                    continue;
                    
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
                //***
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


// -------- USER INTERFACE METHODS -------- //


Model :: Model(structModel struct_model) {
    /*
     *  Model class constructor
     */
    
    this->struct_model = struct_model;
    
    // read in load data, set n_timesteps attribute
    this->_readInLoadData();
    this->struct_model.n_timesteps = this->time_vec_hr.size();
    
    // populate dt_vec_hrs
    this->_populateDeltaVecHr();
    
    // size net_load_vec_kW
    this->net_load_vec_kW.resize(this->struct_model.n_timesteps, 0);
    this->remaining_load_vec_kW.resize(this->struct_model.n_timesteps, 0);
    
    if (this->struct_model.test_flag) {
        std::cout << "Model object constructed at " << this <<
            std::endl;
    }
    
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
    
    std::string _type_str = to_upper(type_str);
    
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
    
    std::string _type_str = to_upper(type_str);
    
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
     *  Method to add Solar asset to the Model
     */
    
    struct_nondisp.nondisp_type = SOLAR;
    struct_nondisp.n_timesteps = this->struct_model.n_timesteps;
    
    Nondispatchable* nondisp_ptr = new Solar(
        struct_nondisp,
        struct_solar
    );
    
    this->nondisp_ptr_vec.push_back(nondisp_ptr);
    
    return;
}


void Model :: addTidal(
    structNondispatchable struct_nondisp,
    structTidal struct_tidal
) {
    /*
     *  Method to add Tidal asset to the Model
     */
    
    struct_nondisp.nondisp_type = TIDAL;
    struct_nondisp.n_timesteps = this->struct_model.n_timesteps;
    
    Nondispatchable* nondisp_ptr = new Tidal(
        struct_nondisp,
        struct_tidal
    );
    
    this->nondisp_ptr_vec.push_back(nondisp_ptr);
    
    return;
}


void Model :: addWave(
    structNondispatchable struct_nondisp,
    structWave struct_wave
) {
    /*
     *  Method to add Wave asset to the Model
     */
    
    struct_nondisp.nondisp_type = WAVE;
    struct_nondisp.n_timesteps = this->struct_model.n_timesteps;
    
    Nondispatchable* nondisp_ptr = new Wave(
        struct_nondisp,
        struct_wave
    );
    
    this->nondisp_ptr_vec.push_back(nondisp_ptr);
    
    return;
}


void Model :: addWind(
    structNondispatchable struct_nondisp,
    structWind struct_wind
) {
    /*
     *  Method to add Wind asset to the Model
     */
    
    struct_nondisp.nondisp_type = WIND;
    struct_nondisp.n_timesteps = this->struct_model.n_timesteps;
    
    Nondispatchable* nondisp_ptr = new Wind(
        struct_nondisp,
        struct_wind
    );
    
    this->nondisp_ptr_vec.push_back(nondisp_ptr);
    
    return;
}


void Model :: addDiesel(
    structDispatchable struct_disp,
    structCombustion struct_combustion,
    structDiesel struct_diesel
) {
    /*
     *  Method to add Diesel asset to the Model
     */
    
    struct_disp.disp_type = DIESEL;
    struct_disp.n_timesteps = this->struct_model.n_timesteps;
    
    Combustion* combustion_ptr = new Diesel(
        struct_disp,
        struct_combustion,
        struct_diesel
    );
    
    this->combustion_ptr_vec.push_back(combustion_ptr);
    
    return;
}


void Model :: addHydro(
    structDispatchable struct_disp,
    structHydro struct_hydro
) {
    /*
     *  Method to add Hydro asset to the Model
     */
    
    struct_disp.disp_type = HYDRO;
    struct_disp.n_timesteps = this->struct_model.n_timesteps;
    
    Dispatchable* disp_ptr = new Hydro(
        struct_disp,
        struct_hydro
    );
    
    this->noncombustion_ptr_vec.push_back(disp_ptr);
    
    return;
}


void Model :: addLiIon(
    structStorage struct_storage,
    structBatteryStorage struct_battery_storage,
    structLiIon struct_liion
) {
    /*
     *  Method to add LiIon asset to the Model
     */
    
    struct_storage.storage_type = STORAGE_LIION;
    struct_storage.n_timesteps = this->struct_model.n_timesteps;
    
    Storage* storage_ptr = new LiIon(
        struct_storage,
        struct_battery_storage,
        struct_liion
    );
    
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
        
        // compute economics
        //...
        
    } 
    
    catch (...) {
        this->clearAssets();
        throw;
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
