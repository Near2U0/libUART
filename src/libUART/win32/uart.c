/**
 *
 * File Name: win32/uart.c
 * Title    : WIN32 UART
 * Project  : libUART
 * Author   : Copyright (C) 2018-2020 Johannes Krottmayer <krjdev@gmail.com>
 * Created  : 2019-11-20
 * Modified : 2020-09-26
 * Revised  : 
 * Version  : 0.2.2.0
 * License  : ISC (see file LICENSE.txt)
 *
 * NOTE: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested. Nonetheless, you can expect most functions to work.
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "../util.h"
#include "../libUART.h"

#include "error.h"
#include "uart.h"

/* COMMPROP structure */
/* COMMPROP.dwMaxBaud member */
#define WIN_BAUD_075                0x00000001
#define WIN_BAUD_110                0x00000002
#define WIN_BAUD_134_5              0x00000004
#define WIN_BAUD_150                0x00000008
#define WIN_BAUD_300                0x00000010
#define WIN_BAUD_600                0x00000020
#define WIN_BAUD_1200               0x00000040
#define WIN_BAUD_1800               0x00000080
#define WIN_BAUD_2400               0x00000100
#define WIN_BAUD_4800               0x00000200
#define WIN_BAUD_7200               0x00000400
#define WIN_BAUD_9600               0x00000800
#define WIN_BAUD_14400              0x00001000
#define WIN_BAUD_19200              0x00002000
#define WIN_BAUD_38400              0x00004000
#define WIN_BAUD_56K                0x00008000
#define WIN_BAUD_57600              0x00040000
#define WIN_BAUD_115200             0x00020000
#define WIN_BAUD_128K               0x00010000
#define WIN_BAUD_USER               0x10000000

/* COMMPROP.dwSettableParams member */
#define WIN_SP_BAUD                 0x0002
#define WIN_SP_DATABITS             0x0004
#define WIN_SP_HANDSHAKING          0x0010
#define WIN_SP_PARITY               0x0001
#define WIN_SP_PARITY_CHECK         0x0020
#define WIN_SP_STOPBITS             0x0008

/* COMMPROP.wSettableData member */
#define WIN_DATABITS_5              0x0001
#define WIN_DATABITS_6              0x0002
#define WIN_DATABITS_7              0x0004
#define WIN_DATABITS_8              0x0008
#define WIN_DATABITS_16             0x0010
#define WIN_DATABITS_16X            0x0020

/* COMMPROP.wSettableStopParity member */
#define WIN_STOPBITS_10             0x0001
#define WIN_STOPBITS_15             0x0002
#define WIN_STOPBITS_20             0x0004
#define WIN_PARITY_NONE             0x0100
#define WIN_PARITY_ODD              0x0200
#define WIN_PARITY_EVEN             0x0400

int get_prop(struct _uart *uart)
{
    BOOL ret;
    
    ret = GetCommProperties(uart->h, &uart->prop);
    
    if (!ret) {
        error("GetCommProperties() failed", 1);
        return -1;
    }
    
    return 0;
}

/* 
 * HACK: Maybe removed on further releases.
 */
int uart_baud_valid(int value)
{
    int E[] = {
        UART_BAUD_110,
        UART_BAUD_300,
        UART_BAUD_600,
        UART_BAUD_1200,
        UART_BAUD_2400,
        UART_BAUD_4800,
        UART_BAUD_9600,
        UART_BAUD_14400,
        UART_BAUD_19200,
        UART_BAUD_38400,
        UART_BAUD_57600,
        UART_BAUD_115200,
        UART_BAUD_128000,
        UART_BAUD_256000
    };
    
    if (enum_contains(E, sizeof(E)/sizeof(E[0]), value))
        return 1;
    
    return 0;
}

int uart_init_baud(struct _uart *uart)
{
    BOOL ret;
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);

    ret = GetCommState(uart->h, &dcb);
    
    if (!ret) {
        error("GetCommState() failed", 1);
        return -1;
    }
    
    if (!(uart->prop.dwSettableParams & SP_BAUD)) {
        error("Hardware/Driver doesn't support settable baud rate", 0);
        return -1;
    }
    
    switch (uart->baud) {
    case UART_BAUD_110:
        dcb.BaudRate = CBR_110;
        break;
    case UART_BAUD_300:
        dcb.BaudRate = CBR_300;
        break;
    case UART_BAUD_600:
        dcb.BaudRate = CBR_600;
        break;
    case UART_BAUD_1200:
        dcb.BaudRate = CBR_1200;
        break;
    case UART_BAUD_2400:
        dcb.BaudRate = CBR_2400;
        break;
    case UART_BAUD_4800:
        dcb.BaudRate = CBR_4800;
        break;
    case UART_BAUD_9600:
        dcb.BaudRate = CBR_9600;
        break;
    case UART_BAUD_19200:
        dcb.BaudRate = CBR_19200;
        break;
    case UART_BAUD_38400:
        dcb.BaudRate = CBR_38400;
        break;
    case UART_BAUD_57600:
        dcb.BaudRate = CBR_57600;
        break;
    case UART_BAUD_115200:
        dcb.BaudRate = CBR_115200;
        break;
    case UART_BAUD_128000:
        dcb.BaudRate = CBR_128000;
        break;
    case UART_BAUD_256000:
        dcb.BaudRate = CBR_256000;
        break;
    default:
        error("Invalid baud rate", 0);
        return -1;
    }
    
    ret = SetCommState(uart->h, &dcb);
    
    if (!ret) {
        error("Baud Rate: SetCommState() failed", 1);
        return -1;
    }
    
    return 0;
}

int uart_init_databits(struct _uart *uart)
{
    BOOL ret;
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    
    ret = GetCommState(uart->h, &dcb);
    
    if (!ret) {
        error("GetCommState() failed", 1);
        return -1;
    }
    
    if (!(uart->prop.dwSettableParams & WIN_SP_DATABITS)) {
        error("Hardware/Driver doesn't support settable data bits length", 0);
        return -1;
    }

    switch (uart->data_bits) {
    case 5:
        if (!(uart->prop.wSettableData & WIN_DATABITS_5)) {
            error("Hardware/Driver doesn't support data bits length 5", 0);
            return -1;
        }
        
        dcb.ByteSize = 5;
        break;
    case 6:
        if (!(uart->prop.wSettableData & WIN_DATABITS_6)) {
            error("Hardware/Driver doesn't support data bits length 6", 0);
            return -1;
        }
        
        dcb.ByteSize = 6;
        break;
    case 7:
        if (!(uart->prop.wSettableData & WIN_DATABITS_7)) {
            error("Hardware/Driver doesn't support data bits length 7", 0);
            return -1;
        }
        
        dcb.ByteSize = 7;
        break;
    case 8:
        if (!(uart->prop.wSettableData & WIN_DATABITS_8)) {
            error("Hardware/Driver doesn't support data bits length 8", 0);
            return -1;
        }
        
        dcb.ByteSize = 8;
        break;
    default:
        error("Invalid Data Bits", 0);
        return -1;
    }
    
    ret = SetCommState(uart->h, &dcb);
    
    if (!ret) {
        error("Data Bits: SetCommState() failed", 1);
        return -1;
    }
    
    return 0;
}

int uart_init_parity(struct _uart *uart)
{
    BOOL ret;
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    
    ret = GetCommState(uart->h, &dcb);
    
    if (!ret) {
        error("GetCommState() failed", 1);
        return -1;
    }
    
    if (!(uart->prop.dwSettableParams & WIN_SP_PARITY)) {
        error("Hardware/Driver doesn't support settable/checkable parity", 0);
        return -1;
    }
    
    switch (uart->parity) {
    case UART_PARITY_NO:
        if (!(uart->prop.wSettableStopParity & WIN_PARITY_NONE)) {
            error("Hardware/Driver doesn't support none parity", 0);
            return -1;
        }
        
        dcb.Parity = NOPARITY;
        break;
    case UART_PARITY_ODD:
        if (!(uart->prop.wSettableStopParity & WIN_PARITY_ODD)) {
            error("Hardware/Driver doesn't support odd parity", 0);
            return -1;
        }
        
        dcb.Parity = ODDPARITY;
        break;
    case UART_PARITY_EVEN:
        if (!(uart->prop.wSettableStopParity & WIN_PARITY_EVEN)) {
            error("Hardware/Driver doesn't support even parity", 0);
            return -1;
        }
        
        dcb.Parity = EVENPARITY;
        break;
    default:
        error("Invalid Parity", 0);
        return -1;
    }
    
    ret = SetCommState(uart->h, &dcb);
    
    if (!ret) {
        error("Parity: SetCommState() failed", 1);
        return -1;
    }
    
    return 0;
}

int uart_init_stopbits(struct _uart *uart)
{
    BOOL ret;
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    
    ret = GetCommState(uart->h, &dcb);
    
    if (!ret) {
        error("GetCommState() failed", 1);
        return -1;
    }
    
    if (!(uart->prop.dwSettableParams & WIN_SP_DATABITS)) {
        error("Hardware/Driver doesn't support settable stop bits length", 0);
        return -1;
    }
    
    switch (uart->stop_bits) {
    case 1:
        if (!(uart->prop.wSettableStopParity & WIN_STOPBITS_10)) {
            error("Hardware/Driver doesn't support stop bits length 1", 0);
            return -1;
        }
        
        dcb.StopBits = ONESTOPBIT;
        break;
    case 2:
        if (!(uart->prop.wSettableStopParity & WIN_STOPBITS_20)) {
            error("Hardware/Driver doesn't support stop bits length 2", 0);
            return -1;
        }
        
        dcb.StopBits = TWOSTOPBITS;
        break;
    default:
        error("Invalid Stop Bits", 0);
        return -1;
    }
    
    ret = SetCommState(uart->h, &dcb);
    
    if (!ret) {
        error("Stop Bits: SetCommState() failed", 1);
        return -1;
    }
    
    return 0;
}

int uart_init_flow(struct _uart *uart)
{
    BOOL ret;
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    
    ret =  GetCommState(uart->h, &dcb);
    
    if (!ret) {
        error("GetCommState() failed", 1);
        return -1;
    }
    
    switch (uart->flow_ctrl) {
    case UART_FLOW_NO:
        dcb.fOutxCtsFlow = FALSE,
        dcb.fOutxDsrFlow = FALSE,
        dcb.fDtrControl = DTR_CONTROL_DISABLE;
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
        dcb.fOutX = FALSE;
        dcb.fInX = FALSE;
        break;
    case UART_FLOW_SOFTWARE:
        dcb.fOutxCtsFlow = FALSE,
        dcb.fOutxDsrFlow = FALSE,
        dcb.fDtrControl = DTR_CONTROL_DISABLE;
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
        dcb.fOutX = TRUE;
        dcb.fInX = TRUE;
        break;
    case UART_FLOW_HARDWARE:
        dcb.fOutxCtsFlow = TRUE,
        dcb.fOutxDsrFlow = TRUE,
        dcb.fDtrControl = DTR_CONTROL_ENABLE;
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
        dcb.fOutX = FALSE;
        dcb.fInX = FALSE;
        break;
    default:
        error("invalid Flow Control", 0);
        return -1;
    }
    
    ret =  SetCommState(uart->h, &dcb);
    
    if (!ret) {
        error("SetCommState() failed", 1);
        return -1;
    }
    
    return 0;
}

int uart_init(struct _uart *uart)
{
    int ret;
    
    /* set baud rate */
    ret = uart_init_baud(uart);
    
    if (ret == -1)
        return -1;
    
    /* set data bits */
    ret = uart_init_databits(uart);
    
    if (ret == -1)
        return -1;
    
    /* set parity */
    ret = uart_init_parity(uart);
    
    if (ret == -1)
        return -1;
    
    /* set stop bits */
    ret = uart_init_stopbits(uart);
    
    if (ret == -1)
        return -1;
    
    /* set flow control */
    ret = uart_init_flow(uart);
    
    if (ret == -1)
        return -1;
    
    return 0;
}

int uart_open(struct _uart *uart)
{
    int ret;
    HANDLE h;
    
    h = CreateFile((LPCTSTR) uart->dev,
                   GENERIC_READ | GENERIC_WRITE,
                   0,
                   NULL,
                   OPEN_EXISTING,
                   0,
                   NULL);
    
    if (h == INVALID_HANDLE_VALUE) {
        error("CreateFile() failed", 1);
        return -1;
    }
    
    uart->h = h;
    
    ret = get_prop(uart);
    
    if (ret == -1) {
        CloseHandle(h);
        return -1;
    }
    
    ret = uart_init(uart);
    
    if (ret == -1) {
        CloseHandle(h);
        return -1;
    }
    
    return 0;
}

void uart_close(struct _uart *uart)
{
    CloseHandle(uart->h);
    free(uart);
    uart = NULL;
}

int uart_send(struct _uart *uart, char *send_buf, int len)
{
    int ret;
    DWORD dwbytestowrite = (DWORD) len;
    DWORD dwbyteswritten;
    
    ret = WriteFile(uart->h, 
                    (LPVOID) send_buf, 
                    dwbytestowrite, 
                    &dwbyteswritten, 
                    NULL);
    
    if (!ret) {
        error("WriteFile() failed", 1);
        return -1;
    }
    
    ret = (int) dwbyteswritten;
    
    if (ret != len) {
        error("could not send all bytes", 0);
        return ret;
    }
    
    return ret;
}

int uart_recv(struct _uart *uart, char *recv_buf, int len)
{
    int ret = 0;
    DWORD dwbytesread = (DWORD) len;
    
    ret = ReadFile(uart->h, 
                   (LPVOID) recv_buf, 
                   (DWORD) len, 
                   &dwbytesread, 
                   NULL);
    
    if (!ret) {
        error("ReadFile() failed", 1);
        return -1;
    }
    
    ret = (int) dwbytesread;
    return ret;
}

int uart_flush(struct _uart *uart)
{
    int ret;
    
    ret = FlushFileBuffers(uart->h);
    
    if (!ret) {
        error("FlushFileBuffers() failed", 1);
        return -1;
    }

    return 0;
}

int uart_set_pin(struct _uart *uart, int pin, int state)
{
    int ret;
    DWORD dwfunc;
    
    switch (pin) {
    case UART_PIN_RTS:
        if (state == UART_PIN_HIGH)
            dwfunc = SETRTS;
        else
            dwfunc = CLRRTS;
        break;
    case UART_PIN_DTR:
        if (state == UART_PIN_HIGH)
            dwfunc = SETDTR;
        else
            dwfunc = CLRDTR;
        break;
    default:
        error("invalid pin", 0);
        return -1;
    }
    
    ret = EscapeCommFunction(uart->h, dwfunc);
    
    if (!ret) {
        error("EscapeCommFunction() failed", 1);
        return -1;
    }

    return 0;
}

int uart_get_pin(struct _uart *uart, int pin, int *state)
{
    int ret = 0;
    DWORD dwmstat;

    ret = GetCommModemStatus(uart->h, &dwmstat);

    if (!ret) {
        error("GetCommModemStatus() failed", 1);
        return -1;
    }
    
    switch (pin) {
    case UART_PIN_CTS:
        if (dwmstat == MS_CTS_ON)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    case UART_PIN_DSR:
        if (dwmstat == MS_DSR_ON)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    case UART_PIN_DCD:
        if (dwmstat == MS_RLSD_ON)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    case UART_PIN_RI:
        if (dwmstat == MS_RING_ON)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    default:
        error("invalid pin", 0);
        ret = -1;
    }
    
    return ret;
}

int uart_get_bytes(struct _uart *uart, int *bytes)
{
    int ret = 0;
    COMSTAT comst;
    DWORD dwerror;
    
    ret = ClearCommError(uart->h, &dwerror, &comst);
    
    if (!ret) {
        error("ClearCommError() failed", 1);
        return -1;
    }
    
    ret = (int) comst.cbInQue;
    
    (*bytes) = ret;
    return 0;
}
