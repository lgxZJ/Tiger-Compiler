OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_myEnvironment.o    	$(OUT_DIR)/myEnvironment.o\
	$(OUT_DIR)/mySymbol.o 			$(OUT_DIR)/makeMemory.o \
	$(OUT_DIR)/typeMaker.o 			$(OUT_DIR)/myTable.o\
	$(OUT_DIR)/testHelper.o

ADDITION_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
ADDITION_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
COMPILE_FLAGS = gcc -g -c -std=c99

VPATH := ../../:../


tests.out: $(OUT_DIR) $(OBJECTS)
	gcc -g $(ADDITION_LIB) -lcunit -coverage -o $(OUT_DIR)/tests.out $(OBJECTS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OUT_DIR)/myEnvironment.o : myEnvironment.c
	$(COMPILE_FLAGS) $(ADDITION_INCLUDE) -fprofile-arcs -ftest-coverage -o $@ $<

$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) $(ADDITION_INCLUDE) -o $@ $<


.PHONY: clean run retest show-coverage
clean:
	rm -f $(OBJECTS) $(OUT_DIR)/tests.out

run:
	$(OUT_DIR)/tests.out

retest:
	make clean
	make
	$(OUT_DIR)/tests.out

show-coverage:
	lcov --directory . --zerocounters
	$(OUT_DIR)/tests.out
	lcov --directory . --capture --output-file coverage.info
	genhtml coverage.info
	firefox index.html &
