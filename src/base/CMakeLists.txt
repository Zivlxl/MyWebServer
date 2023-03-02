set(LIB_SRC
        LogFormatter.cc
        Logging.cc
        Thread.cc)

add_library(libWebServerBase ${LIB_SRC})

target_link_libraries(libWebServerBase pthread rt)

set_target_properties(libWebServerBase PROPERTIES OUTPUT_NAME "WebServerBase")

add_subdirectory(test)