#include <iostream>
#include <chrono>
#include "SpeechlyClient.h"

using speechly::SpeechlyClient;

int main(void) {
  std::cout << "Example App.\nPress 'enter' to toggle recording, press 'q and enter' to quit\n";

  SpeechlyClient speechlyClient(
    /*apiUrl*/ "api.speechly.com",
    /*appId*/ "49d117a2-d538-40aa-9b31-435420805794",
    /*deviceId*/ "bea5ca78-de82-4bd8-83e4-492930153f50", // https://www.uuidgenerator.net/
    /*debug*/ true);

  char key;
  while (std::cin)
  {

    std::cin.get(key);
    

    if (key == 'q')
    {
        break;
    }

    speechlyClient.toggle();
  }

  std::cout << "The End\n";
}