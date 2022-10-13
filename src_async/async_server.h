#pragma once

#include <grpcpp/grpcpp.h>

#include "calculator.pb.h"
#include "calculator.grpc.pb.h"

class ServerImpl final {

    public:

        ~ServerImpl() ;

        void Run();

    private:

        class CallData {

            public:

                CallData(::Calc::Calculator::AsyncService* service, ::grpc::ServerCompletionQueue* cq) ;

                virtual ~CallData();

                virtual void Proceed(bool ok) = 0;

            protected:

                ::Calc::Calculator::AsyncService* service_;
                ::grpc::ServerCompletionQueue* cq_;

                enum class CallStatus {CREATE, PROCESS, FINISH} ;

                CallStatus status_;
            
        };


        class AddCallData final : public CallData {

            public:

                AddCallData(::Calc::Calculator::AsyncService* service, ::grpc::ServerCompletionQueue* cq) ;
                void Proceed(bool ok) override ;
                virtual ~AddCallData() ;

            private:

                ::grpc::ServerContext ctx_;
                ::Calc::NumbersRequest request_;
                ::Calc::NumbersResponse reply_;
                ::grpc::ServerAsyncResponseWriter<::Calc::NumbersResponse> responder_;

        };


        class MultiplyCallData final : public CallData {

            public:

                MultiplyCallData(::Calc::Calculator::AsyncService* service, ::grpc::ServerCompletionQueue* cq) ;
                void Proceed(bool ok) override ;
                virtual ~MultiplyCallData() ;

            private:

                private:

                ::grpc::ServerContext ctx_;
                ::Calc::NumbersRequest request_;
                ::Calc::NumbersResponse reply_;
                ::grpc::ServerAsyncResponseWriter<::Calc::NumbersResponse> responder_;
        };

        class SumCallData final : public CallData {

            public:

                SumCallData(::Calc::Calculator::AsyncService* service, ::grpc::ServerCompletionQueue* cq) ;
                void Proceed(bool ok) override ;

                virtual ~SumCallData() ;

            private:
                
                ::grpc::ServerContext ctx_;
                ::Calc::NumberRequest request_;
                ::Calc::NumbersResponse reply_;
                ::grpc::ServerAsyncReader<::Calc::NumbersResponse, ::Calc::NumberRequest> reader_;

                bool isReading = true;
                bool newIsLaunched = false;
                int totalSum = 0;

        };

    private:

        void HandleRpcs();

    private:

        std::unique_ptr<::grpc::ServerCompletionQueue> cq_;
        ::Calc::Calculator::AsyncService service_;
        std::unique_ptr<::grpc::Server> server_;
};


