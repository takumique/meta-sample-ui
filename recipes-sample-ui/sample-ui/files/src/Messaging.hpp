#ifndef _SRC_MESSAGING_H_
#define _SRC_MESSAGING_H_

#include <string>
#include <mutex>

#include <Runner.hpp>
#include <Runnable.hpp>

#include <mqtt/client.h>

#include <MessageReceivable.hpp>

#ifndef MQTT_ADDRESS
  #define MQTT_ADDRESS "tcp://127.0.0.1:1883"
#endif /* MQTT_ADDRESS */
#ifndef MQTT_CLIENTID
  #define MQTT_CLIENTID "SampleUIClient"
#endif /* MQTT_CLIENTID */
#ifndef MQTT_TOPIC_STATE
  #define MQTT_TOPIC_STATE "ui/state"
#endif /* MQTT_TOPIC_STATE */
#ifndef MQTT_QOS
  #define MQTT_QOS 0
#endif /* MQTT_QOS */

class Messaging : public Runnable {
public:
  Messaging(void);
  virtual ~Messaging(void);
  int Start(void);
  void Stop(void);
  void InterruptRunnable(void);
  int Run(void);
  void RegisterReceiver(MessageReceivable *receivable);
private:
  Runner *runner;
  bool run;
  mqtt::client *client;
  bool connected;
  MessageReceivable *receivable;
  int Connect(void);
  void Disconnect(void);
  int TimedReceive(std::string &payload, unsigned timeout);
};

#endif /* _SRC_MESSAGING_H_ */
