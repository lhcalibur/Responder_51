CC = /Users/cooltouya/Programs/sdcc/bin/sdcc
NO_OPT = --nogcse --noinvariant --noinduction --nojtbound --noloopreverse --nolabelopt --nooverlay --peep-asm

DEBUG = --debug
STDLIB = 
#STDLIB = --nostdlib
STDINC = 
#STDINC = --nostdinc
REENTRANT = --stack-auto
PROCESSOR = -mmcs51
STD = 
#STD = --std-c99
LIBPATH = /Users/cooltouya/Programs/sdcc/share/sdcc/lib/medium


USRDEFINE += 

DRIVERS = ./drivers
INIT = ./init
CONFIGS = ./configs
MAIN = main.rel
OBJ += $(patsubst %.c,%.rel,$(notdir $(wildcard $(DRIVERS)/src/*.c)))

INCLUDES += -I$(DRIVERS)/inc -I$(CONFIGS)

VPATH += $(DRIVERS)/src $(INIT)

CFLAGS += $(STD) $(PROCESSOR) --model-medium -L$(LIBPATH) --iram-size 0x0100 $(INCLUDES) $(DEBUG) --less-pedantic --xram-size 0x0100 --xram-loc 0x80 $(STDLIB) $(STDINC) $(REENTRANT) \
	  --no-peep --int-long-reent --float-reent
PROG = light

all: main $(PROG).bin

$(PROG).bin: main.ihx
	@makebin -p main.ihx $(PROG).bin

main: ./init/main.c $(OBJ)
	@$(CC) $(CFLAGS) ./init/main.c $(OBJ)

CC_COMMAND = $(CC) -c $(CFLAGS) $<
%.rel: %.c
	@$(CC_COMMAND)

CLEAN_COMMAND = rm -f *.adb *.asm *.lst *.rel *.sym *.omf *.mem *.map *.lk *.ihx *.cdb *.rst *.bin
clean :
	@$(CLEAN_COMMAND)
