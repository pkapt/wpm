build: wpm.c
	@gcc wpm.c -o wpm.exe -O2

clean:
	@rm -f *.exe