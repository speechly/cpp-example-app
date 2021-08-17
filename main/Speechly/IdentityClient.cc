#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>

#include "main/Speechly/IdentityClient.h"
#include "main/Speechly/protos/identity.grpc.pb.h"
#include "main/RootCerts.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using speechly::identity::v1::Identity;
using speechly::identity::v1::LoginRequest;
using speechly::identity::v1::LoginResponse;

namespace speechly {

std::string IdentityClient::login(std::string apiUrl, std::string appId, std::string deviceId)
{
  grpc::SslCredentialsOptions ssl_opts;
  ssl_opts.pem_root_certs = RootCerts::getCerts();
  auto channel_creds = grpc::SslCredentials(ssl_opts);
  std::shared_ptr<Channel> identityChannel = grpc::CreateChannel(apiUrl, channel_creds);
  std::unique_ptr<Identity::Stub> identityStub_ = Identity::NewStub(identityChannel);
  LoginRequest request;
  request.set_app_id(appId);
  request.set_device_id(deviceId);
  // request.set_language_code("en-US");
  
  LoginResponse response;
  ClientContext loginContext;
  Status status = identityStub_->Login(&loginContext, request, &response);

  if (!status.ok()) {
    std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    std::cout << "Login RPC failed"  << std::endl;
  }
  return response.token();
}

} // end namespace speechly
