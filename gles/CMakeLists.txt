

cmake_minimum_required(VERSION 3.4.1)

aux_source_directory(src/main/cpp cpp)
add_library(
             hm_gles

             SHARED
             ${cpp}
              )

find_library(
              log-lib
              log )

#GLESv3
target_link_libraries(
                       hm_gles
                       GLESv2
                       GLESv3
                       jnigraphics
                       ${log-lib} )