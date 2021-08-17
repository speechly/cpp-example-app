#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <grpcpp/grpcpp.h>

#include "main/slu.grpc.pb.h"
#include "SpeechlyClient.h"
#include "IdentityClient.h"
#include "SluClient.h"
#include "RootCerts.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using speechly::slu::v1::SLU;
using speechly::slu::v1::SLURequest;
using speechly::slu::v1::SLUResponse;
using speechly::slu::v1::SLUConfig;
using speechly::slu::v1::SLUConfig_Encoding;
using speechly::slu::v1::SLUEvent;
using speechly::slu::v1::SLUEvent_Event;
using speechly::slu::v1::SLUStarted;
using speechly::slu::v1::SLUFinished;
using speechly::slu::v1::SLUIntent;
using speechly::slu::v1::SLUEntity;
using speechly::slu::v1::SLUTranscript;
using speechly::slu::v1::SLUTentativeTranscript;
using speechly::slu::v1::SLUTentativeEntities;

namespace speechly {

SpeechlyClient::SpeechlyClient(std::string apiUrl, std::string appId, std::string deviceId, bool debug) : 
apiUrl(apiUrl), appId(appId), deviceId(deviceId), debug(debug)
{
  initialize();
}

SpeechlyClient::SpeechlyClient(std::string appId, std::string deviceId, bool debug) : 
appId(appId), deviceId(deviceId), debug(debug)
{
  initialize();
}

SpeechlyClient::SpeechlyClient(std::string appId, std::string deviceId) : 
appId(appId), deviceId(deviceId)
{
  initialize();
}

void SpeechlyClient::read()
{
  if (debug)
  {
    std::cout << "reading\n";
  }
  SLUResponse response;
  while (sluClient->stream->Read(&response)) {
    if (response.has_started()) {
      std::cout << "Got started " << response.audio_context() << std::endl;   
    } else if (response.has_intent()) {
      SLUIntent* intent = response.mutable_intent();
    } else if (response.has_transcript()) {
      SLUTranscript* transcript = response.mutable_transcript();
      std::cout << "Got transcript " << transcript->word() << std::endl;
    } else if (response.has_finished()) {
      std::cout << "Got finished " << response.audio_context() << std::endl;
    }
  }
}

void SpeechlyClient::initialize()
{

  if (debug) {
    std::cout << "initializing\n";
  }

  token = IdentityClient::login(apiUrl, appId, deviceId);
  sluClient = new SluClient(apiUrl, token);

  if (debug) {
    std::cout << "initializing\n";
    std::cout << "Bearer " + token << std::endl;
  }

  readThread = std::thread{&SpeechlyClient::read, this};

  mic = new Microphone(sluClient);
  // mic.onAudio = [](int numberOfpackets, float* data) {
    
  // };

}

SpeechlyClient::~SpeechlyClient()
{
}

void SpeechlyClient::start()
{
  if (debug)
  {
    std::cout << "starting\n";
  }
  
  sluClient->start();
  mic->recording = true;
}

void SpeechlyClient::stop()
{
  if (debug)
  {
    std::cout << "stopping\n";
  }
  mic->recording = false;

  sluClient->stop();
}

void SpeechlyClient::toggle()
{
  if (mic->recording)
  {
      stop();   
  }
  else
  {
      start();
  }
}

} // end namespace speechly
