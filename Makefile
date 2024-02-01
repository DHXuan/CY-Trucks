all:build script

clean:
	rm SE1.c
	rm SE2.c
	rm TE1.c
	rm TE2.c
build:
	gcc -o yy /PartieC/SE1.c
	gcc -o zz /PartieC/SE2.c
	gcc -o xx /PartieC/TE1.c
	gcc -o nn /PartieC/TE2.c
run:
	./yy
	./zz
	./xx
	./nn
script:
	chmod 777 SE1.c
	chmod 777 SE2.c
	chmod 777 TE1.c
	chmod 777 TE2.c
	sh option.sh data.csv
	
