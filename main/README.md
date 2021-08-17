# Install dependecies

brew install autoconf automake libtool shtool

git clone --recurse-submodules https://github.com/grpc/grpc.git
cd grpc
git submodule update --init
mkdir -p cmake/build
cd cmake/build
cmake ../..
make
make install


# Generate code from protos
```shell 
protoc --proto_path=./ --grpc_out=./ --cpp_out=./ identity.proto --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin
protoc --proto_path=./ --grpc_out=./ --cpp_out=./ slu.proto --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin
```
