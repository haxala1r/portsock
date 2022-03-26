
# Change this if you like. 
ifeq ($(TARGET),WINDOWS)
	COMPILER := i686-w64-mingw32-g++
else
	COMPILER := g++
endif

CPPFLAGS := -O3 -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code -static
LINFLAGS := $(CPPFLAGS) -D__LINUX
WINFLAGS := $(CPPFLAGS) -D__WINDOWS
AR := ar

main_sources := $(shell find ./src -name "*.cpp")
main_targets := $(patsubst %.cpp,%.o,$(main_sources))

test_sources := $(shell find ./test -name "*.cpp")
test_targets := $(patsubst %.cpp,%.o,$(test_sources))


ifeq ($(TARGET),WINDOWS)
	CPPFLAGS := $(WINFLAGS)
	TEST_FILE := test.exe
	OUTPUT_FILE := libtps.a
else
	CPPFLAGS := $(LINFLAGS)
	TEST_FILE := test.elf
	OUTPUT_FILE := libtps.a
endif

# Some color codes. Not really gonna use all, but it's neat to have
# anyway.
red := \e[0;31m
RED := \e[1;31m
blue := \e[0;34m
BLUE := \e[1;34m
cyan := \e[0;36m
CYAN := \e[1;36m
green := \e[0;32m
GREEN := \e[1;32m
yellow := \e[0;33m
YELLOW := \e[1;33m
PURPLE := \e[1;35m
purple := \e[0;35m
reset_color := \e[0m


.PHONY: test build clean 

# the test executable only runs if the target system is the same as the
# host system.
test: $(TEST_FILE)
	@echo "$(GREEN)Test build finished.$(reset_color)"

build: $(OUTPUT_FILE)
	@echo "$(GREEN)Build finished.$(reset_color)"

clean:
	@rm -f $(main_targets)
	@rm -f $(test_targets)
	@rm -f $(OUTPUT_FILE)
	@rm -f *.dll
	@rm -f *.a
	@rm -f *.elf
	@rm -f *.exe

$(OUTPUT_FILE): $(main_targets)
	@echo "$(CYAN)[BUILD] -> Linking everything$(PURPLE)"
ifeq ($(OUTPUT_FILE),libtps.a)
	$(AR) rc $@ $^ 
endif
ifeq ($(OUTPUT_FILE),libtps.dll)
	@# this is currently unused, I just couldn't get dlls to work.
	$(COMPILER) -shared $^ -lws2_32 -o $@
endif

$(TEST_FILE): $(test_targets) $(OUTPUT_FILE)
ifeq ($(TARGET),WINDOWS)
	@# We need to link with winsock2 as well on windows.
	$(COMPILER) $^ -lws2_32 -mconsole -static-libgcc -static-libstdc++ -o $@
else
	$(COMPILER) $^ -static-libgcc -static-libstdc++ -o $@
endif

%.o : %.cpp
	@echo "$(CYAN)[BUILD] -> Compiling $<$(PURPLE)"
	$(COMPILER) $(CPPFLAGS) -c $^ -o $@
