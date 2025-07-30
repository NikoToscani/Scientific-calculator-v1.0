all: install
.PHONY: all

install:
	cmake -S . \
	-B ../build
	cd ../build; \
	make
.PHONY: install

run: install
	cd ../build; \
	./Scientific_calculator_V1.0
.PHONY: run

uninstall:
	rm -rf CMakeLists.txt.user
	rm -rf ../build/
	rm -rf ../build*/
.PHONY: uninstall

clean:
	rm -rf ../documentation
	rm -rf ../dist/
	rm -rf ../build/
	rm -rf ../build*/
	rm -rf ../.vscode/
	rm -rf report/
	rm -rf CMakeLists.txt.user
	rm -rf test.info
.PHONY: clean

dvi:
	doxygen dxgn_CPP_CALC.conf
	open ../documentation/html/index.html
.PHONY: dvi

dist:
	mkdir -p ../dist/Scientific_calculator_v1.0-1/src
	cp -r . ../dist/Scientific_calculator_v1.0-1/src
	cp ../README.md ../dist/Scientific_calculator_v1.0-1
	cp ../README_RUS.md ../dist/Scientific_calculator_v1.0-1
	tar -czf ../dist/Scientific_calculator_v1.0-1.tar.gz -C ../dist Scientific_calculator_v1.0-1
.PHONY: dist

tests:
	cmake -S . \
	-B ../build
	cd ../build; \
	make tests_modelTests;
.PHONY: tests

coverage:
	cmake -S . \
	-B ../build
	cd ../build; \
	make coverage_modelTests; \
	open ./model/coverage_modelTests_dir/model/index.html
.PHONY: coverage

check_style:
	clang-format -n -style=google *.cc */*.cc */*.h */*/*.cc */*/*.h
.PHONY: check_style

correct_style:
	clang-format -i -style=google *.cc */*.cc */*.h */*/*.cc */*/*.h
.PHONY: correct_style

cppcheck:
	cppcheck --check-config --enable=all --inconclusive --std=c++17 \
	--language=c++ --suppress=missingIncludeSystem *.cc */*.cc */*.h */*/*.cc
.PHONY: cppcheck

valgrind:
	make tests
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all \
	--track-origins=yes -s --verbose -q --show-reachable=yes --num-callers=20 \
	--trace-children=yes --track-fds=yes ../build/model/modelTests
.PHONY: valgrind

leaks:
	make tests
	leaks -quiet -atExit -- ../build/model/modelTests
.PHONY: leaks
