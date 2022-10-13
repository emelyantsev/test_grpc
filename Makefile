GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

all: protoc_middleman protoc_grpc_middleman protoc_middleman2 protoc_grpc_middleman2

protoc_middleman: src/geo.proto
	protoc -I src --cpp_out=src geo.proto

protoc_grpc_middleman: src/geo.proto
	protoc -I src --grpc_out=src --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) geo.proto

protoc_middleman2: src_async/calculator.proto
	protoc -I src_async --cpp_out=src_async calculator.proto

protoc_grpc_middleman2: src_async/calculator.proto
	protoc -I src_async --grpc_out=src_async --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) calculator.proto

clean:
	rm -f src/*pb.h
	rm -f src/*pb.cc
	rm -f src_async/*pb.h
	rm -f src_async/*pb.cc