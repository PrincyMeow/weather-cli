cmake_minimum_required(VERSION 3.16)
project(Weather LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

add_executable(weather
    src/main.cpp
    src/file_utils.cpp
    src/weather_api.cpp
    src/cache_utils.cpp
)

target_link_libraries(weather ${CURL_LIBRARIES})

install(TARGETS weather DESTINATION bin)

#binary file creation .deb and .rpm 
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "weather")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "AristoKat.Apps+weatherApp@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Command-line weather tool")
set(CPACK_PACKAGE_VENDOR "AristoKat")
set(CPACK_PACKAGE_LICENSE "GPL-3.0")

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libcurl4")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "AristoKat")  # Required for .deb
set(CPACK_RPM_PACKAGE_REQUIRES "libcurl")
set(CPACK_RPM_PACKAGE_LICENSE "GPL")           # Required for .rpm

set(CPACK_GENERATOR "DEB;RPM")

include(CPack)
