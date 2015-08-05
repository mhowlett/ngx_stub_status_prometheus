#!/bin/bash

docker run --rm -it -p 8000:8000 -v /git/ngx_stub_status_prometheus:/repo mhowlett/nginx-build-base /bin/bash
