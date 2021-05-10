
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "mdns.h"
#include "cc_wifi.h"
#include "cc_webserver.h"
#include "keep_alive.h"
#include "driver/uart.h"

static const char *TAG = "** ccfw **";

static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    extern const char index_html_start[] asm("_binary_index_html_start");
    extern const char index_html_end[]   asm("_binary_index_html_end");
    httpd_resp_send(req, index_html_start, index_html_end - index_html_start);
    return ESP_OK;
}

static esp_err_t favicon_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "image/x-icon");
    extern const char favicon_ico_start[] asm("_binary_favicon_ico_start");
    extern const char favicon_ico_end[]   asm("_binary_favicon_ico_end");
    httpd_resp_send(req, favicon_ico_start, favicon_ico_end - favicon_ico_start);
    return ESP_OK;
}

static esp_err_t xterm_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/javascript; charset=UTF-8");
    extern const char xterm_js_start[] asm("_binary_xterm_js_start");
    extern const char xterm_js_end[]   asm("_binary_xterm_js_end");
    httpd_resp_send(req, xterm_js_start, xterm_js_end - xterm_js_start);
    return ESP_OK;
}

static const httpd_uri_t root = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = root_get_handler
};

static const httpd_uri_t favicon = {
    .uri       = "/favicon.ico",
    .method    = HTTP_GET,
    .handler   = favicon_get_handler
};

static const httpd_uri_t xterm = {
    .uri       = "/xterm.js",
    .method    = HTTP_GET,
    .handler   = xterm_get_handler
};

static void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        *server = start_webserver();
        httpd_register_uri_handler(*server, &root);
        httpd_register_uri_handler(*server, &favicon);
        httpd_register_uri_handler(*server, &xterm);
    }
}

void app_main(void)
{
    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_AP_START, &connect_handler, &server, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_AP_STOP, &disconnect_handler, &server, NULL));
    ESP_ERROR_CHECK(cc_startAP());

}
