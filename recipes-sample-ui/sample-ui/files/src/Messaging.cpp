#include "Messaging.hpp"

#include <log.h>

Messaging::Messaging(void) {
  run = false;
  runner = new Runner();
  connected = false;
  receivable = nullptr;
  client = new mqtt::client(MQTT_ADDRESS, MQTT_CLIENTID);
}

Messaging::~Messaging(void) {
  Stop();
  delete runner;
  delete client;
}

int Messaging::Start(void) {
  if(!run) {
    run = true;
    runner->Start(this);
  }
  return 0;
}

void Messaging::Stop(void) {
  if(run) {
    runner->Stop();
  }
}

int Messaging::Run(void) {
  while(run) {
    // establish connection here
    if(!connected) {
      Sleep(10000); // 10 seconds
      if(Connect() == 0) {
        connected = true;
        LOG("Connected to MQTT broker\n");
      } else {
        continue;
      }
    }
    // timed receive single message here (synchronously)
    std::string payload;
    int ret = TimedReceive(payload, 100);
    if(ret > 0 && receivable) {
      receivable->OnReceive(payload);
    }
  }
  // disconnect here
  if(connected) {
    Disconnect();
  }
  return 0;
}

void Messaging::InterruptRunnable(void) {
  run = false;
  CancelSleep();
}

void Messaging::RegisterReceiver(MessageReceivable *receivable) {
  this->receivable = receivable;
}

int Messaging::Connect(void) {
  auto connOpts = mqtt::connect_options_builder::v3()
    .keep_alive_interval(std::chrono::seconds(30))
    .automatic_reconnect(std::chrono::seconds(2), std::chrono::seconds(30))
    .clean_session(false)
    .finalize();
  mqtt::connect_response rsp = client->connect(connOpts);
  if (!rsp.is_session_present()) {
    client->subscribe(MQTT_TOPIC_STATE, MQTT_QOS);
  }
  LOG("Successfully connected to MQTT broker\n");
  return 0;
}

void Messaging::Disconnect(void) {
  client->disconnect();
  LOG("Successfully disconnected from MQTT broker\n");
}

int Messaging::TimedReceive(std::string &payload, unsigned timeout) {
  mqtt::const_message_ptr ptr;
  if(client->try_consume_message_for(&ptr, std::chrono::milliseconds(timeout))) {
    if(ptr) {
      LOG("received: %s\n", ptr->to_string().c_str());
      payload = ptr->to_string();
      return 1;
    }
  }
  return 0;
}
