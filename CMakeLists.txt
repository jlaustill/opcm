# !!! WARNING !!! AUTO-GENERATED FILE, PLEASE DO NOT MODIFY IT AND USE
# https://docs.platformio.org/page/projectconf/section_env_build.html#build-flags
#
# If you need to override existing CMake configuration or add extra,
# please create `CMakeListsUser.txt` in the root of project.
# The `CMakeListsUser.txt` will not be overwritten by PlatformIO.

cmake_minimum_required(VERSION 3.13)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project("npcm" C CXX)

include(CMakeListsPrivate.txt)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeListsUser.txt)
include(CMakeListsUser.txt)
endif()

add_custom_target(
    Production ALL
    COMMAND platformio -c clion run "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    Debug ALL
    COMMAND platformio -c clion run --target debug "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Z_DUMMY_TARGET ${SRC_LIST} src/Display/OBD2/OBD2.cpp src/Display/OBD2/OBD2.h include/AppData.h src/Data/Sensors/TransmissionTemperatureSensor.cpp src/Data/Sensors/TransmissionTemperatureSensor.h src/Data/SpeedometerInput.cpp src/Data/SpeedometerInput.h src/Configuration.h src/Data/TachometerInput60Minus2.cpp src/Data/TachometerInput60Minus2.h src/Data/Sensors/TransmissionPressureSensor.cpp src/Data/Sensors/TransmissionPressureSensor.h src/Data/CumminsBus.cpp src/Data/CumminsBus.h src/Display/CanBus.cpp src/Display/CanBus.h src/Display/Nextion.cpp src/Display/Nextion.h)
