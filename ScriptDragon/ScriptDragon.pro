TEMPLATE = app
TARGET = ScriptDragon

load(ubuntu-click)

QT += qml quick printsupport

SOURCES += main.cpp \
    notecarddata.cpp \
    notecardmanager.cpp \
    exportmanager.cpp \
    scriptformatter.cpp \
    filesaverandloader.cpp \
    reportgenerator.cpp \
    namegenerator.cpp

RESOURCES += ScriptDragon.qrc

QML_FILES += $$files(*.qml,true) \
             $$files(*.js,true)

CONF_FILES +=  ScriptDragon.apparmor \
               ScriptDragon.png #Add ScriptDragon.svg here?

AP_TEST_FILES += tests/autopilot/run \
                 $$files(tests/*.py,true)

#show all the files in QtCreator
OTHER_FILES += $${CONF_FILES} \
               $${QML_FILES} \
               $${AP_TEST_FILES} \
               ScriptDragon.desktop

#specify where the config files are installed to
config_files.path = /ScriptDragon
config_files.files += $${CONF_FILES}
INSTALLS+=config_files

#install the desktop file, a translated version is 
#automatically created in the build directory
desktop_file.path = /ScriptDragon
desktop_file.files = $$OUT_PWD/ScriptDragon.desktop
desktop_file.CONFIG += no_check_exist
INSTALLS+=desktop_file

# Default rules for deployment.
target.path = $${UBUNTU_CLICK_BINARY_PATH}
INSTALLS+=target

DISTFILES += \
    CharactersPage.qml \
    LocationsPage.qml \
    RandomGeneratorsPage.qml \
    TextNotecard.qml \
    Character.qml \
    ../po/scriptdragon.dearingj.pot

HEADERS += \
    notecarddata.h \
    notecardmanager.h \
    exportmanager.h \
    scriptformatter.h \
    filesaverandloader.h \
    reportgenerator.h \
    namegenerator.h

#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
