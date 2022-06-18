#include <Arduino.h>
#include <U8g2lib.h>
#include <stdlib.h>
#include <stdio.h>


U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, PB6, PB7, U8X8_PIN_NONE); 
//U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ PB6, /* data=*/ PB7, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

void setup(void) {
  u8g2.begin();
}

int _hora = 9;
int _minuto = 49;
int _segundo = 15;
int _dia = 24;
int _mes = 3;
int _weekday = 5;
bool _state = true;
int _brilho = 0;
int _selection = 1;
int _an = 1;
int _ano = 2022;

const int WAITTIME = 50;
const int STEP = 5;

void loop(void) {
  u8g2.clearBuffer();          // clear the internal memory
  if(_brilho == 0)
    u8g2.setContrast(255);
  else
    u8g2.setContrast(1);
  //MENU
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.drawFrame(3,24,15,20);

  String diastr = String(_dia);
  String messtr = String(_mes);
  u8g2.setFont(u8g2_font_5x7_tr);
  if(_dia < 10) {
    u8g2.setCursor(5,33);
    u8g2.print("0");
    u8g2.setCursor(10,33);
    u8g2.print(diastr);
  }
  else {
    u8g2.setCursor(5,33);
    u8g2.print(diastr);
  }
  if(_mes < 10) {
    u8g2.setCursor(5,41);
    u8g2.print("0");
    u8g2.setCursor(10,41);
    u8g2.print(messtr);
  }
  else {
    u8g2.setCursor(6,41);
    u8g2.print(messtr);
  }
  u8g2.setFont(u8g2_font_tinytim_tr);
  switch(_weekday) {
    case 1:
      u8g2.drawStr(2,53,"DOM");
    break;
    case 2:
      u8g2.drawStr(2,53,"SEG");
    break;
    case 3:
      u8g2.drawStr(2,53,"TER");
    break;
    case 4:
      u8g2.drawStr(2,53,"QUA");
    break;
    case 5:
      u8g2.drawStr(2,53,"QUI");
    break;
    case 6:
      u8g2.drawStr(2,53,"SEX");
    break;
    case 7:
      u8g2.drawStr(2,53,"SAB");
    break;
  }

  switch(_selection) {
    case 1: //HORA
      u8g2.setFontMode(1);
      u8g2.setDrawColor(1); /* color 1 for the box */
      u8g2.drawBox(0, 7, 28, 8);
    
      u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
      u8g2.setFontMode(0);
      u8g2.setDrawColor(0);
      u8g2.drawStr(0,15,"Hora"); // write something to the internal memory
    
      u8g2.setDrawColor(1);
      u8g2.drawStr(45,15,"Alarme");
      u8g2.drawStr(102,15,"Data");
    break;
    case 2: //ALARME
      u8g2.setFontMode(1);
      u8g2.setDrawColor(1); /* color 1 for the box */
      u8g2.drawBox(45, 7, 40, 8);
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0,15,"Hora"); // write something to the internal memory
      
      u8g2.setDrawColor(0);
      u8g2.drawStr(45,15,"Alarme");
      
      u8g2.setDrawColor(1);
      u8g2.drawStr(102,15,"Data");
    break;
    case 3: //DATA
      u8g2.setFontMode(1);
      u8g2.setDrawColor(1); /* color 1 for the box */
      u8g2.drawBox(100, 7, 25, 8);
      
      u8g2.setFontMode(0);
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0,15,"Hora"); // write something to the internal memory
      
      u8g2.setDrawColor(1);
      u8g2.drawStr(45,15,"Alarme");
      
      u8g2.setDrawColor(0);
      u8g2.drawStr(102,15,"Data");
      u8g2.setDrawColor(1);
    break;
  }

  u8g2.setFont(u8g2_font_timB24_tr); 
  String horasstr = String(_hora);
  String minutesstr = String(_minuto);
  if(_hora < 10) {
    u8g2.setCursor(23,50);
    u8g2.print("0");
    u8g2.setCursor(40,50);
    u8g2.print(horasstr);
  }
  else {
    u8g2.setCursor(23,50);
    u8g2.print(horasstr);
  }

    u8g2.drawStr(57,47,":");

  if(_minuto < 10) {
    u8g2.setCursor(69,50);
    u8g2.print("0");
    u8g2.setCursor(86,50);
    u8g2.print(minutesstr);
  }
  else {
    u8g2.setCursor(69,50);
    u8g2.print(minutesstr);
  }
  
  String segundosstr = String(_segundo);

  u8g2.setFont(u8g2_font_ncenB08_tr);
  if(_segundo < 10) {
    u8g2.setCursor(110,37);
    u8g2.print("0");
    u8g2.setCursor(116,37);
    u8g2.print(segundosstr);
  }
  else {
    u8g2.setCursor(110,37);
    u8g2.print(segundosstr);
  }
  
  u8g2.sendBuffer();
  delay(5000);
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_ncenB08_tr);
  if(_selection == 1) 
    u8g2.drawStr(15,12,"Pressione o botao");
  u8g2.setFont(u8g2_font_streamline_interface_essential_alert_t);
  u8g2.drawGlyph(55, 50, 0x0033);
  u8g2.sendBuffer();
  delay(2000);
}
