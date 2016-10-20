OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_myError.o	$(OUT_DIR)/myError.o\
	$(OUT_DIR)/makeMemory.o		$(OUT_DIR)/testHelper.o

COMPILE_FLAGS := gcc -g -c -std=c99
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage

VPATH := ../../:../

all.out :  $(OUT_DIR) $(OBJECTS)
	gcc $(ADDITIONAL_LIB) $(ADDITIONAL_INCLUDE) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -o $@ $<

$(OUT_DIR)/myError.o : myError.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) -o $@ $<


.PHONY : clean retest run show-coverage

clean:
	rm -f $(OBJECTS) $(OUT_DIR)/all.out

retest:
	make clean
	make
	$(OUT_DIR)/all.out

run:
	$(OUT_DIR)/all.out

show-coverage:
	lcov --directory . --zerocounters
	$(OUT_DIR)/all.out
	lcov --directory . --capture --output-file coverage.info
	genhtml -f coverage.info
	firefox index.html &
