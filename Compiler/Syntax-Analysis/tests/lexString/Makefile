OUT_DIR := ./bin
OBJECTS = $(OUT_DIR)/test_lexString.o	$(OUT_DIR)/lexString.o

VPATH = ../../

COMPILE = gcc -g -c -std=c99
LINK := gcc -g

ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
COVERAGE_REPORT_FOLDER := ./coverage
COVERAGE_FILE_FOLDER := ../../../Coverage-Report

.RECIPEPREFIX := >


$(OUT_DIR)/all.out: $(OUT_DIR) $(OBJECTS)
> $(LINK) $(ADDITIONAL_LIB) -coverage -lcunit -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
> mkdir -p $@ 

$(OUT_DIR)/lexString.o : lexString.c lexString.h
> $(COMPILE) $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) -o $@ $<

$(OUT_DIR)/%.o: %.c
> $(COMPILE) $(ADDITIONAL_INCLUDE) -o $@ $<


.PHONY: clean run show-coverage retest
clean:
> rm -f $(OUT_DIR)/test_lexString.out $(OBJECTS)

run:
> $(OUT_DIR)/all.out

retest:
> make clean
> make

show-coverage:
> lcov --directory . --zerocounters
> $(OUT_DIR)/all.out
> lcov --directory . --capture --output-file $(COVERAGE_FILE_FOLDER)/coverage-lexString.info
> genhtml -f $(COVERAGE_FILE_FOLDER)/coverage-lexString.info --output-directory $(COVERAGE_REPORT_FOLDER)
> firefox $(COVERAGE_REPORT_FOLDER)/index.html &
