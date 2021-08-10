build: wpm.c
	@gcc wpm.c -o wpm.exe -O0

clean:
	@rm -f *.exe