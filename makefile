build: wpm.c
	@gcc wpm.c -o wpm.exe

clean:
	@rm -f *.exe