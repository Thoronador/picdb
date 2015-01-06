# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# test - start server
add_test(NAME start_daemon
         COMMAND $<TARGET_FILE:picdbd>)

# Note: The server will be started but NOT stopped during this test,
# potentially leaving it running forever. However, the client-cli testsuite
# has a test named "stop_daemon" which takes care of that. Assuming, the test
# "stop_daemon" always has a corresponding test "start_daemon" run _after_ it,
# this should not be a problem.
# However, it would be better to force a certain order of tests to ensure that
# behaviour.
