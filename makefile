#
#   PGMcpp : PRIMED Grid Modelling Code (in C++) - v2.0
#
#   Anthony Truelove MASc, P.Eng.
#   email:  gears1763@tutanota.com
#   github: gears1763-2
#
#   See license terms
#


#### ---- USER INPUT ---- ####

#...


#### ---- COMPILER FLAGS ---- ####

CXX = g++

# Remove -g for release! This is for gdb symbols.
CXXFLAGS = -O1 -std=c++17 -Wall -g

DEPS = -lpthread


#### ---- BUILD ---- ####

## ------ Nondispatchable ------ ##

SRC_NONDISPATCHABLE = source/assets/nondispatchable/Nondispatchable.cpp
OBJ_NONDISPATCHABLE = object/assets/nondispatchable/Nondispatchable.o

.PHONY: Nondispatchable
Nondispatchable: $(SRC_NONDISPATCHABLE)
	$(CXX) $(CXXFLAGS) -c $(SRC_NONDISPATCHABLE) -o $(OBJ_NONDISPATCHABLE)


## ------ Solar ------ ##

SRC_SOLAR = source/assets/nondispatchable/Solar.cpp
OBJ_SOLAR = object/assets/nondispatchable/Solar.o

.PHONY: Solar
Solar: $(SRC_SOLAR)
	$(CXX) $(CXXFLAGS) -c $(SRC_SOLAR) -o $(OBJ_SOLAR)


## ------ Tidal ------ ##

SRC_TIDAL = source/assets/nondispatchable/Tidal.cpp
OBJ_TIDAL = object/assets/nondispatchable/Tidal.o

.PHONY: Tidal
Tidal: $(SRC_TIDAL)
	$(CXX) $(CXXFLAGS) -c $(SRC_TIDAL) -o $(OBJ_TIDAL)


## ------ Wave ------ ##

SRC_WAVE = source/assets/nondispatchable/Wave.cpp
OBJ_WAVE = object/assets/nondispatchable/Wave.o

.PHONY: Wave
Wave: $(SRC_WAVE)
	$(CXX) $(CXXFLAGS) -c $(SRC_WAVE) -o $(OBJ_WAVE)


## ------ Wind ------ ##

SRC_WIND = source/assets/nondispatchable/Wind.cpp
OBJ_WIND = object/assets/nondispatchable/Wind.o

.PHONY: Wind
Wind: $(SRC_WIND)
	$(CXX) $(CXXFLAGS) -c $(SRC_WIND) -o $(OBJ_WIND)


## ------ Dispatchable ------ ##

SRC_DISPATCHABLE = source/assets/dispatchable/Dispatchable.cpp
OBJ_DISPATCHABLE = object/assets/dispatchable/Dispatchable.o

.PHONY: Dispatchable
Dispatchable: $(SRC_DISPATCHABLE)
	$(CXX) $(CXXFLAGS) -c $(SRC_DISPATCHABLE) -o $(OBJ_DISPATCHABLE)


## ------ Hydro ------ ##

SRC_HYDRO = source/assets/dispatchable/Hydro.cpp
OBJ_HYDRO = object/assets/dispatchable/Hydro.o

.PHONY: Hydro
Hydro: $(SRC_HYDRO)
	$(CXX) $(CXXFLAGS) -c $(SRC_HYDRO) -o $(OBJ_HYDRO)


## ------ Combustion ------ ##

SRC_COMBUSTION = source/assets/dispatchable/combustion/Combustion.cpp
OBJ_COMBUSTION = object/assets/dispatchable/combustion/Combustion.o

.PHONY: Combustion
Combustion: $(SRC_COMBUSTION)
	$(CXX) $(CXXFLAGS) -c $(SRC_COMBUSTION) -o $(OBJ_COMBUSTION)


## ------ Diesel ------ ##

SRC_DIESEL = source/assets/dispatchable/combustion/Diesel.cpp
OBJ_DIESEL = object/assets/dispatchable/combustion/Diesel.o

.PHONY: Diesel
Diesel: $(SRC_DIESEL)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIESEL) -o $(OBJ_DIESEL)


## ------ Storage ------ ##

SRC_STORAGE = source/assets/storage/Storage.cpp
OBJ_STORAGE = object/assets/storage/Storage.o

.PHONY: Storage
Storage: $(SRC_STORAGE)
	$(CXX) $(CXXFLAGS) -c $(SRC_STORAGE) -o $(OBJ_STORAGE)


## ------ HydroStorage ------ ##

SRC_HYDROSTORAGE = source/assets/storage/HydroStorage.cpp
OBJ_HYDROSTORAGE = object/assets/storage/HydroStorage.o

.PHONY: HydroStorage
HydroStorage: $(SRC_HYDROSTORAGE)
	$(CXX) $(CXXFLAGS) -c $(SRC_HYDROSTORAGE) -o $(OBJ_HYDROSTORAGE)


## ------ BatteryStorage ------ ##

SRC_BATTERYSTORAGE = source/assets/storage/batterystorage/BatteryStorage.cpp
OBJ_BATTERYSTORAGE = object/assets/storage/batterystorage/BatteryStorage.o

.PHONY: BatteryStorage
BatteryStorage: $(SRC_BATTERYSTORAGE)
	$(CXX) $(CXXFLAGS) -c $(SRC_BATTERYSTORAGE) -o $(OBJ_BATTERYSTORAGE)


## ------ LiIon ------ ##

SRC_LIION = source/assets/storage/batterystorage/LiIon.cpp
OBJ_LIION = object/assets/storage/batterystorage/LiIon.o

.PHONY: LiIon
LiIon: $(SRC_LIION)
	$(CXX) $(CXXFLAGS) -c $(SRC_LIION) -o $(OBJ_LIION)


## ------ Model ------ ##

SRC_MODEL = source/Model.cpp
OBJ_MODEL = object/Model.o

.PHONY: Model
Model: $(SRC_MODEL)
	$(CXX) $(CXXFLAGS) -c $(SRC_MODEL) -o $(OBJ_MODEL)


## ---- ALL ---- ##

OBJ_NONDISP_HIERARCHY = $(OBJ_SOLAR)\
                        $(OBJ_TIDAL)\
                        $(OBJ_WAVE)\
                        $(OBJ_WIND)\
                        $(OBJ_NONDISPATCHABLE)

MAKE_NONDISP_HIERARCHY = Solar\
                         Tidal\
                         Wave\
                         Wind\
                         Nondispatchable


OBJ_DISP_HIERARCHY = $(OBJ_DIESEL)\
                     $(OBJ_HYDRO)\
                     $(OBJ_COMBUSTION)\
                     $(OBJ_DISPATCHABLE)

MAKE_DISP_HIERARCHY = Diesel\
                      Hydro\
                      Combustion\
                      Dispatchable


OBJ_STORAGE_HIERARCHY = $(OBJ_LIION)\
                        $(OBJ_HYDROSTORAGE)\
                        $(OBJ_BATTERYSTORAGE)\
                        $(OBJ_STORAGE)

MAKE_STORAGE_HIERARCHY = LiIon\
                         HydroStorage\
                         BatteryStorage\
                         Storage\


OBJ_ALL = $(OBJ_NONDISP_HIERARCHY)\
          $(OBJ_DISP_HIERARCHY)\
          $(OBJ_STORAGE_HIERARCHY)\
          $(OBJ_MODEL)


MAKE_ALL = $(MAKE_NONDISP_HIERARCHY)\
           $(MAKE_DISP_HIERARCHY)\
           $(MAKE_STORAGE_HIERARCHY)\
           Model


## ------ test ------ ##

SRC_TEST = test/test.cpp
OBJ_TEST = object/test.o
OUT_TEST = bin/test.out

.PHONY: test
test: $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) $(OBJ_TEST) $(OBJ_ALL) -o $(OUT_TEST) $(DEPS)

$(OBJ_TEST): $(SRC_TEST)
	$(CC) $(CFLAGS) -c $(SRC_TEST) -o $(OBJ_TEST) $(DEPS)


#### ---- TARGETS ---- ####

.PHONY: clean
clean:
	rm -frv bin;
	rm -frv core;
	rm -frv object;
	rm -frv data/output/example;


.PHONY: all
all:
	mkdir -pv bin
	mkdir -pv core
	mkdir -pv object
	mkdir -pv object/assets
	mkdir -pv object/assets/nondispatchable
	mkdir -pv object/assets/dispatchable
	mkdir -pv object/assets/dispatchable/combustion
	mkdir -pv object/assets/storage
	mkdir -pv object/assets/storage/batterystorage
	mkdir -pv data/output
	make $(MAKE_ALL) test


.PHONY: run_test
run_test:
	./bin/test.out
