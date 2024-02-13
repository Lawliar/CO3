# This file is part of SymCC.
#
# SymCC is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# SymCC is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# SymCC. If not, see <https://www.gnu.org/licenses/>.

#
# The base image
#
FROM ubuntu:22.04 AS builder

# Install dependencies
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        build-essential \
        autoconf \
        libtool \
        cmake \
        g++ \
        git \
        ninja-build \
        python2 \
        python3-pip \
        zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

COPY ./deps /CO3_SOURCE/deps/
COPY ./.git /CO3_SOURCE/.git

WORKDIR /CO3_SOURCE/deps/libserialport
RUN ./autogen.sh && ./configure && make

#WORKDIR /CO3_SOURCE/deps/z3/build
#RUN cmake -DCMAKE_INSTALL_PREFIX=`pwd`/install .. && make && make install

WORKDIR /CO3_SOURCE/deps/boost
RUN ./bootstrap.sh && ./b2 --with-filesystem --with-graph --with-program_options


FROM ubuntu:22.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        build-essential \
        autoconf \
        cmake \
        g++ \
        python2 \
        zlib1g-dev \
        clang-14 \
        llvm-14-dev \
        llvm-14-tools \
        libz3-dev \
        && rm -rf /var/lib/apt/lists/*

COPY --from=builder /CO3_SOURCE/deps/libserialport       /CO3_SOURCE/deps/libserialport/
#COPY --from=builder /CO3_SOURCE/deps/z3/build/install    /CO3_SOURCE/deps/z3/build/install/
COPY --from=builder /CO3_SOURCE/deps/boost               /CO3_SOURCE/deps/boost/
COPY ./deps/llvm     /CO3_SOURCE/deps/llvm/
COPY ./sym_backend   /CO3_SOURCE/sym_backend/
COPY ./code_coverage /CO3_SOURCE/code_coverage/
COPY ./USBtest       /CO3_SOURCE/USBtest
COPY ./pass          /CO3_SOURCE/pass

WORKDIR /CO3_SOURCE/sym_backend/build
RUN cmake .. && make
