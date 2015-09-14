
目录树：
	./init/main.c：main.c文件
	./drivers/src：C程序源文件
	./drivers/inc：
	./configs    ：C程序头文件
	./doc	     ：带注释的Makefile


源码阅读建议从主函数main()开始，其中__data, __code,
sfr等定义意义请参考sdcc.pdf

程序的编译依靠Makefile
，编译器选择sdcc，在程序文件根目录下命令行输入make即可（注意，需要在Makefile第一行修改sdcc地址）

根目录下的*.adb *.asm *.lst *.rel *.sym *.omf *.mem *.map *.lk *.ihx *.cdb
*.rst *.bin文件都是编译后生成的文件，具体请参考sdcc.pdf
