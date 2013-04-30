APP_NAME = AIMaze

CONFIG += qt warn_on cascades10
INCLUDEPATH +=  $$quote($$_PRO_FILE_PWD_/oggvorbis)
LIBS += -L$$quote($$_PRO_FILE_PWD_/oggvorbis) -lvorbis -lbbmultimedia -lbbdata -lbbdevice -lOpenAL -lalut -lasound

include(config.pri)
