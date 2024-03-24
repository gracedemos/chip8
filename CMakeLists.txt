cmake_minimum_required(VERSION 3.5)

project(chip8)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

if(LINUX)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	include_directories(${GTK3_INCLUDE_DIRS})
endif()

include_directories(${SDL2_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/nativefiledialog/src/include)

add_executable(
	${PROJECT_NAME}
	src/main.c
	src/app.c
	src/display.c
	src/cpu.c
	src/rom.c
)

if(LINUX)
	add_library(
		nfd
		STATIC
		nativefiledialog/src/nfd_common.c
		nativefiledialog/src/nfd_gtk.c
	)
endif()

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} nfd)

if(LINUX)
	target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})
endif()
