OUT_DIR := ./bin
OBJECTS := $(addprefix $(OUT_DIR)/, tests.o myDirectedGraph.o)

COMPILE_FLAGS := g++ -g -c -std=c++11
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
COVERAGE_FOLDER := ./coverage

.RECIPEPREFIX := >
VPATH := ../../

all.out : $(COVERAGE_FOLDER) $(OBJECTS)
> g++ -g -coverage -o $(OUT_DIR)/all.out $(OBJECTS) -lcppunit -ldl

$(OBJECTS) : | $(OUT_DIR)

$(OUT_DIR):
> mkdir -p $@

$(COVERAGE_FOLDER):
> mkdir -p $@


$(OUT_DIR)/myDirectedGraph.o : myDirectedGraph.cpp
> $(COMPILE_FLAGS) $(COVERAGE_OPTIONS) -o $@ $<

$(OUT_DIR)/%.o : %.cpp
> $(COMPILE_FLAGS) -o $@ $<


.PHONY : clean retest run show-coverage
clean:
> rm -f $(OBJECTS) $(OUT_DIR)/all.out *.gcda *.gcno

retest:
> make clean
> make
> $(OUT_DIR)/all.out

run:
> $(OUT_DIR)/all.out

show-coverage:
> lcov --directory . --zerocounters
> $(OUT_DIR)/all.out
> lcov --directory . --capture --output-file $(COVERAGE_FOLDER)/coverage.info
> genhtml -f $(COVERAGE_FOLDER)/coverage.info --output-directory $(COVERAGE_FOLDER)
> firefox $(COVERAGE_FOLDER)/index.html &
