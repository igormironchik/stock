
cmake_minimum_required( VERSION 3.1 )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE)
endif( NOT CMAKE_BUILD_TYPE )

SET( CMAKE_CXX_STANDARD 14 )

SET( CMAKE_CXX_STANDARD_REQUIRED ON )

project( Stock.Mobile )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt6Core 6.3.2 REQUIRED )
find_package( Qt6Network 6.3.2 REQUIRED )
find_package( Qt6Qml 6.3.2 REQUIRED )
find_package( Qt6Multimedia 6.3.2 REQUIRED )
find_package( Qt6Gui 6.3.2 REQUIRED )
find_package( Qt6Quick 6.3.2 REQUIRED )

add_definitions( -DCFGFILE_QT_SUPPORT -DCFGFILE_DISABLE_STL )

set( SRC qml_cpp_bridge.hpp
	network.hpp
	cfg_file.hpp
	list_model.hpp
	frames.hpp
	image.hpp
	camera_settings.hpp
    main.cpp
	qml_cpp_bridge.cpp
	network.cpp
	cfg_file.cpp
	list_model.cpp
	frames.cpp
	image.cpp
	camera_settings.cpp
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
	${CMAKE_CURRENT_BINARY_DIR}/config.hpp
	${CMAKE_CURRENT_BINARY_DIR}/camera.hpp
	qml/CameraSettings.qml
    qml/Connect.qml
    qml/Actions.qml
    qml/Busy.qml
    qml/Change.qml
    qml/Message.qml
    qml/Search.qml
    qml/SearchResult.qml
	qml/CameraView.qml
	qml/main.qml
	qml/Licenses.qml
	qml/Bookmark.qml )

set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

qt6_add_resources( SRC resources.qrc )

set_property( SOURCE ${CMAKE_CURRENT_BINARY_DIR}/messages.hpp PROPERTY SKIP_AUTOGEN ON )
set_property( SOURCE ${CMAKE_CURRENT_BINARY_DIR}/config.hpp PROPERTY SKIP_AUTOGEN ON )
set_property( SOURCE ${CMAKE_CURRENT_BINARY_DIR}/camera.hpp PROPERTY SKIP_AUTOGEN ON )

file( READ ${CMAKE_CURRENT_SOURCE_DIR}/../desktop/cfgfile_generator_path CFGFILE_EXECUTABLE )
string( STRIP ${CFGFILE_EXECUTABLE} CFGFILE_EXECUTABLE )

add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
	PRE_BUILD
	COMMAND ${CFGFILE_EXECUTABLE} -i config.cfgfile -o ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/camera.hpp
	PRE_BUILD
	COMMAND ${CFGFILE_EXECUTABLE} -i camera.cfgfile -o ${CMAKE_CURRENT_BINARY_DIR}/camera.hpp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/messages.hpp
	PRE_BUILD
	COMMAND ${CFGFILE_EXECUTABLE} -i ${CMAKE_CURRENT_SOURCE_DIR}/../shared/messages.cfgfile -o ${CMAKE_CURRENT_BINARY_DIR}/messages.hpp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory( 3rdparty/qzxing )

include_directories( ${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/cfgfile
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty
	${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qzxing )

qt_add_executable( Stock.Mobile
	MANUAL_FINALIZATION
	${SRC} )

set_property( TARGET Stock.Mobile APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/android )

target_link_libraries( Stock.Mobile PUBLIC qzxing Qt::Qml Qt::Quick Qt::Multimedia Qt::Gui Qt::Network Qt::Core )

qt_finalize_executable( Stock.Mobile )
