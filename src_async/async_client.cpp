#include "async_client.h"


CalculatorClient::CalculatorClient(std::shared_ptr<::grpc::Channel> channel) : stub_(::Calc::Calculator::NewStub(channel) ) {}

int CalculatorClient::AddNumbers(int x, int y) const {

    ::Calc::NumbersRequest request;

    request.set_x(x) ;
    request.set_y(y) ;

    ::Calc::NumbersResponse reply;

    ::grpc::ClientContext context;
    ::grpc::CompletionQueue cq;

    ::grpc::Status status;

    std::unique_ptr<::grpc::ClientAsyncResponseReader<::Calc::NumbersResponse> > rpc(
        stub_->AsyncAddNumbers(&context, request, &cq) 
    );

    rpc->Finish(&reply, &status, (void*) 1) ;

    void* got_tag;
    bool ok = false;
    
    GPR_ASSERT(cq.Next(&got_tag, &ok));

    GPR_ASSERT(got_tag == (void*)1);
    GPR_ASSERT(ok);

    if (status.ok()) {

      return reply.value();
    } 
    else {

      return 0;
    }
}

int CalculatorClient::MultiplyNumbers(int x, int y) const {

    ::Calc::NumbersRequest request;

    request.set_x(x) ;
    request.set_y(y) ;

    ::Calc::NumbersResponse reply;

    ::grpc::ClientContext context;
    ::grpc::CompletionQueue cq;

    ::grpc::Status status;

    std::unique_ptr<::grpc::ClientAsyncResponseReader<::Calc::NumbersResponse> > rpc(
        stub_->AsyncMultiplyNumbers(&context, request, &cq) 
    );

    rpc->Finish(&reply, &status, (void*) 1) ;

    void* got_tag;
    bool ok = false;
    
    GPR_ASSERT(cq.Next(&got_tag, &ok));

    GPR_ASSERT(got_tag == (void*)1);
    GPR_ASSERT(ok);

    if (status.ok()) {

      return reply.value();
    } 
    else {

      return 0;
    }
}