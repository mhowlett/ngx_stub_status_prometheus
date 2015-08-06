# ngx_stub_status_prometheus

This is an nginx module that provides access to the same information as the standard stub_status module but in a format recognized by the <a href="http://prometheus.io">prometheus</a> time-series database.

Only the plain text exposition format is currently supported. I will likely implement the protobuf format in the future, though I think any benefit would be negligible in most scenarios I can think of.


## Installation

Nginx has no dynamic module support - modules must be compiled in. Fortunately, this isn't so hard and there is plenty of information out there to help you out.

Also, to make things even easier, I've created:

1. A Dockerfile / image containing an nginx binary with stub_status_prometheus compiled in as well as a number of other commonly used modules that are not enabled by default (but which are included in the nginx builds in various distros).

2. A set of scripts (*.sh) I use for building nginx in the mhowlett/nginx-build-base container. If you want to customize the build, I recommend using these as it's quicker than building a new docker image on each iteration.

### Docker Image

You can run the container like so:

    docker run -d -p 8000:80 mhowlett/ngx_stub_status_prometheus
  
This starts up nginx with a test configuration. If you browser to http://127.0.0.1:8000/ you should see the status information.

To supply your own configuration, you could add a data volume at the standard config file location:

    docker run -d -v mynginx.conf:/etc/nginx/nginx.conf mhowlett/nginx_stub_status_prometheus

Or you could put it in a different location and specify this as an argument: 

    docker run -d -v youll-need-something-here mhowlett/nginx_stub_status_prometheus nginx -c /absolute/path/to/mynginx.conf

### Building

To cut a long story short:

1. ./fetch-nginx.sh
2. ./docker-up.sh
3. cd /repo
4. /build.sh
5. /usr/sbin/nginx -c /repo/nginx.text.conf


## Usage

    stub_status_prometheus;

note that --with-stub-status is specified in the included build script, but it is not a pre-requisite.

Must be placed within a location section in the nginx.config. prometheus friendly status information will be provided at that location.

Does not expose any nginx variables (c.f. stub_status which does). If you need these variables, you can use stub_status in addition to stub_status_prometheus. note that stub_status does not need to be placed in a location section, you can also put it in the server section.
