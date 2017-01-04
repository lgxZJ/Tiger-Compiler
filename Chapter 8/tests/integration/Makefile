OUT_DIR := ./bin
OBJECTS := $(addprefix $(OUT_DIR)/, tests.o myCanonical.o)

COMPILE_FLAGS = g++ -g -c -std=c++11
LINK_FLAGS = g++ -g -static

VPATH := ../../
.RECIPEPREFIX := >

tests.out: $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
> $(LINK_FLAGS) -o $(OUT_DIR)/all.out $(OBJECTS) ../Libraries/frontend.a -lcppunit

$(OUT_DIR):
> mkdir -p $(OUT_DIR)

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