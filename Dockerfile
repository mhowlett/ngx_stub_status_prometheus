FROM mhowlett/nginx-build-base

RUN \
     BUILDDIR=/build \
  && mkdir /build

COPY config /build
COPY ngx_http_stub_status_prometheus_module.c /build

RUN \
     NGINX_VERSION=1.9.3 \
  && CFLAGS="-O2" \
  && mkdir $BUILDDIR \
  && cd $BUILDDIR \
  && curl -sSL http://nginx.org/download/nginx-${NGINX_VERSION}.tar.gz | tar zxfv - -C . \
  && cd nginx-$NGINX_VERSION \
  && ./configure \
    --prefix=/usr \
    --conf-path=/etc/nginx \
    --add-module=../ \
    --with-http_ssl_module \
    --with-http_spdy_module \
    --with-http_realip_module \
    --with-http_stub_status_module \
  && make \
  && make install

COPY nginx.test.conf /etc/nginx/nginx.conf

CMD nginx
