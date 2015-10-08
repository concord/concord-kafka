//
// Created by bigs on 9/1/15.
//

#include <concord/time_utils.hpp>
#include "KafkaSource.h"
#include <iostream>

namespace concord {

KafkaSource::KafkaSource(KafkaConsumer &consumer, const string &name,
                         const set<string> &ostreams):
  consumer_(consumer),
  name_(name),
  ostreams_(ostreams) { }

void KafkaSource::init(CtxPtr ctx) {
  consumer_.start();
  ctx->setTimer("loop", bolt::timeNowMilli());
}

void KafkaSource::processTimer(CtxPtr ctx, const string &key, int64_t time) {
  int read = 0;
  auto &queue = consumer_.queue;

  pair<folly::fbstring, folly::fbstring> record;
  while (read++ < 1024 && queue.read(record)) {
    for (auto &stream : ostreams_) {
      ctx->produceRecord(stream, record.first.toStdString(),
                         record.second.toStdString());
      ctx->produceRecord(stream, record.first.toStdString(),
                         record.second.toStdString());
      ctx->produceRecord(stream, record.first.toStdString(),
                         record.second.toStdString());
      ctx->produceRecord(stream, record.first.toStdString(),
                         record.second.toStdString());
      ctx->produceRecord(stream, record.first.toStdString(),
                         record.second.toStdString());
    }
  }

  ctx->setTimer(key, time + 50);
}

}

