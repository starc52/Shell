all: echo ls pinfo pwd starcsh
echo: echo.c
	gcc -o echo echo.c
ls: ls.c
	gcc -o ls ls.c
pinfo: pinfo.c
	gcc -o pinfo pinfo.c
pwd: pwd.c
	gcc -o pwd pwd.c
starcsh: starcsh.c
	gcc -o starcsh starcsh.c