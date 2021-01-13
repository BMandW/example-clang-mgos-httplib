#include <mgos.h>
#include <mgos_bme280.h>

#include "httplib.h"

struct mgos_bme280* bme280;
struct mgos_bme280_data* bme280data;

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
    if (mgos_bme280_read(bme280, bme280data) == 0) {
        LOG(LL_INFO, ("Temperature: %.1f, Humid: %.1f", bme280data->temp, bme280data->humid));
        struct json_out out = JSON_OUT_BUF(req->raw_body, sizeof(req->raw_body));
        json_printf(&out, "{%Q: [%.1f, %.1f]}", "values", bme280data->temp, bme280data->humid);
    } else {
        LOG(LL_INFO, ("Failt to read BME280 data"));
        return;
    };

    // リクエスト送信
    HTTPRes_t* res = http_send(req);

    LOG(LL_INFO, ("Response Status: %d", res->status));

    if (res->success) {
        // ヘッダ値取得
        char* hv = http_res_header_value(res, "content-type");
        bool isJson = false;
        if (hv != NULL) {
            LOG(LL_INFO, ("Response ContentType=[%s]", hv));
            if (strcmp(hv, "application/json")) {
                isJson = true;
            }
        }
        char* hv2 = http_res_header_value(res, "date");
        if (hv2 != NULL) {
            LOG(LL_INFO, ("Date=[%s]", hv));
        }
        // レスポンスボディ取得
        LOG(LL_INFO, ("Response Body=[%s]", res->body));

        // JSON parse
        if (isJson) {
            bool* success = NULL;
            if (json_scanf(res->body, strlen(res->body), "{success: %B}", &success) != 0) {
                LOG(LL_INFO, ("JSON attr[\"success\"]=%s", success ? "TRUE" : "FALSE"));
            };
        }
    } else {
        LOG(LL_INFO, ("Request is not Success: %d", res->status));
    }

    // レスポンス開放
    http_res_free(res);
    LOG(LL_INFO, ("@Finish"));
}
enum mgos_app_init_result mgos_app_init(void) {
    bme280 = mgos_bme280_i2c_create(0x76);
    bme280data = mgos_bme280_data_create();
    mgos_set_timer(30000, true, timer_handler, NULL);

    return MGOS_APP_INIT_SUCCESS;
}
