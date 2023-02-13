INCLUDE_PATH := ../main/model

all: graph_mlp_test matrix_mlp_test

graph_mlp_test:
	g++ -Wall -Wextra \
	utils.cc \
	graph_mlp_tests.cc \
	$(INCLUDE_PATH)/config.cc \
	$(INCLUDE_PATH)/letter.cc \
	$(INCLUDE_PATH)/graph-mlp/*.cc \
	-I$(INCLUDE_PATH) -lgtest -o tests.out
	./tests.out

matrix_mlp_test:
	g++ -Wall -Wextra \
	utils.cc \
	matrix_mlp_tests.cc \
	$(INCLUDE_PATH)/config.cc \
	$(INCLUDE_PATH)/matrix-mlp/*.cc \
	-I$(INCLUDE_PATH) -lgtest -o tests.out
	./tests.out
