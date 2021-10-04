cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(GPIO)

add_executable(GPIO Opdracht1_gpio.c)
include_directories(/usr/include/mariadb /usr/include/mariadb/mysql)

target_link_libraries(GPIO mariadb)
target_link_libraries(GPIO PJ_RPI)