build:
	@mkdir -p dist
	@gcc main.c src/line.c src/console.c -o dist/wpm.exe -O0

clean:
	@rm -rf dist