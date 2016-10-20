OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_typeChecker.o	$(OUT_DIR)/typeChecker.o\
	$(OUT_DIR)/typeMaker.o			$(OUT_DIR)/makeMemory.o\
	$(OUT_DIR)/mySymbol.o			$(OUT_DIR)/testHelper.o\
	$(OUT_DIR)/myTable.o

VPATH := ../../:../

COMPILE_FLAGS := gcc -g -c -std=c99
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib

all.out : $(OUT_DIR) $(OBJECTS)
	gcc $(ADDITIONAL_LIB) $(ADDITIONAL_INCLUDE) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)


$(OUT_DIR)/typeChecker.o : typeChecker.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -fprofile-arcs -ftest-coverage -o $@ $<

$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -o $@ $<

.PHONY : clean run retest show-coverage
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
	lcov --directory . --capture --output-file coverage.info
	genhtml coverage.info
	firefox index.html &
