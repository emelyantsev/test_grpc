FROM ubuntu:latest

RUN apt-get -y update && apt-get install -y \
  autoconf \
  automake \
  build-essential \
  cmake \
  curl \
  g++ \
  git \
  libtool \
  make \
  pkg-config \
  unzip \
  && apt-get clean -y

ENV MY_INSTALL_DIR=$HOME/.local
ENV PATH="$MY_INSTALL_DIR/bin:${PATH}"

ENV GRPC_RELEASE_TAG v1.49.1

RUN git clone --recurse-submodules -b ${GRPC_RELEASE_TAG} --depth 1 --shallow-submodules https://github.com/grpc/grpc

RUN echo "-- building grpc" && cd grpc && mkdir -p cmake/build && cd cmake/build && \
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../.. \
&& make && make install

COPY src /app/src
COPY CMakeLists.txt /app
COPY common.cmake /app

WORKDIR /app

RUN mkdir -p cmake/build && cd cmake/build && cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ../.. && make

WORKDIR /app/cmake/build

EXPOSE 50051

ENTRYPOINT [ "./run_sync_server" ]