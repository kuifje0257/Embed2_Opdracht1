cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(WiringPi)

add_executable(WiringPi Opdracht1_wiringpi.c)
include_directories(/usr/include/mariadb /usr/include/mariadb/mysql)

target_link_libraries(WiringPi mariadb)

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(WiringPi ${WIRINGPI_LIBRARIES})