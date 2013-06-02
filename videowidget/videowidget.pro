TEMPLATE = app
TARGET = SFT

QT += multimedia multimediawidgets

HEADERS = \
    videoplayer.h \
    syllabe.h \
    syllabemanager.h \
    sentence.h \
    parolestextedit.h

SOURCES = \
    main.cpp \
    videoplayer.cpp \
    syllabe.cpp \
    syllabemanager.cpp \
    sentence.cpp \
    parolestextedit.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/videowidget
INSTALLS += target

QT+=widgets
