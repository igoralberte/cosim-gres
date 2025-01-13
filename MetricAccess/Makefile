SIMS_PATH=../sims
# LIBDIR = $(shell pg_config --libdir)
# INCLUDEDIRSERVER = $(shell pg_config --includedir-server)
LIBDIR = '/usr/local/pgsql/lib'
INCLUDEDIRSERVER = '/usr/local/pgsql/include/server'
SPI = '/usr/local/pgsql/include/server/executor'

DISTANCE_FUNCTIONS:
	g++ -std=c++11 -I${SIMS_PATH}/include -I../DistanceFunctions -I${INCLUDEDIRSERVER} -L${INCLUDEDIRSERVER} metric_access.cpp -c -fPIC -m64
	g++ -std=c++11 -shared -o metric_access.so metric_access.o -L${SIMS_PATH}/lib -larboretum -lartemis
	rm *.o
	cp metric_access.so ${LIBDIR}
clean:
	rm *.so
