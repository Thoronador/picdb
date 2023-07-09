# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(picdb)

enable_testing()

# Recurse into subdirectory for picdbd
add_subdirectory (picdbv/daemon)

# Recurse into subdirectory for picdb-client-cli
add_subdirectory (picdbv/client-cli)

# Recurse into subdirectory for picdbv
add_subdirectory (picdbv/viewer)

# Recurse into subdirectory for testsuite
add_subdirectory (picdbv/testsuite)
