#ifndef SLUCLIENT_H
#define SLUCLIENT_H

#include <memory>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "main/Speechly/protos/slu.grpc.pb.h"

using grpc::ClientContext;
using grpc::Channel;
using grpc::ClientReaderWriter;

using speechly::slu::v1::SLU;
using speechly::slu::v1::SLURequest;
using speechly::slu::v1::SLUResponse;

namespace speechly {

class SluClient {
private:
  ClientContext context;
  
  std::unique_ptr<SLU::Stub> sluStub_;

  std::shared_ptr<Channel> sluChannel;

public:

  SluClient(std::string apiUrl, std::string token);

  std::shared_ptr<ClientReaderWriter<SLURequest, SLUResponse> > stream;

  void start();

  void stop();

  void sendAudio(int* audio, int number);

};

} // end namespace speechly
#endif //SLUCLIENT_H