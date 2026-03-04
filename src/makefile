all: main_sim.exe

main_sim.exe: consigne.o visualisationT.o visualisationC.o regulation.o simulateur.o main_sim.o
	gcc consigne.o visualisationT.o visualisationC.o regulation.o simulateur.o main_sim.o -o main_sim.exe 

consigne.o: consigne.c consigne.h define.h
	gcc -c consigne.c 

visualisationT.o: visualisationT.c visualisationT.h define.h
	gcc -c visualisationT.c

visualisationC.o: visualisationC.c visualisationC.h define.h
	gcc -c visualisationC.c

regulation.o: regulation.c regulation.h define.h
	gcc -c regulation.c

simulateur.o: simulateur.c simulateur.h define.h
	gcc -c simulateur.c

main_sim.o: main_sim.c define.h
	gcc -c main_sim.c

clean:
	del /f *.o main_sim.exe