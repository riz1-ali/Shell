shell:\
			shell.o \
			get_command.o \
			split_command.o \
			print_prompt.o \
			ls.o \
			cd.o \
			pwd.o \
			echo.o \
			pinfo.o \
			remindme.o \
			clock.o \
			exec_command.o \
			env.o \
			pipe.o\
		
		gcc -g -w -o shell $^

clean :
	@rm -f *.o shell

shell.o : shell.c
	gcc -g -w -c shell.c

get_command.o : get_command.h
	gcc -g -w -c get_command.c	

split_command.o : split_command.h
	gcc -g -w -c split_command.c	

print_prompt.o : print_prompt.h
	gcc -g -w -c print_prompt.c	

ls.o : ls.h
	gcc -g -w -c ls.c

pipe.o : pipe.h
	gcc -g -w -c pipe.c

cd.o : cd.h
	gcc -g -w -c cd.c

pwd.o : pwd.h
	gcc -g -w -c pwd.c

echo.o : echo.h
	gcc -g -w -c echo.c

pinfo.o : pinfo.h
	gcc -g -w -c pinfo.c

remindme.o : remindme.h
	gcc -g -w -c remindme.c

clock.o : clock.h
	gcc -g -w -c clock.c

exec_command.o : exec_command.h
	gcc -g -w -c exec_command.c

env.o : env.h
	gcc -g -w -c env.c