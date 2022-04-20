QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    main.cpp \
    mainwindow.cpp \
    map.cpp

HEADERS += \
    database.h \
    mainwindow.h \
    map.h

FORMS += \
    mainwindow.ui \
    schoolStore.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    Distances.csv \
    New Distances.csv \
    New Distances.csv \
    New Souvenirs.csv \
    New Souvenirs.csv \
    Souvenirs.csv \
    resources/CC_logo.png \
    resources/CC_logo_mini-removebg.png \
    resources/down-arrow-icon.png \
    resources/plan_route_logo.png \
    resources/school_store_logo.png \
    resources/up-arrow-icon.png

RESOURCES += \
    resources/resources.qrc
