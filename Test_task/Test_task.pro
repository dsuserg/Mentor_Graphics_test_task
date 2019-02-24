TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    rules_base.cpp \
    parser_base.cpp \
    rule_coexist.cpp

LIBS += -lstdc++fs

DISTFILES += \
    task

HEADERS += \
    parser.h \
    rule.h \
    parser_base.h \
    rule_base.h \
    parser_dir.h \
    parser_file.h \
    rule_coexist.h
