# gRPC Function Call Overhead

gRPC is implemented atop HTTP/2 which still uses TCP for tport layer
Thus, one could measure the overhead of the gRPC function call by comparing the
time it takes for a gRPC function call with no data versus establishing a TCP
connection.