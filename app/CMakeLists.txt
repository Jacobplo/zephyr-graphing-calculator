cmake_minimum_required(VERSION 3.31)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(graphing-calculator)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/function.c)
target_sources(app PRIVATE src/stack.c)
target_sources(app PRIVATE ../drivers/LED/led.c)
target_sources(app PRIVATE ../drivers/BTN/btn.c)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../drivers
)
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../drivers/BTN
)
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../drivers/LED
)
