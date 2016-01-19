GCC = g++ 

INC_DIR = ../core/include
OBJ_DIR = ../core/obj

INC=-I/usr/include/irods/ -I/usr/include/irods/boost/ -I/usr/include/libxml2 -I${INC_DIR}

all: msiLoadMetadataFromXml

msiLoadMetadataFromXml:
	${GCC} ${INC} -DRODS_SERVER -fPIC -shared -g -Wno-deprecated -Wno-write-strings -o $@.so $@.cpp /usr/lib/libirods_client.a
	
clean:
	@rm -f ./*.so
