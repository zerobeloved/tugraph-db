
FROM centos:7.3.1611

RUN yum update -y && yum install -y \
    git \
    gcc \
    gcc-c++ \
    make \
    wget \
    bzip2 \
    openssl-static \
    libtool \
    libtool-ltdl \
    libtool-ltdl-devel \
    unzip \
    byacc \
    libuuid-devel \
    rpm-build \
    rpmdevtools \
    rpmdev-setuptree \
    autoconf \
    automake \
    libtoolize \
    texinfo \
    java-1.8.0-openjdk* \
    libcurl-devel.x86_64

# install g++ 5.4.0
RUN wget http://ftp.gnu.org/gnu/gcc/gcc-5.4.0/gcc-5.4.0.tar.gz \
    && tar -xvf gcc-5.4.0.tar.gz && cd gcc-5.4.0 \
    && ./contrib/download_prerequisites && mkdir build && cd build \
    && ../configure -enable-checking=release -enable-languages=c,c++ -disable-multilib \
    && make -j4 && make install \
    && cp  ./x86_64-*-linux-gnu/libstdc++-v3/src/.libs/libstdc++.so.6.0.21 /usr/lib64/ \
    && cd /usr/lib64 && ln -sf libstdc++.so.6.0.21 libstdc++.so.6 \
    && rm -rf /gcc-* && ln -s /usr/local/bin/gcc /usr/local/bin/cc
# install python3
RUN wget https://www.python.org/ftp/python/3.6.9/Python-3.6.9.tgz \
    && tar zxf Python-3.6.9.tgz && cd Python-3.6.9 && ./configure --prefix=/usr/local \
    && make -j4 && make install && python3 -m pip install pexpect requests && rm -rf /Python*
# install boost-1.68
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.68.0/source/boost_1_68_0.tar.gz \
    && tar zxf boost_1_68_0.tar.gz && cd boost_1_68_0 \
    && ./bootstrap.sh --with-libraries=system,random,thread,filesystem,chrono,atomic,date_time,regex,stacktrace \
    && ./b2 -j4 cxxflags="-std=c++14 -fPIC" install && rm -rf /boost_*
# install node.js
RUN wget https://npm.taobao.org/mirrors/node/v10.13.0/node-v10.13.0.tar.gz \
    && tar xvf node-v10.13.0.tar.gz && cd node-v10.13.0/ && ./configure \
    && make -j4 && make install && npm config set registry https://registry.npm.taobao.org && rm -rf /node-*
# install lcov
RUN yum -y install perl-Digest-MD5 \
    && wget http://downloads.sourceforge.net/ltp/lcov-1.14-1.noarch.rpm \
    && rpm -ivh lcov-1.14-1.noarch.rpm \
    && rm lcov-1.14-1.noarch.rpm
# install prometheus
RUN cd opt \
    && wget https://github.com/prometheus/prometheus/releases/download/v2.36.1/prometheus-2.36.1.linux-amd64.tar.gz \
    && tar xvfz prometheus-*.tar.gz \
    && rm -r prometheus-2.36.1.linux-amd64.tar.gz
# install grafana
COPY grafana-enterprise-8.5.5.linux-amd64.tar.gz /opt
RUN cd opt \
    && tar -zxvf grafana-enterprise-8.5.5.linux-amd64.tar.gz \
    && rm -r grafana-enterprise-8.5.5.linux-amd64.tar.gz

# install pytest
RUN python3 -m pip install pytest

COPY TuGraph-3.3.0-1.el7.x86_64.rpm /opt
RUN cd opt \
    && rpm --force --nodeps -ivh TuGraph-3.3.0-1.el7.x86_64.rpm

COPY demo /root/demo


ENV JAVA_HOME=/usr/local/lib64:/usr/lib/jvm/java-1.8.0/jre/lib/amd64/server \
    LD_LIBRARY_PATH=/usr/local/lib64:/usr/lib/jvm/java-1.8.0/jre/lib/amd64/server:$LD_LIBRARY_PATH \
    PYTHONPATH=/usr/local/lib64:$PYTHONPATH \
