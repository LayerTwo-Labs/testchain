Testchain is a template sidechain that can be used as the starting point for creating Drivechain sidechains.

## Drivechain (BIPs 300+301)
Drivechain allows Bitcoin to create, delete, send BTC to, and receive BTC from “Layer-2”s called “sidechains”. Sidechains are Altcoins that lack a native “coin” – instead, BTC must first be sent over.

BIP 300: https://github.com/bitcoin/bips/blob/master/bip-0300.mediawiki

BIP 301: https://github.com/bitcoin/bips/blob/master/bip-0301.mediawiki

Learn more about Drivechain here: http://drivechain.info

For an example sidechain implementation, see: https://github.com/drivechain-project/sidechains

## Building Testchain

Testchain is built and released for Linux, macOS and Windows. The only supported
way of building Testchain is through cross-compiling from Linux. If you only want
to build node binaries, you can disable building the UI through passing `NO_QT=1`
to the `make -C ./depends` call. If the build crashes unexpectedly, try reducing
the amount of concurrency by removing the `-j` parameter.

### Linux

These instructions have been tested with Ubuntu 20.04 (Focal).

```bash
# install build dependencies 
$ sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3

# compile dependencies
$ make -C ./depends -j

# Compile binaries
$ ./autogen.sh
$ export CONFIG_SITE=$PWD/depends/x86_64-pc-linux-gnu/share/config.site
$ ./configure
$ ./make -j

# binaries are located in src/testchaind, src/testchain-cli and src/qt/testchain-qt
```

### macOS

Building Testchain requires a very old version of Ubuntu. A version that is 
known to work is 14.04 (Jerry). An old version of the macOS SDK is also required.
For convenience, a [Docker image](https://hub.docker.com/r/barebitcoin/testchain-macos-builder)
is provided that has all the required build dependencies pre-installed. 
If you want to set up your local environment to match this, it is recommended
to take a look at the [Dockerfile](./contrib/Dockerfile.macosbuilder) that 
produced this image. 

```bash
# start the Docker container that will build the binaries
$ docker run --rm -it \
    --workdir /testchain -v $PWD:/testchain \
    barebitcoin/testchain-macos-builder bash

# from within the Docker container shell

# compile dependencies
$ make -C ./depends -j

# compile the binaries
$ export CONFIG_SITE=$PWD/depends/x86_64-apple-darwin11/share/config.site
$ ./autogen.sh
$ ./configure.sh
$ make -j

# binaries are located (on the host machine) 
# at src/testchaind, src/testchain-cli and src/qt/testchain-qt
```

### Windows

These instructions have been tested with Ubuntu 20.04 (Focal).

```bash
# install dependencies
$ sudo apt install g++-mingw-w64-x86-64 \
                build-essential libtool autotools-dev automake \
                libssl-dev libevent-dev \
                pkg-config bsdmainutils curl git \
                python3-setuptools python-is-python3 \

# configure the Windows toolchain
$ sudo update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix

# compile dependencies
$ make -C ./depends HOST=x86_64-w64-mingw32 -j

# compile the binaries
$ export CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site
$ ./autogen.sh
$ ./configure.sh
$ make -j

# binaries are located at src/testchaind.exe, src/testchain-cli.exe and
# src/qt/testchain-qt.exe
```


## License
Bitcoin Core (and Drivechain) are released under the terms of the MIT license. See COPYING for more information or see https://opensource.org/licenses/MIT.
