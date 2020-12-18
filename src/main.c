#include "httplib.h"
#include "mgos.h"

static void timer_handler(void* arg) {
    // C実装
    LOG(LL_INFO, ("C.httlib Example/ Sys.uptime: %f", mgos_uptime()));

    char* url = (char*)mgos_sys_config_get_api_url();
    HTTPReq_t* req = http_create_req(url, HTTP_METHOD_POST, HTTP_CT_JSON);
    http_req_add_header(req, "X-Imprement", "C-lang");

    struct json_out out = JSON_OUT_BUF(req->raw_body, sizeof(req->raw_body));
    json_printf(&out, "{%Q: [%d, %d]}", "values", 11, 12);

    HTTPRes_t* res = http_send(req);

    LOG(LL_INFO, ("Response Status: %d", res->status));
    char buff[128];
    http_res_header_value(res, "content-type", buff, sizeof(buff));
    if (buff != NULL) {
        LOG(LL_INFO, ("Response ContentType=[%s]", buff));
    }
    LOG(LL_INFO, ("Response Body=[%s]", res->body));

    http_res_free(res);
}
enum mgos_app_init_result mgos_app_init(void) {
    mgos_set_timer(30000, true, timer_handler, NULL);
    // mgos_msleep((uint32_t)15000);

    return MGOS_APP_INIT_SUCCESS;
}
