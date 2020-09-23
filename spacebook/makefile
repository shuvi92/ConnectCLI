

OSPL_LIBS = -lpthread -lddskernel -ldcpssacpp
LIBS=-L${OSPL_HOME}/lib ${OSPL_LIBS}

CFLAGS = -Wall -O0 -g -I. \
         -I./include -I${OSPL_HOME}/include/dcps/C++/SACPP \
         -I${OSPL_HOME}/include/sys \
         -I${OSPL_HOME}/include

CXXFLAGS = -std=c++11
BOOSTFLAG = -lboost_unit_test_framework

all: spacebook
rebuild: clean all

IDL_GENERATED_H= \
                 ccpp_tsn.h \
                 tsnDcps.h \
                 tsnDcps_impl.h \
                 tsn.h \
                 tsnSplDcps.h

IDL_GENERATED_CPP=\
                 tsn.cpp \
                 tsnDcps.cpp \
                 tsnDcps_impl.cpp \
                 tsnSplDcps.cpp

IDL_GENERATED=${IDL_GENERATED_H} ${IDL_GENERATED_CPP}

${IDL_GENERATED}: idl/tsn.idl
	${OSPL_HOME}/bin/idlpp -l cpp idl/tsn.idl

COMMON_CPP= src/CheckStatus.cpp src/DDSEntityManager.cpp src/util.cpp

COMMON_H= src/dds_io.h src/CheckStatus.h src/DDSEntityManager.h

CUSTOM_HO = src/spacebook.cpp src/user.o src/post.o src/ops.o src/view.o src/controller.o src/screen.o src/communicator.o


spacebook: ${IDL_GENERATED_H} ${IDL_GENERATED_CPP} ${CUSTOM_HO} ${COMMON_CPP} ${COMMON_H}
	g++ -o $@ ${CXXFLAGS} ${CFLAGS} $^ ${LIBS}

user.o: src/user.cpp src/user.h 
	g++ ${CXXFLAGS}

post.o: src/post.cpp src/post.h 
	g++ ${CXXFLAGS}

screen.o: src/screen.cpp src/screen.h 
	g++ ${CXXFLAGS}

src/ops.o: src/ops.cpp src/ops.h
	g++ ${CXXFLAGS} src/ops.cpp -o src/ops.o -c ${CFLAGS}

src/view.o: src/view.cpp src/view.h 
	g++ ${CXXFLAGS} src/view.cpp -o src/view.o -c ${CFLAGS}

src/controller.o: src/controller.cpp src/controller.h
	g++ ${CXXFLAGS} src/controller.cpp -o src/controller.o -c ${CFLAGS}

src/communicator.o: src/communicator.cpp src/communicator.h
	g++ ${CXXFLAGS} src/communicator.cpp -o src/communicator.o -c ${CFLAGS}

test: unit_test.o
	g++ -o unit_test unit_test.o ${BOOSTFLAG}
    
unit_test.o:
	g++ -c src/unit_test.cpp ${BOOSTFLAG}


clean:
	-rm -f spacebook
	-rm -f ${IDL_GENERATED_H} ${IDL_GENERATED_CPP}
	-rm -f ospl-error.log ospl-info.log
	-rm -f src/*.o
