//#include "Telas.h"
#include <U8g2lib.h>

#define menuMin 1 // VALOR MÍNIMO DE NAVEGAÇÃO DO MENU
#define menuMax 3 // VALOR MÁXIMO DE NAVEGAÇÃO DO MENU
#define pinBuzzer1  PA11
#define pinBuzzer2  PA12

enum PinAssignments {
  encoderPinA = PA1,   // right
  encoderPinB = PA0,   // left
  selectButton = PB9
};

volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 0;   // change management

int menu = 0;
int menuHora = 0;
int menuAlarme = 0;
int menuData = 0;
int menuA1 = 0;
int menuA2 = 0;
int menuA3 = 0;
int menuAlarmeSelection = 0;

int hora_config = 0;
int minuto_config = 0;
int day_config = 0;
int month_config = 0;
int year_config = 0;
int dayweek_config = 1;
bool onoff_config = 0;
int horaAlarme_config = 0;
int minutoAlarme_config = 0;
bool onoff2_config = 0;
int horaAlarme2_config = 0;
int minutoAlarme2_config = 0;
bool onoff3_config = 0;
int horaAlarme3_config = 0;
int minutoAlarme3_config = 0;

unsigned long tempoSegundo;
unsigned long tempoDisplay;
bool girouEncoder = true;
bool botaoApertado = false;
static boolean rotating = false;    // debounce management
bool SwitchPress = false;

int retornar = 0;
bool retornarHora = false;
bool retornarMinuto = false;
bool retornarDayweek = false;
bool retornarDia = false;
bool retornarMes = false;
bool retornarAno = false;
bool retornarA1 = false;
bool retornarA2 = false;
bool retornarA3 = false;
bool retornarOnoff = false;
bool retornarHoraAlarme = false;
bool retornarMinutoAlarme = false;

bool flag_standby = false;
int flag_menuhora = 0;
int flag_init = 0;

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;

extern uint16 AddressWriteA1;
extern uint16 AddressWriteA2;
extern uint16 AddressWriteA3;

extern uint16 Status;
extern uint16 Data;

//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, PB6, PB7, U8X8_PIN_NONE); 
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, PB6, PB7, U8X8_PIN_NONE);

extern void gravaAlarme(uint8_t _alarme_num, uint16_t _hora, uint16_t _minutos);
extern uint16_t leAlarme(uint8_t _alarme_num);

//TELAS
extern void Menu_init(void);
extern void Menu_standby(int _hora, int _minuto);
extern void Menu_standby2(int _hora, int _minuto);
extern void Menu_standby_alarm(int _hora, int _minuto);
extern void Menu_standby_alarm2(int _hora, int _minuto);
extern void Menu(int _selection, int _weekday, int _dia, int _mes, int _hora, int _minuto);
extern void ConfigHora(void);
extern void ConfigHora1(int _hora, int _minuto);
extern void ConfigHora2(int _hora, int _minuto);
extern void ConfigHora3(int _hora, int _minuto);
extern void ConfigAlarme(void);
extern void ConfigData(void);
extern void Data_config(int _selection, int _dayweek, int _day, int _month, int _year);
extern void SelectAlarme(int _selection);
extern void AlarmeA_config(int _selection, int _an, bool _state, int _hora, int _minuto);
////

void giraEncoder();
void tocabuzzer(int _pinbuzzer, int _pinbuzzer2);
void wait4Standby();
void atualizaDisplay();

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(selectButton, INPUT);
  pinMode(PC13, OUTPUT);
  pinMode(PA5, OUTPUT);
  pinMode(pinBuzzer1, OUTPUT);
  pinMode(pinBuzzer2, OUTPUT);

  // turn on pullup resistors
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  digitalWrite(selectButton, HIGH);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE);

  attachInterrupt(digitalPinToInterrupt(selectButton), doSelect, FALLING);

  flag_init = 0;

  Serial.begin(115200);  // output
  u8g2.begin();

  delay(1000); //delay pois a interrupção do bottão aciona na inicialização
}

// main loop, work is done by interrupt service routines, this one only prints stuff
void loop() {
  //// ESPERA CLICK DO BOTAO PARA INICIALIZAR
  while(flag_init == 0) {
    Menu_init();
    digitalWrite(PA5, LOW);
    botaoApertado = false;
  }
  ////
  digitalWrite(PA5, HIGH);

  rotating = true;  // reset the debouncer
  menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax); //3 menus

  giraEncoder();
  
  atualizaDisplay();
  SwitchPress = false;
  
  switch (menu) {
    case 1: { //// MENU PRINCIPAL: SELEÇÃO HORA
      tempoDisplay = millis();
      while(girouEncoder == false) {
        Serial.println(millis() - tempoDisplay);
        Serial.println("Menu Hora");
        Menu(menu, dayweek_config, day_config, month_config, hora_config, minuto_config);
        //--------------------
        if(SwitchPress) { ////TELA HORA CONFIGURACAO
          SwitchPress = false;
          retornar = 0;
          while(retornar == 0) {
            menuHora = constrain( (menuHora + (encoderPos - lastReportedPos) ), menuMin, menuMax); //3 menus
            giraEncoder();
            switch(menuHora) {
              case 1://hora
                while(girouEncoder == false) {
                  Serial.println("Config Hora");
                  ConfigHora1(hora_config, minuto_config);
                  if(SwitchPress) { ////AJUSTE DE HORA
                    SwitchPress = false;
                    Serial.println("CONFIG HORA SELECIONADO");
                    retornarHora = false;
                    while(retornarHora == false) {
                      Serial.println("CONFIGURE A HORA");
                      hora_config = constrain( (hora_config + (encoderPos - lastReportedPos) ), 0, 23); //24 opções
                      giraEncoder();
                      ConfigHora1(hora_config, minuto_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        retornarHora = true;
                        Serial.println("HORA CONFIGURADA");
                        Serial.println(hora_config);
                      }
                    }
                    SwitchPress = false;
                  } //// FIM AJUSTE DE HORA
                  menuHora = constrain( (menuHora + (encoderPos - lastReportedPos) ), menuMin, menuMax); //3 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 2://minuto
                while(girouEncoder == false) {
                  Serial.println("Config Minuto");
                  ConfigHora2(hora_config, minuto_config);
                  if(SwitchPress) { ////AJUSTE DE MINUTO
                    SwitchPress = false;
                    Serial.println("CONFIG MINUTO SELECIONADO");
                    retornarMinuto = false;
                    while(retornarMinuto == false) {
                      Serial.println("CONFIGURE O MINUTO");
                      minuto_config = constrain( (minuto_config + (encoderPos - lastReportedPos) ), 0, 59); //60 opções
                      giraEncoder();
                      ConfigHora2(hora_config, minuto_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        retornarMinuto = true;
                        Serial.println("MINUTO CONFIGURADO");
                        Serial.println(minuto_config);
                      }
                    }
                    SwitchPress = false;
                  } //// FIM AJUSTE DE MINUTO
                  menuHora = constrain( (menuHora + (encoderPos - lastReportedPos) ), menuMin, menuMax); //3 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 3://ok
                while(girouEncoder == false && retornar == 0) {
                  Serial.println("Bottao OK");
                  ConfigHora3(hora_config, minuto_config);
                  if(SwitchPress) {
                    Serial.println("RETORNAR");
                    SwitchPress = false;
                    retornar = 1;
                    /////// TODO: COLOCAR AQUI SET RTC
                    Menu(menu, dayweek_config, day_config, month_config, hora_config, minuto_config);
                  }
                  menuHora = constrain( (menuHora + (encoderPos - lastReportedPos) ), menuMin, menuMax); //3 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
            }
          }
          Serial.println("====== HORA SELECIONADA ======");
          Serial.print("menu: ");
          Serial.println(menu);
          SwitchPress = false;
        } ////FIM TELA HORA CONFIGURACAO
        //--------------------
        menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax);
        giraEncoder();
        wait4Standby(); //espera para standby
      }
      girouEncoder = false;
      break;
    } //// FIM MENU PRINCIPAL: SELEÇÃO HORA
    case 2: { //// MENU PRINCIPAL: SELEÇÃO ALARME
      tempoDisplay = millis();
      while(girouEncoder == false) {
        Serial.println(millis() - tempoDisplay);
        Serial.println("Menu Hora");
        Menu(menu, dayweek_config, day_config, month_config, hora_config, minuto_config);
        //--------------------
        if(SwitchPress) { ////TELA ALARMES CONFIGURACAO
          SwitchPress = false;
          retornar = 0;
          while(retornar == 0) {
            menuAlarmeSelection = constrain( (menuAlarmeSelection + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
            giraEncoder();
            switch(menuAlarmeSelection) {
              case 1://A1
                while(girouEncoder == false) {
                  Serial.println("A1 Select");
                  SelectAlarme(menuAlarmeSelection);
                  if(SwitchPress) { ////TELA ALARME 1:
                    SwitchPress = false;
                    Serial.println("ALARME 1");
                    retornarA1 = false;
                    while(retornarA1 == false) {
                      menuA1 = constrain( (menuA1 + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                      giraEncoder();
                      Serial.println("A1: CONFIGURE O ON OFF");
                      switch(menuA1) {
                        case 1://ON OFF
                          while(girouEncoder == false) {
                            Serial.println("ON OFF");
                            AlarmeA_config(menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              Serial.println("ON OFF SELECIONADO");
                              retornarOnoff = false;
                              while(retornarOnoff == false) {
                                Serial.println("CONFIGURE O ESTADO");
                                onoff_config = constrain( (onoff_config + (encoderPos - lastReportedPos) ), 0, 1); //2 opções
                                giraEncoder();
                                AlarmeA_config(menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  retornarOnoff = true;
                                  Serial.println("ONOFF CONFIGURADO");
                                  Serial.println(onoff_config);
                                }
                              }
                              SwitchPress = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA1 = constrain( (menuA1 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM ON OFF
                        case 2://A1:HORA
                          while(girouEncoder == false) {
                            Serial.println("A1: HORA");
                            AlarmeA_config(menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              Serial.println("A1:HORA SELECIONADO");
                              retornarHoraAlarme = false;
                              while(retornarHoraAlarme == false) {
                                Serial.println("CONFIGURE A HORA");
                                horaAlarme_config = constrain( (horaAlarme_config + (encoderPos - lastReportedPos) ), 0, 23); //24 opções
                                giraEncoder();
                                AlarmeA_config(menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  retornarHoraAlarme = true;
                                  Serial.println("HORA CONFIGURADA");
                                  Serial.println(horaAlarme_config);
                                }
                              }
                              SwitchPress = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA1 = constrain( (menuA1 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A1:HORA
                        case 3://A1:MINUTO
                          while(girouEncoder == false) {
                            Serial.println("A1: MINUTO");
                            AlarmeA_config(menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              Serial.println("A1:MINUTO SELECIONADO");
                              retornarMinutoAlarme = false;
                              while(retornarMinutoAlarme == false) {
                                Serial.println("CONFIGURE OS MINUTOS");
                                minutoAlarme_config = constrain( (minutoAlarme_config + (encoderPos - lastReportedPos) ), 0, 59); //60 opções
                                giraEncoder();
                                AlarmeA_config(menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  retornarMinutoAlarme = true;
                                  Serial.println("MINUTO CONFIGURADO");
                                  Serial.println(minutoAlarme_config);
                                }
                              }
                              SwitchPress = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA1 = constrain( (menuA1 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A1:MINUTO
                        case 4://OK
                          while(girouEncoder == false && retornarA1 == false) {
                            Serial.println("Bottao OK");
                            AlarmeA_config(menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) {
                              Serial.println("RETORNAR");
                              SwitchPress = false;
                              retornarA1 = true;
                              SelectAlarme(menuAlarmeSelection);
                            }
                            menuA1 = constrain( (menuA1 + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM OK
                      }
                    }
                    SwitchPress = false;
                  } //// FIM TELA ALARME 1
                  menuAlarmeSelection = constrain( (menuAlarmeSelection + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 2://A2
                while(girouEncoder == false) {
                  Serial.println("A2 Select");
                  SelectAlarme(menuAlarmeSelection);
                  if(SwitchPress) { ////TELA ALARME 1:
                    SwitchPress = false;
                    Serial.println("ALARME 2");
                    retornarA2 = false;
                    while(retornarA2 == false) {
                      menuA2 = constrain( (menuA2 + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                      giraEncoder();
                      Serial.println("A2: CONFIGURE O ON OFF");
                      switch(menuA2) {
                        case 1://ON OFF
                          while(girouEncoder == false) {
                            Serial.println("ON OFF");
                            AlarmeA_config(menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              Serial.println("ON OFF SELECIONADO");
                              retornarOnoff = false;
                              while(retornarOnoff == false) {
                                Serial.println("CONFIGURE O ESTADO");
                                onoff2_config = constrain( (onoff2_config + (encoderPos - lastReportedPos) ), 0, 1); //2 opções
                                giraEncoder();
                                AlarmeA_config(menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  retornarOnoff = true;
                                  Serial.println("ONOFF CONFIGURADO");
                                  Serial.println(onoff2_config);
                                }
                              }
                              SwitchPress = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA2 = constrain( (menuA2 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM ON OFF
                        case 2://A2:HORA
                          while(girouEncoder == false) {
                            Serial.println("A2: HORA");
                            AlarmeA_config(menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              Serial.println("A2:HORA SELECIONADO");
                              retornarHoraAlarme = false;
                              while(retornarHoraAlarme == false) {
                                Serial.println("CONFIGURE A HORA");
                                horaAlarme2_config = constrain( (horaAlarme2_config + (encoderPos - lastReportedPos) ), 0, 23); //24 opções
                                giraEncoder();
                                AlarmeA_config(menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  retornarHoraAlarme = true;
                                  Serial.println("HORA CONFIGURADA");
                                  Serial.println(horaAlarme2_config);
                                }
                              }
                              SwitchPress = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA2 = constrain( (menuA2 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A2:HORA
                        case 3://A2:MINUTO
                          while(girouEncoder == false) {
                            Serial.println("A2: MINUTO");
                            AlarmeA_config(menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              Serial.println("A2:MINUTO SELECIONADO");
                              retornarMinutoAlarme = false;
                              while(retornarMinutoAlarme == false) {
                                Serial.println("CONFIGURE OS MINUTOS");
                                minutoAlarme2_config = constrain( (minutoAlarme2_config + (encoderPos - lastReportedPos) ), 0, 59); //60 opções
                                giraEncoder();
                                AlarmeA_config(menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  retornarMinutoAlarme = true;
                                  Serial.println("MINUTO CONFIGURADO");
                                  Serial.println(minutoAlarme2_config);
                                }
                              }
                              SwitchPress = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA2 = constrain( (menuA2 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A2:MINUTO
                        case 4://OK
                          while(girouEncoder == false && retornarA2 == false) {
                            Serial.println("Bottao OK");
                            AlarmeA_config(menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) {
                              Serial.println("RETORNAR");
                              SwitchPress = false;
                              retornarA2 = true;
                              SelectAlarme(menuAlarmeSelection);
                            }
                            menuA2 = constrain( (menuA2 + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM OK
                      }
                    }
                    SwitchPress = false;
                  } //// FIM TELA ALARME 2
                  menuAlarmeSelection = constrain( (menuAlarmeSelection + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 3://A3
                while(girouEncoder == false) {
                  Serial.println("A3 Select");
                  SelectAlarme(menuAlarmeSelection);
                  if(SwitchPress) { ////TELA ALARME 3:
                    SwitchPress = false;
                    Serial.println("ALARME 3");
                    retornarA3 = false;
                    while(retornarA3 == false) {
                      menuA3 = constrain( (menuA3 + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                      giraEncoder();
                      Serial.println("A3: CONFIGURE O ON OFF");
                      switch(menuA3) {
                        case 1://ON OFF
                          while(girouEncoder == false) {
                            Serial.println("ON OFF");
                            AlarmeA_config(menuA3, menuAlarmeSelection, onoff3_config, horaAlarme3_config, minutoAlarme3_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              Serial.println("ON OFF SELECIONADO");
                              retornarOnoff = false;
                              while(retornarOnoff == false) {
                                Serial.println("CONFIGURE O ESTADO");
                                onoff3_config = constrain( (onoff3_config + (encoderPos - lastReportedPos) ), 0, 1); //2 opções
                                giraEncoder();
                                AlarmeA_config(menuA3, menuAlarmeSelection, onoff3_config, horaAlarme3_config, minutoAlarme3_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  retornarOnoff = true;
                                  Serial.println("ONOFF CONFIGURADO");
                                  Serial.println(onoff3_config);
                                }
                              }
                              SwitchPress = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA3 = constrain( (menuA3 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM ON OFF
                        case 2://A3:HORA
                          while(girouEncoder == false) {
                            Serial.println("A3: HORA");
                            AlarmeA_config(menuA3, menuAlarmeSelection, onoff3_config, horaAlarme3_config, minutoAlarme3_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              Serial.println("A3:HORA SELECIONADO");
                              retornarHoraAlarme = false;
                              while(retornarHoraAlarme == false) {
                                Serial.println("CONFIGURE A HORA");
                                horaAlarme3_config = constrain( (horaAlarme3_config + (encoderPos - lastReportedPos) ), 0, 23); //24 opções
                                giraEncoder();
                                AlarmeA_config(menuA3, menuAlarmeSelection, onoff3_config, horaAlarme3_config, minutoAlarme3_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  retornarHoraAlarme = true;
                                  Serial.println("HORA CONFIGURADA");
                                  Serial.println(horaAlarme3_config);
                                }
                              }
                              SwitchPress = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA3 = constrain( (menuA3 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A3:HORA
                        case 3://A3:MINUTO
                          while(girouEncoder == false) {
                            Serial.println("A3: MINUTO");
                            AlarmeA_config(menuA3, menuAlarmeSelection, onoff3_config, horaAlarme3_config, minutoAlarme3_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              Serial.println("A3:MINUTO SELECIONADO");
                              retornarMinutoAlarme = false;
                              while(retornarMinutoAlarme == false) {
                                Serial.println("CONFIGURE OS MINUTOS");
                                minutoAlarme3_config = constrain( (minutoAlarme3_config + (encoderPos - lastReportedPos) ), 0, 59); //60 opções
                                giraEncoder();
                                AlarmeA_config(menuA3, menuAlarmeSelection, onoff3_config, horaAlarme3_config, minutoAlarme3_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  retornarMinutoAlarme = true;
                                  Serial.println("MINUTO CONFIGURADO");
                                  Serial.println(minutoAlarme3_config);
                                }
                              }
                              SwitchPress = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA3 = constrain( (menuA3 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A3:MINUTO
                        case 4://OK
                          while(girouEncoder == false && retornarA3 == false) {
                            Serial.println("Bottao OK");
                            AlarmeA_config(menuA3, menuAlarmeSelection, onoff3_config, horaAlarme3_config, minutoAlarme3_config);
                            if(SwitchPress) {
                              Serial.println("RETORNAR");
                              SwitchPress = false;
                              retornarA3 = true;
                              SelectAlarme(menuAlarmeSelection);
                            }
                            menuA3 = constrain( (menuA3 + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM OK
                      }
                    }
                    SwitchPress = false;
                  } //// FIM TELA ALARME 3
                  menuAlarmeSelection = constrain( (menuAlarmeSelection + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 4:
                while(girouEncoder == false && retornar == 0) {
                  Serial.println("Bottao OK");
                  SelectAlarme(menuAlarmeSelection);
                  if(SwitchPress) {
                    Serial.println("RETORNAR");
                    SwitchPress = false;
                    retornar = 1;
                    Menu(menu, dayweek_config, day_config, month_config, hora_config, minuto_config);
                  }
                  menuAlarmeSelection = constrain( (menuAlarmeSelection + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
            }
          }
        } ////FIM TELA ALARME CONFIGURACAO
        //--------------------
        menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax);
        giraEncoder();
        wait4Standby(); //espera para standby
      }
      girouEncoder = false;
        break;
      } //// FIM MENU PRINCIPAL: SELEÇÃO ALARME
      case 3: { //// MENU PRINCIPAL: SELEÇÃO DATA
      tempoDisplay = millis();
      while(girouEncoder == false) {
        Menu(menu, dayweek_config, day_config, month_config, hora_config, minuto_config);
//        Data_config(menuData, dayweek_config, day_config, month_config, year_config);
        Serial.println(millis() - tempoDisplay);
        Serial.println("Menu Data");
        if(SwitchPress) { ////TELA DATA CONFIGURACAO
          SwitchPress = false;
          retornar = 0;
          while(retornar == 0) {
            menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
            giraEncoder();
            switch(menuData) {
              case 1://weekday
                while(girouEncoder == false) {
                  Serial.println("Config Weekday");
                  Data_config(menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE DAYWEEK
                    SwitchPress = false;
                    Serial.println("CONFIG DAYWEEK SELECIONADO");
                    retornarDayweek = false;
                    while(retornarDayweek == false) {
                      Serial.println("CONFIGURE O DIA DA SEMANA");
                      dayweek_config = constrain( (dayweek_config + (encoderPos - lastReportedPos) ), 1, 7); //7 opções
                      giraEncoder();
                      Data_config(menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        retornarDayweek = true;
                        Serial.println("DAYWEEK CONFIGURADO");
                        Serial.println(dayweek_config);
                      }
                    }
                    SwitchPress = false;
                  } //// FIM AJUSTE DE DAYWEEK
                  menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 2://dia
                while(girouEncoder == false) {
                  Serial.println("Config Dia");
                  Data_config(menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE DIA
                    SwitchPress = false;
                    Serial.println("CONFIG DIA SELECIONADO");
                    retornarDia = false;
                    while(retornarDia == false) {
                      Serial.println("CONFIGURE O DIA");
                      day_config = constrain( (day_config + (encoderPos - lastReportedPos) ), 1, 31); //7 opções
                      giraEncoder();
                      Data_config(menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        retornarDia = true;
                        Serial.println("DIA CONFIGURADO");
                        Serial.println(day_config);
                      }
                    }
                    SwitchPress = false;
                  } //// FIM AJUSTE DE DIA
                  menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 3://mes
                while(girouEncoder == false) {
                  Serial.println("Config Dia");
                  Data_config(menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE MES
                    SwitchPress = false;
                    Serial.println("CONFIG MES SELECIONADO");
                    retornarMes = false;
                    while(retornarMes == false) {
                      Serial.println("CONFIGURE O MES");
                      month_config = constrain( (month_config + (encoderPos - lastReportedPos) ), 1, 12); //12 opções
                      giraEncoder();
                      Data_config(menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        retornarMes = true;
                        Serial.println("MES CONFIGURADO");
                        Serial.println(month_config);
                        CalendarControl(day_config, month_config, year_config);
                      }
                    }
                    SwitchPress = false;
                  } //// FIM AJUSTE DE MES
                  menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 4://ano
                while(girouEncoder == false) {
                  Serial.println("Config Ano");
                  Data_config(menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE ANO
                    SwitchPress = false;
                    Serial.println("CONFIG ANO SELECIONADO");
                    retornarAno = false;
                    while(retornarAno == false) {
                      Serial.println("CONFIGURE O ANO");
                      year_config = constrain( (year_config + (encoderPos - lastReportedPos) ), 1, 99); //99 opções
                      giraEncoder();
                      Data_config(menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        retornarAno = true;
                        Serial.println("ANO CONFIGURADO");
                        Serial.println(year_config);
                      }
                    }
                    SwitchPress = false;
                  } //// FIM AJUSTE DE ANO
                  menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 5://ok
                while(girouEncoder == false && retornar == 0) {
                  Serial.println("Bottao OK");
                  Data_config(menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) {
                    Serial.println("RETORNAR");
                    SwitchPress = false;
                    retornar = 1;
                    CalendarControl(day_config, month_config, year_config);
                    /////// TODO: COLOCAR AQUI SET RTC
                    Menu(menu, dayweek_config, day_config, month_config, hora_config, minuto_config);
                  }
                  menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
            }
          }
          Serial.println("====== DATA SELECIONADA ======");
          Serial.print("menu: ");
          Serial.println(menu);
          SwitchPress = false;
        } ////FIM TELA DATA CONFIGURACAO
        menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax);
        giraEncoder();
        wait4Standby(); //espera para standby
      }
      girouEncoder = false;
        
        break;
    } //// FIM MENU PRINCIPAL: SELEÇÃO DATA
//    default: {
//        if (girouEncoder) {
//          Menu();
//          Serial.println("Tela Menu");
//        }
//        
//        girouEncoder = false;
//
//        break;
//      }
  }

  lastReportedPos = encoderPos; //SALVA A POSIÇÃO ATUAL DO ENCODER PARA COMPARAR
}

void giraEncoder() {
  if (lastReportedPos != encoderPos) {
//    encoderPos = (encoderPos > NUM_MENU_ITEMS -1 )?0:encoderPos;
    Serial.print("encoderPos:");
    Serial.println(encoderPos, DEC);
    girouEncoder = true;
    lastReportedPos = encoderPos;
//    tocabuzzer(pinBuzzer1, pinBuzzer2);
    tempoDisplay = millis();
  }
}

void tocabuzzer(int _pinbuzzer, int _pinbuzzer2) {
  digitalWrite(_pinbuzzer, HIGH);
  digitalWrite(_pinbuzzer2, HIGH);
  delay(50);
  digitalWrite(_pinbuzzer, LOW);
  digitalWrite(_pinbuzzer2, LOW);
}

void wait4Standby() {
  if (millis() - tempoDisplay > 5000) {
    tempoSegundo = millis();
    while(girouEncoder == false && SwitchPress == false) {
      Serial.println("STANDBY");
      if(onoff_config == 1 || onoff2_config == 1 || onoff3_config == 1) {
        Menu_standby_alarm(hora_config, minuto_config);
      }
      else { 
        Menu_standby(hora_config, minuto_config);
      }
      if (millis() - tempoSegundo > 1000) {
        if(onoff_config == 1 || onoff2_config == 1 || onoff3_config == 1) {
          Menu_standby_alarm2(hora_config, minuto_config);
        }
        else {
          Menu_standby2(hora_config, minuto_config);
        }
        tempoSegundo = millis();
      }
    }
  }
}

void atualizaDisplay() {
  if ((botaoApertado == true) || (lastReportedPos != encoderPos)) {
    botaoApertado = false;
//    flag_standby == false;
    Serial.println("RESET STANDBY");
    tempoDisplay = millis();
  }

  if (millis() - tempoDisplay > 5000) {
    Serial.println("STANDBY");
    Menu_standby(hora_config, minuto_config);
//    flag_standby = true;
  }

}

void CalendarControl(int _day, int _month, int _year) { // Não tem tratamento para ano bissexto (Fevereiro com 29 dias)
  if(_day > 28 && _month == 2) { //Fevereiro
    _day = 1;
    _month = 1;
    _year = 1;
  }
  if(_day > 30 && (_month == 4 || _month == 6 || _month == 9 || _month == 11)) { //Meses com apenas 30 dias
    _day = 1;
    _month = 1;
    _year = 1;
  }
}

// Interrupt on A changing state
void doEncoderA() {
    // debounce
  if ( rotating ) delay (1);  // wait a little until the bouncing is done

    // Test transition, did things really change?
  if ( digitalRead(encoderPinA) != A_set ) { // debounce once more
    A_set = !A_set;

    // adjust counter + if A leads B
    if ( A_set && !B_set ) {
      encoderPos += 1;
      girouEncoder = true;
    }

    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB() {
  if (rotating) delay (1);
  if (digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if (B_set && !A_set) {
      encoderPos -= 1;
      girouEncoder = true;
    }
    rotating = false;
  }
}

void doSelect() {
    botaoApertado = true;
    tempoDisplay = millis();
    flag_init = 1;
    if(flag_init == 1) {
      SwitchPress = true;
      tocabuzzer(pinBuzzer1, pinBuzzer2);
//      tempoDisplay = millis();
    }
    digitalWrite(PC13, HIGH);
//  int tempoDebounce = 100;
//  static unsigned long swDebounce;
//  bool sw;
//  static bool swAnt = 1;
//
//  sw  = digitalRead(selectButton);
//
//  if ( millis() - swDebounce  > tempoDebounce) {
//
//    if (!sw && swAnt) {
//      swDebounce = millis();
//      swAnt = sw;
//      botaoApertado = true;
//
//    } else if (sw && !swAnt) {
//      swDebounce = millis();
//      swAnt = sw;
//    }
//  }
//
//  botaoApertado = false;
}
