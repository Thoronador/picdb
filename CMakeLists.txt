# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

enable_testing()

# Recurse into subdirectory for picdbd
add_subdirectory (picdbv/daemon)

# Recurse into subdirectory for picdbv
add_subdirectory (picdbv)

# Recurse into subdirectory for testsuite
add_subdirectory (picdbv/testsuite)
