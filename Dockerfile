FROM mhowlett/nginx-build-base

RUN mkdir /root/build
COPY config /root/build/
COPY ngx_http_stub_status_prometheus_module.c /root/build/

RUN \
     NGINX_VERSION=1.9.3 \
  && CFLAGS="-O2" \
  && cd /root/build \
  && curl -sSL http://nginx.org/download/nginx-${NGINX_VERSION}.tar.gz | tar zxfv - -C . \
  && cd nginx-$NGINX_VERSION \
  && ./configure \
    --prefix=/usr \
    --conf-path=/etc/nginx/nginx.conf \
    --add-module=../ \
    --with-http_ssl_module \
    --with-http_spdy_module \
    --with-http_realip_module \
    --with-http_stub_status_module \
  && make \
  && make install \
  && rm -rf /etc/nginx/nginx.conf

COPY nginx.conf /etc/nginx/

CMD nginx
