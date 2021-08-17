#include <iostream>
#include <AudioToolbox/AudioToolbox.h>
#include "Microphone.h"

using speechly::Microphone;

static void CheckError(OSStatus error, const char *operation) {
  if (error == noErr) return;
  char errorString[20];
  // See if it appears to be a 4-char-code
  *(UInt32 *)(errorString + 1) = CFSwapInt32HostToBig(error);
  if (isprint(errorString[1]) && isprint(errorString[2]) && isprint(errorString[3]) && isprint(errorString[4])) {
      errorString[0] = errorString[5] = '\''; errorString[6] = '\0';
   } else {
       // No, format it as an integer
       sprintf(errorString, "%d", (int)error);
       fprintf(stderr, "Error: %s (%s)\n", operation, errorString); exit(1);
   }
}

void MyAQInputCallback(void *inUserData, AudioQueueRef inQueue, AudioQueueBufferRef inBuffer, const AudioTimeStamp *inStartTime, UInt32 inNumPackets, const AudioStreamPacketDescription *inPacketDesc) {
    Microphone *mic = (Microphone *)inUserData;
    if (inNumPackets > 0) {
      if (mic->recording) {
        int* values = (int *) inBuffer->mAudioData;
        mic->sluClient->sendAudio(values, inNumPackets * 2); // 16-bit
      }
    }
    CheckError(
        AudioQueueEnqueueBuffer(inQueue, inBuffer, 0, NULL),
        "AudioQueueEnqueueBuffer failed"
    );
}


namespace speechly {

Microphone::Microphone(speechly::SluClient* sluClient) : sluClient(sluClient)
{
  memset(&recordFormat, 0, sizeof(recordFormat));
  recordFormat.mFormatID = kAudioFormatLinearPCM;
  recordFormat.mChannelsPerFrame = 1;
  recordFormat.mSampleRate = freq;
  recordFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagsNativeEndian | kLinearPCMFormatFlagIsPacked;
  recordFormat.mBitsPerChannel = 16; // 32
  UInt32 propSize = sizeof(recordFormat);
  AudioFormatGetProperty(kAudioFormatProperty_FormatInfo, 0, NULL, &propSize, &recordFormat);
  CheckError(
    AudioFormatGetProperty(kAudioFormatProperty_FormatInfo, 0, NULL, &propSize, &recordFormat),
    "AudioFormatGetProperty failed"
  );

  queue = {0};

  void (*cb)(void *inUserData, AudioQueueRef, AudioQueueBufferRef, const AudioTimeStamp *inStartTime, UInt32, const AudioStreamPacketDescription *);

  cb = MyAQInputCallback;
  // Set callback
  CheckError(
    AudioQueueNewInput(&recordFormat, cb, this, NULL, NULL, 0, &queue),
    "AudioQueueNewInput failed"
  );
  UInt32 size = sizeof(recordFormat);
  CheckError(
    AudioQueueGetProperty(queue, kAudioConverterCurrentOutputStreamDescription, &recordFormat, &size),
    "Couldn't get queue's format"
  );

  int bufferIndex;
  for (bufferIndex = 0; bufferIndex < 3; ++bufferIndex) {
    AudioQueueBufferRef buffer;
    CheckError(
      AudioQueueAllocateBuffer(queue, bufferByteSize, &buffer),
      "AudioQueueAllocateBuffer failed"
    );
    CheckError(
      AudioQueueEnqueueBuffer(queue, buffer, 0, NULL),
      "AudioQueueEnqueueBuffer failed"
    );
  }
  recording = false;

  // START
  CheckError(AudioQueueStart(queue, NULL), "AudioQueueStart failed");

  std::cout << "Microphone recording\n";
}

void Microphone::start()
{
  recording = true;
}

void Microphone::stop()
{
  recording = false;
}

Microphone::~Microphone()
{
}

} // end namespace speechly
