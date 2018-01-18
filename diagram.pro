TEMPLATE      = app
HEADERS       = diagramwindow.h \
                link.h \
                node.h \
                propertiesdialog.h \
    helpdialog.h
SOURCES       = diagramwindow.cpp \
                link.cpp \
                main.cpp \
                node.cpp \
                propertiesdialog.cpp \
    helpdialog.cpp
FORMS         = propertiesdialog.ui \
    helpdialog.ui
RESOURCES     = resources.qrc

DISTFILES += \
    images/triangle.png
