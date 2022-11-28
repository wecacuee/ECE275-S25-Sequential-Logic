# This Dockerfile installs Quartus-web 13.0sp1 compatible with DE0 Cyclone III
# Usage download Quartus-web-${VERSION}-linux.tar next to the Dockerfile
FROM  ubuntu:20.04 as base
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    rm -rf /var/lib/apt/lists/*
RUN apt-add-repository -y ppa:linuxuprising/libpng12
RUN dpkg --add-architecture i386
RUN apt-get update && \
    apt-get install -y libsm6 libfreetype6 libpng12-0 \
    libxft2:i386 libxext6:i386 libncurses5:i386 gcc-multilib g++-multilib libc6:i386 && \
    rm -rf /var/lib/apt/lists/*

RUN mkdir -p /root/src

ENV VERSION 13.1.0.162
ENV ALTERA_INSTALL_DIR /root/altera/13.0sp1
COPY QuartusSetupWeb-${VERSION}.run ModelSimSetup*.run cyclone*.qdz /root/src/

RUN cd /root/src && \
    chmod +x QuartusSetupWeb-${VERSION}.run && \
    env QUARTUS_64BIT=1 ./QuartusSetupWeb-${VERSION}.run --mode unattended --unattendedmodeui none --installdir $ALTERA_INSTALL_DIR

ENV QUARTUS_INSTALL_DIR $ALTERA_INSTALL_DIR/quartus/

RUN cd /root/src && \
    chmod +x ModelSimSetup-${VERSION}.run && \
    ./ModelSimSetup-${VERSION}.run --mode unattended --unattendedmodeui none --installdir $ALTERA_INSTALL_DIR --modelsim_edition modelsim_ase

ENV MODELSIM_INSTALL_DIR $ALTERA_INSTALL_DIR/modelsim_ase/



#### Install libpng-12 from source if ppa:linuxuprising/libpng12 is not available
# RUN wget https://netactuate.dl.sourceforge.net/project/libpng/libpng12/1.2.59/libpng-1.2.59.tar.xz -O /root/src/libpng-1.2.59.tar.xz && \
#     cd /root/src/ && \
#     tar xjf libpng-1.2.59.tar.xz && \
#     cd libpng-1.2.59/ && \
#     ./configure && \
#     make && \
#     make install && \
#     cd /root/src && \
#     rm -rf libpng-1.2.59/ libpng-1.2.59.tar.xz

#### Update udev rules on a linux machine. Docker running with root might not need this
# COPY 51-usbblaster.rules /etc/udev/rules.d/
# RUN udevadm control --reload-rules && udevadm trigger
RUN apt-get update \
    && apt-get install -y wget build-essential libxrender1:i386 \
    && rm -rf /var/lib/apt/lists/*
RUN wget https://cfhcable.dl.sourceforge.net/project/freetype/freetype2/2.12.1/freetype-2.12.1.tar.gz -O /root/src/freetype-2.12.1.tar.gz \
    && cd /root/src/ \
    && tar xf /root/src/freetype-2.12.1.tar.gz \
    && cd /root/src/freetype-2.12.1 \
    && ./configure --build=i386-pc-linux-gnu "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32" --with-zlib=no --with-png=no --with-harfbuzz=no \
    && make install
RUN rm -rf /root/src/
RUN mkdir -p /opt && mv /root/altera/ /opt/ && chmod -R +rx /opt/altera/

RUN dpkg --add-architecture amd64
RUN apt-get update \
    && apt-get install -y  libxext6:amd64 libfontconfig1:amd64 libxrender1:amd64 \
    && rm -rf /var/lib/apt/lists/*

ENV MODELSIM_DIR /opt/altera/13.0sp1/modelsim_ase
RUN sed -i -e 's/linux_rh60/linux/' $MODELSIM_DIR/bin/vsim
RUN sed -i -e 's/MTI_VCO_MODE:-""/MTI_VCO_MODE:-"32"/' $MODELSIM_DIR/bin/vsim
RUN ln -s $MODELSIM_DIR/linux $MODELSIM_DIR/bin/linux
RUN mkdir -p $MODELSIM_DIR/bin/lib32 && cp /usr/local/lib/libfreetype* $MODELSIM_DIR/bin/lib32
ENV LD_LIBRARY_PATH $MODELSIM_DIR/bin/lib32:$LD_LIBRARY_PATH

ENV QUARTUS_64BIT 1
CMD ["$QUARTUS_INSTALL_DIR/bin/quartus"]
