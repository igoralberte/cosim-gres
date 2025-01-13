SIMS_PATH=../sims
# LIBDIR = $(shell pg_config --libdir)
# INCLUDEDIRSERVER = $(shell pg_config --includedir-server)
LIBDIR = '/usr/local/pgsql/lib'
INCLUDEDIRSERVER = '/usr/local/pgsql/include/server'

DISTANCE_FUNCTIONS:
	g++ -I${SIMS_PATH}/include -I${INCLUDEDIRSERVER} distance_functions.cpp -c -fPIC -m64 -std=c++11
	g++ -shared -o distance_functions.so distance_functions.o -fPIC -m64
	rm *.o
	cp distance_functions.so ${LIBDIR}
clean:
	rm *.so
