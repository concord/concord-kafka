//
// Created by bigs on 8/31/15.
//

#ifndef CONCORD_KAFKA_KAFKACONSUMER_H
#define CONCORD_KAFKA_KAFKACONSUMER_H

#include <librdkafka/rdkafkacpp.h>
#include <folly/ProducerConsumerQueue.h>
#include <memory>
#include <map>
#include <thread>
#include <functional>
#include <folly/FBString.h>

namespace concord {

using namespace std;

class KafkaConsumer {

  public:
    KafkaConsumer(uint32_t queueSize, string brokerList, string topic,
                  string groupId, int32_t partition = 0);
    ~KafkaConsumer() {}

    void start() {
      running_ = true;
      consumer_->start(topic_.get(), 0, RdKafka::Topic::OFFSET_BEGINNING);
      auto f = mem_fn(&KafkaConsumer::pollQueue);
      thread(f, this).detach();
    }
    void stop() {
      running_ = false;
    }

  public:
    folly::ProducerConsumerQueue<pair<folly::fbstring, folly::fbstring>> queue;

  private:
    bool setConfigOptions(shared_ptr<RdKafka::Conf> conf,
                          map<string, string> options);
    void pollQueue();

  private:
    shared_ptr<RdKafka::Conf>     consumerConf_ {nullptr};
    shared_ptr<RdKafka::Conf>     topicConf_    {nullptr};
    unique_ptr<RdKafka::Consumer> consumer_     {nullptr};
    unique_ptr<RdKafka::Topic>    topic_        {nullptr};

    bool running_ {false};
    int32_t partition_;
};

}

#endif //CONCORD_KAFKA_KAFKACONSUMER_H
