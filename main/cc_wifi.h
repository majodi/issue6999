/* Common functions to establish Wi-Fi connection.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
 */

#pragma once
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Configure Wi-Fi, start AP, wait for AP started
 *
 * This helper function starts an AP
 *
 * @return ESP_OK on successful start of AP
 */
esp_err_t cc_startAP(void);

/**
 * @brief Configure Wi-Fi, connect to an AP, wait for IP address
 *
 * This helper function connects to an AP
 *
 * @return ESP_OK on successful connect
 */
esp_err_t cc_connectToAP(void);

#ifdef __cplusplus
}
#endif
