# Configure outside librarys' include path here.
# This project needs libchardet, LeafLogger, SetOperation.
# Qt Creator's "Add Library" function can help.
# Don't forget to rename this file to "outsideLibrary.pri"

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../LeafLogger-release/ -lLeafLogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../LeafLogger-release/ -lLeafLoggerd
else:unix: LIBS += -L$$PWD/../LeafLogger-release/ -lLeafLogger

INCLUDEPATH += $$PWD/../LeafLogger-release/include
DEPENDPATH += $$PWD/../LeafLogger-release/include
