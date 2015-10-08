//
// Created by bigs on 9/1/15.
//

#ifndef CONCORD_KAFKA_KAFKASOURCE_H
#define CONCORD_KAFKA_KAFKASOURCE_H

#include <concord/Computation.hpp>
#include "KafkaConsumer.h"
#include <vector>

namespace concord {

using namespace std;
using namespace bolt;

class KafkaSource : public Computation {
  public:
    KafkaSource(KafkaConsumer &consumer, const string &name,
                const set<string> &ostreams);

    void init(CtxPtr ctx) override;

    void processRecord(Computation::CtxPtr ctx,
                       FrameworkRecord &&r) override {}

    void processTimer(CtxPtr ctx, const string &key, int64_t time) override;

    Metadata metadata() override {
      Metadata md;
      md.ostreams = ostreams_;
      md.name = name_;

      return md;
    }
  private:
    KafkaConsumer &consumer_;
    const string name_;
    const set<string> ostreams_;
};

}

#endif //CONCORD_KAFKA_KAFKASOURCE_H
