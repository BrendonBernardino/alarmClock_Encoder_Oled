#include <U8g2lib.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
TwoWire WIRE2 (2,I2C_FAST_MODE);
#define Wire WIRE2
RtcDS3231<TwoWire> Rtc(Wire);

#define menuMin 1 // VALOR MÍNIMO DE NAVEGAÇÃO DO MENU
#define menuMax 3 // VALOR MÁXIMO DE NAVEGAÇÃO DO MENU

enum PinAssignments {
  encoderPinA =     PA1,   // right
  encoderPinB =     PA0,   // left
  selectButton =    PB9,
  buzzer_Passivo =  PA11,
  buzzer_Ativo =    PA12,
  ldr =             PB1,
  micro_Sw =        PB13,
  led_Yellow =      PC13,
  led_Red =         PA5,
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

int menu = 0;
int menuHora = 0;
int menuAlarme = 0;
int menuData = 0;
int menuA1 = 0;
int menuA2 = 0;
int menuA3 = 0;
int menuAlarmeSelection = 0;

uint8_t set_segundo = 0;

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
unsigned long tempoBuzzer;
bool girouEncoder = true;
bool botaoApertado = false;
static boolean rotating = false;    // debounce management
bool SwitchPress = false;
bool microswApertado = false;
bool disparouAlarme = false;
uint8_t flag_lowBright = 0;

int retornar = 0;
bool retornarHora = false;
bool retornarMinuto = false;

bool retornarA1 = false;
bool retornarA2 = false;
bool retornarA3 = false;
bool retornarOnoff = false;
bool retornarHoraAlarme = false;
bool retornarMinutoAlarme = false;

bool retornarDayweek = false;
bool retornarDia = false;
bool retornarMes = false;
bool retornarAno = false;

int flag_menuhora = 0;
int flag_init = 0;
int flag_init2 = 0;

int leituraLDR = 0;

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;

extern int Status;

//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, PB6, PB7, U8X8_PIN_NONE); 
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, PB6, PB7, U8X8_PIN_NONE);

void timeStr(uint8_t _hora, uint8_t _minuto, uint8_t _segundo);
void dateStr(uint8_t dia, uint8_t _mes, uint16_t _ano);

extern void gravaOnOff(uint8_t _selection, uint8_t _onoff);
extern int leOnOff(uint8_t _selection);
extern void gravaFlag(uint8_t _flag);
extern int leFlag();
extern void gravaAlarme(uint8_t _alarme_num, uint16_t _hora, uint16_t _minutos);
extern int leAlarme(uint8_t _alarme_num, uint8_t _hora_min);

//TELAS
extern void Menu_Alarm_Triggered(void);
extern void Menu_init(void);
extern void Menu_standby(uint8_t _brilho, int _hora, int _minuto);
extern void Menu_standby2(uint8_t _brilho, int _hora, int _minuto);
extern void Menu_standby_alarm(uint8_t _brilho, int _hora, int _minuto);
extern void Menu_standby_alarm2(uint8_t _brilho, int _hora, int _minuto);
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
void LightLevel(void);

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(selectButton, INPUT);
  pinMode(led_Yellow, OUTPUT);
  pinMode(led_Red, OUTPUT);
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

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE);

  attachInterrupt(digitalPinToInterrupt(selectButton), doSelect, FALLING);

  attachInterrupt(digitalPinToInterrupt(micro_Sw), stopAlarmClock, FALLING);

  attachInterrupt(digitalPinToInterrupt(rtc_Sqw), alarmTriggered, FALLING);

  flag_init = 0;

  Serial.begin(115200);  // output
  Wire.begin();

  u8g2.begin();
  Rtc.Begin();

//  flag_init2 = leFlag();
//  if(flag_init2 == 0) {
//    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
//    Rtc.SetDateTime(compiled);
//    flag_init2 = 1;
//    gravaFlag(flag_init2);
//  }
//  else {
//    RtcDateTime now = Rtc.GetDateTime();
//    timeStr(now.Hour(), now.Minute(), now.Second());
//    dateStr(now.Day(), now.Month(), now.Year());
//    RtcDateTime timeDateAtual = RtcDateTime(buffdate, bufftime);
//    Rtc.SetDateTime(timeDateAtual);
//  }
//  timeStr(12, 30, 30);
//  dateStr(21, 2, 22);
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Rtc.SetDateTime(compiled);

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);

  Rtc.LatchAlarmsTriggeredFlags();
}

// main loop, work is done by interrupt service routines, this one only prints stuff
void loop() {
  //// ESPERA CLICK DO BOTAO PARA INICIALIZAR
  while(flag_init == 0) {
    Menu_init();
    digitalWrite(PA5, LOW);
//    botaoApertado = false;
  }
  while(botaoApertado == true) {
    botaoApertado = false;
    delay(2000);
  }
  ////
  digitalWrite(PA5, HIGH);

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
        Serial.println(millis() - tempoDisplay);
        Serial.println("Menu Hora");
        Serial.println("Micro SW: ");
        Serial.println(microswApertado);
        wait4Standby(); //espera para standby
        LightLevel();
        RtcDateTime now = Rtc.GetDateTime();
        Menu(flag_lowBright, menu, now.DayOfWeek()+1, now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
//        Menu(flag_lowBright, menu, dayweek_config, day_config, month_config, hora_config, minuto_config, 2);
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
                  Serial.println("Config Hora");
                  LightLevel();
                  Time_Config(flag_lowBright, menuHora, hora_config, minuto_config);
                  if(SwitchPress) { ////AJUSTE DE HORA
                    SwitchPress = false;
                    botaoApertado = false;
                    Serial.println("CONFIG HORA SELECIONADO");
                    retornarHora = false;
                    while(retornarHora == false) {
                      Serial.println("CONFIGURE A HORA");
                      hora_config = constrain( (hora_config + (encoderPos - lastReportedPos) ), 0, 23); //24 opções
                      giraEncoder();
                      LightLevel();
                      Time_Config(flag_lowBright, menuHora, hora_config, minuto_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarHora = true;
                        Serial.println("HORA CONFIGURADA");
                        Serial.println(hora_config);
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
                  Serial.println("Config Minuto");
                  LightLevel();
                  Time_Config(flag_lowBright, menuHora, hora_config, minuto_config);
                  if(SwitchPress) { ////AJUSTE DE MINUTO
                    SwitchPress = false;
                    botaoApertado = false;
                    Serial.println("CONFIG MINUTO SELECIONADO");
                    retornarMinuto = false;
                    while(retornarMinuto == false) {
                      Serial.println("CONFIGURE O MINUTO");
                      minuto_config = constrain( (minuto_config + (encoderPos - lastReportedPos) ), 0, 59); //60 opções
                      giraEncoder();
                      LightLevel();
                      Time_Config(flag_lowBright, menuHora, hora_config, minuto_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarMinuto = true;
                        Serial.println("MINUTO CONFIGURADO");
                        Serial.println(minuto_config);
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
                  Serial.println("Bottao OK");
                  LightLevel();
                  Time_Config(flag_lowBright, menuHora, hora_config, minuto_config);
                  if(SwitchPress) {
                    Serial.println("RETORNAR");
                    SwitchPress = false;
                    botaoApertado = false;
                    retornar = 1;
                    /////// TODO: COLOCAR AQUI SET RTC
//                    RtcDateTime nowTime = Rtc.GetDateTime();
                    timeStr(hora_config, minuto_config, set_segundo);
                    dateStr(1, 1, 1);
                    RtcDateTime now = RtcDateTime(__DATE__, __TIME__);
                    Rtc.SetDateTime(now);
                    //
                    LightLevel();
                    Menu(flag_lowBright, menu, now.DayOfWeek()+1, now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
//                    Menu(flag_lowBright, menu, dayweek_config, day_config, month_config, hora_config, minuto_config, 2);
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
        Serial.println(millis() - tempoDisplay);
        Serial.println("Menu Alarme");
        Serial.println("Micro SW: ");
        Serial.println(microswApertado);
        wait4Standby(); //espera para standby
        LightLevel();
        RtcDateTime now = Rtc.GetDateTime();
        Menu(flag_lowBright, menu, now.DayOfWeek()+1, now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
//        Menu(flag_lowBright, menu, dayweek_config, day_config, month_config, hora_config, minuto_config, 2);
        //--------------------
        if(SwitchPress) { ////TELA ALARMES CONFIGURACAO
          SwitchPress = false;
          botaoApertado = false;
          retornar = 0;
          while(retornar == 0) {
            menuAlarmeSelection = constrain( (menuAlarmeSelection + (encoderPos - lastReportedPos) ), menuMin, 3); //3 menus
            giraEncoder();
            switch(menuAlarmeSelection) {
              case 1://A1
                while(girouEncoder == false) {
                  Serial.println("A1 Select");
                  LightLevel();
                  SelectAlarme(flag_lowBright, menuAlarmeSelection);
                  if(SwitchPress) { ////TELA ALARME 1:
                    SwitchPress = false;
                    botaoApertado = false;
                    horaAlarme_config = leAlarme(menuAlarmeSelection, 1);
                    minutoAlarme_config = leAlarme(menuAlarmeSelection, 2);
                    Serial.println("ALARME 1");
                    retornarA1 = false;
                    while(retornarA1 == false) {
                      menuA1 = constrain( (menuA1 + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
                      giraEncoder();
                      Serial.println("A1: CONFIGURE O ON OFF");
                      switch(menuA1) {
                        case 1://ON OFF
//                          onoff_config = leOnOff(menuAlarmeSelection);
                          while(girouEncoder == false) {
                            Serial.println("ON OFF");
                            LightLevel();
                            onoff_config = leOnOff(menuAlarmeSelection);
                            AlarmeA_config(flag_lowBright, menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              botaoApertado = false;
                              Serial.println("ON OFF SELECIONADO");
                              retornarOnoff = false;
                              while(retornarOnoff == false) {
                                Serial.println("CONFIGURE O ESTADO");
                                onoff_config = constrain( (onoff_config + (encoderPos - lastReportedPos) ), 0, 1); //2 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarOnoff = true;
                                  gravaOnOff(menuAlarmeSelection, onoff_config);
                                  Serial.println("ONOFF CONFIGURADO");
                                  Serial.println(onoff_config);
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA1 = constrain( (menuA1 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM ON OFF
                        case 2://A1:HORA
                          while(girouEncoder == false) {
                            Serial.println("A1: HORA");
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              botaoApertado = false;
                              Serial.println("A1:HORA SELECIONADO");
                              retornarHoraAlarme = false;
                              while(retornarHoraAlarme == false) {
                                Serial.println("CONFIGURE A HORA");
                                horaAlarme_config = constrain( (horaAlarme_config + (encoderPos - lastReportedPos) ), 0, 23); //24 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarHoraAlarme = true;
                                  Serial.println("HORA CONFIGURADA");
                                  Serial.println(horaAlarme_config);
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA1 = constrain( (menuA1 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A1:HORA
                        case 3://A1:MINUTO
                          while(girouEncoder == false) {
                            Serial.println("A1: MINUTO");
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              botaoApertado = false;
                              Serial.println("A1:MINUTO SELECIONADO");
                              retornarMinutoAlarme = false;
                              while(retornarMinutoAlarme == false) {
                                Serial.println("CONFIGURE OS MINUTOS");
                                minutoAlarme_config = constrain( (minutoAlarme_config + (encoderPos - lastReportedPos) ), 0, 59); //60 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarMinutoAlarme = true;
                                  Serial.println("MINUTO CONFIGURADO");
                                  Serial.println(minutoAlarme_config);
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA1 = constrain( (menuA1 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A1:MINUTO
                        case 4://OK
                          while(girouEncoder == false && retornarA1 == false) {
                            Serial.println("Bottao OK");
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA1, menuAlarmeSelection, onoff_config, horaAlarme_config, minutoAlarme_config);
                            if(SwitchPress) {
                              Serial.println("RETORNAR");
                              SwitchPress = false;
                              botaoApertado = false;
                              retornarA1 = true;
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
                            menuA1 = constrain( (menuA1 + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
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
                  Serial.println("A2 Select");
                  LightLevel();
                  SelectAlarme(flag_lowBright, menuAlarmeSelection);
                  if(SwitchPress) { ////TELA ALARME 2:
                    SwitchPress = false;
                    botaoApertado = false;
                    horaAlarme2_config = leAlarme(menuAlarmeSelection, 1);
                    minutoAlarme2_config = leAlarme(menuAlarmeSelection, 2);
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
                            LightLevel();
                            onoff2_config = leOnOff(menuAlarmeSelection);
                            AlarmeA_config(flag_lowBright, menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              botaoApertado = false;
                              Serial.println("ON OFF SELECIONADO");
                              retornarOnoff = false;
                              while(retornarOnoff == false) {
                                Serial.println("CONFIGURE O ESTADO");
                                onoff2_config = constrain( (onoff2_config + (encoderPos - lastReportedPos) ), 0, 1); //2 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarOnoff = true;
                                  gravaOnOff(menuAlarmeSelection, onoff2_config);
                                  Serial.println("ONOFF CONFIGURADO");
                                  Serial.println(onoff2_config);
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA2 = constrain( (menuA2 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM ON OFF
                        case 2://A2:HORA
                          while(girouEncoder == false) {
                            Serial.println("A2: HORA");
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              botaoApertado = false;
                              Serial.println("A2:HORA SELECIONADO");
                              retornarHoraAlarme = false;
                              while(retornarHoraAlarme == false) {
                                Serial.println("CONFIGURE A HORA");
                                horaAlarme2_config = constrain( (horaAlarme2_config + (encoderPos - lastReportedPos) ), 0, 23); //24 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarHoraAlarme = true;
                                  Serial.println("HORA CONFIGURADA");
                                  Serial.println(horaAlarme2_config);
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA2 = constrain( (menuA2 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A2:HORA
                        case 3://A2:MINUTO
                          while(girouEncoder == false) {
                            Serial.println("A2: MINUTO");
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) { ////AJUSTE DE ONOFF
                              SwitchPress = false;
                              botaoApertado = false;
                              Serial.println("A2:MINUTO SELECIONADO");
                              retornarMinutoAlarme = false;
                              while(retornarMinutoAlarme == false) {
                                Serial.println("CONFIGURE OS MINUTOS");
                                minutoAlarme2_config = constrain( (minutoAlarme2_config + (encoderPos - lastReportedPos) ), 0, 59); //60 opções
                                giraEncoder();
                                LightLevel();
                                AlarmeA_config(flag_lowBright, menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                                girouEncoder = false;
                                if(SwitchPress) {
                                  SwitchPress = false;
                                  botaoApertado = false;
                                  retornarMinutoAlarme = true;
                                  Serial.println("MINUTO CONFIGURADO");
                                  Serial.println(minutoAlarme2_config);
                                }
                              }
                              SwitchPress = false;
                              botaoApertado = false;
                            } //// FIM AJUSTE DE ON OFF
                            menuA2 = constrain( (menuA2 + (encoderPos - lastReportedPos) ), menuMin, 4); //3 menus
                            giraEncoder();
                          }
                          girouEncoder = false;
                        break;//FIM A2:MINUTO
                        case 4://OK
                          while(girouEncoder == false && retornarA2 == false) {
                            Serial.println("Bottao OK");
                            LightLevel();
                            AlarmeA_config(flag_lowBright, menuA2, menuAlarmeSelection, onoff2_config, horaAlarme2_config, minutoAlarme2_config);
                            if(SwitchPress) {
                              Serial.println("RETORNAR");
                              SwitchPress = false;
                              botaoApertado = false;
                              retornarA2 = true;
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
                            menuA2 = constrain( (menuA2 + (encoderPos - lastReportedPos) ), menuMin, 4); //4 menus
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
                  Serial.println("Bottao OK");
                  LightLevel();
                  SelectAlarme(flag_lowBright, menuAlarmeSelection);
                  if(SwitchPress) {
                    Serial.println("RETORNAR");
                    SwitchPress = false;
                    botaoApertado = false;

                    Rtc.LatchAlarmsTriggeredFlags();
                    retornar = 1;
                    LightLevel();
                    RtcDateTime now = Rtc.GetDateTime();
                    Menu(flag_lowBright, menu, now.DayOfWeek()+1, now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
//                    Menu(flag_lowBright, menu, dayweek_config, day_config, month_config, hora_config, minuto_config, 2);
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
        wait4Standby(); //espera para standby
        LightLevel();
        RtcDateTime now = Rtc.GetDateTime();
        Menu(flag_lowBright, menu, now.DayOfWeek()+1, now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
//        Menu(flag_lowBright, menu, dayweek_config, day_config, month_config, hora_config, minuto_config, 2);
//        Data_config(menuData, dayweek_config, day_config, month_config, year_config);
        Serial.println(millis() - tempoDisplay);
        Serial.println("Menu Data");
        Serial.println("Micro SW: ");
        Serial.println(microswApertado);
        if(SwitchPress) { ////TELA DATA CONFIGURACAO
          SwitchPress = false;
          botaoApertado = false;
          retornar = 0;
          while(retornar == 0) {
            menuData = constrain( (menuData + (encoderPos - lastReportedPos) ), menuMin, 5); //5 menus
            giraEncoder();
            switch(menuData) {
              case 1://weekday
                while(girouEncoder == false) {
                  Serial.println("Config Weekday");
                  LightLevel();
                  Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE DAYWEEK
                    SwitchPress = false;
                    botaoApertado = false;
                    Serial.println("CONFIG DAYWEEK SELECIONADO");
                    retornarDayweek = false;
                    while(retornarDayweek == false) {
                      Serial.println("CONFIGURE O DIA DA SEMANA");
                      dayweek_config = constrain( (dayweek_config + (encoderPos - lastReportedPos) ), 1, 7); //7 opções
                      giraEncoder();
                      LightLevel();
                      Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarDayweek = true;
                        Serial.println("DAYWEEK CONFIGURADO");
                        Serial.println(dayweek_config);
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
                  Serial.println("Config Dia");
                  LightLevel();
                  Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE DIA
                    SwitchPress = false;
                    botaoApertado = false;
                    Serial.println("CONFIG DIA SELECIONADO");
                    retornarDia = false;
                    while(retornarDia == false) {
                      Serial.println("CONFIGURE O DIA");
                      day_config = constrain( (day_config + (encoderPos - lastReportedPos) ), 1, 31); //7 opções
                      giraEncoder();
                      LightLevel();
                      Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarDia = true;
                        Serial.println("DIA CONFIGURADO");
                        Serial.println(day_config);
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
                  Serial.println("Config Dia");
                  LightLevel();
                  Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE MES
                    SwitchPress = false;
                    botaoApertado = false;
                    Serial.println("CONFIG MES SELECIONADO");
                    retornarMes = false;
                    while(retornarMes == false) {
                      Serial.println("CONFIGURE O MES");
                      month_config = constrain( (month_config + (encoderPos - lastReportedPos) ), 1, 12); //12 opções
                      giraEncoder();
                      LightLevel();
                      Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarMes = true;
                        Serial.println("MES CONFIGURADO");
                        Serial.println(month_config);
                        CalendarControl(day_config, month_config, year_config);
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
                  Serial.println("Config Ano");
                  LightLevel();
                  Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) { ////AJUSTE DE ANO
                    SwitchPress = false;
                    botaoApertado = false;
                    Serial.println("CONFIG ANO SELECIONADO");
                    retornarAno = false;
                    while(retornarAno == false) {
                      Serial.println("CONFIGURE O ANO");
                      year_config = constrain( (year_config + (encoderPos - lastReportedPos) ), 1, 99); //99 opções
                      giraEncoder();
                      LightLevel();
                      Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                      girouEncoder = false;
                      if(SwitchPress) {
                        SwitchPress = false;
                        botaoApertado = false;
                        retornarAno = true;
                        Serial.println("ANO CONFIGURADO");
                        Serial.println(year_config);
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
                  Serial.println("Bottao OK");
                  LightLevel();
                  Data_config(flag_lowBright, menuData, dayweek_config, day_config, month_config, year_config);
                  if(SwitchPress) {
                    Serial.println("RETORNAR");
                    SwitchPress = false;
                    botaoApertado = false;
                    retornar = 1;
                    CalendarControl(day_config, month_config, year_config);
                    /////// TODO: COLOCAR AQUI SET RTC
//                    RtcDateTime now = Rtc.GetDateTime();
//                    timeStr(now.Hour(), now.Minute(), now.Second());
                    dateStr(day_config, month_config, year_config);
                    RtcDateTime SetDate = RtcDateTime(buffdate, bufftime);
                    Rtc.SetDateTime(SetDate);
                    ////
                    LightLevel();
                    Menu(flag_lowBright, menu, now.DayOfWeek(), now.Day(), now.Month(), now.Hour(), now.Minute(), now.Second());
//                    Menu(flag_lowBright, menu, dayweek_config, day_config, month_config, hora_config, minuto_config, 2);
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
//    encoderPos = (encoderPos > NUM_MENU_ITEMS -1 )?0:encoderPos;
    Serial.print("encoderPos:");
    Serial.println(encoderPos, DEC);
    girouEncoder = true;
    lastReportedPos = encoderPos;
//    tocabuzzer(pinBuzzer1, pinBuzzer2);
    tempoDisplay = millis();
  }
}

void tocabuzzer() {
  tempoBuzzer = millis();
  digitalWrite(buzzer_Ativo, HIGH);
  delay(500);
  digitalWrite(buzzer_Ativo, LOW);
  delay(500);
  
//  if (millis() - tempoBuzzer > 1000) {
//    tempoBuzzer = millis();
//    digitalWrite(buzzer_Ativo, !digitalRead(buzzer_Ativo));
//  }
}

void alarmTriggeredOrNot() {
  if(disparouAlarme == true) {
    DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();
    while(microswApertado == false) {
      Serial.println("DESPERTADOR");
      tocabuzzer();
      Menu_Alarm_Triggered();
    }
  }
}

void wait4Standby() {
  if (millis() - tempoDisplay > 5000) {
    tempoSegundo = millis();
    while(girouEncoder == false && SwitchPress == false && microswApertado == false && disparouAlarme == false) {
      Serial.println("STANDBY");
      RtcDateTime now = Rtc.GetDateTime();
      if(onoff_config == 1 || onoff2_config == 1) {
        LightLevel();
        Menu_standby_alarm(flag_lowBright, now.Hour(), now.Minute());
//        Menu_standby_alarm(flag_lowBright, hora_config, minuto_config);
      }
      else { 
        LightLevel();
        Menu_standby(flag_lowBright, now.Hour(), now.Minute());
//          Menu_standby(flag_lowBright, hora_config, minuto_config);
      }
      if (millis() - tempoSegundo > 1000) {
        if(onoff_config == 1 || onoff2_config == 1) {
          LightLevel();
          Menu_standby_alarm2(flag_lowBright, now.Hour(), now.Minute());
//          Menu_standby_alarm2(flag_lowBright, hora_config, minuto_config);
        }
        else {
          LightLevel();
          Menu_standby2(flag_lowBright, now.Hour(), now.Minute());
        }
        tempoSegundo = millis();
      }
    }
  }
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
    Serial.println("RESET STANDBY");
    tempoDisplay = millis();
  }

  if (millis() - tempoDisplay > 5000) {
    Serial.println("STANDBY");
    LightLevel();
    RtcDateTime now = Rtc.GetDateTime();
    Menu_standby(flag_lowBright, now.Hour(), now.Minute());
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
      digitalWrite(buzzer_Passivo, HIGH);
      delay(100);
      digitalWrite(buzzer_Passivo, LOW);
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
      digitalWrite(buzzer_Passivo, HIGH);
      delay(100);
      digitalWrite(buzzer_Passivo, LOW);
    }
    rotating = false;
  }
}

void doSelect() {
  delay(100);
  botaoApertado = true;
  digitalWrite(buzzer_Passivo, HIGH);
  delay(100);
  digitalWrite(buzzer_Passivo, LOW);
  tempoDisplay = millis();
  flag_init = 1;
  if(flag_init == 1) {
    SwitchPress = true;
  //      tempoDisplay = millis();
  }
  digitalWrite(led_Yellow, HIGH);
  Serial.println("SW pressionado");
}

void stopAlarmClock() {
  microswApertado = true;
  Serial.println("PARAR ALARME");
  disparouAlarme = false;
  Rtc.LatchAlarmsTriggeredFlags();
}

void alarmTriggered() {
  disparouAlarme = true;
  Serial.println("Alarme disparado!");
//  tempoBuzzer = millis();
//  tocabuzzer();
//  Menu_init();
//  while(microswApertado == false) {
//    
//  }
//  if (disparouAlarme) {
//    Serial.println("Alarme disparado!");
//    disparouAlarme = false; // Limpa flag
//   
//    // Descobre qual alarme foi
//    DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();
//   
//    if (flag & DS3231AlarmFlag_Alarm1) {
////      onoff_config = leOnOff(menuAlarmeSelection);
//      if(onoff_config) {
//        tempoBuzzer = millis();
//        while(microswApertado == false) {
//          tocabuzzer();
//          Menu_init();
//          //TODO: TELA DE ALARME 1, ESPERANDO PRA SER PRESSIONADA
//        }
//      }
////      else {
////        disparouAlarme = false;
////      }
//      Serial.println("Alarme 1 disparado!");
//    }
//    if (flag & DS3231AlarmFlag_Alarm2) {
////      onoff2_config = leOnOff(menuAlarmeSelection);
//      if(onoff2_config) {
//        tempoBuzzer = millis();
//        while(microswApertado == false) {
//          tocabuzzer();
//          Menu_init();
//          //TODO: TELA DE ALARME 2, ESPERANDO PRA SER PRESSIONADA
//        }
//      }
////      else {
////        disparouAlarme = false;
////      }
//      Serial.println("Alarme 2 disparado!");
//    }
//  }
}
