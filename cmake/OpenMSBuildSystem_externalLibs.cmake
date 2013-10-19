
## contrib

set(CONTRIB_CUSTOM_DIR CACHE DOC "DEPRECATED: Please use CMAKE_FIND_ROOT_PATH instead! User defined location of contrib dir. If left empty we assume the contrib to be in OpenMS/contrib!")
set(CONTRIB_DIR ${PROJECT_SOURCE_DIR}/contrib/ CACHE INTERNAL "Final contrib path after looking at CMAKE_FIND_ROOT_PATH. Defaults to OpenMS/contrib")

IF ("${CMAKE_FIND_ROOT_PATH}" STREQUAL "")
	IF (NOT "${CONTRIB_CUSTOM_DIR}" STREQUAL "")
		MESSAGE("Please do no longer use -DCONTRIB_CUSTOM_DIR. This option is deprecated. Please use -DCMAKE_FIND_ROOT_PATH instead.")
		LIST(INSERT CONTRIB_DIR 0 ${CONTRIB_CUSTOM_DIR})
	ELSE()
		# Append a few unusual default search directories for convenience
		# if no FIND ROOT PATH was specified.
		LIST(APPEND CONTRIB_DIR /opt/local /usr/local)
	ENDIF()
ENDIF()


# Append all contrib dirs to the (potentially empty) FIND_ROOT_PATH.
# This will be the final search order used by regular CMAKE modules (by default)
# and by the OpenMS macros (via CONTRIB_DIR).
LIST(APPEND CMAKE_FIND_ROOT_PATH "${CONTRIB_DIR}")
SET(TMP "")
FOREACH(CUSTOM_PATH ${CMAKE_FIND_ROOT_PATH})
	GET_FILENAME_COMPONENT(ABS_PATH ${CUSTOM_PATH} ABSOLUTE)
	LIST(APPEND TMP ${ABS_PATH})
ENDFOREACH()
SET(CMAKE_FIND_ROOT_PATH "${TMP}")
SET(CONTRIB_DIR "${CMAKE_FIND_ROOT_PATH}")

MESSAGE(STATUS "CMake find root path: ${CMAKE_FIND_ROOT_PATH}")

set(CONTRIB_INCLUDE_DIR "" CACHE INTERNAL "contrib include dir")
set(CONTRIB_LIB_DIR "" CACHE INTERNAL "contrib lib dir")
foreach(CONTRIB_PATH ${CONTRIB_DIR})
  list(APPEND CONTRIB_INCLUDE_DIR "${CONTRIB_PATH}/include/")
  list(APPEND CONTRIB_LIB_DIR "${CONTRIB_PATH}/lib/")
endforeach()
message(STATUS "Contrib search directories:  ${CONTRIB_DIR}")
message(STATUS "Contrib library directories: ${CONTRIB_LIB_DIR}")
message(STATUS "Contrib include directories: ${CONTRIB_INCLUDE_DIR}")

###########################################################
###							 find libs (for linking)								###
###########################################################
## on windows we need the *.lib versions (dlls alone won't do for linking)
## On Windows: never mix Release/Debug versions of libraries. Leads to strange segfaults, stack corruption etc, due to different runtime libs ...
##             compiler-wise: use the same compiler for contrib and OpenMS!

#set which library extensions are preferred (we prefer shared libraries)
if(NOT MSVC)
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a")
endif()
if (APPLE)
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib;.a")
endif()

OPENMS_CHECKLIB(CONTRIB_XERCESC "xerces-c_3;xerces-c_static_3;libxerces-c;xerces-c" "xerces-c_3D;xerces-c_static_3D;libxerces-c;xerces-c" "xerces_c")

# GSL and GSL-CBLAS
FIND_PACKAGE(GSL)
if (GSL_FOUND)
	include_directories(${GSL_INCLUDE_DIRS})
  message(STATUS "Found GSL version " ${GSL_VERSION_STRING})
else()
  message(FATAL_ERROR "GSL not found!")
endif()

## BOOST
option(BOOST_USE_STATIC "Use Boost static libraries." ON)
set(Boost_USE_STATIC_LIBS ${BOOST_USE_STATIC})
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_DEBUG TRUE)
add_definitions(/DBOOST_ALL_NO_LIB) ## disable auto-linking of boost libs (boost tends to guess wrong lib names)
set(Boost_COMPILER "")
# help boost finding it's packages
set(Boost_ADDITIONAL_VERSIONS "1.47.0" "1.48.0" "1.49.0" "1.50.0" "1.51.0" "1.52.0")
# 1st attempt does not explicitly requires boost to enable second check (see below)
FIND_PACKAGE(Boost 1.42.0 COMPONENTS iostreams date_time math_c99 regex)

if(Boost_FOUND)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  message(STATUS "Found Boost version ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}" )
  set(CF_OPENMS_BOOST_VERSION_MAJOR ${Boost_MAJOR_VERSION})
	set(CF_OPENMS_BOOST_VERSION_MINOR ${Boost_MINOR_VERSION})
  set(CF_OPENMS_BOOST_VERSION_SUBMINOR ${Boost_SUBMINOR_VERSION})
	set(CF_OPENMS_BOOST_VERSION ${Boost_VERSION})
else()
  message(FATAL_ERROR "Boost or one of its components not found!")
endif()

## SEQAN
FIND_PACKAGE(SEQAN 1.4.0)
if(SEQAN_FOUND)
	INCLUDE_DIRECTORIES(${SEQAN_INCLUDE_DIRS})
  message(STATUS "Found SEQAN version ${SEQAN_VERSION_MAJOR}.${SEQAN_VERSION_MINOR}.${SEQAN_VERSION_PATCH}" )
else()
  message(FATAL_ERROR "SeqAn could not be found. Please install it from www.seqan.de or download and install the OpenMS contrib package.")
endif()

## libsvm
if (WIN32) ## find manually on Windows, as find_package() does not know about debug lib
	OPENMS_CHECKLIB(LIBSVM_LIBRARY "libsvm;svm" "libsvmd;svmd;libsvm;svm" "libSVM")
endif()
FIND_PACKAGE(libSVM 2.91) ## will not overwrite LIBSVM_LIBRARY if defined already
if (LIBSVM_FOUND)
	INCLUDE_DIRECTORIES(${LIBSVM_INCLUDE_DIRS})
	message(STATUS "Found LibSVM version " ${LIBSVM_VERSION})
	set(CF_OPENMS_LIBSVM_VERSION_MAJOR ${LIBSVM_MAJOR_VERSION})
	set(CF_OPENMS_LIBSVM_VERSION_MINOR ${LIBSVM_MINOR_VERSION})
	set(CF_OPENMS_LIBSVM_VERSION ${LIBSVM_VERSION})
	set(DEP_LIBSVM_LIBRARY ${LIBSVM_LIBRARY} ${LIBSVM_LIBRARIES}) # combine for consistent use later
else()
	message(FATAL_ERROR "LibSVM not found!")
endif()

if (${USE_COINOR}) 
	set(CF_USECOINOR 1) 
	OPENMS_CHECKLIB(CONTRIB_CBC1 "libCbc;Cbc" "libCbcd;Cbc" "COIN-OR Cbc")
	OPENMS_CHECKLIB(CONTRIB_CBC2 "libCgl;Cgl" "libCgld;Cgl" "COIN-OR Cgl")
	OPENMS_CHECKLIB(CONTRIB_CBC3 "libClp;Clp" "libClpd;Clp" "COIN-OR Clp")
	OPENMS_CHECKLIB(CONTRIB_CBC4 "libCoinUtils;CoinUtils" "libCoinUtilsd;CoinUtils" "COIN-OR Utils")
	OPENMS_CHECKLIB(CONTRIB_CBC5 "libOsi;Osi" "libOsid;Osi" "COIN-OR Osi")
	OPENMS_CHECKLIB(CONTRIB_CBC6 "libOsiClp;OsiClp" "libOsiClpd;OsiClp" "COIN-OR OsiClp")
	set(CONTRIB_CBC ${CONTRIB_CBC1} ${CONTRIB_CBC2} ${CONTRIB_CBC3} ${CONTRIB_CBC4} ${CONTRIB_CBC5} ${CONTRIB_CBC6} )
else()
	set(CF_USECOINOR 0)
	set(CONTRIB_CBC)	
endif()

## Find GLPK
FIND_PACKAGE(GLPK REQUIRED)
if (GLPK_FOUND)
	INCLUDE_DIRECTORIES(${GLPK_INCLUDE_DIRS})
	message(STATUS "Found GLPK version " ${GLPK_VERSION_STRING})
	set(CF_OPENMS_GLPK_VERSION_MAJOR ${GLPK_VERSION_MAJOR})
	set(CF_OPENMS_GLPK_VERSION_MINOR ${GLPK_VERSION_MINOR})
	set(CF_OPENMS_GLPK_VERSION ${GLPK_VERSION_STRING})
else()
	message(FATAL_ERROR "GLPK not found!")
endif()

# Find zlib
FIND_PACKAGE(ZLIB REQUIRED)
if (ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  message(STATUS "Found zlib version ${ZLIB_VERSION_STRING}")
else()
  message(FATAL_ERROR "zlib not found!")
endif()

# Find bzip2
FIND_PACKAGE(BZip2 REQUIRED)
if (BZIP2_FOUND)
  include_directories(${BZIP2_INCLUDE_DIR})
  message(STATUS "Found bzip2 version ${BZIP2_VERSION_STRING}")
else()
  message(FATAL_ERROR "bzip2 not found!")
endif()

if(MSVC)
	## needed to locate libs (put this above ADD_LIBRARY() - otherwise it will not work)
	link_directories(${CONTRIB_LIB_DIR})
endif()

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/include/) ## for configured files, e.g. config.h
INCLUDE_DIRECTORIES(${CONTRIB_INCLUDE_DIR})

#except for the contrib libs, prefer shared libraries
if(NOT MSVC AND NOT APPLE)
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a")
endif()

################################
## QT
################################
SET(QT_MIN_VERSION "4.5.0")
## obsolete when CMake MinRequiredVersion becomes >= 2.8.5
if ("${CMAKE_VERSION}" VERSION_LESS "2.8.5" AND NOT(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")) ##
	set(phonon_extra QtPhonon)
	message(STATUS "Trying to find Phonon explicitly... If you run into trouble with Qt Phonon, try to disable it in build system")
else()
	set(phonon_extra)
	message(STATUS "Not trying to find Phonon explicitly... If you run into trouble with Qt Phonon, try to enable it in build system")
endif()
FIND_PACKAGE(Qt4 REQUIRED QtXML QtNetwork QtSQL QtOpenGL QtSVG QtTest QtWebKit ${phonon_extra})

IF (NOT QT4_FOUND)
  message(STATUS "QT4 not found!")
	message(FATAL_ERROR "To find a custom Qt installation use: cmake <..more options..> -D QT_QMAKE_EXECUTABLE='<path_to_qmake(.exe)' <src-dir>")
ENDIF()
INCLUDE(${QT_USE_FILE})
INCLUDE(UseQt4)
INCLUDE_DIRECTORIES(${QT_INCLUDES})

## FIX
## - QT4 library list (VS2010 requires semicolon separated lists, previous versions use space, thus a lib named "opengl32.lib glu32.lib gdi32.lib user32.lib delayimp.lib" now poses a problem)
## - this fix will be obsolete with CMake 2.8.1
set(QT_LIBRARIES_TMP "")
foreach (qtlib ${QT_LIBRARIES})
	if (${qtlib} STREQUAL "opengl32.lib glu32.lib gdi32.lib user32.lib delayimp.lib")
		list(APPEND QT_LIBRARIES_TMP "opengl32.lib;glu32.lib;gdi32.lib;user32.lib;delayimp.lib")
		message(STATUS "Fixing QT library list... done")
	else()
		list(APPEND QT_LIBRARIES_TMP ${qtlib})
	endif()
endforeach()
set(QT_LIBRARIES ${QT_LIBRARIES_TMP})
## ENDFIX

