#python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. pattern.proto
python Pattern_Client.py $@
