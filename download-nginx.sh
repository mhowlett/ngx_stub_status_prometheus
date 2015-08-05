#!/bin/bash

NGINX_VERSION=1.9.3

curl -s -L -O "http://nginx.org/download/nginx-$NGINX_VERSION.tar.gz"
tar xvf nginx-$NGINX_VERSION.tar.gz