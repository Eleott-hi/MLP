QT += widgets core gui printsupport

CONFIG += c++17 optimize_full
QMAKE_CXXFLAGS += -std=c++17

INCLUDEPATH += model/ view/

SOURCES += \
    main.cc \
    model/config.cc \
    model/letter.cc \
    model/metrics.cc \
    model/model.cc \
    model/neural_network.cc \
    model/graph-mlp/graph_mlp.cc \
    model/graph-mlp/layer.cc \
    model/graph-mlp/neuron.cc \
    model/matrix-mlp/matrix_mlp.cc \
    model/matrix-mlp/matrix.cc \
    view/main_form.cc \
    view/scribble_widget.cc \
    view/shared/qcustomplot.cc

HEADERS += \
    controller.h \
    signal_handler.h \
    model/config.h \
    model/letter.h \
    model/metrics.h \
    model/model.h \
    model/neural_network.h \
    model/graph-mlp/graph_mlp.h \
    model/graph-mlp/layer.h \
    model/graph-mlp/neuron.h \
    model/matrix-mlp/matrix_mlp.h \
    model/matrix-mlp/matrix.h \
    view/main_form.h \
    view/scribble_widget.h \
    view/shared/qcustomplot.h

FORMS += \
    view/main_form.ui
