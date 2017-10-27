python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. fibonacci.proto
python Fibonacci_Server.py $@