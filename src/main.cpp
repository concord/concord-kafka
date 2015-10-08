#include "KafkaConsumer.h"
#include "KafkaSource.h"
#include <concord/Computation.hpp>
#include <memory>

int main(int argc, char **argv) {
  using namespace concord;
  using namespace std;

  KafkaConsumer consumer(4096, "10.240.202.47:9092", "words4g", "cc-kafka", 0);
  // KafkaConsumer consumer(16384, "127.0.0.1:9092", "words-2", "cc-kafka", 0);
  set<string> ostreams;
  ostreams.insert("logs");
  auto source = make_shared<KafkaSource>(consumer, "word-source", ostreams);

  bolt::client::serveComputation(source, argc, argv);
  return 0;
}
