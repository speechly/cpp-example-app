#ifndef IDENTITYCLIENT_H
#define IDENTITYCLIENT_H

namespace speechly {

class IdentityClient {
public:

  static std::string login(std::string apiUrl, std::string appId, std::string deviceId);

};

} // end namespace speechly
#endif //IDENTITYCLIENT_H
