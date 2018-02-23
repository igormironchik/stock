
TEMPLATE = app

TARGET = Stock

QT += widgets network sql

CONFIG += c++11

DEFINES += CFGFILE_QT_SUPPORT

HEADERS = mainwindow.hpp \
    view.hpp \
	options.hpp \
	server.hpp \
	db.hpp \
	by_product_model.hpp \
	by_place_model.hpp \
	db_signals.hpp \
	product.hpp \
	by_product_sort_model.hpp \
	by_place_view.hpp \
	by_product_view.hpp \
	by_place_sort_model.hpp \
	sort_filter_base_model.hpp \
	rename.hpp \
	edit_desc.hpp

SOURCES = mainwindow.cpp \
    main.cpp \
    view.cpp \
	options.cpp \
	server.cpp \
	db.cpp \
	by_product_model.cpp \
	by_place_model.cpp \
	db_signals.cpp \
	product.cpp \
	by_product_sort_model.cpp \
	by_place_view.cpp \
	by_product_view.cpp \
	by_place_sort_model.cpp \
	sort_filter_base_model.cpp \
	rename.cpp \
	edit_desc.cpp
    
RESOURCES = resources.qrc

EXE_EXT =

win32 {
    RC_FILE = app.rc
	EXE_EXT = .exe
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

CFGFILE_GENERATOR = $$shell_path( $$absolute_path( $${OUT_PWD}/../../3rdparty/cfgfile/cfgfile.generator$${EXE_EXT} ) )

write_file( $${PWD}/cfgfile_generator_path, CFGFILE_GENERATOR)

generate_cfg.commands = ${CFGFILE_GENERATOR} \
-i ${QMAKE_FILE_IN} -o $${OUT_PWD}/${QMAKE_FILE_BASE}.hpp

PRE_TARGETDEPS += compiler_generate_cfg_make_all

FORMS += \
    options.ui \
    product.ui \
    rename.ui \
    edit_desc.ui
