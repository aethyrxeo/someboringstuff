function(addTest)
    set(options "")
    set(oneValueArgs TARGET )
    set(multiValueArgs HEADERS SOURCES)

    cmake_parse_arguments(addTest "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(target_name "${PROJECT_NAME}_${addTest_TARGET}")

    set_source_files_properties(${addTest_SOURCES} PROPERTIES LANGUAGE CXX )
	add_executable(${target_name} "")
	target_sources(${target_name} PRIVATE ${addTest_SOURCES})
	target_include_directories(${target_name} PUBLIC ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include ${addTest_HEADERS})

    if(${Boost_LIBRARY_DIR} NOT STREQUAL "")
	    target_link_libraries(${target_name} PUBLIC ${Boost_LIBRARY_DIR})
    endif()
endfunction(addTest)