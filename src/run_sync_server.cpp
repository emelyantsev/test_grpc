#include "sync_server.h"

#include <iostream>
#include <string>
#include <memory>


void RunServer(const std::string& address, const std::string& port) {
  
  std::string server_address = address + ":" + port;
  
  GeoManagerImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  
  std::cout << "Server listening on " << server_address << std::endl;
  
  server->Wait();

}

int main(int argc, char* argv[]) {

    std::string addressStr = "0.0.0.0";
    std::string portStr = "50051";

    if (argc > 1) {

        addressStr = argv[1] ;
    }

    if (argc > 2) {

        portStr = argv[2] ;
    }

    RunServer(addressStr, portStr);

    return 0;
}