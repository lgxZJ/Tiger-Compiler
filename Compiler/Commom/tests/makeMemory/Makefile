OUT_DIR := ./bin
OBJECTS :=  $(OUT_DIR)/Test_makeMemory.o 	$(OUT_DIR)/makeMemory.o

COMPILE_FLAG := gcc -g -c -std=c99
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage

COVERAGE_FILE_FOLDER := ../../../Coverage-Report
COVERAGE_REPORT_FOLDER := ./coverage

VPATH := ../../



all:  $(OUT_DIR) $(OBJECTS)
	gcc -g $(ADDITIONAL_LIB) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
	mkdir -k $@


$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAG) $(ADDITIONAL_INCLUDE) -o $@ $<

$(OUT_DIR)/makeMemory.o : makeMemory.c makeMemory.h
	$(COMPILE_FLAG) $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) -o $@ $<


.PHONY: clean show-coverage run
clean:
	rm -f $(OUT_DIR)/Test_makeMemory.out $(OBJECTS)

run:
	$(OUT_DIR)/all.out

show-coverage:
	lcov --directory . --zerocounters
	$(OUT_DIR)/all.out
	lcov --directory . --capture --output-file $(COVERAGE_FILE_FOLDER)/coverage-makeMemory.info
	genhtml -f $(COVERAGE_FILE_FOLDER)/coverage-makeMemory.info --output-directory $(COVERAGE_REPORT_FOLDER)
	firefox $(COVERAGE_REPORT_FOLDER)/index.html &
