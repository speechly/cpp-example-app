#ifndef SPEECHLYCLIENT_H
#define SPEECHLYCLIENT_H

#include <memory>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "SluClient.h"
#include "Microphone.h"

using grpc::ClientContext;
using grpc::Channel;
using grpc::ClientReaderWriter;

using speechly::slu::v1::SLU;
using speechly::slu::v1::SLURequest;
using speechly::slu::v1::SLUResponse;

namespace speechly {

class SpeechlyClient {
private:

  std::thread readThread;

  std::string apiUrl = "api.speechly.com";

  bool debug = false;

  std::string appId;

  std::string deviceId;

  std::string token;

  speechly::SluClient* sluClient;

  speechly::Microphone* mic;

  void initialize();

public:

  SpeechlyClient(std::string apiUrl, std::string appId, std::string deviceId, bool debug);

  SpeechlyClient(std::string appId, std::string deviceId, bool debug);

  SpeechlyClient(std::string appId, std::string deviceId);

  void start();

  void stop();

  void toggle();

  void read();

  ~SpeechlyClient();

};

} // end namespace speechly
#endif //SPEECHLYCLIENT_H
