# ngx_stub_status_prometheus

This is an nginx module that provides the same information as the standard <a href="http://nginx.org/en/docs/http/ngx_http_stub_status_module.html">stub_status module</a> but in a format recognized by the <a href="http://prometheus.io">prometheus</a> time-series database.

Information is currently only produced in the plain text exposition format. I will likely add the protobuf format in the future, though as noted in the Prometheus Client Library Guidelines document, any benefit would probably not be significant in most scenarios.

## Installation

Nginx has no dynamic module support - modules must be compiled in. Fortunately, this isn't so hard and there is plenty of information out there to help you out.

To make things even easier, this repo also includes:

1. A Dockerfile / image containing an nginx binary that includes stub_status_prometheus as well as a number of other commonly used modules that are not enabled by default (but which are included in the nginx builds in various distros). You can either use this docker image, or copy the nginx executable out of it into your own environment. Note that this image is auto-built by docker hub from Dockerfiles on github so hopefully you are able to trust it.

2. A set of scripts (*.sh) I use for building nginx in the mhowlett/nginx-build-base container. If you want to customize the build, you could use these as it's quicker than building a new docker image on each iteration.

### Docker Image

You can run the container like so:

    docker run -d -p 8000:8000 mhowlett/ngx-stub-status-prometheus

This starts up nginx with a test configuration. If you browse to http://127.0.0.1:8000/metrics you should see the status information.

To supply your own configuration, you could add a data volume at the standard nginx config file location:

    docker run -d -v mynginx.conf:/etc/nginx/nginx.conf mhowlett/nginx-stub-status-prometheus

Or you could put it in a different location and specify this as an argument:

    docker run -d -v you-will-need-something-here mhowlett/nginx-stub-status-prometheus nginx -c /absolute/path/to/mynginx.conf

### Building

To cut a long story short:

1. ./fetch-nginx.sh
2. ./docker-up.sh
3. cd /repo
4. /build.sh
5. /usr/sbin/nginx -c /repo/nginx.conf


## Usage

Place the stub_status_prometheus directive in a /metrics location context, as expected by prometheus.
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


In order to prevent public access, you can use the auto and deny directives, or https and basic auth as described here: http://prometheus.io/docs/operating/configuration/#scrape-configurations-scrape_config

Unlike stub_status, stub_status_prometheus does not expose any variables corresponding to the status information.
If you need these, you can use the stub_status module alongside the prometheus status module.
Note that the stub_status directive can be placed in a server context for this purpose (does not need to be placed in location context).

Misc notes:

1. The stub_status_prometheus directive takes no parameters.
2. The statistics reported are impacted by requests to the metrics endpoint by the prometheus server.
3. The status information exposed relates to the entire nginx server, not the server context in which it is contained.
