# ngx_stub_status_prometheus

This is an nginx module that provides access to the same information as the standard stub_status module but in a format recognized by the <a href="http://prometheus.io">prometheus</a> time-series database.

Only the plain text exposition format is currently supported. I will likely add the protobuf format in the future, though as noted in the Prometheus Client Library Guidelines document I expect any benefit would not be significant in most scenarios.


## Installation

Nginx has no dynamic module support - modules must be compiled in. Fortunately, this isn't so hard and there is plenty of information out there to help you out.

Also, to make things even easier, I've created:

1. A Dockerfile / image containing an nginx binary with stub_status_prometheus compiled in as well as a number of other commonly used modules that are not enabled by default (but which are included in the nginx builds in various distros). You can either use this docker image, or copy the nginx executable out of it into your own environment. Note that this image is auto-built by docker hub from Dockerfiles on github so hopefully you are able to trust it.

2. A set of scripts (*.sh) I use for building nginx in the mhowlett/nginx-build-base container. If you want to customize the build, I recommend using these as it's quicker than building a new docker image on each iteration.

### Docker Image

You can run the container like so:

    docker run -d -p 8000 mhowlett/ngx_stub_status_prometheus
  
This starts up nginx with a test configuration. If you browser to http://127.0.0.1:8000/metrics you should see the status information.

To supply your own configuration, you could add a data volume at the standard config file location:

    docker run -d -v mynginx.conf:/etc/nginx/nginx.conf mhowlett/nginx_stub_status_prometheus

Or you could put it in a different location and specify this as an argument: 

    docker run -d -v you-will-need-something-here mhowlett/nginx_stub_status_prometheus nginx -c /absolute/path/to/mynginx.conf

### Building

To cut a long story short:

1. ./fetch-nginx.sh
2. ./docker-up.sh
3. cd /repo
4. /build.sh
5. /usr/sbin/nginx -c /repo/nginx.conf


## Usage

Place the stub_status_prometheus directive in a location context. 
Here is a complete, minimalistic configuration:

    events {
    }

    http {
      server {
        listen 80;
        location /metrics {
          stub_status_prometheus;
        }
      }
    }

This will serve status information at /metrics.

If this server is publically accessible, the status information will be as well. In order to prevent public access, you can use https and basic auth as described here: http://prometheus.io/docs/operating/configuration/#scrape-configurations-scrape_config

Unlike stub_status, stub_status_prometheus does not expose any variables corresponding to the status information. 
If you need these, you can use the stub_status module alongside the prometheus status module.
Note that the stub_status directive can be placed in a server context for this purpose (does not need to be placed in location context).

The stub_status_prometheus directive takes no parameters.
