#include <myproto/message.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <iostream>

class MessageService final : public protobenchmark::Messages::Service {
public:
  virtual ::grpc::Status GetM0(::grpc::ServerContext *context,
                               const google::protobuf::Empty *request,
                               google::protobuf::Empty *result) {
    std::cout << "Server: Received M0 call " << std::endl;
    return grpc::Status::OK;
  }

  virtual ::grpc::Status GetM1(::grpc::ServerContext *context,
                               const ::protobenchmark::M1 *request,
                               ::protobenchmark::M1 *m1) {
    std::cout << "Server: Received M1 call with " << std::endl;
    // request->arg1()
    //           << std::endl;
    // m1->set_arg1(1 + request->arg1());
    return grpc::Status::OK;
  }

  virtual ::grpc::Status GetM2(::grpc::ServerContext *context,
                               const ::protobenchmark::M2 *request,
                               ::protobenchmark::M2 *m2) {
    std::cout << "Server: Received M2 call " << std::endl;
    // m2->set_arg1(1);
    // m2->set_arg2(2);
    return grpc::Status::OK;
  }

  virtual ::grpc::Status GetM3(::grpc::ServerContext *context,
                               const ::protobenchmark::M3 *request,
                               ::protobenchmark::M3 *m3) {
    std::cout << "Server: Received M3 call " << std::endl;
    // m3->set_arg1(1);
    // m3->set_arg2(2);
    // m3->set_arg3(3);
    return grpc::Status::OK;
  }
};

int main(int argc, char *argv[]) {
  std::cout << "Server starting!" << std::endl;
  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

  MessageService message_service;
  builder.RegisterService(&message_service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();

  return 0;
}