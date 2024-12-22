cmake_minimum_required( VERSION 3.28 )


project( jadecpp LANGUAGES CXX )

set( PROJECT_NAME jadecpp )
set( PROJECT_BRIEF "A modern GUI framework working with C++Elements for the jade markup language and jinja-like programming." )
set( PROJECT_VERSION "0.1.0" )

set( CMAKE_CXX_STANDARD 26 )

if( UNIX )
    add_definitions( -fpermissive )
endif()

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

include_directories( src )

file( GLOB SRCFILES src/*.cpp )
add_library( jadecpp STATIC ${SRCFILES} )

set( ENABLE_DOC_jadecpp TRUE )
if( ENABLE_DOC_jadecpp )
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		message("-- Enabled doc_jadecpp target for make")
        # CMAKE_CURRENT_SOURCE_DIR is where the last call to CMake's project was,
        # CMAKE_CURRENT_BINARY_DIR is where the build of the binaries is performed
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		add_custom_target(doc_jadecpp ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating documentation with Doxygen")
	endif()
endif()