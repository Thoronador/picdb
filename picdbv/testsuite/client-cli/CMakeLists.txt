cmake_minimum_required (VERSION 3.8)

# add client tests
# -- add some simple tests for client-cli
# test for --help parameter
add_test(NAME client_cli_help
         COMMAND $<TARGET_FILE:client_cli> --help)

# test for --version parameter
add_test(NAME client_cli_version
         COMMAND $<TARGET_FILE:client_cli> --version)

# test for stopping the server
add_test(NAME stop_daemon
         COMMAND $<TARGET_FILE:client_cli> stop)
