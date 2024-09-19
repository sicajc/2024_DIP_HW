# Grab the src files and compile them
FLIP_DIR= ./src/flip.cpp
RE_DIR= ./src/resolution.cpp
CROP_DIR = ./src/crop.cpp


flips: compile_flips run
resolution: compile_resolution run
crop: compile_crop run

compile_flips: $(FLIP_DIR)
	g++ -g -o hw1.out $(FLIP_DIR)
compile_resolution: $(RE_DIR)
	g++ -g -o hw1.out $(RE_DIR)
compile_crop: $(CROP_DIR)
	g++ -g -o hw1.out $(CROP_DIR)

run:
	./hw1.out

clean:
	rm -f hw1.out
	rm -f output/*