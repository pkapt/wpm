build: wpm.c
	@gcc wpm.c -o wpm.exe -O0
	@mkdir -p dist
	@mv wpm.exe dist

clean:
	@rm -rf dist