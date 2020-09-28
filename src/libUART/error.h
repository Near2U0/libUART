/**
 *
 * File Name: src/libUART/error.h
 * Title    : libUART C Library: Error Handling
 * Project  : libUART
 * Author   : Copyright (C) 2018-2020 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-11-20
 * Modified : 2020-09-28
 * Revised  : 
 * Version  : 0.2.0.0
 * License  : ISC (see file LICENSE.txt)
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */

#ifndef LIBUART_ERROR_H
#define LIBUART_ERROR_H

void error_set_errno(int num);
int error_get_errno(void);
void error_enable_msg(int enable);
void error(const char *func, const char *err_msg, int detail);

#endif
