shell: main.o ch_dir.o lets_echo.o call_back.o p_info.o redirection.o prompt.o piping.o
	gcc main.o ch_dir.o lets_echo.o call_back.o prompt.o redirection.o piping.o p_info.o -o shell
main.o : main.c
	gcc -c main.c
ch_dir.o:  ch_dir.c header.h
	gcc -c ch_dir.c
lets_echo.o:lets_echo.c header1.h
	gcc -c lets_echo.c
call_back.o:call_back.c header2.h
	gcc -c call_back.c
p_info.o:p_info.c header3.h
	gcc -c p_info.c
prompt.o:prompt.c header4.h
	gcc -c prompt.c
redirection.o:redirection.c header5.h
	gcc -c redirection.c
piping.o:piping.c header6.h header5.h
	gcc -c piping.c
clean:
	rm -f *.o shell
