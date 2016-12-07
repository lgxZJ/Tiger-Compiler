OUT_DIR := ./bin
OBJECTS := $(addprefix $(OUT_DIR)/, tests.o myCanonical.o)

COMPILE_FLAGS = g++ -g -c -std=c++11
LINK_FLAGS = g++ -g -static -coverage
COVERAGE_FOLDER := ./coverage

VPATH := ../../
.RECIPEPREFIX := >

tests.out: $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
> $(LINK_FLAGS) -o $(OUT_DIR)/all.out $(OBJECTS) ../Libraries/frontend.a -lcppunit

$(OUT_DIR):
> mkdir -p $(OUT_DIR)

$(COVERAGE_FOLDER):
> mkdir -p $@


$(OUT_DIR)/myCanonical.o : myCanonical.cpp
> $(COMPILE_FLAGS) -fprofile-arcs -ftest-coverage -o $@ $<

$(OUT_DIR)/%.o : %.cpp
> $(COMPILE_FLAGS) -o $@ $<


.PHONY: clean run retest show-coverage
clean:
>rm -f $(OBJECTS) $(OUT_DIR)/all.out

run:
> $(OUT_DIR)/all.out

retest:
> make clean
> make
> $(OUT_DIR)/all.out

show-coverage:
> lcov --directory . --zerocounters
> $(OUT_DIR)/all.out
> lcov --directory . --capture --output-file $(COVERAGE_FOLDER)/coverage.info
> genhtml -f $(COVERAGE_FOLDER)/coverage.info --output-directory $(COVERAGE_FOLDER)
> firefox $(COVERAGE_FOLDER)/index.html &
 
