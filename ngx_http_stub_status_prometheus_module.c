#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <nginx.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static char *ngx_http_stub_status_prometheus(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t  ngx_http_stub_status_prometheus_commands[] = {

  { ngx_string("stub_status_prometheus"),
    NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
    ngx_http_stub_status_prometheus,
    0,
    0,
    NULL },

    ngx_null_command
};


static ngx_http_module_t  ngx_http_stub_status_prometheus_module_ctx = {
  NULL,                          /* preconfiguration */
  NULL,                          /* postconfiguration */
  NULL,                          /* create main configuration */
  NULL,                          /* init main configuration */
  NULL,                          /* create server configuration */
  NULL,                          /* merge server configuration */
  NULL,                          /* create location configuration */
  NULL                           /* merge location configuration */
};


ngx_module_t ngx_http_stub_status_prometheus_module = {
  NGX_MODULE_V1,
  &ngx_http_stub_status_prometheus_module_ctx, /* module context */
  ngx_http_stub_status_prometheus_commands,   /* module directives */
  NGX_HTTP_MODULE,               /* module type */
  NULL,                          /* init master */
  NULL,                          /* init module */
  NULL,                          /* init process */
  NULL,                          /* init thread */
  NULL,                          /* exit thread */
  NULL,                          /* exit process */
  NULL,                          /* exit master */
  NGX_MODULE_V1_PADDING
};

static ngx_int_t ngx_http_stub_status_prometheus_handler(ngx_http_request_t *r)
{
  size_t             size;
  ngx_int_t          rc;
  ngx_buf_t         *b;
  ngx_chain_t        out;
  ngx_atomic_int_t   ap, hn, ac, rq, rd, wr, wa;

  if (r->method != NGX_HTTP_GET && r->method != NGX_HTTP_HEAD) {
    return NGX_HTTP_NOT_ALLOWED;
  }

  rc = ngx_http_discard_request_body(r);

  if (rc != NGX_OK) {
    return rc;
  }

  ngx_str_set(&r->headers_out.content_type, "text/plain; version=0.0.4");

  if (r->method == NGX_HTTP_HEAD) {
    r->headers_out.status = NGX_HTTP_OK;

    rc = ngx_http_send_header(r);

    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
      return rc;
    }
  }

  size =
    sizeof("# HELP nginx_active_connections_current Current number of active connections\n") - 1 +
    sizeof("# TYPE nginx_active_connections_current gauge\n") - 1 +
    sizeof("nginx_active_connections_current \n") +
    NGX_ATOMIC_T_LEN +
    sizeof("# HELP nginx_connections_current Number of connections currently being processed by nginx\n") - 1 +
    sizeof("# TYPE nginx_connections_current gauge\n") - 1 +
    sizeof("nginx_connections_current{state=\"reading\"} \n") +
    sizeof("nginx_connections_current{state=\"writing\"} \n") +
    sizeof("nginx_connections_current{state=\"waiting\"} \n") +
    3 * NGX_ATOMIC_T_LEN +
    sizeof("# HELP nginx_connections_accepted_total Total number of connections accepted by nginx\n") - 1 +
    sizeof("# TYPE nginx_connections_accepted_total counter\n") - 1 +
    sizeof("nginx_connections_accepted_total \n") +
    NGX_ATOMIC_T_LEN +
    sizeof("# HELP nginx_connections_handled_total Total number of connections handled by nginx\n") - 1 +
    sizeof("# TYPE nginx_connections_handled_total counter\n") - 1 +
    sizeof("nginx_connections_handled_total \n") +
    NGX_ATOMIC_T_LEN +
    sizeof("# HELP nginx_requests_total Total number of requests processed by nginx\n") - 1 +
    sizeof("# TYPE nginx_requests_total counter\n") - 1 +
    sizeof("nginx_requests_total \n") +
    NGX_ATOMIC_T_LEN;

  b = ngx_create_temp_buf(r->pool, size);
  if (b == NULL) {
    return NGX_HTTP_INTERNAL_SERVER_ERROR;
  }

  out.buf = b;
  out.next = NULL;

  ap = *ngx_stat_accepted;
  hn = *ngx_stat_handled;
  ac = *ngx_stat_active;
  rq = *ngx_stat_requests;
  rd = *ngx_stat_reading;
  wr = *ngx_stat_writing;
  wa = *ngx_stat_waiting;

  b->last = ngx_cpymem(
    b->last,
           "# HELP nginx_active_connections_current Current number of active connections\n",
    sizeof("# HELP nginx_active_connections_current Current number of active connections\n") - 1
  );

  b->last = ngx_cpymem(
    b->last,
           "# TYPE nginx_active_connections_current gauge\n",
    sizeof("# TYPE nginx_active_connections_current gauge\n") - 1
  );

  b->last = ngx_sprintf(
    b->last,
    "nginx_active_connections_current %uA\n", ac
  );

  b->last = ngx_cpymem(
    b->last,
           "# HELP nginx_connections_current Number of connections currently being processed by nginx\n",
    sizeof("# HELP nginx_connections_current Number of connections currently being processed by nginx\n") - 1
  );

  b->last = ngx_cpymem(
    b->last,
           "# TYPE nginx_connections_current gauge\n",
    sizeof("# TYPE nginx_connections_current gauge\n") - 1
  );

  b->last = ngx_sprintf(
    b->last,
    "nginx_connections_current{state=\"reading\"} %uA\n", rd
  );

  b->last = ngx_sprintf(
    b->last,
    "nginx_connections_current{state=\"writing\"} %uA\n", wr
  );

  b->last = ngx_sprintf(
    b->last,
    "nginx_connections_current{state=\"waiting\"} %uA\n", wa
  );

  b->last = ngx_cpymem(
    b->last,
           "# HELP nginx_connections_accepted_total Total number of connections accepted by nginx\n",
    sizeof("# HELP nginx_connections_accepted_total Total number of connections accepted by nginx\n") - 1
  );

  b->last = ngx_cpymem(
    b->last,
           "# TYPE nginx_connections_accepted_total counter\n",
    sizeof("# TYPE nginx_connections_accepted_total counter\n") - 1
  );

  b->last = ngx_sprintf(
    b->last,
    "nginx_connections_accepted_total %uA\n", ap
  );

  b->last = ngx_cpymem(
    b->last,
           "# HELP nginx_connections_handled_total Total number of connections handled by nginx\n",
    sizeof("# HELP nginx_connections_handled_total Total number of connections handled by nginx\n") - 1
  );

  b->last = ngx_cpymem(
    b->last,
           "# TYPE nginx_connections_handled_total counter\n",
    sizeof("# TYPE nginx_connections_handled_total counter\n") - 1
  );

  b->last = ngx_sprintf(
    b->last,
    "nginx_connections_handled_total %uA\n", hn
  );

  b->last = ngx_cpymem(
    b->last,
           "# HELP nginx_requests_total Total number of requests processed by nginx\n",
    sizeof("# HELP nginx_requests_total Total number of requests processed by nginx\n") - 1
  );

  b->last = ngx_cpymem(
    b->last,
           "# TYPE nginx_requests_total counter\n",
    sizeof("# TYPE nginx_requests_total counter\n") - 1
  );

  b->last = ngx_sprintf(
    b->last,
    "nginx_requests_total %uA\n", rq
  );

  r->headers_out.status = NGX_HTTP_OK;
  r->headers_out.content_length_n = b->last - b->pos;

  b->last_buf = (r == r->main) ? 1 : 0;

  rc = ngx_http_send_header(r);

  if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
    return rc;
  }

  return ngx_http_output_filter(r, &out);
}


static char *ngx_http_stub_status_prometheus(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_http_core_loc_conf_t  *clcf;

  clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
  clcf->handler = ngx_http_stub_status_prometheus_handler;

  return NGX_CONF_OK;
}
