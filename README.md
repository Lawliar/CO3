## dependent utility:
- pkg-config

## init submodules
- git submodule init
- git submodule update

## build boost graph and program_options
- cd to deps/boost
- git submodule init
- git submodule update
- ./bootstrap.sh --prefix=./install
- ./b2 install --with-graph --with-program_options

## build libserialport
- cd to deps/libserialport
- ./autogen.sh
- ./configure
- make
- sudo make install
