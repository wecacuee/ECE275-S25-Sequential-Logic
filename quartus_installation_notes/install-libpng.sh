wget https://netactuate.dl.sourceforge.net/project/libpng/libpng12/1.2.59/libpng-1.2.59.tar.xz -O /tmp/libpng-1.2.59.tar.xz && \
    cd /tmp/ && \
    tar xf libpng-1.2.59.tar.xz && \
    cd libpng-1.2.59/ && \
    ./configure --prefix ${PREFIX:-/usr/local} && \
    make && \
    make install && \
    cd /tmp/ && \
    rm -rf libpng-1.2.59/ libpng-1.2.59.tar.xz
