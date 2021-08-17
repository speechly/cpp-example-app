#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <AudioToolbox/AudioToolbox.h>
#include "SluClient.h"

namespace speechly {
  
class Microphone {
private:
  short freq = 16000;
  AudioStreamBasicDescription recordFormat;
  AudioQueueRef queue;
  int bufferByteSize = 512;
public:
  Microphone(speechly::SluClient* sluClient);
  bool recording;
  speechly::SluClient* sluClient;
  void start();
  void stop();
  ~Microphone();
};

} // end namespace speechly
#endif //MICROPHONE_H