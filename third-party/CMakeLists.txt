function(add_cpu_features)
  set(BUILD_TESTING OFF)
  set(BUILD_SHARED_LIBS ON)
  add_subdirectory("cpu_features")
endfunction()

add_cpu_features()
