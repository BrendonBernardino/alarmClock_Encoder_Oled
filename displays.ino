void Menu_init(void);
void Menu_standy(int _hora, int _minuto);
void Menu_standby2(int _hora, int _minuto);
void Menu_standby_alarm(int _hora, int _minuto);
void Menu_standby_alarm2(int _hora, int _minuto);
void Menu(int _selection, int _weekday, int _dia, int _mes, int _hora, int _minuto);
void ConfigHora(void);
void ConfigHora1(int _hora, int _minuto);
void ConfigHora2(int _hora, int _minuto);
void ConfigHora3(int _hora, int _minuto);
void ConfigAlarme(void);
void ConfigData(void);
void Data_config(int _selection, int _dayweek, int _day, int _month, int _year);
void SelectAlarme(int _selection);
void AlarmeA_config(int _selection, int _an, bool _state, int _hora, int _minuto);



void Menu_init(void) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_open_iconic_embedded_4x_t);
  u8g2.drawGlyph(50, 30, 0x004e);
  u8g2.sendBuffer();
}

void Menu_standby(int _hora, int _minuto) {
  u8g2.clearBuffer();
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_timB24_tr);
  String horasstr = String(_hora);
  String minutesstr = String(_minuto);
  if(_hora < 10) {
    u8g2.setCursor(25,32);
    u8g2.print("0");
    u8g2.setCursor(41,32);
    u8g2.print(horasstr);
  }
  u8g2.setCursor(25,32);
  u8g2.print(horasstr);

//  u8g2.drawStr(57,32," ");
  u8g2.drawStr(57,30,":");

  if(_minuto < 10) {
    u8g2.setCursor(68,32);
    u8g2.print("0");
    u8g2.setCursor(84,32);
    u8g2.print(minutesstr);
  }
  u8g2.setCursor(68,32);
  u8g2.print(minutesstr);
  
//  u8g2.drawStr(25,32,"12:30");
  u8g2.sendBuffer();
}

void Menu_standby2(int _hora, int _minuto) {
  u8g2.clearBuffer();
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_timB24_tr);
  String horasstr = String(_hora);
  String minutesstr = String(_minuto);
  if(_hora < 10) {
    u8g2.setCursor(25,32);
    u8g2.print("0");
    u8g2.setCursor(41,32);
    u8g2.print(horasstr);
  }
  u8g2.setCursor(25,32);
  u8g2.print(horasstr);

//  u8g2.drawStr(57,32," ");
//  u8g2.drawStr(57,35,":");

  if(_minuto < 10) {
    u8g2.setCursor(68,32);
    u8g2.print("0");
    u8g2.setCursor(84,32);
    u8g2.print(minutesstr);
  }
  u8g2.setCursor(68,32);
  u8g2.print(minutesstr);
  
//  u8g2.drawStr(25,32,"12:30");
  u8g2.sendBuffer();
}

void Menu_standby_alarm(int _hora, int _minuto) {
  u8g2.clearBuffer();
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawGlyph(0, 21, 0x23f0);
//  u8g2.setFont(u8g2_font_streamline_interface_essential_alert_t);
//  u8g2.drawGlyph(5, 21, 0x0032);
  u8g2.setFont(u8g2_font_timB24_tr);
  String horasstr = String(_hora);
  String minutesstr = String(_minuto);
  if(_hora < 10) {
    u8g2.setCursor(25,32);
    u8g2.print("0");
    u8g2.setCursor(41,32);
    u8g2.print(horasstr);
  }
  u8g2.setCursor(25,32);
  u8g2.print(horasstr);

//  u8g2.drawStr(57,32," ");
  u8g2.drawStr(57,30,":");

  if(_minuto < 10) {
    u8g2.setCursor(68,32);
    u8g2.print("0");
    u8g2.setCursor(84,32);
    u8g2.print(minutesstr);
  }
  u8g2.setCursor(68,32);
  u8g2.print(minutesstr);
//  u8g2.drawStr(25,32,"12:30");
  u8g2.sendBuffer();
}

void Menu_standby_alarm2(int _hora, int _minuto) {
  u8g2.clearBuffer();
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawGlyph(0, 21, 0x23f0);
//  u8g2.setFont(u8g2_font_streamline_interface_essential_alert_t);
//  u8g2.drawGlyph(5, 21, 0x0032);
  u8g2.setFont(u8g2_font_timB24_tr);
  String horasstr = String(_hora);
  String minutesstr = String(_minuto);
  if(_hora < 10) {
    u8g2.setCursor(25,32);
    u8g2.print("0");
    u8g2.setCursor(41,32);
    u8g2.print(horasstr);
  }
  u8g2.setCursor(25,32);
  u8g2.print(horasstr);

//  u8g2.drawStr(57,32," ");
//  u8g2.drawStr(57,35,":");

  if(_minuto < 10) {
    u8g2.setCursor(68,32);
    u8g2.print("0");
    u8g2.setCursor(84,32);
    u8g2.print(minutesstr);
  }
  u8g2.setCursor(68,32);
  u8g2.print(minutesstr);
//  u8g2.drawStr(25,32,"12:30");
  u8g2.sendBuffer();
}

void Menu(int _selection, int _weekday, int _dia, int _mes, int _hora, int _minuto) {
  int segundos = 23;
  
  u8g2.clearBuffer();          // clear the internal memory
  //MENU
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_ncenB08_tr);
//  u8g2.drawLine(30, 5, 100, 5);
  u8g2.drawFrame(3,9,15,16);

  String diastr = String(_dia);
  String messtr = String(_mes);
  u8g2.setFont(u8g2_font_tinytim_tr);
  if(_dia < 10) {
    u8g2.setCursor(5,16);
    u8g2.print("0");
    u8g2.setCursor(10,16);
    u8g2.print(diastr);
  }
  else {
    u8g2.setCursor(5,16);
    u8g2.print(diastr);
  }
  if(_mes < 10) {
    u8g2.setCursor(5,23);
    u8g2.print("0");
    u8g2.setCursor(10,23);
    u8g2.print(messtr);
  }
  else {
    u8g2.setCursor(5,23);
    u8g2.print(messtr);
  }
//  u8g2.drawStr(5,16,"21");
//  u8g2.drawStr(5,23,"02");
  u8g2.setFont(u8g2_font_tinytim_tr);
  switch(_weekday) {
    case 1:
      u8g2.drawStr(2,32,"DOM");
    break;
    case 2:
      u8g2.drawStr(2,32,"SEG");
    break;
    case 3:
      u8g2.drawStr(2,32,"TER");
    break;
    case 4:
      u8g2.drawStr(2,32,"QUA");
    break;
    case 5:
      u8g2.drawStr(2,32,"QUI");
    break;
    case 6:
      u8g2.drawStr(2,32,"SEX");
    break;
    case 7:
      u8g2.drawStr(2,32,"SAB");
    break;
  }

  switch(_selection) {
    case 1: //HORA
      u8g2.setFontMode(1);
      u8g2.setDrawColor(1); /* color 1 for the box */
      u8g2.drawBox(0, 0, 28, 8);
    
      u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
      u8g2.setFontMode(0);
      u8g2.setDrawColor(0);
      u8g2.drawStr(0,8,"Hora"); // write something to the internal memory
    
      u8g2.setDrawColor(1);
      u8g2.drawStr(45,8,"Alarme");
      u8g2.drawStr(102,8,"Data");
    break;
    case 2: //ALARME
      u8g2.setFontMode(1);
      u8g2.setDrawColor(1); /* color 1 for the box */
      u8g2.drawBox(45, 0, 40, 8);
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0,8,"Hora"); // write something to the internal memory
      
      u8g2.setDrawColor(0);
      u8g2.drawStr(45,8,"Alarme");
      
      u8g2.setDrawColor(1);
      u8g2.drawStr(102,8,"Data");
    break;
    case 3: //DATA
      u8g2.setFontMode(1);
      u8g2.setDrawColor(1); /* color 1 for the box */
      u8g2.drawBox(100, 0, 25, 8);
      
      u8g2.setFontMode(0);
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0,8,"Hora"); // write something to the internal memory
      
      u8g2.setDrawColor(1);
      u8g2.drawStr(45,8,"Alarme");
      
      u8g2.setDrawColor(0);
      u8g2.drawStr(102,8,"Data");
      u8g2.setDrawColor(1);
    break;
  }

  u8g2.setFont(u8g2_font_timB24_tr);//u8g2_font_luBIS19_tn  
  String horasstr = String(_hora);
  String minutesstr = String(_minuto);
  if(_hora < 10) {
    u8g2.setCursor(25,32);
    u8g2.print("0");
    u8g2.setCursor(41,32);
    u8g2.print(horasstr);
  }
  else {
    u8g2.setCursor(25,32);
    u8g2.print(horasstr);
  }

//  u8g2.drawStr(57,32," ");
  u8g2.drawStr(57,30,":");

  if(_minuto < 10) {
    u8g2.setCursor(68,32);
    u8g2.print("0");
    u8g2.setCursor(84,32);
    u8g2.print(minutesstr);
  }
  else {
    u8g2.setCursor(68,32);
    u8g2.print(minutesstr);
  }
  
  String segundosstr = String(segundos);

  u8g2.setFont(u8g2_font_ncenB08_tr);
  if(segundos < 10) {
    u8g2.setCursor(110,32);
    u8g2.print("0");
    u8g2.setCursor(115,32);
    u8g2.print(segundosstr);
  }
  else {
    u8g2.setCursor(110,32);
    u8g2.print(segundosstr);
  }
//  if(segundos == 59) {
//    segundos = 0;
//    minutes++;
//  }
  
//  u8g2.drawStr(25,32,"12:30");
//  u8g2.setFont(u8g2_font_ncenB08_tr);
//  u8g2.drawStr(110,32,"25");
  u8g2.sendBuffer();
}

void ConfigHora(void) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(49,8,"HORA");  // write something to the internal memory
  u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
  u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
//  u8g2.drawStr(49,8,"OK");  // write something to the internal memory
  u8g2.setFont(u8g2_font_inb21_mn);
  u8g2.drawStr(20,30,"12");
  u8g2.drawStr(60,30,":");
  u8g2.drawStr(80,30,"30");
  u8g2.sendBuffer();          // transfer internal memory to the display
}

void ConfigHora1(int _hora, int _minuto) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(49,8,"HORA");  // write something to the internal memory
  u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
  u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
//  u8g2.drawStr(49,8,"OK");  // write something to the internal memory
  u8g2.setFont(u8g2_font_inb21_mn);

  String horasstr = String(_hora);
  String minutesstr = String(_minuto);
  if(_hora < 10) {
    u8g2.setCursor(15,30);
    u8g2.print("0");
    u8g2.setCursor(32,30);
    u8g2.print(horasstr);
  }
  else {
    u8g2.setCursor(15,30);
    u8g2.print(horasstr);
  }
  if(_minuto < 10) {
    u8g2.setCursor(75,30);
    u8g2.print("0");
    u8g2.setCursor(92,30);
    u8g2.print(minutesstr);
  }
  else {
    u8g2.setCursor(75,30);
    u8g2.print(minutesstr);
  }
//  u8g2.drawStr(15,30,"12");
  u8g2.drawLine(15, 31, 50, 31);
  u8g2.drawStr(55,30,":");
//  u8g2.drawStr(75,30,"30");
  u8g2.sendBuffer();
}

void ConfigHora2(int _hora, int _minuto) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(49,8,"HORA");  // write something to the internal memory
  u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
  u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
//  u8g2.drawStr(49,8,"OK");  // write something to the internal memory

  u8g2.setFont(u8g2_font_inb21_mn);
  String horasstr = String(_hora);
  String minutesstr = String(_minuto);
  if(_hora < 10) {
    u8g2.setCursor(15,30);
    u8g2.print("0");
    u8g2.setCursor(32,30);
    u8g2.print(horasstr);
  }
  else {
    u8g2.setCursor(15,30);
    u8g2.print(horasstr);
  }
  if(_minuto < 10) {
    u8g2.setCursor(75,30);
    u8g2.print("0");
    u8g2.setCursor(92,30);
    u8g2.print(minutesstr);
  }
  else {
    u8g2.setCursor(75,30);
    u8g2.print(minutesstr);
  }
  
//  u8g2.drawStr(15,30,"12");
  u8g2.drawLine(75, 31, 110, 31);
  u8g2.drawStr(55,30,":");
//  u8g2.drawStr(75,30,"30");
  u8g2.sendBuffer();
}

void ConfigHora3(int _hora, int _minuto) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(49,8,"HORA");  // write something to the internal memory
  u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
  u8g2.drawButtonUTF8(115, 6, U8G2_BTN_INV|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
//  u8g2.drawStr(49,8,"OK");  // write something to the internal memory

  u8g2.setFont(u8g2_font_inb21_mn);
  String horasstr = String(_hora);
  String minutesstr = String(_minuto);
  if(_hora < 10) {
    u8g2.setCursor(15,30);
    u8g2.print("0");
    u8g2.setCursor(32,30);
    u8g2.print(horasstr);
  }
  else {
    u8g2.setCursor(15,30);
    u8g2.print(horasstr);
  }
  if(_minuto < 10) {
    u8g2.setCursor(75,30);
    u8g2.print("0");
    u8g2.setCursor(92,30);
    u8g2.print(minutesstr);
  }
  else {
    u8g2.setCursor(75,30);
    u8g2.print(minutesstr);
  }
  
//  u8g2.drawStr(15,30,"12");
//  u8g2.drawLine(75, 31, 110, 31);
  u8g2.drawStr(55,30,":");
//  u8g2.drawStr(75,30,"30");
  u8g2.sendBuffer();   
}

void ConfigAlarme(void) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(49,8,"HORA");  // write something to the internal memory
  u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
  u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
//  u8g2.drawStr(49,8,"OK");  // write something to the internal memory
  u8g2.setFont(u8g2_font_inb21_mn);
  u8g2.drawStr(20,30,"12");
  u8g2.drawStr(60,30,":");
  u8g2.drawStr(80,30,"30");
  u8g2.sendBuffer();          // transfer internal memory to the display
}

void ConfigData(void) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(49,8,"DATA");  // write something to the internal memory
  u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
  u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
//  u8g2.drawStr(49,8,"OK");  // write something to the internal memory
  u8g2.setFont(u8g2_font_inb21_mn);
  u8g2.drawStr(0,30,"21");
  u8g2.drawStr(30,30,"/");
  u8g2.drawStr(45,30,"02");
  u8g2.drawStr(77,30,"/");
  u8g2.drawStr(92,30,"22");
  u8g2.sendBuffer();          // transfer internal memory to the display
}

void Data_config(int _selection, int _dayweek, int _day, int _month, int _year) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(49,8,"DATA");  // write something to the internal memory

  switch(_selection) {
    case 1:
      u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.setFont(u8g2_font_micro_mr);
      switch(_dayweek) {
        case 1:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  1, "DOM");
        break;
        case 2:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  1, "SEG");
        break;
        case 3:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  1, "TER");
        break;
        case 4:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  1, "QUA");
        break;
        case 5:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  1, "QUI");
        break;
        case 6:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  1, "SEX");
        break;
        case 7:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  1, "SAB");
        break;
      }
    break;
    case 2:
      u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.setFont(u8g2_font_micro_mr);
      switch(_dayweek) {
        case 1:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "DOM");
        break;
        case 2:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SEG");
        break;
        case 3:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "TER");
        break;
        case 4:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "QUA");
        break;
        case 5:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "QUI");
        break;
        case 6:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SEX");
        break;
        case 7:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SAB");
        break;
      }
      u8g2.drawLine(0, 31, 29, 31);
    break;
    case 3:
      u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.setFont(u8g2_font_micro_mr);
      switch(_dayweek) {
        case 1:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "DOM");
        break;
        case 2:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SEG");
        break;
        case 3:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "TER");
        break;
        case 4:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "QUA");
        break;
        case 5:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "QUI");
        break;
        case 6:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SEX");
        break;
        case 7:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SAB");
        break;
      }
      u8g2.drawLine(45, 31, 76, 31);
    break;
    case 4:
      u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.setFont(u8g2_font_micro_mr);
      switch(_dayweek) {
        case 1:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "DOM");
        break;
        case 2:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SEG");
        break;
        case 3:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "TER");
        break;
        case 4:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "QUA");
        break;
        case 5:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "QUI");
        break;
        case 6:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SEX");
        break;
        case 7:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SAB");
        break;
      }
      u8g2.drawLine(92, 31, 124, 31);
    break;
    case 5:
      u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_INV|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.setFont(u8g2_font_micro_mr);
      switch(_dayweek) {
        case 1:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "DOM");
        break;
        case 2:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SEG");
        break;
        case 3:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "TER");
        break;
        case 4:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "QUA");
        break;
        case 5:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "QUI");
        break;
        case 6:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SEX");
        break;
        case 7:
          u8g2.drawButtonUTF8(10, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  1, "SAB");
        break;
      }
    break;
  }
//  u8g2.drawButtonUTF8(10, 6, U8G2_BTN_INV|U8G2_BTN_HCENTER, 0,  2,  0, "DOM");
//  u8g2.drawStr(49,8,"OK");  // write something to the internal memory
  u8g2.setFont(u8g2_font_inb21_mn);
//  u8g2.drawLine(92, 31, 124, 31);

  String daystr = String(_day);
  String monthstr = String(_month);
  String yearstr = String(_year);
  if(_day < 10) {
    u8g2.setCursor(0,30);
    u8g2.print("0");
    u8g2.setCursor(15,30);
    u8g2.print(daystr);
  }
  else {
    u8g2.setCursor(0,30);
    u8g2.print(daystr);
  }
  if(_month < 10) {
    u8g2.setCursor(45,30);
    u8g2.print("0");
    u8g2.setCursor(61,30);
    u8g2.print(monthstr);
  }
  else {
    u8g2.setCursor(45,30);
    u8g2.print(monthstr);
  }
  if(_year < 10) {
    u8g2.setCursor(92,30);
    u8g2.print("0");
    u8g2.setCursor(108,30);
    u8g2.print(yearstr);
  }
  else {
    u8g2.setCursor(92,30);
    u8g2.print(yearstr);
  }
    
//  u8g2.drawStr(0,30,"21");
  u8g2.drawStr(30,30,"/");
//  u8g2.drawStr(45,30,"02");
  u8g2.drawStr(77,30,"/");
//  u8g2.drawStr(92,30,"22");
  u8g2.sendBuffer();          // transfer internal memory to the display 
}



void SelectAlarme(int _selection) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(40,8,"ALARME");  // write something to the internal memory
  u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
  
  switch(_selection) {
    case 1:
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.drawLine(0, 31, 29, 31);
    break;
    case 2:
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.drawLine(45, 31, 76, 31);
    break;
    case 3:
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.drawLine(92, 31, 124, 31);
    break;
    case 4:
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_INV|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
    break;
  }
  
  u8g2.setFont(u8g2_font_inb21_mr);
  u8g2.drawStr(0,30,"A1");
  u8g2.drawStr(45,30,"A2");
  u8g2.drawStr(92,30,"A3");
  u8g2.sendBuffer();
}


void AlarmeA_config(int _selection, int _an, bool _state, int _hora, int _minuto) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  switch(_an) {
    case 1:
      u8g2.drawStr(55,8,"A1");  // write something to the internal memory
    break;
    case 2:
      u8g2.drawStr(55,8,"A2");  // write something to the internal memory
    break;
    case 3:
      u8g2.drawStr(55,8,"A3");  // write something to the internal memory
    break;
  }

  switch(_selection) {
    case 1:
      u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.setFont(u8g2_font_tinytim_tr);
      if(_state == true)
        u8g2.drawButtonUTF8(15, 6, U8G2_BTN_INV|U8G2_BTN_HCENTER, 0,  2,  0, "ON");
      else
        u8g2.drawButtonUTF8(15, 6, U8G2_BTN_INV|U8G2_BTN_HCENTER, 0,  2,  0, "OFF");
    break;
    case 2:
      u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.setFont(u8g2_font_tinytim_tr);
      if(_state == true)
        u8g2.drawButtonUTF8(15, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  0, "ON");
      else
        u8g2.drawButtonUTF8(15, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  0, "OFF");
      u8g2.drawLine(15, 31, 50, 31);
    break;
    case 3:
      u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW0|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.setFont(u8g2_font_tinytim_tr);
      if(_state == true)
        u8g2.drawButtonUTF8(15, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  0, "ON");
      else
        u8g2.drawButtonUTF8(15, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  0, "OFF");
      u8g2.drawLine(75, 31, 110, 31);
    break;
    case 4:
      u8g2.setFont(u8g2_font_5x7_t_cyrillic);  // choose a suitable font
      u8g2.drawButtonUTF8(115, 6, U8G2_BTN_INV|U8G2_BTN_HCENTER, 0,  2,  0, "OK");
      u8g2.setFont(u8g2_font_tinytim_tr);
      if(_state == true)
        u8g2.drawButtonUTF8(15, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  0, "ON");
      else
        u8g2.drawButtonUTF8(15, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  0, "OFF");
    break;
  }
//  u8g2.drawStr(49,8,"OK");  // write something to the internal memory
  
  u8g2.setFont(u8g2_font_inb21_mn);

  String horasstr = String(_hora);
  String minutesstr = String(_minuto);
  if(_hora < 10) {
    u8g2.setCursor(15,30);
    u8g2.print("0");
    u8g2.setCursor(32,30);
    u8g2.print(horasstr);
  }
  else {
    u8g2.setCursor(15,30);
    u8g2.print(horasstr);
  }
  if(_minuto < 10) {
    u8g2.setCursor(75,30);
    u8g2.print("0");
    u8g2.setCursor(92,30);
    u8g2.print(minutesstr);
  }
  else {
    u8g2.setCursor(75,30);
    u8g2.print(minutesstr);
  }
//  u8g2.drawStr(15,30,"12");
//  u8g2.drawLine(75, 31, 110, 31);
  u8g2.drawStr(55,30,":");
//  u8g2.drawStr(75,30,"30");
  u8g2.sendBuffer();
}
