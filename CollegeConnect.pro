QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminmenu.cpp \
    database.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    map.cpp \
    schoolstore.cpp \
    shoppingcart.cpp \
    tripplanner.cpp

HEADERS += \
    adminmenu.h \
    database.h \
    login.h \
    mainwindow.h \
    map.h \
    schoolstore.h \
    shoppingcart.h \
    tripplanner.h

FORMS += \
    adminmenu.ui \
    login.ui \
    mainwindow.ui \
    schoolStore.ui \
    shoppingcart.ui \
    tripplanner.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    Distances.csv \
    NewSouvenirs.csv \
    NewUniversities.csv \
    Souvenirs.csv \
    resources/CC_logo.png \
    resources/CC_logo_mini-removebg.png \
    resources/arrow_down-removebg.png \
    resources/arrow_down.png \
    resources/arrow_up-removebg.png \
    resources/arrow_up.png \
    resources/bubble-removebg.png \
    resources/bubble.png \
    resources/down-arrow-icon.png \
    resources/menu-removebg.png \
    resources/plan_route_logo-removebg.png \
    resources/plan_route_logo.png \
    resources/school_store_logo-removebg.png \
    resources/school_store_logo.png \
    resources/up-arrow-icon.png

RESOURCES += \
    resources/resources.qrc
