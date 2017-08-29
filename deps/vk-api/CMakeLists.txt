project(vk-api)

find_package(Libcurl REQUIRED)

include_directories(${LIBCURL_INCLUDE_DIRS}
	${OBS_JANSSON_INCLUDE_DIRS})

set(vk-api_HEADERS
	vk-api/api.h
	vk-api/params.h
	vk-api/methods.h)
set(vk-api_SOURCES
	vk-api/api.c
	vk-api/params.c
	vk-api/methods.c)

add_library(vk-api STATIC
	${vk-api_SOURCES}
	${vk-api_HEADERS})

target_include_directories(vk-api
	PUBLIC .)

if(NOT MSVC AND NOT MINGW)
	target_compile_options(vk-api PRIVATE -fPIC)
endif()

target_link_libraries(vk-api
	PRIVATE ${LIBCURL_LIBRARIES}
	PRIVATE ${OBS_JANSSON_IMPORT}
	PRIVATE libobs)
