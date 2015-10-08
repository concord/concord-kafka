//
// Created by bigs on 8/31/15.
//

#include "KafkaConsumer.h"
#include <iostream>

namespace concord {

KafkaConsumer::KafkaConsumer(uint32_t queueSize, string brokerList,
                             string topic, string groupId, int32_t partition):
  queue(queueSize), partition_(partition) {
  using namespace RdKafka;
  std::string err;

  consumerConf_.reset(Conf::create(Conf::CONF_GLOBAL));
  std::map<string, string> options = {
    {"metadata.broker.list", brokerList},
    {"group.id", groupId}
  };

  if (!setConfigOptions(consumerConf_, options)) {
    throw runtime_error("Failed to configure Kafka consumer!");
  }

  consumer_.reset(Consumer::create(consumerConf_.get(), err));

  if (!consumer_) {
    throw runtime_error("Failed to configure Kafka consumer: " + err);
  }

  topicConf_.reset(Conf::create(Conf::CONF_TOPIC));
  topic_.reset(
    Topic::create(consumer_.get(), topic, topicConf_.get(), err)
  );

  if (!topic_) {
    throw runtime_error("Failed to configure Kafka topic: " + err);
  }
}

bool KafkaConsumer::setConfigOptions(shared_ptr<RdKafka::Conf> conf,
                                     map<string, string> options) {
  bool result = true;
  string err;
  for (auto &kv : options) {
    auto status = conf->set(kv.first, kv.second, err);
    if (status != RdKafka::Conf::CONF_OK) {
      result = false;
      cerr << "Error configuring Kafka: " << err << endl;
    }
  }

  return result;
}

void KafkaConsumer::pollQueue() {
  using namespace RdKafka;
  static const int kSleepDurationMs = 50;

  while (running_) {
    if (queue.isFull()) {
      this_thread::sleep_for(chrono::milliseconds(kSleepDurationMs));
    } else {
      Message *message = consumer_->consume(topic_.get(), partition_,
                                            kSleepDurationMs);

      if (message->err() != ERR_NO_ERROR) {
        cerr << "Error: " << err2str(message->err()) << endl;
      } else {
        folly::fbstring payload((char *) message->payload(), message->len());
        folly::fbstring key(message->key() ? *(message->key()) : "");
        auto pair = make_pair(move(key), move(payload));
        while (running_ && !queue.write(pair)) {
          this_thread::sleep_for(chrono::milliseconds(1));
        }
      }
    }
    this_thread::yield();
  }
}

}

