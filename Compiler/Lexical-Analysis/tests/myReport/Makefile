OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_myReport.o	$(OUT_DIR)/myReport.o

VPATH := ../../

EXTRA_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
EXTRA_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include

COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
COVERAGE_REPORT_FOLDER := ./coverage
COVERAGE_FILE_FOLDER := ../../../Coverage-Report
LINK := gcc -g -std=c99
COMPILE := gcc -g -c -std=c99

all.out: $(OUT_DIR) $(OBJECTS)
	$(LINK) $(EXTRA_LIB) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
	mkdir -p $@

$(OUT_DIR)/myReport.o : myReport.c
	$(COMPILE) $(EXTRA_INCLUDE) $(COVERAGE_OPTIONS) -o $@ $<

$(OUT_DIR)/%.o: %.c
	$(COMPILE) $(EXTRA_INCLUDE) -o $@ $<


.PHONY: clean run retest show-coverage
clean:
	rm -f $(OBJECTS) $(OUT_DIR)/all.out

run:
	$(OUT_DIR)/all.out

retest:
	make clean
	make
	$(OUT_DIR)/all.out

show-coverage:
	lcov --directory . --zerocounters
	$(OUT_DIR)/all.out
	lcov --directory . --capture --output-file $(COVERAGE_FILE_FOLDER)/coverage-myReport.info
	genhtml -f $(COVERAGE_FILE_FOLDER)/coverage-myReport.info --output-directory $(COVERAGE_REPORT_FOLDER)
	firefox $(COVERAGE_REPORT_FOLDER)/index.html &
