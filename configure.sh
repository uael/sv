# configure.sh --
#

set -ex

prefix=/usr/local
if test -d /lib64
then libdir=${prefix}/lib64
else libdir=${prefix}/lib
fi

../configure \
    --config-cache				\
    --cache-file=../config.cache		\
    --enable-maintainer-mode                    \
    --disable-static --enable-shared            \
    --prefix="${prefix}"			\
    --libdir="${libdir}"                        \
    CFLAGS='-Wextra -pedantic -O3'		\
    "$@"

### end of file
