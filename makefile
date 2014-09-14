CC = g++
CFLAGS = -Wall -g

MCPricer : pricer.o models.o exotics.o mcengine.o normals.o mrg32k3a.o
	${CC} ${CFLAGS} pricer.o models.o mcengine.o normals.o mrg32k3a.o exotics.o -o MCPricer
models.o : models.cpp structs.h mcengine.h
	${CC} ${CFLAGS} -c models.cpp
pricer.o : pricer.cpp structs.h mcengine.h models.h payoffs.h exotics.h
	${CC} ${CFLAGS} -c pricer.cpp
mcengine.o : mcengine.cpp structs.h models.h exotics.h normals.h mrg32k3a.h
	${CC} ${CFLAGS} -c mcengine.cpp
normals.o: normals.cpp mrg32k3a.h
	${CC} ${CFLAGS} -c normals.cpp
mrg32k3a.o: mrg32k3a.cpp
	${CC} ${CFLAGS} -c mrg32k3a.cpp
exotics.o : exotics.cpp structs.h
	${CC} ${CFLAGS} -c exotics.cpp
