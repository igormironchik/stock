
cmake_minimum_required( VERSION 3.1 )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE)
endif( NOT CMAKE_BUILD_TYPE )

SET( CMAKE_CXX_STANDARD 14 )

SET( CMAKE_CXX_STANDARD_REQUIRED ON )

project( Stock )

add_subdirectory( 3rdparty/cfgfile/generator )

add_subdirectory( src )

set( CFGFILE_GENERATOR_EXECUTABLE
	${CMAKE_CURRENT_BINARY_DIR}/3rdparty/cfgfile/generator/cfgfile.generator${CMAKE_EXECUTABLE_SUFFIX} )
