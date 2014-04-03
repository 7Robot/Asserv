cmake_minimum_required(VERSION 2.6.0)

project(Asserv C)

add_library(asserv SHARED odo.c pid.c asserv.c motion.c
    ramp_speed.c ramp_dist.c)

add_executable(test_ramp_speed test_ramp_speed.c ramp_speed.c)
add_executable(test_ramp_dist test_ramp_dist.c ramp_dist.c)

target_link_libraries(test_ramp_speed m)
target_link_libraries(test_ramp_dist m)
