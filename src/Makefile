CC=gcc
CRUN_TYPE=TEST
CFLAGS= -lm -Wall -Werror -Wextra -std=c11 -O2 -funroll-loops -mavx -g -D$(CRUN_TYPE) 
CTEST_FLAGS= -lm $(shell pkg-config --cflags --libs check)

COBJECTS=*.o
CFILES=*.c 
HFILES=*.h
CTEST=test_check
TEST_RUNNER=tester

INSTALL_ROOT=./bin
PROJNAME=ViewerV1
VERSION=1.0
PROF = 3DViewer/3DViewer.pro

LINUXLIBS = -lm -lsubunit

all: install

install: qmake
	make -f Qmakefile install INSTALL_ROOT=$(INSTALL_ROOT)

uninstall: qmake
	make -f Qmakefile uninstall INSTALL_ROOT=$(INSTALL_ROOT)

rebuild: clean all

dvi: docs
	open Docs/html/index.html

dist: clean
	tar --totals -cvf "$(PROJNAME)-$(VERSION).tar"  ./*
	gzip "$(PROJNAME)-$(VERSION).tar"

test: clean objective 
	@$(CC) $(CFLAGS) *.o $(CTEST_FLAGS)   -o $(TEST_RUNNER)
	@./$(TEST_RUNNER)

objective:
	@$(CC) $(CFLAGS) -c $(CFILES)

checkStyle:
	cp ../materials/linters/.clang-format
	clang-format -n -style=Google $(CFILES) $(HFILES) $(CTEST).c
	rm -f .clang-format

open:
ifeq ($(shell uname),Darwin)
	@open -a "Google Chrome" report/index.html
else
	@xdg-open report/index.html
endif


gcov_report: clean
	@$(CC) $(CFLAGS)  -fprofile-arcs -ftest-coverage  $(CFILES) -o gcov_report $(CTEST_FLAGS)
	@./gcov_report
	@lcov -t "matrix test" -o matrix_test.info -c -d .
	@genhtml -o report matrix_test.info
	@mv *.gcda *.gcno *.info gcov_profile

qmake: $(PROF)
	qmake -o Qmakefile $(PROF)

docs: .PHONY
	doxygen


clean: .PHONY 
	-qmake -o Qmakefile $(PROF)
	-make -f Qmakefile clean
	-rm -rf *.o* *.out* *.a $(TEST_RUNNER) gcov_profile/*.info gcov_profile/*.gcda
	-rm -rf gcov_profile/*.gcno *.index report test/*.dSYM* *.data gcov_report *.gcda *.gcno *.dSYM
	-rm -f test test.c *.o *.out *.a *.gc* *.info
	-rm -f .qmake.stash
	-rm -rf opt
	-rm *.gz 
	-rm -rf ./*dSYM
	-rm -rf bin
	-rm -f Qmakefile 
	-rm -f Calculator
	-rm -rf Docs
.PHONY :
