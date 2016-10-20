OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_mySemantic.o	$(OUT_DIR)/mySemantic.o\
	$(OUT_DIR)/myEnvironment.o	$(OUT_DIR)/mySymbol.o\
	$(OUT_DIR)/typeMaker.o		$(OUT_DIR)/makeMemory.o\
	$(OUT_DIR)/myTable.o		$(OUT_DIR)/abstractSyntaxMaker.o\
	$(OUT_DIR)/myError.o		$(OUT_DIR)/typeChecker.o\
	$(OUT_DIR)/testHelper.o		$(OUT_DIR)/recursiveDecs.o\
	$(OUT_DIR)/testFixture.o	$(OUT_DIR)/breakChecker.o

COMPILE_FLAGS := gcc -g -c -std=c99
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include

VPATH := ../../:../

all : $(OUT_DIR) $(OBJECTS)
	gcc -g $(ADDITIONAL_LIB) -lcunit -coverage -o  $(OUT_DIR)/tests.out $(OBJECTS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# make rules
$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -o $@ $<
$(OUT_DIR)/mySemantic.o : mySemantic.c
	$(COMPILE_FLAGS) -fprofile-arcs -ftest-coverage $(ADDITIONAL_INCLUDE) -o $@ $<

.PHONY: clean run show-coverage rebuild
clean:
	rm $(OBJECTS) $(OUT_DIR)/tests.out

run:
	$(OUT_DIR)/tests.out

show-coverage:
	lcov --directory . --zerocounters
	$(OUT_DIR)/tests.out
	lcov --directory . --capture --output-file code-coverage.info
	genhtml -f code-coverage.info
	firefox index.html &

retest:
	make clean
	make
	$(OUT_DIR)/tests.out
