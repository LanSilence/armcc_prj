#设置编译器路径  因为armcc需要注册才能使用，因此无法将编译器复制到此处
PATH = D:\Keil_v5\ARM\ARMCC\bin

CC = $(PATH)\armcc
ASMC = $(PATH)\armasm.exe
FROM_ELF = $(PATH)\fromelf.exe
LINK = $(PATH)\armlink.exe

#工程名称
RROJECT_NAME = FM33LC0XX_Detector

#CPU类型，内核类型 M0，M0+ M1 M3 M4 具体使用armcc --list cpu 查看
CPU_TYPE = Cortex-M0

#芯片类型，这个如果设置不对，会报错，然后去报错的位置复制正确的即可，是一个宏定义
CHIP_TYPE = FM33LC0XX

#汇编编译选项
#--cpu Cortex-M4.fp cpu型号是Cortex-M4.fp
#--pd "__UVISION_VERSION SETA 514"  编译之前将__UVISION_VERSION赋值为514，后者同理
ASMCOMPILE_FLAG = --cpu $(CPU_TYPE) -g --apcs=interwork --pd "__UVISION_VERSION SETA 514" --pd "$(CHIP_TYPE) SETA 1" --xref

CCOMPILE_FLAG = --cpu $(CPU_TYPE) --c99 -g -O2 --apcs=interwork --split_sections -D__UVISION_VERSION="514" -D$(CHIP_TYPE)


#test_prj.sct 分散加载文件，由此文件指定代码加载地址以及代码运行地址
#info 输出指定信息
#--list 生成指定的映射文件，包括变量以及段的地址等等
LINK_FLAG = --cpu $(CPU_TYPE) --strict --scatter "E:\mycode\github\automatic\code\MDK-ARM\FM33LC0XX_Detector.sct" --summary_stderr --info summarysizes --map --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers --list "$(Bulid)\Listings\$(RROJECT_NAME).map"


#intel 32位hex格式
#输出文件名为test_prj.hex
#基地址为0x08000000
FROM_ELF_FLAG = --i32 --output=$(BIN_DIR)\$(RROJECT_NAME).hex --base=0x00000000


#debug文件夹里的makefile文件需要最后执行，所以这里需要执行的子目录要排除debug文件夹，这里使用awk排除了debug文件夹，读取剩下的文件夹
#SUBDIRS=E:\mycode\test_armcc\main
SUBDIRS= Drivers\FM33LC0xx_LL_Driver\Src \
         Drivers\CMSIS\Device\FM\FM33xx\Source \
         Drivers\CMSIS\Device\FM\FM33xx\Source\Templates\ARM \
         MF-config\Src \
         BSP \
         main \
         Timer_task \
         User_app \
         drv \


	
#无需下一行的注释代码，因为我们已经知道debug里的makefile是最后执行的，所以最后直接去debug目录下执行指定的makefile文件就行，具体下面有注释
#DEBUG=$(shell ls -l | grep ^d | awk '{if($$9 == "debug") print $$9}')
#记住当前工程的根目录路径
ROOT_DIR=$(shell chdir)
ifndef release
	Bulid = $(ROOT_DIR)\MDK-ARM\debug
else
	Bulid = $(ROOT_DIR)\MDK-ARM\release
endif


INCLUDE_DIR = $(ROOT_DIR)\BSP \
			  $(ROOT_DIR)\main \
			  $(ROOT_DIR)\Timer_task \
			  $(ROOT_DIR)\User_app \
			  $(ROOT_DIR)\drv \
			  $(ROOT_DIR)\Drivers\FM33LC0xx_LL_Driver\Inc \
			  $(ROOT_DIR)\Drivers\CMSIS\Device\FM\FM33xx\Include \
			  $(ROOT_DIR)\Inc \
			  $(ROOT_DIR)\MF-config\Inc

#INCLUDE_DIR = -I $(shell chdir)
#最终bin文件的名字，可以更改为自己需要的
BIN=myapp.exe
#目标文件所在的目录
OBJS_DIR=$(Bulid)\Objects
#bin文件所在的目录
BIN_DIR=$(Bulid)\bin
#获取当前目录下的c文件集，放在变量CUR_SOURCE中
CUR_SOURCE=${wildcard *.c}
#将对应的c文件名转为o文件后放在下面的CUR_OBJS变量中
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

INCLUDE_DIR := $(patsubst %, -I %, $(INCLUDE_DIR))

SFILES := $(foreach dir, $(SUBDIRS), $(wildcard $(dir)/*.S))
CFILES := $(foreach dir, $(SUBDIRS), $(wildcard $(dir)/*.c))

SFILENDIR := $(notdir $(SFILES))
CFILENDIR := $(notdir $(CFILES))


#将以下变量导出到子shell中，本次相当于导出到子目录下的makefile中
export CC BIN OBJS_DIR BIN_DIR ROOT_DIR INCLUDE_DIR FROM_ELF_FLAG LINK_FLAG ASMCOMPILE_FLAG CCOMPILE_FLAG ASMC FROM_ELF LINK RROJECT_NAME
#注意这里的顺序，需要先执行SUBDIRS最后才能是DEBUGs
all:$(SUBDIRS) $(CUR_OBJS) $(Bulid)
#递归执行子目录下的makefile文件，这是递归执行的关键
$(SUBDIRS):ECHO
	make -C $@
    #直接去debug目录下执行makefile文件
$(Bulid):ECHO
	make -C $(Bulid)
ECHO:
	@echo $(SUBDIRS)
#将c文件编译为o文件，并放在指定放置目标文件的目录中即OBJS_DIR
$(CUR_OBJS):%.o:%.c
	$(CC) -c O2 $(INCLUDE_DIR) $^ -o $(OBJS_DIR)/$@ --omf_browse $(subst .c,.crf,$<) --depend $(subst .c,.d,$<)
$(CUR_SCOBJS):%.o:%.s
	$(ASMC)  -c -O2 $(INCLUDE_DIR) $< -o $(OBJS_DIR)\$@ --list $(subst .s,.lst,$<) --depend $(subst .s,.d,$<)

clean:
	del /q $(BIN_DIR)\*
	del /q $(OBJS_DIR)\*
	del /q $(Bulid)\Listings\*
