#include <mgos.h>

#include "httplib.h"

static void timer_handler(void* arg) {
    // C実装
    LOG(LL_INFO, ("C.httlib Example/ Sys.uptime: %f", mgos_uptime()));

    // configからリクエスト送信先api.urlを取得
    char* url = (char*)mgos_sys_config_get_api_url();

    // リクエスト作成
    HTTPReq_t* req = http_create_req(url, HTTP_METHOD_POST, HTTP_CT_JSON);
    // ヘッダ設定
    http_req_add_header(req, "X-Imprement", "C-lang");

    // リクエストボディ設定(JSON)
    struct json_out out = JSON_OUT_BUF(req->raw_body, sizeof(req->raw_body));
    json_printf(&out, "{%Q: [%d, %d]}", "values", 11, 12);

    // リクエスト送信
    HTTPRes_t* res = http_send(req);

    LOG(LL_INFO, ("Response Status: %d", res->status));

    if (res->success) {
        // ヘッダ値取得
        char* hv = http_res_header_value(res, "content-type");
        if (hv != NULL) {
            LOG(LL_INFO, ("Response ContentType=[%s]", hv));
        }
        char* hv2 = http_res_header_value(res, "date");
        if (hv2 != NULL) {
            LOG(LL_INFO, ("Date=[%s]", hv));
        }
        // レスポンスボディ取得
        LOG(LL_INFO, ("Response Body=[%s]", res->body));

        // JSON parse
        bool* success = NULL;
        if (json_scanf(res->body, strlen(res->body), "{success: %B}", &success) != 0) {
            LOG(LL_INFO, ("JSON attr[\"success\"]=%s", success ? "TRUE" : "FALSE"));
        };
    }

    // レスポンス開放
    http_res_free(res);
}
enum mgos_app_init_result mgos_app_init(void) {
    mgos_set_timer(30000, true, timer_handler, NULL);
    // mgos_msleep((uint32_t)15000);

    return MGOS_APP_INIT_SUCCESS;
}
