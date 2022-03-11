#include <U8g2lib.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
TwoWire WIRE2 (2,I2C_FAST_MODE);
#define Wire WIRE2
RtcDS3231<TwoWire> Rtc(Wire);

const PROGMEM uint8_t menuMin = 1; // VALOR MÍNIMO DE NAVEGAÇÃO DO MENU
const PROGMEM uint8_t menuMax = 3; // VALOR MÁXIMO DE NAVEGAÇÃO DO MENU

enum PinAssignments {
  encoderPinA =     PA1,   // right
  encoderPinB =     PA0,   // left
  selectButton =    PB9,
  buzzer_Passivo =  PA11,
  buzzer_Ativo =    PA12,
  ldr =             PB1,
  micro_Sw =        PB13,
  led_Yellow =      PA6,
  led_Red =         PA5,
  led_Red2 =        PA4,
  led_Red3 =        PA3,
  led_Red4 =        PA2,
  led_Red5 =        PA7,
  led_Red6 =        PB0,
  oled_Scl =        PB6,
  oled_Sda =        PB7,
  rtc_Scl =         PB10,
  rtc_Sda =         PB11,
  rtc_Sqw =         PB14  
};

volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 0;   // change management

char bufftime[8];
char buffdate[12];

uint8_t settime = 0;
uint8_t setdate = 0;

uint8_t menu = 0;
uint8_t menuHora = 0;
uint8_t menuAlarme = 0;
uint8_t menuData = 0;
uint8_t menuA = 0;
uint8_t menuAlarmeSelection = 0;

uint8_t set_segundo = 0;

uint8_t hora_config = 0;
uint8_t minuto_config = 0;
uint8_t day_config = 0;
uint8_t month_config = 0;
uint16_t year_config = 0;
uint8_t dayweek_config = 1;
bool onoff_config = 0;
uint8_t horaAlarme_config = 0;
uint8_t minutoAlarme_config = 0;
bool onoff2_config = 0;
uint8_t horaAlarme2_config = 0;
uint8_t minutoAlarme2_config = 0;
bool onoff3_config = 0;

unsigned long tempoSegundo;
unsigned long tempoDisplay;
bool girouEncoder = true;
bool botaoApertado = false;
static boolean rotating = false;    // debounce management
bool SwitchPress = false;
bool microswApertado = false;
bool disparouAlarme = false;
uint8_t flag_lowBright = 0;

uint8_t retornar = 0;
bool retornarHora = false;

//bool retornarA = false;
//bool retornarAlarme = false;
//
//bool retornarData = false;

uint8_t flag_init = 0;
uint8_t flag_init2 = 0;

uint16_t leituraLDR = 0;

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;

extern int Status;

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, PB6, PB7, U8X8_PIN_NONE); 
//U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, PB6, PB7, U8X8_PIN_NONE);

void timeStr(uint8_t _hora, uint8_t _minuto, uint8_t _segundo);
void dateStr(uint8_t dia, uint8_t _mes, uint16_t _ano);

extern void gravaOnOff(uint8_t _selection, uint8_t _onoff);
extern int leOnOff(uint8_t _selection);
extern void gravaAlarme(uint8_t _alarme_num, uint16_t _hora, uint16_t _minutos);
extern int leAlarme(uint8_t _alarme_num, uint8_t _hora_min);

//TELAS
extern void Menu_Alarm_Triggered(void);
extern void Menu_init(void);
extern void Menu_standby(uint8_t _selection, uint8_t _brilho, int _hora, int _minuto, uint8_t _dia, uint8_t _mes);
extern void Menu_standby_alarm(uint8_t _selection, uint8_t _brilho, int _hora, int _minuto, uint8_t _dia, uint8_t _mes);
extern void Menu(uint8_t _brilho, int _selection, int _weekday, int _dia, int _mes, int _hora, int _minuto, uint8_t _segundo);
extern void Time_Config(uint8_t _brilho, int _selection, int _hora, int _minuto);
extern void Data_config(uint8_t _brilho, int _selection, int _dayweek, int _day, int _month, int _year);
extern void SelectAlarme(uint8_t _brilho, int _selection);
extern void AlarmeA_config(uint8_t _brilho, int _selection, int _an, bool _state, int _hora, int _minuto);
////
void tocabuzzer();
void alarmTriggeredOrNot();
void giraEncoder();
void wait4Standby();
void atualizaDisplay();
void LedConfirma(void);
void LightLevel(void);

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(selectButton, INPUT);
  pinMode(led_Yellow, OUTPUT);
  pinMode(led_Red, OUTPUT);
  pinMode(led_Red2, OUTPUT);
  pinMode(led_Red3, OUTPUT);
  pinMode(led_Red4, OUTPUT);
  pinMode(led_Red5, OUTPUT);
  pinMode(led_Red6, OUTPUT);
  pinMode(micro_Sw, INPUT);
  pinMode(ldr, INPUT);
  pinMode(buzzer_Passivo, OUTPUT);
  pinMode(buzzer_Ativo, OUTPUT);
  pinMode(rtc_Sqw, INPUT);

  // turn on pullup resistors
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  digitalWrite(selectButton, HIGH);
  digitalWrite(micro_Sw , HIGH);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(selectButton), doSelect, FALLING);
  attachInterrupt(digitalPinToInterrupt(micro_Sw), stopAlarmClock, FALLING);
  attachInterrupt(digitalPinToInterrupt(rtc_Sqw), alarmTriggered, FALLING);

  Serial.begin(115200);
  Wire.begin();
  u8g2.begin();
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Rtc.SetDateTime(compiled);

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
  Rtc.LatchAlarmsTriggeredFlags();
}

void loop() {
  //// ESPERA CLICK DO BOTAO PARA INICIALIZAR
  while(flag_init == 0) {
    Menu_init();
    digitalWrite(PA5, LOW);
  }
  ////
  if(flag_init2 == 0) {
    digitalWrite(led_Red, HIGH);
    digitalWrite(led_Red2, HIGH);
    delay(1000);
    digitalWrite(led_Red, LOW);
    digitalWrite(led_Red2, LOW);
    digitalWrite(led_Red3, HIGH);
    digitalWrite(led_Red4, HIGH);
    delay(1000);
    digitalWrite(led_Red3, LOW);
    digitalWrite(led_Red4, LOW);
    digitalWrite(led_Red5, HIGH);
    digitalWrite(led_Red6, HIGH);
    delay(1000);
    digitalWrite(led_Red5, LOW);
    digitalWrite(led_Red6, LOW);
    flag_init2 = 1;
  }

  rotating = true;  // reset the debouncer
  menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax); //3 menus

  giraEncoder();
  
  atualizaDisplay();
  SwitchPress = false;
  botaoApertado = false;
  switch (menu) {
    case 1: { //// MENU PRINCIPAL: SELEÇÃO HORA
      tempoDisplay = millis();
      while(girouEncoder == false) {
        wait4Standby(); //espera para standby
        LightLevel();
        RtcDateTime now = Rtc.GetDateTime();
        if(settime == 1) {
          RtcDateTime SetTime = RtcDateTime(__DATE__, bufftime);
          Rtc.SetDateTime(SetTime);
          settime = 0;
        }
        Menu(flag_lowBright, menu, now.DayOfWeek()+1, now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
        //--------------------
        if(SwitchPress) { ////TELA HORA CONFIGURACAO
          SwitchPress = false;
          botaoApertado = false;
          retornar = 0;
          while(retornar == 0) {
            menuHora = constrain( (menuHora + (encoderPos - lastReportedPos) ), menuMin, menuMax); //3 menus
            giraEncoder();
            switch(menuHora) {
              case 1://hora
                while(girouEncoder == false) {
                  LightLevel();
                  Time_Config(flag_lowBright, menuHora, hora_config, minuto_config);
                  if(SwitchPress) { ////AJUSTE DE HORA
                    SwitchPress = false;
                    botaoApertado = false;
                    retornarHora = false;
                    while(retornarHora == false) {
                      hora_config = constrain( (hora_config + (encoderPos - lastReportedPos) ), 0, 23); //24 opções
                      giraEncoder();
                      LightLevel();
                      Time_Config(flag_lowBright, menuHora, hora_config, minuto_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarHora = true;
                      }
                    }
                    SwitchPress = false;
                    botaoApertado = false;
                  } //// FIM AJUSTE DE HORA
                  menuHora = constrain( (menuHora + (encoderPos - lastReportedPos) ), menuMin, menuMax); //3 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 2://minuto
                while(girouEncoder == false) {
                  LightLevel();
                  Time_Config(flag_lowBright, menuHora, hora_config, minuto_config);
                  if(SwitchPress) { ////AJUSTE DE MINUTO
                    SwitchPress = false;
                    botaoApertado = false;
                    retornarHora = false;
                    while(retornarHora == false) {
                      minuto_config = constrain( (minuto_config + (encoderPos - lastReportedPos) ), 0, 59); //60 opções
                      giraEncoder();
                      LightLevel();
                      Time_Config(flag_lowBright, menuHora, hora_config, minuto_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarHora = true;
                      }
                    }
                    SwitchPress = false;
                    botaoApertado = false;
                  } //// FIM AJUSTE DE MINUTO
                  menuHora = constrain( (menuHora + (encoderPos - lastReportedPos) ), menuMin, menuMax); //3 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 3://ok
                while(girouEncoder == false && retornar == 0) {
                  LightLevel();
                  Time_Config(flag_lowBright, menuHora, hora_config, minuto_config);
                  if(SwitchPress) {
                    SwitchPress = false;
                    botaoApertado = false;
                    retornar = 1;
                    timeStr(hora_config, minuto_config, set_segundo);
                    settime = 1;
                    LightLevel();
                  }
//                  dateStr(1, 1, 1);
                  menuHora = constrain( (menuHora + (encoderPos - lastReportedPos) ), menuMin, menuMax); //3 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
            }
          }
          SwitchPress = false;
          botaoApertado = false;
        } ////FIM TELA HORA CONFIGURACAO
        //--------------------
        menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax);
        giraEncoder();
        alarmTriggeredOrNot();
        microswApertado = false;
        wait4Standby(); //espera para standby
      }
      girouEncoder = false;
      break;
    } //// FIM MENU PRINCIPAL: SELEÇÃO HORA
    case 2: { //// MENU PRINCIPAL: SELEÇÃO ALARME
      tempoDisplay = millis();
      while(girouEncoder == false) {
        wait4Standby(); //espera para standby
        LightLevel();
        RtcDateTime now = Rtc.GetDateTime();
        Menu(flag_lowBright, menu, now.DayOfWeek()+1, now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
        //--------------------
        if(SwitchPress) { ////TELA ALARMES CONFIGURACAO
          SwitchPress = false;
          botaoApertado = false;
          retornar = 0;
          while(retornar == 0) {
            menuAlarmeSelection = constrain( (menuAlarmeSelection + (encoderPos - lastReportedPos) ), menuMin, 3); //3 menus
            giraEncoder();
            bool retornarA = false;
            switch(menuAlarmeSelection) {
              case 1://A1
                while(girouEncoder == false) {
                  LightLevel();
                  SelectAlarme(flag_lowBright, menuAlarmeSelection);
                  if(SwitchPress) { ////TELA ALARME 1:
                    SwitchPress = false;
                    botaoApertado = false;
                    horaAlarme_config = leAlarme(menuAlarmeSelection, 1);
                    minutoAlarme_config = leAlarme(menuAlarmeSelection, 2);
                    retornarA = false;
                    while(retornarA == false) {
                      menuA = constrain( (menuA + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                      giraEncoder();
                      bool retornarAlarme = false;
                      switch(menuA) {
                        case 1://ON OFF
//                          onoff_config = leOnOff(menuAlarmeSelection);
                          while(girouEncoder == false) {
                            LightLevel();
                            onoff_config = leOnOff(menuAlarmeSelection);
                            AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              botaoApertado = false;
                              retornarAlarme = false;
                              while(retornarAlarme == false) {
                                onoff_config = constrain( (onoff_config + (encoderPos - lastReportedPos) ), 0, 1); //2 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarAlarme = true;
                                  gravaOnOff(menuAlarmeSelection, onoff_config);
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA = constrain( (menuA + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM ON OFF
                        case 2://A1:HORA
                          while(girouEncoder == false) {
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) { ////AJUSTE DE HORA
                              SwitchPress = false;
                              botaoApertado = false;
                              retornarAlarme = false;
                              while(retornarAlarme == false) {
                                horaAlarme_config = constrain( (horaAlarme_config + (encoderPos - lastReportedPos) ), 0, 23); //24 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarAlarme = true;
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE HORA
                            menuA = constrain( (menuA + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A1:HORA
                        case 3://A1:MINUTO
                          while(girouEncoder == false) {
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) { ////AJUSTE DE MINUTO
                              SwitchPress = false;
                              botaoApertado = false;
                              retornarAlarme = false;
                              while(retornarAlarme == false) {
                                minutoAlarme_config = constrain( (minutoAlarme_config + (encoderPos - lastReportedPos) ), 0, 59); //60 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarAlarme = true;
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE MINUTO
                            menuA = constrain( (menuA + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A1:MINUTO
                        case 4://OK
                          while(girouEncoder == false && retornarA == false) {
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) {
                              SwitchPress = false;
                              botaoApertado = false;
                              retornarA = true;
                              gravaOnOff(menuAlarmeSelection, onoff_config);
                              DS3231AlarmOne alarm1(
                                      1,
                                      horaAlarme_config,
                                      minutoAlarme_config, 
                                      0,
                                      DS3231AlarmOneControl_HoursMinutesSecondsMatch);
                              Rtc.SetAlarmOne(alarm1);
                              gravaAlarme(menuAlarmeSelection, horaAlarme_config, minutoAlarme_config);
                              LightLevel();
                              SelectAlarme(flag_lowBright, menuAlarmeSelection);
                            }
                            menuA = constrain( (menuA + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM OK
                      }
                    }
                    SwitchPress = false;
                    botaoApertado = false;
                  } //// FIM TELA ALARME 1
                  menuAlarmeSelection = constrain( (menuAlarmeSelection + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 2://A2
                while(girouEncoder == false) {
                  LightLevel();
                  SelectAlarme(flag_lowBright, menuAlarmeSelection);
                  if(SwitchPress) { ////TELA ALARME 2:
                    SwitchPress = false;
                    botaoApertado = false;
                    horaAlarme2_config = leAlarme(menuAlarmeSelection, 1);
                    minutoAlarme2_config = leAlarme(menuAlarmeSelection, 2);
                    retornarA = false;
                    while(retornarA == false) {
                      menuA = constrain( (menuA + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                      giraEncoder();
                      bool retornarAlarme = false;
                      switch(menuA) {
                        case 1://ON OFF
                          while(girouEncoder == false) {
                            LightLevel();
                            onoff2_config = leOnOff(menuAlarmeSelection);
                            AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              botaoApertado = false;;
                              retornarAlarme = false;
                              while(retornarAlarme == false) {
                                onoff2_config = constrain( (onoff2_config + (encoderPos - lastReportedPos) ), 0, 1); //2 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarAlarme = true;
                                  gravaOnOff(menuAlarmeSelection, onoff2_config);
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA = constrain( (menuA + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM ON OFF
                        case 2://A2:HORA
                          while(girouEncoder == false) {
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) { ////AJUSTE DE HORA
                              SwitchPress = false;
                              botaoApertado = false;
                              retornarAlarme = false;
                              while(retornarAlarme == false) {
                                horaAlarme2_config = constrain( (horaAlarme2_config + (encoderPos - lastReportedPos) ), 0, 23); //24 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarAlarme = true;
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE HORA
                            menuA = constrain( (menuA + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A2:HORA
                        case 3://A2:MINUTO
                          while(girouEncoder == false) {
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) { ////AJUSTE DE MINUTO
                              SwitchPress = false;
                              botaoApertado = false;
                              retornarAlarme = false;
                              while(retornarAlarme == false) {
                                minutoAlarme2_config = constrain( (minutoAlarme2_config + (encoderPos - lastReportedPos) ), 0, 59); //60 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarAlarme = true;
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE MINUTO
                            menuA = constrain( (menuA + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A2:MINUTO
                        case 4://OK
                          while(girouEncoder == false && retornarA == false) {
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) {
                              SwitchPress = false;
                              botaoApertado = false;
                              retornarA = true;
                              gravaOnOff(menuAlarmeSelection, onoff2_config);
                              DS3231AlarmTwo alarm2(
                                      1,
                                      horaAlarme2_config,
                                      minutoAlarme2_config,
                                      DS3231AlarmTwoControl_HoursMinutesMatch);
                              Rtc.SetAlarmTwo(alarm2);
                              LightLevel();
                              SelectAlarme(flag_lowBright, menuAlarmeSelection);
                              gravaAlarme(menuAlarmeSelection, horaAlarme2_config, minutoAlarme2_config);
                            }
                            menuA = constrain( (menuA + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM OK
                      }
                    }
                    SwitchPress = false;
                    botaoApertado = false;
                  } //// FIM TELA ALARME 2
                  menuAlarmeSelection = constrain( (menuAlarmeSelection + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 3:
                while(girouEncoder == false && retornar == 0) {
                  LightLevel();
                  SelectAlarme(flag_lowBright, menuAlarmeSelection);
                  if(SwitchPress) {
                    SwitchPress = false;
                    botaoApertado = false;
                    Rtc.LatchAlarmsTriggeredFlags();
                    retornar = 1;
                    LightLevel();
                    RtcDateTime now = Rtc.GetDateTime();
                    Menu(flag_lowBright, menu, now.DayOfWeek()+1, now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
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
        alarmTriggeredOrNot();
        microswApertado = false;
        wait4Standby(); //espera para standby
      }
      girouEncoder = false;
      break;
      } //// FIM MENU PRINCIPAL: SELEÇÃO ALARME
      case 3: { //// MENU PRINCIPAL: SELEÇÃO DATA
      tempoDisplay = millis();
      while(girouEncoder == false) {
        if(setdate == 1) {
          RtcDateTime SetDate = RtcDateTime(buffdate, bufftime);
          Rtc.SetDateTime(SetDate);
          setdate = 0;
        }
        wait4Standby(); //espera para standby
        LightLevel();
        RtcDateTime now = Rtc.GetDateTime();
        Menu(flag_lowBright, menu, now.DayOfWeek()+1, now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
        if(SwitchPress) { ////TELA DATA CONFIGURACAO
          SwitchPress = false;
          botaoApertado = false;
          retornar = 0;
          while(retornar == 0) {
            menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
            giraEncoder();
            bool retornarData = false;
            switch(menuData) {
              case 1://weekday
                while(girouEncoder == false) {
                  LightLevel();
                  Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE DAYWEEK
                    SwitchPress = false;
                    botaoApertado = false;
                    retornarData = false;
                    while(retornarData == false) {
                      dayweek_config = constrain( (dayweek_config + (encoderPos - lastReportedPos) ), 1, 7); //7 opções
                      giraEncoder();
                      LightLevel();
                      Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarData = true;
                      }
                    }
                    SwitchPress = false;
                    botaoApertado = false;
                  } //// FIM AJUSTE DE DAYWEEK
                  menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 2://dia
                while(girouEncoder == false) {
                  LightLevel();
                  Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE DIA
                    SwitchPress = false;
                    botaoApertado = false;
                    retornarData = false;
                    while(retornarData == false) {
                      day_config = constrain( (day_config + (encoderPos - lastReportedPos) ), 1, 31); //7 opções
                      giraEncoder();
                      LightLevel();
                      Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarData = true;
                      }
                    }
                    SwitchPress = false;
                    botaoApertado = false;
                  } //// FIM AJUSTE DE DIA
                  menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 3://mes
                while(girouEncoder == false) {
                  LightLevel();
                  Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE MES
                    SwitchPress = false;
                    botaoApertado = false;
                    retornarData = false;
                    while(retornarData == false) {
                      month_config = constrain( (month_config + (encoderPos - lastReportedPos) ), 1, 12); //12 opções
                      giraEncoder();
                      LightLevel();
                      Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarData = true;
                      }
                    }
                    SwitchPress = false;
                    botaoApertado = false;
                  } //// FIM AJUSTE DE MES
                  menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 4://ano
                while(girouEncoder == false) {
                  LightLevel();
                  Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE ANO
                    SwitchPress = false;
                    botaoApertado = false;
                    retornarData = false;
                    while(retornarData == false) {
                      year_config = constrain( (year_config + (encoderPos - lastReportedPos) ), 1, 99); //99 opções
                      giraEncoder();
                      LightLevel();
                      Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarData = true;
                      }
                    }
                    SwitchPress = false;
                    botaoApertado = false;
                  } //// FIM AJUSTE DE ANO
                  menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
              case 5://ok
                while(girouEncoder == false && retornar == 0) {
                  LightLevel();
                  Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) {
                    SwitchPress = false;
                    botaoApertado = false;
                    retornar = 1;
                    CalendarControl(day_config, month_config, year_config);
                    dateStr(day_config, month_config, year_config);
//                    RtcDateTime SetDate = RtcDateTime(buffdate, bufftime);
//                    Rtc.SetDateTime(SetDate);
                    ////
                    LightLevel();
                    Menu(flag_lowBright, menu, now.DayOfWeek(), now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
                  }
                  menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
                  giraEncoder();
                }
                girouEncoder = false;
              break;
            }
          }
          SwitchPress = false;
          botaoApertado = false;
        } ////FIM TELA DATA CONFIGURACAO
        menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax);
        giraEncoder();
        alarmTriggeredOrNot();
        microswApertado = false;
        wait4Standby(); //espera para standby
      }
      girouEncoder = false;
        
        break;
    } //// FIM MENU PRINCIPAL: SELEÇÃO DATA
  }

  lastReportedPos = encoderPos; //SALVA A POSIÇÃO ATUAL DO ENCODER PARA COMPARAR
}

void giraEncoder() {
  if (lastReportedPos != encoderPos) {
    girouEncoder = true;
    lastReportedPos = encoderPos;
    tempoDisplay = millis();
    digitalWrite(led_Yellow, HIGH);
  }
}

void tocabuzzer() {
  digitalWrite(buzzer_Ativo, HIGH);
  digitalWrite(led_Red, HIGH);
  digitalWrite(led_Red2, HIGH);
  digitalWrite(led_Red3, HIGH);
  delay(100);
  digitalWrite(buzzer_Ativo, LOW);
  digitalWrite(led_Red, LOW);
  digitalWrite(led_Red2, LOW);
  digitalWrite(led_Red3, LOW);
  delay(100);

  digitalWrite(buzzer_Ativo, HIGH);
  digitalWrite(led_Red, HIGH);
  digitalWrite(led_Red2, HIGH);
  digitalWrite(led_Red3, HIGH);
  delay(100);
  digitalWrite(buzzer_Ativo, LOW);
  digitalWrite(led_Red, LOW);
  digitalWrite(led_Red2, LOW);
  digitalWrite(led_Red3, LOW);
  delay(100);

  digitalWrite(buzzer_Ativo, HIGH);
  digitalWrite(led_Red, HIGH);
  digitalWrite(led_Red2, HIGH);
  digitalWrite(led_Red3, HIGH);
  delay(100);
  digitalWrite(buzzer_Ativo, LOW);
  digitalWrite(led_Red, LOW);
  digitalWrite(led_Red2, LOW);
  digitalWrite(led_Red3, LOW);
  delay(100);

  digitalWrite(buzzer_Ativo, HIGH);
  digitalWrite(led_Red, HIGH);
  digitalWrite(led_Red2, HIGH);
  digitalWrite(led_Red3, HIGH);
  delay(100);
  digitalWrite(buzzer_Ativo, LOW);
  digitalWrite(led_Red, LOW);
  digitalWrite(led_Red2, LOW);
  digitalWrite(led_Red3, LOW);
  delay(1000);
}

void alarmTriggeredOrNot() {
  if(disparouAlarme == true) {
    DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();
    while(microswApertado == false) {
      tocabuzzer();
      Menu_Alarm_Triggered();
    }
  }
}

void wait4Standby() {
  if (millis() - tempoDisplay > 5000) {
    tempoSegundo = millis();
    digitalWrite(led_Yellow, LOW);
    while(girouEncoder == false && SwitchPress == false && microswApertado == false && disparouAlarme == false) {
      RtcDateTime now = Rtc.GetDateTime();
      if(onoff_config == 1 || onoff2_config == 1) {
        LightLevel();
        Menu_standby_alarm(1, flag_lowBright, now.Hour(), now.Minute(), now.Day(), now.Month());
      }
      else { 
        LightLevel();
        Menu_standby(1, flag_lowBright, now.Hour(), now.Minute(), now.Day(), now.Month());
      }
      if (millis() - tempoSegundo > 1000) {
        if(onoff_config == 1 || onoff2_config == 1) {
          LightLevel();
          Menu_standby_alarm(2, flag_lowBright, now.Hour(), now.Minute(), now.Day(), now.Month());
        }
        else {
          LightLevel();
          Menu_standby(2, flag_lowBright, now.Hour(), now.Minute(), now.Day(), now.Month());
        }
        tempoSegundo = millis();
      }
    }
  }
  else
    digitalWrite(led_Yellow, HIGH);
}

void timeStr(uint8_t _hora, uint8_t _minuto, uint8_t _segundo) {
  if(_hora < 10) {
    if(_minuto < 10){
      if(_segundo < 10)
        sprintf(bufftime, "0%d:0%d:0%d", _hora, _minuto, _segundo);
      else
        sprintf(bufftime, "0%d:0%d:%d", _hora, _minuto, _segundo);
    }
    else {
      if(_segundo < 10)
        sprintf(bufftime, "0%d:%d:0%d", _hora, _minuto, _segundo);
      else
        sprintf(bufftime, "0%d:%d:%d", _hora, _minuto, _segundo);
    }
  }
  else {
    if(_minuto < 10) {
      if(_segundo < 10)
        sprintf(bufftime, "%d:0%d:0%d", _hora, _minuto, _segundo);
      else
        sprintf(bufftime, "%d:0%d:%d", _hora, _minuto, _segundo);
    }
    else {
      if(_segundo < 10)
        sprintf(bufftime, "%d:%d:0%d", _hora, _minuto, _segundo);
      else
        sprintf(bufftime, "%d:%d:%d", _hora, _minuto, _segundo);
    }
  }
}

void dateStr(uint8_t _dia, uint8_t _mes, uint16_t _ano) {
  //Dec 22 2013
  _ano = _ano + 2000;
  switch(_mes) {
    case 1:
      if(_dia < 10)
        sprintf(buffdate, "Jan 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Jan %d %d", _dia, _ano);
    break;
    case 2:
      if(_dia < 10)
        sprintf(buffdate, "Feb 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Feb %d %d", _dia, _ano);
    break;
    case 3:
      if(_dia < 10)
        sprintf(buffdate, "Mar 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Mar %d %d", _dia, _ano);
    break;
    case 4:
      if(_dia < 10)
        sprintf(buffdate, "Apr 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Apr %d %d", _dia, _ano);
    break;
    case 5:
      if(_dia < 10)
        sprintf(buffdate, "May 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "May %d %d", _dia, _ano);
    break;
    case 6:
      if(_dia < 10)
        sprintf(buffdate, "Jun 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Jun %d %d", _dia, _ano);
    break;
    case 7:
      if(_dia < 10)
        sprintf(buffdate, "Jul 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Jul %d %d", _dia, _ano);
    break;
    case 8:
      if(_dia < 10)
        sprintf(buffdate, "Aug 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Aug %d %d", _dia, _ano);
    break;
    case 9:
      if(_dia < 10)
        sprintf(buffdate, "Sep 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Sep %d %d", _dia, _ano);
    break;
    case 10:
      if(_dia < 10)
        sprintf(buffdate, "Oct 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Oct %d %d", _dia, _ano);
    break;
    case 11:
      if(_dia < 10)
        sprintf(buffdate, "Nov 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Nov %d %d", _dia, _ano);
    break;
    case 12:
      if(_dia < 10)
        sprintf(buffdate, "Dec 0%d %d", _dia, _ano);
      else
        sprintf(buffdate, "Dec %d %d", _dia, _ano);
    break;
  }
}

void atualizaDisplay() {
  if ((botaoApertado == true) || (lastReportedPos != encoderPos)) {
    botaoApertado = false;
    tempoDisplay = millis();
  }

  if (millis() - tempoDisplay > 5000) {
    LightLevel();
    RtcDateTime now = Rtc.GetDateTime();
    Menu_standby(1, flag_lowBright, now.Hour(), now.Minute(), now.Day(), now.Month());
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

void LightLevel(void) {
  flag_lowBright = 1;
  leituraLDR = analogRead(ldr);
  if(leituraLDR < 40)
    flag_lowBright = 1;
  else
    flag_lowBright = 0;
}

void LedConfirma(void) {
  digitalWrite(led_Red, HIGH);
  digitalWrite(led_Red2, HIGH);
  digitalWrite(led_Red3, HIGH);
  digitalWrite(led_Red4, HIGH);
  digitalWrite(led_Red5, HIGH);
  digitalWrite(led_Red6, HIGH);
  delay(1000);
  digitalWrite(led_Red, LOW);
  digitalWrite(led_Red2, LOW);
  digitalWrite(led_Red3, LOW);
  digitalWrite(led_Red4, LOW);
  digitalWrite(led_Red5, LOW);
  digitalWrite(led_Red6, LOW);
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
      digitalWrite(buzzer_Ativo, HIGH);
      delay(100);
      digitalWrite(buzzer_Ativo, LOW);
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
      digitalWrite(buzzer_Ativo, HIGH);
      delay(100);
      digitalWrite(buzzer_Ativo, LOW);
    }
    rotating = false;
  }
}

void doSelect() {
  botaoApertado = true;
  digitalWrite(buzzer_Ativo, HIGH);
  LedConfirma();
  digitalWrite(buzzer_Ativo, LOW);
  tempoDisplay = millis();
  flag_init = 1;
  if(flag_init == 1)
    SwitchPress = true;
  digitalWrite(led_Yellow, HIGH);
}

void stopAlarmClock() {
  microswApertado = true;
  disparouAlarme = false;
  Rtc.LatchAlarmsTriggeredFlags();
}

void alarmTriggered() {
  disparouAlarme = true;
}
