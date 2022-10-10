GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

all: protoc_middleman protoc_grpc_middleman

protoc_middleman: src/geo.proto
	protoc -I src --cpp_out=src geo.proto

protoc_grpc_middleman: src/geo.proto
	protoc -I src --grpc_out=src --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) geo.proto

clean:
	rm -f src/*pb.h
	rm -f src/*pb.cc