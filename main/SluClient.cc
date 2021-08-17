#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <grpcpp/grpcpp.h>

#include "main/slu.grpc.pb.h"
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

class MyCustomAuthenticator : public grpc::MetadataCredentialsPlugin {
 public:
  MyCustomAuthenticator(const grpc::string& ticket) : ticket_(ticket) {}

  grpc::Status GetMetadata(
      grpc::string_ref service_url, grpc::string_ref method_name,
      const grpc::AuthContext& channel_auth_context,
      std::multimap<grpc::string, grpc::string>* metadata) override {
    metadata->insert(std::make_pair("authorization", ticket_));
    return grpc::Status::OK;
  }

 private:
  grpc::string ticket_;
};

SluClient::SluClient(std::string apiUrl, std::string token)
{
  grpc::SslCredentialsOptions ssl_opts;
  ssl_opts.pem_root_certs = RootCerts::getCerts();
  auto channel_creds = grpc::SslCredentials(ssl_opts);

  std::shared_ptr<grpc::CallCredentials> call_creds = grpc::MetadataCredentialsFromPlugin(
    std::unique_ptr<grpc::MetadataCredentialsPlugin>(
        new MyCustomAuthenticator("Bearer " + token)));

  sluChannel = grpc::CreateChannel(apiUrl, grpc::CompositeChannelCredentials(channel_creds, call_creds));
  sluStub_ = slu::v1::SLU::NewStub(sluChannel);

  std::shared_ptr<ClientReaderWriter<SLURequest, SLUResponse> > streamPointer(sluStub_->Stream(&context));
  
  stream = streamPointer;

  SLUConfig config;
  config.set_encoding(SLUConfig_Encoding::SLUConfig_Encoding_LINEAR16);
  config.set_sample_rate_hertz(16000);
  config.set_language_code("en-US");

  SLURequest sluRequest;
  sluRequest.mutable_config()->CopyFrom(config);

  stream->Write(sluRequest);
}

void SluClient::start()
{
  SLUEvent event;
  event.set_event(slu::v1::SLUEvent_Event_START);
  SLURequest sluRequest;
  sluRequest.mutable_event()->CopyFrom(event);
  stream->Write(sluRequest);
}

void SluClient::stop()
{
  SLUEvent event;
  event.set_event(slu::v1::SLUEvent_Event_STOP);
  SLURequest sluRequest;
  sluRequest.mutable_event()->CopyFrom(event);
  stream->Write(sluRequest);
}


void SluClient::sendAudio(int* audio, int number)
{
  SLURequest sluRequest;
  sluRequest.set_audio(audio, number*2);
  stream->Write(sluRequest);
}

} // end namespace speechly