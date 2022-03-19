
# Change this if you like. 

ifeq ($(TARGET),WINDOWS)
	CPP := i686-w64-mingw32-g++
else
	CPP := g++
endif

CPPFLAGS := -O3 -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code -static
LINFLAGS := $(CPPFLAGS) -D__LINUX
WINFLAGS := $(CPPFLAGS) -D__WINDOWS
AR := ar

main_sources := $(shell find ./src -name "*.cpp")
main_targets := $(patsubst %.cpp,%.o,$(main_sources))

LINOUT := libtps.a
WINOUT := libtps.dll

ifeq ($(TARGET),WINDOWS)
	OUTPUT_FILE ?= libtps.a
	CPPFLAGS := $(WINFLAGS)
else
	OUTPUT_FILE ?= libtps.a
	CPPFLAGS := $(LINFLAGS)
	BUILD_CMD := $(AR) rc $(OUTPUT_FILE) $(main_targets)
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


.PHONY: build clean

build: $(OUTPUT_FILE)
	@echo "$(GREEN)Build finished.$(reset_color)"

clean:
	@rm -f $(main_targets)
	@rm -f $(OUTPUT_FILE)

$(OUTPUT_FILE): $(main_targets)
	$(AR) rc $@ $^

%.o : %.cpp
	@echo "$(CYAN)[BUILD] -> Compiling $<$(reset_color)"
	$(CPP) $(CPPFLAGS) -c $^ -o $@
