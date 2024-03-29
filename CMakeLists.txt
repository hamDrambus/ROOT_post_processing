cmake_minimum_required(VERSION 3.0)
project(Post_processing)

find_package(Boost 1.33 REQUIRED)
find_package(ROOT 6.04 REQUIRED)
#Note that Minuit2 is required for ROOT! Default Minuit is not thread-safe! 
#For that use -Dminuit2=ON option during ROOT compilation

message(STATUS "ROOT include directories: ${ROOT_INCLUDE_DIRS}")

set(ROOT_DICTIONARY_FILE post_processor_dict.cpp)
set(ROOT_DICTIONARY_HEADER post_processor_rdict.pcm)
#set(ROOT_LIBS -lGui -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lGeom -lm -lSpectrum -lThread)

file(GLOB SOURCES "source/*.cpp")
file(GLOB HEADERS_FULL "include/*.h")
set(HEADERS "")
foreach(src_file ${HEADERS_FULL})
    get_filename_component(src_file_name "${src_file}" NAME)
    list(APPEND HEADERS "${src_file_name}")
endforeach()
#LinkDef.h must be the last for rootcint
list(REMOVE_ITEM HEADERS LinkDef.h)
list(APPEND HEADERS LinkDef.h)
message(STATUS "Header file list: ${HEADERS}")
set (USERNAME $ENV{USER})
message(STATUS "Username: ${USERNAME}")

add_custom_command(
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${ROOT_DICTIONARY_FILE}
    COMMAND rootcint -f ${CMAKE_CURRENT_BINARY_DIR}/${ROOT_DICTIONARY_FILE} -c -I${CMAKE_SOURCE_DIR}/include ${HEADERS}
    DEPENDS ${HEADERS_FULL}
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#use the flags used in ROOT's compilation
set(CMAKE_CXX_FLAGS ${ROOT_CXX_FLAGS})
#use the flags used in ROOT's compilation
set(CMAKE_SHARED_LINKER_FLAGS ${ROOT_EXE_LINKER_FLAGS})
message(STATUS "CXX compilation flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "CXX linking flags: ${CMAKE_SHARED_LINKER_FLAGS}")
message(STATUS "ROOT libraries: ${ROOT_LIBRARIES}")

add_definitions(-DNDEBUG -DR__HAVE_CONFIG)
include_directories(${Boost_INCLUDE_DIR} ${ROOT_INCLUDE_DIRS} include)
add_library(post_processing SHARED ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/${ROOT_DICTIONARY_FILE})
target_link_libraries(post_processing PUBLIC ${ROOT_LIBRARIES})
install(TARGETS post_processing DESTINATION ${CMAKE_BUILD_TYPE})
