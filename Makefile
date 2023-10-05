all: print_message generate run

print_message:
	@echo "Compiling..."

generate:
	mkdir -p build
	g++ -o ./build/program ./src/main.cpp

run:
	./build/program
