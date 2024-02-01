all:build script

clean:
	rm SE1.c
	rm SE2.c
	rm TE1.c
	rm TE2.c
build:
	gcc yy -o SE1.c
	gcc zz -o SE2.c
	gcc xx -o TE1.c
	gcc nn -o TE2.c
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
	sh T.sh
	sh S.sh
