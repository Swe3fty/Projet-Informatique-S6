all: sim

sim: main_sim.exe

usb: main_usb.exe

main_sim.exe: consigne.o visualisationT.o visualisationC.o regulation.o simulateur.o main_sim.o
	gcc consigne.o visualisationT.o visualisationC.o regulation.o simulateur.o main_sim.o -o main_sim.exe 

main_usb.exe: consigne.o visualisationT.o visualisationC.o regulation.o commande.o releve.o main_usb.o ftd2xx.lib
	gcc consigne.o visualisationT.o visualisationC.o regulation.o commande.o releve.o main_usb.o ftd2xx.lib -o main_usb.exe

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

commande.o: commande.c commande.h define.h
	gcc -c commande.c

releve.o: releve.c releve.h define.h
	gcc -c releve.c

main_sim.o: main_sim.c define.h
	gcc -c main_sim.c

main_usb.o: main_usb.c define.h
	gcc -c main_usb.c

clean:
	del /f *.o main_sim.exe main_usb.exe