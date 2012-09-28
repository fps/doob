cmake_minimum_required (VERSION 2.6)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
#set(CMAKE_BUILD_TYPE Release)
project (doob)

set(PROJECT_VERSION "0.0.1")
set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
        | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_command(
	COMMAND xsdcxx cxx-tree --generate-serialization --generate-ostream --output-dir ${PROJECT_BINARY_DIR}/ ${PROJECT_SOURCE_DIR}/doob.xsd
	OUTPUT ${PROJECT_BINARY_DIR}/doob.hxx ${PROJECT_BINARY_DIR}/doob.cxx
	DEPENDS ${PROJECT_SOURCE_DIR}/doob.xsd
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/
)

if(NO_JACK_SESSION) 
	add_definitions(-DNO_JACK_SESSION)
endif()

#add_definitions(-O3 -ffast-math -march=native -mtune=native -funsafe-math-optimizations -funroll-loops)
#add_definitions(-pg)

# add_definitions(-std=c++0x)

set(CMAKE_EXE_LINKER_FLAGS)

find_package(PkgConfig)

find_package(Boost COMPONENTS program_options)

find_package(Qt4)
qt4_wrap_cpp(moc_srcs main_window.h song_view.h qfunctor.h)
include(${QT_USE_FILE})
#add_executable(doob disposable.cc heap.cc  main.cc  main_window.cc  ${PROJECT_BINARY_DIR}/doob.cxx ${moc_srcs})
add_executable(doob main.cc main_window.cc singleton.cc ${moc_srcs})



pkg_check_modules(DOOB samplerate sndfile jack)
find_library(XERCES_C xerces-c)
target_link_libraries(doob ${XERCES_C} ${QT_LIBRARIES} samplerate sndfile jack ${Boost_PROGRAM_OPTIONS_LIBRARY})
include_directories(${QT_INCLUDE_DIRS} ${DOOB_INCLUDE_DIRS} /usr/local/include)
include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})

install(TARGETS doob RUNTIME DESTINATION bin)

