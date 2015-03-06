#/bin/bash

if [ "$(uname)" == "Darwin" ]; then
  mkdir -p config m4 && \
    aclocal -I m4 --install && \
    glibtoolize --force --copy && \
    autoheader && \
    automake --foreign --add-missing --copy && \
    autoconf
else
  mkdir -p config m4 && \
    aclocal -I m4 --install && \
    libtoolize --force --copy && \
    autoheader && \
    automake --foreign --add-missing --copy && \
    autoconf
fi

