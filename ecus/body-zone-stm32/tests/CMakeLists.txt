# Host unit tests for the body zone ECU (no hardware needed).
# Spec 001-body-ecu-bringup, SC-004. Run from this folder:
#   cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure
cmake_minimum_required(VERSION 3.22)
project(body_zone_host_tests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.6.1
    GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(unity)

set(APP ${CMAKE_CURRENT_SOURCE_DIR}/../app)
set(STRICT_FLAGS -Wall -Wextra -Wpedantic -Wconversion -Werror)

enable_testing()

# One executable per module under test. Production sources compile with
# strict warnings; hardware access is replaced by fakes in fakes/.
function(add_unit_test name)
    add_executable(${name} unit/${name}.c ${ARGN})
    target_include_directories(${name} PRIVATE ${APP} ${APP}/bsp ${APP}/sched ${APP}/functions fakes)
    target_link_libraries(${name} PRIVATE unity)
    target_compile_options(${name} PRIVATE ${STRICT_FLAGS})
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_unit_test(test_debounce   ${APP}/bsp/debounce.c)
add_unit_test(test_sched      ${APP}/sched/sched.c)
add_unit_test(test_ecu_state  ${APP}/functions/ecu_state.c fakes/fake_bsp_led.c)
