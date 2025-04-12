#include "main.h"
#include "st7567.h"
#include "font.h"

extern SPI_HandleTypeDef hspi1;

void ST7567sendCommand(uint8_t data){
  CS_ON;
  COMMAND;
  HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
  CS_OFF;
}

void ST7567sendData(uint8_t data){
  CS_ON;
  DATA;
  HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
  CS_OFF;
}

void ST7567reset(void){
  RESET_ON;
  HAL_Delay(200);
  RESET_OFF;
}

void ST7567poweroff(void) {
  ST7567sendCommand(ST7567_DISPLAY_OFF);
  ST7567sendCommand(ST7567_ALLPTS_ON);
}

void ST7567poweron(void) {
  ST7567sendCommand(ST7567_ALLPTS_NORMAL);
  ST7567sendCommand(ST7567_DISPLAY_ON);
}

void ST7567clear(void) {
  for(uint8_t page = 0;page<4;page++) {
    ST7567sendCommand(ST7567_PAGE_ADDRESS | page);
    ST7567sendCommand(ST7567_COLUMN_UPPER);
    ST7567sendCommand(ST7567_COLUMN_LOWER);
    for(uint16_t number=0;number<132;number++) ST7567sendData(0x00);
  }
}

void ST7567char(uint8_t symbol, uint8_t page, uint8_t col) {
  ST7567sendCommand(ST7567_PAGE_ADDRESS | page);
  ST7567sendCommand(ST7567_COLUMN_UPPER | ((col >> 4) & 0x0F));
  ST7567sendCommand(col & 0x0F);
  for (uint8_t k = 0; k < FONT_SIZE_X; k++ ) ST7567sendData(nlcd_Font[symbol][k]);
}

void ST7567string(uint8_t* buff, uint8_t len, uint8_t page, uint8_t col) {
  uint8_t symbol;
  ST7567sendCommand(ST7567_PAGE_ADDRESS | page);
  ST7567sendCommand(ST7567_COLUMN_UPPER | ((col >> 4) & 0x0F));
  ST7567sendCommand(col & 0x0F);
  for(uint8_t index=0;index<len;index++) {
    symbol = buff[index];
    for (uint8_t k = 0; k < FONT_SIZE_X; k++ ) ST7567sendData(nlcd_Font[symbol][k]);
    ST7567sendData(0x00);
  }
}

void ST7567init(void){
  ST7567reset();
  ST7567sendCommand(ST7567_BIAS_9);
  ST7567sendCommand(ST7567_DIRECTION_REVERSE);
  ST7567sendCommand(ST7567_COM_NORMAL);
  ST7567sendCommand(ST7567_REGULATION_RATIO | 0x04);
  ST7567sendCommand(ST7567_SETEV);
  ST7567sendCommand(0x20);
  ST7567sendCommand(ST7567_POWERCTRL_VB);
  ST7567sendCommand(ST7567_POWERCTRL_VB | ST7567_POWERCTRL_VR);
  ST7567sendCommand(ST7567_POWERCTRL_VB | ST7567_POWERCTRL_VR | ST7567_POWERCTRL_VF);
  ST7567sendCommand(ST7567_SETBOOSTER);
  ST7567sendCommand(ST7567_SETBOOSTER4X);

  ST7567sendCommand(ST7567_EXT_COMMAND_SET);
  ST7567sendCommand(ST7567_SETTING_MODE);
  ST7567sendCommand(ST7567_EXT_COMMAND_UNSET);
  ST7567sendCommand(ST7567_DUTY | 0x06);
  ST7567sendCommand(ST7567_BIAS | 0x03);
  ST7567sendCommand(ST7567_FREQ);  
  ST7567sendCommand(ST7567_DISPLAY_ON);

  ST7567sendCommand(ST7567_START_LINE);
  ST7567sendCommand(ST7567_PAGE_ADDRESS);
  ST7567sendCommand(ST7567_COLUMN_UPPER);
  ST7567sendCommand(ST7567_COLUMN_LOWER);
  ST7567clear();
}
