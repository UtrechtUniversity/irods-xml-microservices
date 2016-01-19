GCC = g++ 

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj

INC=-I/usr/include/irods/ -I/usr/include/irods/boost/ -I${INC_DIR}

all: lib

lib:
	${GCC} ${INC} -c -DRODS_SERVER -fPIC -g -Wno-deprecated -Wno-write-strings -o ${OBJ_DIR}/era.o ${SRC_DIR}/eraUtil.c
	
clean:
	@rm -f ${OBJ_DIR}/*.o