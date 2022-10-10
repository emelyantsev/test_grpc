

GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`


all: protoc_middleman protoc_grpc_middleman

# run_sync_server:
# 	g++ -I/home/aleksey/.local/include --std=c++14 src/run_sync_server.cpp src/sync_server.cpp src/generated/geo.pb.cc src/generated/geo.grpc.pb.cc -o build/run_sync_server -L/home/aleksey/.local/lib -L/usr/local/lib `pkg-config --libs protobuf grpc++` -lgrpc++ -pthread -lgrpc++_reflection -ldl 


protoc_middleman: src/geo.proto
	protoc -I src --cpp_out=src/generated  geo.proto
#	@touch build/protoc_middleman

protoc_grpc_middleman: src/geo.proto
	protoc -I src --grpc_out=src/generated --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) geo.proto
#	@touch build/protoc_middleman

clean:
	rm -f src/generated/*