GCC = g++ 

INC_DIR = ../core/include
OBJ_DIR = ../core/obj

INC=-I/usr/include/irods/ -I/usr/include/irods/boost/ `xml2-config --cflags` -I${INC_DIR}

all: libmsiXmlDocSchemaValidate

libmsiXmlDocSchemaValidate:
	${GCC} ${INC} -DRODS_SERVER -fPIC -shared -g -Wno-deprecated -Wno-write-strings -o $@.so $@.cpp `xml2-config --libs`  /usr/lib/libirods_client.a
	
clean:
	@rm -f ./*.so
