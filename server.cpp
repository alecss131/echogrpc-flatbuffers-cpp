#include <grpcpp/grpcpp.h>
#include "greeter.grpc.fb.h"
#include "greeter_generated.h"
#include <iostream>
#include <string>

class GreeterServiceImpl final : public Greeter::Service {
    grpc::Status SayHello(
            grpc::ServerContext *context,
            const flatbuffers::grpc::Message <HelloRequest> *request_msg,
            flatbuffers::grpc::Message <HelloReply> *response_msg) override {
        const HelloRequest *request{request_msg->GetRoot()};
        const std::string &name{request->name()->str()};
        std::cout << "New client request with name: " << name << std::endl;
        flatbuffers::grpc::MessageBuilder mb{};
        auto msg_offset{mb.CreateString("Hello, " + name + "!")};
        auto hello_offset{CreateHelloReply(mb, msg_offset)};
        mb.Finish(hello_offset);
        *response_msg = mb.ReleaseMessage<HelloReply>();
        return grpc::Status::OK;
    }
};

int main() {
    GreeterServiceImpl service{};
    grpc::ServerBuilder builder{};
    builder.AddListeningPort("127.0.0.1:50051", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    auto server{builder.BuildAndStart()};
    std::cout << "Server listening on port: 50051" << std::endl;
    server->Wait();
    return 0;
}
