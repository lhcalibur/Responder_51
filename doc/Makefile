####################### Makefile 的一些定义####################
### 具体设置请参考sdcc.pdf
# 编译器打开路径
CC = /Users/cooltouya/Programs/sdcc/bin/sdcc	
# 不启用优化的选项
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
# 库文件链接路径
LIBPATH = /Users/cooltouya/Programs/sdcc/share/sdcc/lib/medium 		


USRDEFINE += 

# 文件路径设置

DRIVERS = ./drivers	
INIT = ./init
CONFIGS = ./configs
MAIN = main.rel		
# 文件路径中的.c文件去掉路径头并把.c换成.rel
OBJ += $(patsubst %.c,%.rel,$(notdir $(wildcard $(DRIVERS)/src/*.c)))	
# 头文件搜索路径

INCLUDES += -I$(DRIVERS)/inc -I$(CONFIGS)	
# 源文件搜索路径
VPATH += $(DRIVERS)/src $(INIT)	
CFLAGS += $(STD) $(PROCESSOR) --model-medium -L$(LIBPATH) --iram-size 0x0100 $(INCLUDES) $(DEBUG) --less-pedantic --xram-size 0x0100 --xram-loc 0x80 $(STDLIB) $(STDINC) $(REENTRANT) \
	  --no-peep --int-long-reent --float-reent
# 程序名字

PROG = light	
####################### Makefile 编译开始的地方################
# 目标文件 light.bin

all: main $(PROG).bin				

# light.bin 依靠main.ihx生成，下面要先生成main.ihx

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
