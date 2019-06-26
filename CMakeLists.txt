cmake_minimum_required(VERSION 3.13)

set (PROJECT interpriter)

project (${PROJECT})

find_package(PkgConfig REQUIRED)

set (SOURCES
	main.c
    cpu.c
    instructions.c
	)
add_executable (${PROJECT} ${SOURCES})
