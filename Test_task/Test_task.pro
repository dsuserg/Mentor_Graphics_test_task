TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    parser_base.cpp \
    rule_coexist.cpp \
    functions.cpp \
    rule_same_file_set.cpp \
    rule_base.cpp \
    rule_file_must_contain.cpp \
    parser_simple.cpp

LIBS += -lstdc++fs

DISTFILES += \
    task

HEADERS += \
    parser.h \
    rule.h \
    parser_base.h \
    rule_base.h \
    rule_coexist.h \
    functions.h \
    rule_same_file_set.h \
    rule_file_must_contain.h \
    parser_simple.h
