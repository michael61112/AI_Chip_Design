LIB_DIR=/usr/local/systemc-2.3.4/lib-linux64
INC_DIR=/usr/local/systemc-2.3.4/include
LIB=-lsystemc-2.3.4
RPATH=-Wl,-rpath,/usr/local/systemc-2.3.4/lib-linux64

# export SYSTEMC_HOME=/usr/local/systmec-2.3.4
# export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib-linux64

O = run
C = *.cpp

all:
	g++ -I . -I  $(INC_DIR) -L . -L $(LIB_DIR) -o $(O) $(C) $(LIB) $(RPATH)

clean:
	rm -rf $(O)
