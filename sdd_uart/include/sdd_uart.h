/*
Author: erling.r.jellum@ntnu.no
*/

#ifndef SDD_UART_H
#define SDD_UART_H
#include <stdint.h>

#include "common/cbuf.h"
#include "flexpret.h"

/**
 * @brief This is the PRIVATE part of the sdd_uart config.
 * It should never be written to by the user and thus all the fields are const.
 */
typedef struct {
  const int ns_per_bit;
  const cbuf_t *cbuf;
  const lock_t lock;
} sdd_uart_private_config_t;

/**
 * @brief This is the user-facing configuration structure for sdd_uart.
 * The fields, except `__misc`, should be set by the user before
 *
 */
typedef struct {
  bool initialized;
  uint32_t port;
  size_t pin;
  size_t baud;
  size_t buf_size;
  sdd_uart_private_config_t __misc;
} sdd_uart_config_t;

/**
 * @brief Starts a TX service running on a dedicated HART. This function will
 * only return with an error
 *
 * @param uart
 * @return fp_ret_t If this function returns, it is with an error
 */
fp_ret_t sdd_uart_tx_service_run(sdd_uart_config_t *uart);

/**
 * @brief This function is non-blocking and schedules the transmission of a
 * byte. This assumes that that the tx_service has been started and is
 * up-and-running
 * @param uart
 * @param byte
 * @param len
 * @return fp_ret_t
 */
fp_ret_t sdd_uart_tx_service_send(sdd_uart_config_t *uart, char *byte,
                                  size_t len);
/**
 * @brief Start a UART receiver service on the current HART. This function
 * should not return
 *
 * @param uart
 * @return fp_ret_t
 */
fp_ret_t sdd_uart_rx_service_run(sdd_uart_config_t *uart);

/**
 * @brief Fetch received data from the rx_service.
 *
 * @param uart
 * @param byte
 * @return fp_ret_t
 */
fp_ret_t sdd_uart_rx_service_recv(sdd_uart_config_t *uart, char *byte);

/**
 * @brief This is a non-service function. It does a blocking receive
 * of data from the external pin. This might block indefinetly.
 *
 * @param uart
 * @param byte
 * @return fp_ret_t
 */
fp_ret_t sdd_uart_blocking_recv(sdd_uart_config_t *uart, char *byte);

/**
 * @brief This is a non-service function. It performs a send operation.
 * It is blocking in the sense that it is the caller which is actually
 * performing the bit-banging which results in sending
 *
 * @param uart
 * @param byte
 * @return fp_ret_t
 */
fp_ret_t sdd_uart_blocking_send(sdd_uart_config_t *uart, char byte);

#endif