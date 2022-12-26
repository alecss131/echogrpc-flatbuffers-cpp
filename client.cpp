#include <grpcpp/grpcpp.h>
#include "greeter.grpc.fb.h"
#include "greeter_generated.h"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    std::string name{"World"};
    if (argc == 2) {
        name = argv[1];
    }
    auto channel{grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials())};
    auto stub{Greeter::NewStub(channel)};
    grpc::ClientContext context{};
    flatbuffers::grpc::MessageBuilder mb{};
    auto name_offset{mb.CreateString(name)};
    auto request_offset{CreateHelloRequest(mb, name_offset)};
    mb.Finish(request_offset);
    auto request_msg{mb.ReleaseMessage<HelloRequest>()};
    flatbuffers::grpc::Message <HelloReply> response_msg{};
    auto status{stub->SayHello(&context, request_msg, &response_msg)};
    if (status.ok()) {
        auto *response{response_msg.GetRoot()};
        std::cout << "Server response: " << response->message()->str() << std::endl;
    } else {
        std::cout << "error" << std::endl;
    }
    return 0;
}