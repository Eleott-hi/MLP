# @authors   pintoved (Pinto Veda)
#            cgreenbe (Classie Greenbeard)
#            bgreydon (Bail Greydon)

#------------------------------------------------#
#   INGREDIENTS                                  #
#------------------------------------------------#
# NAME           project name
# VERSION        build version
#
# OS             operating system
# FILES          all .cc .h files
#
# INSTALL_DIR    installation path
# FILES          executable file name
#
# cmake          build system executable

NAME         := MLP
VERSION      := 1.0

OS           := $(shell uname -s)
FILES        := $(shell find . -regex '.*\(cc\|h\)' -not -path "*shared*")

BUILD_DIR    := ../build
INSTALL_DIR  := ../install

ifeq ($(OS), Darwin)
APP		     := $(BUILD_DIR)/$(NAME).app
else
APP		     := $(BUILD_DIR)/$(NAME)
endif

ifeq ($(OS), Darwin)
OPEN 	     := open 
else
OPEN	     := ./
endif

#------------------------------------------------#
#   RECIPES                                      #
#------------------------------------------------#
# all        default goal
# install
# uninstall
# clean      remove binary
# dvi
# dist
# build      build program
# linter     run code style check
# linter-fix run code style fix
# cppcheck   run static code analys

all: run

run: build
	$(OPEN)$(APP)

install: build
	mkdir -p $(INSTALL_DIR)
	cp -r $(APP) $(INSTALL_DIR)

uninstall:
	rm -rfv $(INSTALL_DIR)

dvi:
	latex docs.tex docs.dvi

dist: build dvi
	tar -czvf $(NAME)-$(VERSION).tar.gz */ *.cc *.h *.txt docs.dvi Makefile					

build:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

clean:
	rm -rfv $(BUILD_DIR)* $(INSTALL_DIR) logs/ \
	*.tar.gz *.aux *.log *.dvi *.out *.toc  *.user

linter:
	clang-format -n -style=Google $(FILES)
	
linter-fix:
	clang-format -i -style=Google $(FILES)

cppcheck:
	cppcheck --language=c++ --std=c++17 \
	--enable=all \
	--suppress=missingInclude \
	--suppress=unusedFunction \
	$(FILES)

#------------------------------------------------#
#   SPEC                                         #
#------------------------------------------------#

.PHONY: install uninstall clean dvi dist build linter cppcheck
.SILENT:
