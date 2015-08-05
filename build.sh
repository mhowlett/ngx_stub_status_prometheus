#!/bin/bash

NGINX_VERSION=1.9.3

pushd "nginx-$NGINX_VERSION"
# General configuration options are as suggested by the book:
# NGINX - A Practical Guide to High Performance
CFLAGS="-O2" ./configure            \
    --prefix=/usr                   \
    --conf-path=/etc/nginx          \
    --add-module=../                \
    --with-http_ssl_module          \
    --with-http_spdy_module         \
    --with-http_realip_module       \
    --with-http_stub_status_module 
make
make install
popd
