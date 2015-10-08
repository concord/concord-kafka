cmake_minimum_required(VERSION 2.8)
project(concord_kafka)

include_directories(/usr/local/include src)
link_directories(/home/bigs/code/concord/build)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_FILES src/main.cpp src/KafkaConsumer.cpp src/KafkaConsumer.h src/KafkaSource.cpp src/KafkaSource.h)

add_executable(concord_kafka ${SOURCE_FILES})

target_link_libraries(concord_kafka
   jemalloc
   folly
   pthread
   bolt
   rdkafka++
   rdkafka
   rt
)

