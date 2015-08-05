# ngx_stub_status_prometheus

COMPLETELY NOT IMPLEMENTED! 

The stub doesn't even compile yet, let alone have any functionaltiy in it.

This is an nginx module that provides access to the same information as the standard stub_status module but in a format recognized by the <a href="http://prometheus.io">prometheus</a> time-series database.

Unfortunately nginx has no dynamic module support - modules must be compiled in.

Fortunately, this isn't that hard and there is plenty of information out there to help you out.

To make things even easier, I've created a docker image (mhowlett/nginx-build-base) that provides a suitable environment and these long-winded instructions:

There is a script included in this repo called docker-up.sh which can be used to bring up the nginx build container. Before you run it, you'll need to edit it to specify the absolute path of this git repository on your system.

You'll also need to get a copy the nginx source and put it in the top level of the git repository. This can be done by running the script download-nginx.sh (it fetches version 1.9.3 and then un-tars it).

Now you're ready to go. 

    ./docker-up.sh
    cd /repo
    ./build.sh

to test:

    /usr/sbin/nginx -c /repo/nginx.test.conf

note that --with-stub-status is specified in the included build script, but it is not a pre-requisite.

## Usage

    stub_status_prometheus;

Must be placed within a location section in the nginx.config. prometheus friendly status information will be provided at that location.

Does not expose any nginx variables (c.f. stub_status which does). If you need these variables, you can use stub_status in addition to stub_status_prometheus. note that stub_status does not need to be placed in a location section, you can also put it in the server section.
