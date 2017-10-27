# Fibonacci RPC client

# Standard library imports
import json
import sys

# Imports from third party packages
import grpc

# Imports from current project
import fibonacci_pb2


def run():
    if len(sys.argv) != 4:
        print("Usage: %s HOST PORT TEST_CASES_FILE" % (sys.argv[1]))
        sys.exit(0)

    host = sys.argv[1]
    port = sys.argv[2]
    tests_file = sys.argv[3]
    fp = open(tests_file)
    tests = json.load(fp)
    fp.close()
    channel = grpc.insecure_channel('%s:%s' % (host, port))
    stub = fibonacci_pb2.FibonacciComputerStub(channel)
    success = True
    for index in tests:
        result = str(tests[index])
        response = stub.GetNthFibonacciNumber(fibonacci_pb2.FibRequest(number=int(index)))
        if response.number != result:
            success = False
            break

    if success:
        print("Success")
    else:
        print("Failure")
    return


if __name__ == "__main__":
    run()
