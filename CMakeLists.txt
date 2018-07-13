cmake_minimum_required (VERSION 3.11)
project(modbus_analyzer)

# custom CMake Modules are located in the cmake directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

set(SOURCES 
src/ModbusAnalyzer.cpp
src/ModbusAnalyzer.h
src/ModbusAnalyzerModbusExtension.h
src/ModbusAnalyzerResults.cpp
src/ModbusAnalyzerResults.h
src/ModbusAnalyzerSettings.cpp
src/ModbusAnalyzerSettings.h
src/ModbusSimulationDataGenerator.cpp
src/ModbusSimulationDataGenerator.h
)

add_analyzer_plugin(modbus_analyzer SOURCES ${SOURCES})
