# gRPC experiments

https://www.f-ax.de/dev/2020/11/08/grpc-plugin-cmake-support.html

## CMake Procedure

```
mkdir -p build
cd build
cmake ..
cd ..
cmake --build build --verbose
cmake --build build --target client/server/proto
```

## Measure overhead of serialising/deserialising small argument

RPC call with 0 arg vs 1 small arg

```
taskset 1 ./build/server/server
taskset 2 ./build/client/client
```