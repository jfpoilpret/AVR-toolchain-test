# Which avr-gcc to use?
# This is the 3.5.3 or 3.5.4 toolchain: it works properly
#AVR_PATH:=~/bin
# This is the new 3.6.0 or 3.6.1 toolchain: it fails miserably
AVR_PATH:=/usr/bin

# Force target settings
TARGET=test

VARIANT:=ARDUINO_UNO
MCU:=atmega328p
ARCH:=avr5
F_CPU:=16000000UL

# Set output paths
config:=$(VARIANT)-$(subst 000000UL,MHz,$(F_CPU))
objdir:=build/$(config)
depdir:=deps/$(config)
distdir:=dist/$(config)
target:=$(distdir)/$(TARGET)

sources:=register.cpp
objects:=$(patsubst %,$(objdir)/%.o,$(basename $(sources)))
deps:=$(patsubst %,$(depdir)/%.d,$(basename $(sources)))

$(shell mkdir -p $(dir $(objects)) >/dev/null)
$(shell mkdir -p $(dir $(deps)) >/dev/null)
$(shell mkdir -p $(distdir) >/dev/null)

rm:=rm -f
ar:=ar
ranlib:=ranlib
cxx:=$(AVR_PATH)/avr-g++
nm:=$(AVR_PATH)/avr-nm
objcopy:=$(AVR_PATH)/avr-objcopy
objdump:=$(AVR_PATH)/avr-objdump
objsize:=$(AVR_PATH)/avr-size

cxxflags:=-mmcu=$(MCU) -DF_CPU=$(F_CPU) -D$(VARIANT) -DNO_ABI -fno-exceptions -Wextra -flto -std=gnu++11 -felide-constructors -Os -ffunction-sections -fdata-sections -mcall-prologues -g -Wall -std=c++11
ldflags = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -D$(VARIANT) -fno-exceptions -Wextra -flto -std=gnu++11 -felide-constructors -Os -ffunction-sections -fdata-sections -mcall-prologues -Wl,--gc-sections -Wl,--relax -Wl,-Map,$@.map
depflags = -MT $@ -MMD -MP -MF $(depdir)/$*.Td

compile.cc = $(cxx) $(depflags) $(cxxflags) -c -o $@
precompile =
postcompile = mv -f $(depdir)/$*.Td $(depdir)/$*.d
link.o = $(cxx) $(ldflags) -o $@

.PHONY: build
build: $(target)

.PHONY: clean
clean:
	$(rm) -r $(objdir) $(depdir) $(distdir)

$(objdir)/%.o: %.cpp
$(objdir)/%.o: %.cpp $(depdir)/%.d
	$(precompile)
	$(compile.cc) $(abspath $<)
	$(postcompile)

# Main target project using FastArduino
$(target): $(objects)
	$(rm) $@ $@.eep $@.nm $@.map
	$(link.o) $(abspath $^)
	$(objcopy) -R .eeprom -O ihex $@ $@.hex
	$(objcopy) -j .eeprom --change-section-lma .eeprom=0 -O ihex $@ $@.eep
	$(nm) --synthetic -S -C --size-sort $@ >$@.nm
	$(objdump) -m $(ARCH) -x -d -C $@ >$@.dump
	$(objsize) -C --mcu=$(MCU) $@

.PRECIOUS = $(depdir)/%.d
$(depdir)/%.d: ;

