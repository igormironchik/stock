
defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

!minQtVersion(5, 15, 0) {
    message("Cannot build StockMobile with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.15.0.")
}

TEMPLATE = app
TARGET = Stock.Mobile
QT += qml multimedia
CONFIG += c++14 qzxing_qml
DEFINES += CFGFILE_QT_SUPPORT CFGFILE_DISABLE_STL

HEADERS = qml_cpp_bridge.hpp \
	network.hpp \
	cfg_file.hpp \
	list_model.hpp \
	frames.hpp \
	image.hpp

SOURCES = main.cpp \
	qml_cpp_bridge.cpp \
	network.cpp \
	cfg_file.cpp \
	list_model.cpp \
	frames.cpp \
	image.cpp

RESOURCES = resources.qrc

DISTFILES += qml/main.qml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    qml/CameraSettings.qml \
    qml/Connect.qml \
    qml/Actions.qml \
    qml/Busy.qml \
    qml/Change.qml \
    qml/Message.qml \
    qml/Search.qml \
    qml/SearchResult.qml \
    qml/CameraView.qml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += android/AndroidManifest.xml

include( ../../3rdparty/qzxing/qzxing/QZXing.pri )
include( ../shared/shared.pri )
include( ../../3rdparty/cfgfile/cfgfile/cfgfile.pri )

TO_GENERATE = ../shared/messages.cfgfile \
	config.cfgfile

QMAKE_EXTRA_COMPILERS += generate_cfg
generate_cfg.name = CONF_GEN
generate_cfg.input = TO_GENERATE
generate_cfg.output = ${QMAKE_FILE_BASE}.hpp
generate_cfg.CONFIG = no_link
generate_cfg.variable_out = HEADERS

CFGFILE_GENERATOR = $$cat( $$PWD/../desktop/cfgfile_generator_path )

generate_cfg.commands = $${CFGFILE_GENERATOR} \
-i ${QMAKE_FILE_IN} -o $${OUT_PWD}/${QMAKE_FILE_BASE}.hpp

PRE_TARGETDEPS += compiler_generate_cfg_make_all

ANDROID_ABIS = armeabi-v7a x86_64
