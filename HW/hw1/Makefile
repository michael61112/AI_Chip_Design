LIB_DIR=-L$(SYSTEMC_HOME)/lib-linux64
INC_DIR=-I$(SYSTEMC_HOME)/include
RPATH = -Wl,-rpath=$(SYSTEMC_HOME)/lib-linux64
LIB=-lsystemc -lm

APP = run
C = *.cpp

all:
	g++ -I . $(INC_DIR) -L . $(LIB_DIR) -o $(APP) $(C) $(LIB) $(RPATH)
clean:
	rm -rf $(APP)