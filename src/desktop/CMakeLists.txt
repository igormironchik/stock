
project( Stock.Desktop )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt6Core REQUIRED )
find_package( Qt6Widgets REQUIRED )
find_package( Qt6Gui REQUIRED )
find_package( Qt6Network REQUIRED )
find_package( Qt6Sql REQUIRED )

add_definitions( -DCFGFILE_QT_SUPPORT )

set( SRC mainwindow.hpp
    view.hpp
	options.hpp
	server.hpp
	db.hpp
	by_product_model.hpp
	by_place_model.hpp
	db_signals.hpp
	product.hpp
	by_product_sort_model.hpp
	by_place_view.hpp
	by_product_view.hpp
	by_place_sort_model.hpp
	sort_filter_base_model.hpp
	rename.hpp
	edit_desc.hpp
	amount.hpp
	word_wrap_delegate.hpp
    mainwindow.cpp
    main.cpp
    view.cpp
	options.cpp
	server.cpp
	db.cpp
	by_product_model.cpp
	by_place_model.cpp
	db_signals.cpp
	product.cpp
	by_product_sort_model.cpp
	by_place_view.cpp
	by_product_view.cpp
	by_place_sort_model.cpp
	sort_filter_base_model.cpp
	rename.cpp
	edit_desc.cpp
	amount.cpp
	word_wrap_delegate.cpp
	options.ui
    product.ui
    rename.ui
    edit_desc.ui
    amount.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/tcp_socket.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/datagrams.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/constants.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/exceptions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/tcp_socket.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/exceptions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/buffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../shared/datagrams.cpp
	${CMAKE_CURRENT_BINARY_DIR}/messages.hpp
	${CMAKE_CURRENT_BINARY_DIR}/config.hpp )

set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

qt6_add_resources( SRC resources.qrc )

set_property( SOURCE ${CMAKE_CURRENT_BINARY_DIR}/messages.hpp PROPERTY SKIP_AUTOGEN ON )
set_property( SOURCE ${CMAKE_CURRENT_BINARY_DIR}/config.hpp PROPERTY SKIP_AUTOGEN ON )

add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
	PRE_BUILD
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../3rdparty/cfgfile/generator/${CMAKE_CFG_INTDIR}/cfgfile.generator${CMAKE_EXECUTABLE_SUFFIX} -i config.cfgfile -o ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/messages.hpp
	PRE_BUILD
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../3rdparty/cfgfile/generator/${CMAKE_CFG_INTDIR}/cfgfile.generator${CMAKE_EXECUTABLE_SUFFIX} -i ${CMAKE_CURRENT_SOURCE_DIR}/../shared/messages.cfgfile -o ${CMAKE_CURRENT_BINARY_DIR}/messages.hpp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set( CFGFILE_GENERATOR_EXECUTABLE
	${CMAKE_CURRENT_BINARY_DIR}/../../3rdparty/cfgfile/generator/cfgfile.generator${CMAKE_EXECUTABLE_SUFFIX} )

configure_file( cfgfile_generator_path.in ${CMAKE_CURRENT_SOURCE_DIR}/cfgfile_generator_path )

if( WIN32 )
	list( APPEND SRC app.rc )
endif()

include_directories( ${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/cfgfile
	${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/widgets/include
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty )

add_executable( Stock.Desktop.App WIN32 ${SRC} )

target_link_libraries( Stock.Desktop.App widgets Qt6::Widgets Qt6::Gui Qt6::Network Qt6::Sql Qt6::Core )

add_dependencies( Stock.Desktop.App cfgfile.generator )
