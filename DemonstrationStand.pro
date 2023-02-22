QT       += core gui serialport network opengl bluetooth printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    architect/architect.cpp \
    architect/threaddependobject.cpp \
    channels/abstractadapter.cpp \
    channels/bluetoothadapter.cpp \
    channels/serialportadapter.cpp \
    channels/tcpadapter.cpp \
    channels/udpadapter.cpp \
    factories/adapterfactory.cpp \
    helpers/crchelper.cpp \
    helpers/filewriterhelper.cpp \
    helpers/loghelper.cpp \
    helpers/mathhelper.cpp \
    helpers/nonlockwaiter.cpp \
    helpers/openchannelhelper.cpp \
    helpers/settingshelper.cpp \
    main.cpp \
    navigation/calibrationmanager.cpp \
    navigation/filtermanager.cpp \
    navigation/navigationalgorithmmanager.cpp \
    navigation/navigationmanager.cpp \
    navigation/ustmanager.cpp \
    protocols/mainprotocolhelper.cpp \
    ui/displayform.cpp \
    ui/graphsform.cpp \
    ui/graphwindow.cpp \
    ui/listitemdelegate.cpp \
    ui/mainwindow.cpp \
    ui/modelingform.cpp \
    ui/qcustomplot.cpp \
    ui/settingsform.cpp \
    ui/yprpainter.cpp


HEADERS += \
    architect/architect.h \
    architect/threaddependobject.h \
    channels/abstractadapter.h \
    channels/bluetoothadapter.h \
    channels/serialportadapter.h \
    channels/tcpadapter.h \
    channels/udpadapter.h \
    defines/adapterdefines.h \
    defines/applicationconstants.h \
    defines/navigationstructs.h \
    defines/registrykeys.h \
    factories/adapterfactory.h \
    helpers/crchelper.h \
    helpers/filewriterhelper.h \
    helpers/loghelper.h \
    helpers/mathhelper.h \
    helpers/nonlockwaiter.h \
    helpers/openchannelhelper.h \
    helpers/settingshelper.h \
    navigation/calibrationmanager.h \
    navigation/filtermanager.h \
    navigation/navigationalgorithmmanager.h \
    navigation/navigationmanager.h \
    navigation/ustmanager.h \
    protocols/mainprotocol.h \
    protocols/mainprotocolhelper.h \
    ui/displayform.h \
    ui/graphsform.h \
    ui/graphwindow.h \
    ui/listitemdelegate.h \
    ui/mainwindow.h \
    ui/modelingform.h \
    ui/qcustomplot.h \
    ui/settingsform.h \
    ui/yprpainter.h \
    version/version.h \
    version/versioninfo.h

FORMS += \
    ui/displayform.ui \
    ui/graphsform.ui \
    ui/graphwindow.ui \
    ui/listitemdelegate.ui \
    ui/mainwindow.ui \
    ui/modelingform.ui \
    ui/settingsform.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    README.md \
    resources/mainIcon.ico \
    resources/resources.rc \
    setup/bundle.wxs \
    setup/license.rtf \
    setup/setup.wxs

RC_FILE += resources/resources.rc

RESOURCES += \
    resources/resources.qrc

