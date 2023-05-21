

TARGET := game

# 编译器的名字
CC := arm-linux-gcc -pthread


# 头文件搜索路径
INC := -I ./inc 
INC += 

# 依赖的库的路径及依赖库
LIBS := -L ./libs -L pthread 


#项目所有的.c文件列表
CSRCS := $(wildcard *.c)
# CSRCS += $(wildcard media/*.c)



#项目中所有的 .o文件列表
OBJS := $(patsubst  %.c, %.o, $(CSRCS))


$(TARGET): $(OBJS) 
	$(CC) $^  $(LIBS) -o $@
	cp $@ /home/dk/tftp
	rm -rf $(OBJS)
	rm -rf $(TARGET)

%.o: %.c 
	$(CC) -c $< $(INC) -o  $@


#清除目标
#删掉所有的.o以及目标文件
clean:
	rm -rf $(OBJS)
	rm -rf $(TARGET)

