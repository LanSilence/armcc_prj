


#以下同根目录下的makefile的相同代码的解释
CUR_CSOURCE_FILE=${wildcard *.c}
CUR_SSOURCE_FILE=${wildcard *.s}

CUR_COBJS=${patsubst %.c, %.o, $(CUR_CSOURCE_FILE)}
CUR_SOBJS=${patsubst %.s, %.o, $(CUR_SSOURCE_FILE)}
all:$(CUR_SOBJS) $(CUR_COBJS)
# $(SUBDIRS):ECHO
# 	make -C $@
$(CUR_COBJS):%.o:%.c
#$(CC) -c $^ $(INCLUDE_DIR) -o $(ROOT_DIR)\$(OBJS_DIR)\$@
	$(CC)  -c -O2 $(INCLUDE_DIR) $(CCOMPILE_FLAG)  $< -o $(OBJS_DIR)\$@ --omf_browse $(subst .c,.crf,$(OBJS_DIR)\$<) --depend $(subst .c,.d,$(OBJS_DIR)\$<)
$(CUR_SOBJS):%.o:%.s
#$(CC) -c $^ $(INCLUDE_DIR) -o $(ROOT_DIR)\$(OBJS_DIR)\$@
	$(ASMC) $(INCLUDE_DIR) $(ASMCOMPILE_FLAG) $< -o $(OBJS_DIR)\$@ --list $(subst .s,.lst,$(OBJS_DIR)\$<) --depend $(subst .s,.d,$(OBJS_DIR)\$<)

ECHO:
	@echo $(SUBDIRS)