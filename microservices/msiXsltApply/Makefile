GCC = g++ 

INC_DIR = ../core/include
OBJ_DIR = ../core/obj

INC=-I/usr/include/irods/ -I/usr/include/irods/boost/ `xslt-config --cflags` -I${INC_DIR}

all: libmsiXsltApply 

libmsiXsltApply:
	${GCC} ${INC} -DRODS_SERVER -fPIC -shared -g -Wno-deprecated -Wno-write-strings -o $@.so $@.cpp `xslt-config --libs` /usr/lib/libirods_client.a
	
clean:
	@rm -f ./*.so
