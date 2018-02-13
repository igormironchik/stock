
TEMPLATE = app

TARGET = Stock

QT += widgets network sql

DEFINES += CFGFILE_QT_SUPPORT

HEADERS = mainwindow.hpp \
    view.hpp \
	options.hpp \
	server.hpp \
	db.hpp \
	by_product_model.hpp \
	by_place_model.hpp

SOURCES = mainwindow.cpp \
    main.cpp \
    view.cpp \
	options.cpp \
	server.cpp \
	db.cpp \
	by_product_model.cpp \
	by_place_model.cpp
    
RESOURCES = resources.qrc

win32 {
    RC_FILE = app.rc
}

OTHER_FILES += config.cfgfile

include( ../../3rdparty/cfgfile/cfgfile/cfgfile.pri )
include( ../shared/shared.pri )

TO_GENERATE = config.cfgfile \
	../shared/messages.cfgfile

QMAKE_EXTRA_COMPILERS += generate_cfg
generate_cfg.name = CONF_GEN
generate_cfg.input = TO_GENERATE
generate_cfg.output = ${QMAKE_FILE_BASE}.hpp
generate_cfg.CONFIG = no_link
generate_cfg.variable_out = HEADERS

generate_cfg.commands = $$shell_path( $$absolute_path( $${OUT_PWD}/../../3rdparty/cfgfile/cfgfile.generator ) ) \
-i ${QMAKE_FILE_IN} -o $${OUT_PWD}/${QMAKE_FILE_BASE}.hpp

PRE_TARGETDEPS += compiler_generate_cfg_make_all

FORMS += \
    options.ui
