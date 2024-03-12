vcpkg_installed\x64-windows\tools\protobuf\protoc.exe -I=gRPCServer/proto --cpp_out=gRPCServer/proto todo_list.proto
vcpkg_installed\x64-windows\tools\protobuf\protoc.exe -I=gRPCServer/proto --grpc_out=gRPCServer/proto --plugin=protoc-gen-grpc=vcpkg_installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe todo_list.proto
