void Menu_standy();
void Menu();
void MenuHora();
void MenuAlarme();
void MenuData();
void Hora();
//void Alarme();
void Data();

void Menu_standby(void) {
  u8g2.clearBuffer();
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_timB24_tr);
//  String horasstr = String(horas);
//  String minutesstr = String(minutes);
//  u8g2.setCursor(25,32);
//  u8g2.print(horasstr);
//
//  u8g2.drawStr(57,32," ");
//  u8g2.drawStr(57,32,":");
//
//  u8g2.setCursor(68,32);
//  u8g2.print(minutesstr);
//  
//  String segundosstr = String(segundos);
//
//  u8g2.setFont(u8g2_font_ncenB08_tr);
//  if(segundos < 10) {
//    u8g2.setCursor(110,32);
//    u8g2.print("0");
//    u8g2.setCursor(115,32);
//    u8g2.print(segundosstr);
//  }
//  else {
//    u8g2.setCursor(110,32);
//    u8g2.print(segundosstr);
//  }
//  if(segundos == 59) {
//    segundos = 0;
//    minutes++;
//  }
  
  u8g2.drawStr(28,32,"12:30");
  u8g2.sendBuffer();
}

void Menu(void) {
  u8g2.clearBuffer();          // clear the internal memory
  //MENU
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,8,"Hora");
  u8g2.drawStr(50,8,"Alarme");
  u8g2.drawStr(105,8,"Data");
//  u8g2.drawLine(30, 5, 100, 5);
  u8g2.setFont(u8g2_font_timB24_tr);//u8g2_font_luBIS19_tn
  
//  String horasstr = String(horas);
//  String minutesstr = String(minutes);
//  u8g2.setCursor(25,32);
//  u8g2.print(horasstr);
//
//  u8g2.drawStr(57,32," ");
//  u8g2.drawStr(57,32,":");
//
//  u8g2.setCursor(68,32);
//  u8g2.print(minutesstr);
//  
//  String segundosstr = String(segundos);
//
//  u8g2.setFont(u8g2_font_ncenB08_tr);
//  if(segundos < 10) {
//    u8g2.setCursor(110,32);
//    u8g2.print("0");
//    u8g2.setCursor(115,32);
//    u8g2.print(segundosstr);
//  }
//  else {
//    u8g2.setCursor(110,32);
//    u8g2.print(segundosstr);
//  }
//  if(segundos == 59) {
//    segundos = 0;
//    minutes++;
//  }
  
  u8g2.drawStr(28,32,"12:30");
  u8g2.sendBuffer();
}

void MenuHora(void) {
  u8g2.clearBuffer();
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

  u8g2.setFont(u8g2_font_timB24_tr);//u8g2_font_luBIS19_tn
//  u8g2.drawStr(25,32,"12:30");

//  String horasstr = String(horas);
//  String minutesstr = String(minutes);
//  u8g2.setCursor(25,32);
//  u8g2.print(horasstr);
//
//  u8g2.drawStr(57,32," ");
//  u8g2.drawStr(57,32,":");
//
//  u8g2.setCursor(68,32);
//  u8g2.print(minutesstr);
//  
//  String segundosstr = String(segundos);
//
//  u8g2.setFont(u8g2_font_ncenB08_tr);
//  if(segundos < 10) {
//    u8g2.setCursor(110,32);
//    u8g2.print("0");
//    u8g2.setCursor(115,32);
//    u8g2.print(segundosstr);
//  }
//  else {
//    u8g2.setCursor(110,32);
//    u8g2.print(segundosstr);
//  }
//  if(segundos == 59) {
//    segundos = 0;
//    minutes++;
//  }

  u8g2.drawStr(25,32,"12:30");
  u8g2.sendBuffer();          // transfer internal memory to the display
}

void MenuAlarme(void) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1); /* color 1 for the box */
//  u8g2.drawBox(0, 0, 30, 8);
  u8g2.drawBox(45, 0, 40, 8);

  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.drawStr(0,8,"Hora"); // write something to the internal memory
  
  u8g2.setDrawColor(0);
  u8g2.drawStr(45,8,"Alarme");
  
  u8g2.setDrawColor(1);
  u8g2.drawStr(102,8,"Data");

  u8g2.setFont(u8g2_font_timB24_tr);//u8g2_font_luBIS19_tn

//  String horasstr = String(horas);
//  String minutesstr = String(minutes);
//  u8g2.setCursor(25,32);
//  u8g2.print(horasstr);
//
//  u8g2.drawStr(57,32," ");
//  u8g2.drawStr(57,32,":");
//
//  u8g2.setCursor(68,32);
//  u8g2.print(minutesstr);
//  
//  String segundosstr = String(segundos);
//
//  u8g2.setFont(u8g2_font_ncenB08_tr);
//  if(segundos < 10) {
//    u8g2.setCursor(110,32);
//    u8g2.print("0");
//    u8g2.setCursor(115,32);
//    u8g2.print(segundosstr);
//  }
//  else {
//    u8g2.setCursor(110,32);
//    u8g2.print(segundosstr);
//  }
//  if(segundos == 59) {
//    segundos = 0;
//    minutes++;
//  }
  u8g2.drawStr(25,32,"12:30");
  u8g2.sendBuffer();          // transfer internal memory to the display
}

void MenuData(void) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1); /* color 1 for the box */
  u8g2.drawBox(100, 0, 25, 8);

  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.drawStr(0,8,"Hora"); // write something to the internal memory
  
  u8g2.setDrawColor(1);
  u8g2.drawStr(45,8,"Alarme");
  
  u8g2.setDrawColor(0);
  u8g2.drawStr(102,8,"Data");

  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_timB24_tr);//u8g2_font_luBIS19_tn
//  u8g2.drawStr(25,32,"12:30");

//  String horasstr = String(horas);
//  String minutesstr = String(minutes);
//  u8g2.setCursor(25,32);
//  u8g2.print(horasstr);
//
//  u8g2.drawStr(57,32," ");
//  u8g2.drawStr(57,32,":");
//
//  u8g2.setCursor(68,32);
//  u8g2.print(minutesstr);
//  
//  String segundosstr = String(segundos);
//
//  u8g2.setFont(u8g2_font_ncenB08_tr);
//  if(segundos < 10) {
//    u8g2.setCursor(110,32);
//    u8g2.print("0");
//    u8g2.setCursor(115,32);
//    u8g2.print(segundosstr);
//  }
//  else {
//    u8g2.setCursor(110,32);
//    u8g2.print(segundosstr);
//  }
//  if(segundos == 59) {
//    segundos = 0;
//    minutes++;
//  }

  u8g2.drawStr(25,32,"12:30");
  u8g2.sendBuffer();          // transfer internal memory to the display
}

void Hora(void) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(49,8,"HORA");  // write something to the internal memory
  u8g2.setFont(u8g2_font_u8glib_4_tr);  // choose a suitable font
  u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  0, "OK" );
//  u8g2.drawStr(49,8,"OK");  // write something to the internal memory
  u8g2.setFont(u8g2_font_inb21_mn);
  u8g2.drawStr(20,30,"12");
  u8g2.drawStr(60,30,":");
  u8g2.drawStr(80,30,"30");
  u8g2.sendBuffer();          // transfer internal memory to the display
}

void Data(void) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(49,8,"DATA");  // write something to the internal memory
  u8g2.setFont(u8g2_font_u8glib_4_tr);  // choose a suitable font
  u8g2.drawButtonUTF8(115, 6, U8G2_BTN_BW1|U8G2_BTN_HCENTER, 0,  2,  0, "OK" );
//  u8g2.drawStr(49,8,"OK");  // write something to the internal memory
  u8g2.setFont(u8g2_font_inb21_mn);
  u8g2.drawStr(0,30,"21");
  u8g2.drawStr(30,30,"/");
  u8g2.drawStr(45,30,"02");
  u8g2.drawStr(77,30,"/");
  u8g2.drawStr(92,30,"22");
  u8g2.sendBuffer();          // transfer internal memory to the display
}
