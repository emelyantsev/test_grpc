#include "sync_server.h"

#include <iostream>
#include <string>
#include <memory>
#include <csignal>
#include <thread>


// grpc::Server* ptr_server = nullptr;

// void signal_handler(int signal) {


//   std::cout << "Handle signal" << std::endl;

//   if (ptr_server != nullptr) {

//     std::thread t( &grpc::ServerInterface::Shutdown, ptr_server );
//   }
// }



void RunServer() {
  
  std::string server_address("0.0.0.0:50051");
  
  GeoManagerImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  
  //ptr_server = server.get();
  //std::signal(SIGINT, signal_handler) ;

  std::cout << "Server listening on " << server_address << std::endl;
  
  server->Wait();


  std::cout << "Graceful shutdown after sigint" << std::endl;
}

int main() {


    RunServer();


    return 0;
}