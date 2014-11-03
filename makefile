# makefile INDUS Lab1
# Xavier Mawet 2014
.SILENT:

OBJS = common.o NSignals.o NSharedMemory.o NSemaphore.o NMessageQueue.o

all: pAccueil pTri pTraitement

pAccueil: Accueil/main.c hospital.h $(OBJS)
		echo creation de pAccueil...
		$(CC) -o pAccueil Accueil/main.c $(OBJS)

pTri: Tri/main.c hospital.h $(OBJS)
		echo creation de pTri...
		$(CC) -o pTri Tri/main.c $(OBJS)

pTraitement: Traitement/main.c hospital.h $(OBJS)
		echo creation de pTraitement...
		$(CC) -o pTraitement Traitement/main.c $(OBJS)

common.o:	lib/common.c lib/common.h
		echo creation de common.o...
		$(CC) -c lib/common.c

NSignals.o:	lib/NSignals.c lib/NSignals.h
		echo creation de NSignals.o...
		$(CC) -c lib/NSignals.c

NSharedMemory.o:	lib/NSharedMemory.c lib/NSharedMemory.h
		echo creation de NSharedMemory.o...
		$(CC) -c lib/NSharedMemory.c

NSemaphore.o:	lib/NSemaphore.c lib/NSemaphore.h
		echo creation de NSemaphore.o...
		$(CC) -c lib/NSemaphore.c

NMessageQueue.o:	lib/NMessageQueue.c lib/NMessageQueue.h
		echo creation de NMessageQueue.o...
		$(CC) -c lib/NMessageQueue.c