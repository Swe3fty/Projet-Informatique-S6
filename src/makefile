all:
	gcc -c consigne.c
	gcc -c visualisationT.c
	gcc -c visualisationC.c
	gcc -c regulation.c
	gcc -c simulateur.c
	gcc -c main_sim.c
	gcc -c simulateur.c
	gcc consigne.o visualisationT.o visualisationC.o regulation.o simulateur.o main_sim.o -o main_sim.exe 

clean:
	del /f *.o main_sim.exe