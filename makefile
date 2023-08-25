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
CXXFLAGS = -O1 -std=c++17 -Wall
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


## ------ Model ------ ##

SRC_MODEL = source/Model.cpp
OBJ_MODEL = object/Model.o

.PHONY: Model
Model: $(SRC_MODEL)
	$(CXX) $(CXXFLAGS) -c $(SRC_MODEL) -o $(OBJ_MODEL)


## ---- ALL ---- ##

OBJ_ALL = $(OBJ_SOLAR)\
          $(OBJ_TIDAL)\
          $(OBJ_WAVE)\
          $(OBJ_WIND)\
          $(OBJ_NONDISPATCHABLE)\
          $(OBJ_MODEL)


MAKE_ALL = Solar\
           Tidal\
           Wave\
           Wind\
           Nondispatchable\
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
	mkdir -pv data/output
	make $(MAKE_ALL) test


.PHONY: run_test
run_test:
	./bin/test.out
