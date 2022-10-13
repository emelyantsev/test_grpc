#pragma once

#include <memory>

#include <grpcpp/grpcpp.h>

#include "calculator.pb.h"
#include "calculator.grpc.pb.h"

class CalculatorClient {

    public:

        CalculatorClient(std::shared_ptr<::grpc::Channel> channel) ;

        int AddNumbers(int x, int y) const ;
        int MultiplyNumbers(int x, int y) const ;

    private:

        std::unique_ptr<::Calc::Calculator::Stub> stub_;
};