#include "async_server.h"

#include <string>

void ServerImpl::Run() {

    std::string server_address("0.0.0.0:50051");

    ::grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, ::grpc::InsecureServerCredentials() );
    builder.RegisterService(&service_);

    cq_ = builder.AddCompletionQueue();

    server_ = builder.BuildAndStart();

    std::cout << "Server listening on " << server_address << std::endl;

    HandleRpcs();
}


void ServerImpl::HandleRpcs() {

    new AddCallData( &service_, cq_.get() ) ;
    new MultiplyCallData( &service_, cq_.get() ) ;
    new SumCallData( &service_, cq_.get() ) ;
    
    void* tag;  
    bool ok;

    while ( true ) {

      GPR_ASSERT(cq_->Next(&tag, &ok));
      //GPR_ASSERT(ok);
      static_cast<CallData*>(tag)->Proceed(ok);   
    }
}

ServerImpl::~ServerImpl() {

    server_->Shutdown();
    cq_->Shutdown();

    std::cout << "ServerImpl destructed" << std::endl;
}


ServerImpl::CallData::CallData(::Calc::Calculator::AsyncService* service, ::grpc::ServerCompletionQueue* cq) : service_(service), cq_(cq), status_(CallStatus::CREATE) {}


ServerImpl::CallData::~CallData() {

    std::cout << "CallData destructed" << std::endl;
}

ServerImpl::AddCallData::AddCallData(::Calc::Calculator::AsyncService* service, ::grpc::ServerCompletionQueue* cq) : CallData(service, cq), responder_(&ctx_){
    Proceed(true);
}


void ServerImpl::AddCallData::Proceed(bool ok) {

    if (status_ == CallStatus::CREATE) {

        status_ = CallStatus::PROCESS;
        service_->RequestAddNumbers(&ctx_, &request_, &responder_, cq_, cq_, this);
    } 
    else if (status_ == CallStatus::PROCESS) {
        
        new AddCallData(service_, cq_);

        reply_.set_value(request_.x() + request_.y() ) ;
        status_ = CallStatus::FINISH;
        responder_.Finish(reply_, ::grpc::Status::OK, this);
    } 
    else {
    
        GPR_ASSERT(status_ == CallStatus::FINISH);
        delete this;
    }

}

ServerImpl::AddCallData::~AddCallData() {

    std::cout << "AddCalldata destructed" << std::endl;
}

ServerImpl::MultiplyCallData::MultiplyCallData(::Calc::Calculator::AsyncService* service, ::grpc::ServerCompletionQueue* cq) : CallData(service, cq), responder_(&ctx_) {
    Proceed(true);
}

void ServerImpl::MultiplyCallData::Proceed(bool ok) {

    if (status_ == CallStatus::CREATE) {

        status_ = CallStatus::PROCESS;
        service_->RequestMultiplyNumbers(&ctx_, &request_, &responder_, cq_, cq_, this);
    } 
    else if (status_ == CallStatus::PROCESS) {
        
        new MultiplyCallData(service_, cq_);

        reply_.set_value(request_.x() * request_.y() ) ;
        status_ = CallStatus::FINISH;
        responder_.Finish(reply_, ::grpc::Status::OK, this);
    } 
    else {
    
        GPR_ASSERT(status_ == CallStatus::FINISH);
        delete this;
    }

}

ServerImpl::MultiplyCallData::~MultiplyCallData() {

    std::cout << "MultiplyCallData destructed" << std::endl;
}


ServerImpl::SumCallData::SumCallData(::Calc::Calculator::AsyncService* service, ::grpc::ServerCompletionQueue* cq) : CallData(service, cq), reader_(&ctx_){
    Proceed(true);
}


void ServerImpl::SumCallData::Proceed(bool ok) {

    
    if (status_ == CallStatus::CREATE) {

        status_ = CallStatus::PROCESS;
        service_->RequestSumNumbers(&ctx_, &reader_, cq_, cq_, this);
    } 
    else if (status_ == CallStatus::PROCESS) {

         if (!newIsLaunched) {

             new SumCallData(service_, cq_);
             newIsLaunched = true;
        }

        if (isReading && ok) {
            reader_.Read(&request_, this) ;
            totalSum += request_.value();
        }
        else {
            status_ = CallStatus::FINISH;
        }
        
        if (!ok) {

            reply_.set_value( totalSum );
            reader_.Finish(reply_, ::grpc::Status::OK, this) ;
            isReading = false;
        }

    } 
    else {
    
        GPR_ASSERT(status_ == CallStatus::FINISH);
        delete this;
    }

}

ServerImpl::SumCallData::~SumCallData() {

    std::cout << "SumCallData destructed" << std::endl;
}