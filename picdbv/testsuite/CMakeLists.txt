# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# Recurse into subdirectory for daemon-related tests.
add_subdirectory (daemon)

# Recurse into subdirectory for daemon-related tests.
add_subdirectory (client-cli)

# Recurse into subdirectory for escaping test
add_subdirectory (escaping)

# Recurse into subdirectory for Serializer tests.
add_subdirectory (serializer)
