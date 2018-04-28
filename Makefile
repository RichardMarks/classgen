.PHONY: clean test

./bin/classgen: ./build/main.o
	mkdir -p $(dir $@)
	clang++ $< -o $@

./build/main.o: ./src/main.cpp
	mkdir -p $(dir $@)
	clang++ -std=c++14 -I./src -O0 -c $< -o $@

test: ./bin/classgen
	clang++ -DTEST -std=c++14 -I./src -O0 ./src/main.cpp -o ./bin/classgen_test

clean:
	$(RM) -r ./bin
	$(RM) -r ./build
