#include <iostream>
#include <grpcpp/grpcpp.h>

#include "async_client.h"

int main() {

    CalculatorClient client( ::grpc::CreateChannel("localhost:50051", ::grpc::InsecureChannelCredentials() ) ) ;

    int x = 8;
    int y = 4;

    int result = client.AddNumbers(x, y) ;
    std::cout << result << std::endl;
    assert(result == x + y) ;

    result = client.MultiplyNumbers(x, y) ;

    std::cout << result << std::endl;
    assert(result == x * y) ;


    return 0;
}