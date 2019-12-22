//
// Created by sl on 19.02.17.
//

#include "mhz19.h"

/*
  Источник - https://revspace.nl/MHZ19
   2000 ppm range: 0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x07, 0xD0, 0x8F
   5000 ppm range: 0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB
*/

uint8_t GAS_CONCENTRATION_READ[] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
//uint8_t CALIBRATE_ZERO[] = {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};
uint8_t SET_RANGE_CMD[] = {0xff, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB}; // 0-5000 ppm
uint8_t ABC_OFF_CMD[] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};

void usart_setup(uint32_t usart, uint32_t baud, uint32_t bits, uint32_t stopbits, uint32_t mode, uint32_t parity,
                 uint32_t flowcontrol) {
    usart_disable(usart);

    // Настраиваем
    usart_set_baudrate(usart, baud);
    usart_set_databits(usart, bits);
    usart_set_stopbits(usart, stopbits);
    usart_set_mode(usart, mode);
    usart_set_parity(usart, parity);
    usart_set_flow_control(usart, flowcontrol);

    /* Enable USART Receive interrupt. */
    USART_CR1(usart) |= USART_CR1_RXNEIE;

    usart_enable(usart);
}

volatile uint8_t mhz19_cmdIndex = 99; // prevent garbage in iterrupt
volatile uint8_t mhz19LastResponse[9] = {0,};

wchar_t *statusString = L"неизвестен";

void mhz19_init(void) {
    usart_setup(MH_Z19_USART, 9600, 8, USART_STOPBITS_1, USART_MODE_TX_RX, USART_PARITY_NONE, USART_FLOWCONTROL_NONE);
}

void mhz19_readConcentrationCmd(void) {
    for (int i = 0; i < 9; i++)
        usart_send_blocking(MH_Z19_USART, GAS_CONCENTRATION_READ[i]);
    mhz19_cmdIndex = 0;

/*
 * Reading without interrupts
 */
/*
    for (int i = 0; i < 9; i++) {
        mhz19LastResponse[i] = usart_recv_blocking(MH_Z19_USART);

        // sometime read 0xFF twice
        if((i == 1) && (mhz19LastResponse[0] == 0xFF) && (mhz19LastResponse[1] == 0xFF))
            i = 0;
    }
*/
}

/*
void mhz19_calibrateZero() {
  for (int i = 0; i < 9; i++)
    usart_send_blocking(MH_Z19_USART, CALIBRATE_ZERO[i]);
}
*/

void mhz19_set_5000_diap()
{
    for (int i = 0; i < 9; i++)
        usart_send_blocking(MH_Z19_USART, SET_RANGE_CMD[i]);
}

void mhz19_set_abc_off()
{
    for (int i = 0; i < 9; i++)
        usart_send_blocking(MH_Z19_USART, ABC_OFF_CMD[i]);
}

void mhz19_isrHandler(uint8_t data) {
    if (mhz19_cmdIndex >= 9)
        return;
    mhz19LastResponse[mhz19_cmdIndex] = data;
    mhz19_cmdIndex++;

    if(mhz19_cmdIndex == 2) {
        if((mhz19LastResponse[0] == 0xFF) && (mhz19LastResponse[1] == 0xFF))
                mhz19_cmdIndex = 1;
    }
}

uint16_t mhz19_lastConcentration(uint16_t calibrated) {
    MHZ19_RESPONSE * r = (MHZ19_RESPONSE *) mhz19LastResponse;
    return r->HH * 256 + r->LL -calibrated;
}

int16_t mhz19_lastTempCelsius() {
    MHZ19_RESPONSE * r = (MHZ19_RESPONSE *) mhz19LastResponse;
    return r->TT - 40;
}

uint8_t *mhz19_lastResp() {
    return (uint8_t *)mhz19LastResponse;
}

wchar_t * mhz19_lastStatus() {
    MHZ19_RESPONSE * r = (MHZ19_RESPONSE *) mhz19LastResponse;
    statusString = L"........";
    uint8_t ss = r->SS;
    for (uint8_t i=0; i<8;i++) {
        if (ss & 0x01)
            statusString[7-i] = L'l';
        else
            statusString[7-i] = L'0';
        ss >>= 1;
    }
    return statusString;
}


uint8_t mhz19_calcLastCrc() {
    return 0xff - (mhz19LastResponse[1] + mhz19LastResponse[2] + mhz19LastResponse[3] +
            mhz19LastResponse[4] + mhz19LastResponse[5] + mhz19LastResponse[6] + mhz19LastResponse[7])
            + 0x01;
}
