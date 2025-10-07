# Get App version from version.txt and put it in APP_VERSION
function(get_version)
    set(file_name "version.txt")
    file(READ "${CMAKE_CURRENT_LIST_DIR}/${file_name}" file_contents)
    string(REGEX MATCH "VER_MAJOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract major version number from ${file_name}")
    endif()
    set(ver_major ${CMAKE_MATCH_1})

    string(REGEX MATCH "VER_MINOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract minor version number from ${file_name}")
    endif()

    set(ver_minor ${CMAKE_MATCH_1})
    string(REGEX MATCH "VER_PATCH ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract patch version number from ${file_name}")
    endif()
    set(ver_patch ${CMAKE_MATCH_1})

    set(APP_VERSION_MAJOR ${ver_major} PARENT_SCOPE)
    set(APP_VERSION_MINOR ${ver_minor} PARENT_SCOPE)
    set(APP_VERSION_PATCH ${ver_patch} PARENT_SCOPE)
    set(APP_VERSION "${ver_major}.${ver_minor}.${ver_patch}" PARENT_SCOPE)
endfunction()

function(copy_dlls_to_target TARGET_NAME DLL_SOURCE_DIR)
	file(GLOB DLL_FILES "${DLL_SOURCE_DIR}/*.dll")
	foreach(DLL_FILE ${DLL_FILES})
		get_filename_component(FILE_NAME ${DLL_FILE} NAME)
		add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E echo "${FILE_NAME} copied!"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${DLL_FILE}"
				"$<TARGET_FILE_DIR:${TARGET_NAME}>"
		)
	endforeach()
endfunction()