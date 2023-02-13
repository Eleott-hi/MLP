OS := $(shell uname -s)
PROJECT_NAME := MLP
VERSION := 1.0
INSTALLATION_PATH := ~/

ifeq ($(OS), Darwin)
EXEC_FILE_NAME := $(PROJECT_NAME).app
else
EXEC_FILE_NAME := $(PROJECT_NAME)
endif

ifeq ($(OS), Darwin)
QMAKE := qmake
else
QMAKE := qmake6
endif

MAIN_SOURCES := $(shell find main -name "*.cc" -not -path "*shared*") \
		        $(shell find main -name "*.h" -not -path "*shared*")

TESTS_SOURCES := $(shell find unit-tests -name "*.cc") \
		         $(shell find unit-tests -name "*.h")

all: install run

install: build
	cp build/$(EXEC_FILE_NAME) $(INSTALLATION_PATH)

uninstall:
	rm -v $(INSTALLATION_PATH)$(EXEC_FILE_NAME)

clean:
	rm -rfv *.tar.gz \
	*.aux *.log *.dvi *.out *.toc *.user \
	build*/ logs/ unit-tests/tests.out

dvi:
	latex docs.tex docs.dvi

dist: build dvi
	tar -czvf $(PROJECT_NAME)-$(VERSION).tar.gz \
	build/$(EXEC_FILE_NAME) \
	docs.dvi \
	main/* \
	unit-tests/* \
	Makefile

tests:
	cd unit-tests; make

run:
ifeq ($(OS), Darwin)
	cd $(INSTALLATION_PATH); open $(EXEC_FILE_NAME)
else
	cd $(INSTALLATION_PATH); ./$(EXEC_FILE_NAME)
endif

build: clean
	mkdir -p build
	cp -r main/* build/
	cd build; $(QMAKE)
	cd build; make

cppcheck:
	cppcheck --language=c++ --std=c++17 \
	--enable=all \
	--suppress=unusedFunction \
	--suppress=missingInclude \
	$(MAIN_SOURCES) \
	$(TESTS_SOURCES)

linter:
	clang-format -n -style=google \
	$(MAIN_SOURCES) \
	$(TESTS_SOURCES)

check: cppcheck linter
