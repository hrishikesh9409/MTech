# Program by Hrishikesh N - AM.EN.P2CSN16008
# 7 - 2 - 2017
# Program to print the nth Fibonacci number using grpc

# Pattern RPC server


# Standard library imports
import concurrent
import json
import os
import sys
import time

# Imports from third party packages
import grpc

# Imports from current project
import fibonacci_pb2
import fibonacci_pb2_grpc

port = int(sys.argv[1])

def fib(number):
	a = 0
	b = 1
	for i in range(number-1):
		a,b = b,a+b
	return a

class FibonacciServer(fibonacci_pb2.FibonacciComputerServicer):
	def __init__(self):
		super(FibonacciServer, self).__init__()
		return

	def GetNthFibonacciNumber(self, request, context):
		number = str(fib(request.number))
		response = fibonacci_pb2.FibResponse(number = number)
		return response

def serve():
    server = grpc.server(concurrent.futures.ThreadPoolExecutor(max_workers=10))
    fibonacci_pb2.add_FibonacciComputerServicer_to_server(FibonacciServer(), server)
    server.add_insecure_port('[::]:%d' % (port))
    server.start()
    try:
        while True:
            time.sleep(60 * 60 * 24)
    except KeyboardInterrupt:
        server.stop(0)

    return


if __name__ == "__main__":
    serve()



