//#include "Telas.h"
#include <U8g2lib.h>

#define menuMin 1 // VALOR MÍNIMO DE NAVEGAÇÃO DO MENU
#define menuMax 3 // VALOR MÁXIMO DE NAVEGAÇÃO DO MENU

enum PinAssignments {
  encoderPinA = PA1,   // right
  encoderPinB = PA0,   // left
  selectButton = PB9
};

volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 0;   // change management
int menu = 0;
int menuHora = 0;
unsigned long tempoDisplay;
bool girouEncoder = true;
bool botaoApertado = false;
static boolean rotating = false;    // debounce management
bool SwitchPress = false;

bool flag_standby = false;
int flag_menuhora = 0;
int flag_init = 0;

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;

//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, PB6, PB7, U8X8_PIN_NONE); 
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, PB6, PB7, U8X8_PIN_NONE);

extern void Menu_standby(void);
extern void Menu(void);
extern void MenuHora(void);
extern void MenuAlarme(void);
extern void MenuData(void);
extern void Hora();
extern void Data();

void giraEncoder();
void wait4Standby();
void atualizaDisplay();

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(selectButton, INPUT);
  pinMode(PC13, OUTPUT);
  pinMode(PA5, OUTPUT);

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
  while(flag_init == 0) {
    digitalWrite(PA5, LOW);
    botaoApertado = false;
  }
  digitalWrite(PA5, HIGH);

  rotating = true;  // reset the debouncer
  menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax);

  giraEncoder();
  
  atualizaDisplay();
  SwitchPress = false;
  
  switch (menu) {
    case 1: {
      tempoDisplay = millis();
      while(girouEncoder == false) {
        Serial.println(millis() - tempoDisplay);
        Serial.println("Menu Hora");
        MenuHora();
        menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax);
        giraEncoder();
        wait4Standby(); //espera para standby
      }
      girouEncoder = false;
//        if (girouEncoder) {
//          while(girouEncoder == false) {
//            Serial.println("Bottao Hora");
//            MenuHora();
//            if (lastReportedPos != encoderPos) {
//              girouEncoder = true;
//              //    lastReportedPos = encoderPos;
//            }
//          }
//        Serial.println("Bottao Hora");
//        MenuHora();
//        }
//        menuHora = constrain( (menuHora + (encoderPos - lastReportedPos) ), menuMin, menuMax);
//        flag_menuhora = 0;
//        if(botaoApertado == true) {
//          Serial.println("Botao pressionado");
//          flag_menuhora = 1;
//          botaoApertado = false;
//        }
//
//        while(flag_menuhora == 1) {
//          switch(menu) {
//            case 1: //hora
//  //              Hora(hora);
//              Serial.println("Selecionando hora");
//            break;
//            case 2: //minuto
//              Serial.println("Selecionando minuto");
//            break;
//            case 3: // ok (retornar para Menu, atualizar RTC)
//              if(botaoApertado == true) {
//                flag_menuhora = 0;
//              }
//              Serial.println("Selecionando OK");
//            break;
//          }
//          menuHora = menu;
//        }
//        girouEncoder = false;

      break;
    }
    case 2: {
      tempoDisplay = millis();
        while(girouEncoder == false) {
          MenuAlarme();
          Serial.println(millis() - tempoDisplay);
          Serial.println("Menu Alarme");
          menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax);
          giraEncoder();
          wait4Standby(); //espera para standby
        }
        girouEncoder = false;
        
//        if (girouEncoder) {
//        MenuAlarme();
//        Serial.println("Tela Alarme");
////        }
//
//        girouEncoder = false;
        
        break;
      }
    case 3: {
      tempoDisplay = millis();
      while(girouEncoder == false) {
        MenuData();
        Serial.println(millis() - tempoDisplay);
        Serial.println("Menu Data");
        menu = constrain( (menu + (encoderPos - lastReportedPos) ), menuMin, menuMax);
        giraEncoder();
        wait4Standby(); //espera para standby
      }
      girouEncoder = false;
//        if (girouEncoder) {
//        MenuData();
//        Serial.println("Tela Data");
////        }
//        girouEncoder = false;
        
        break;
    }
    default: {
        if (girouEncoder) {
          Menu();
          Serial.println("Tela Menu");
        }
        
        girouEncoder = false;

        break;
      }
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
    tempoDisplay = millis();
  }
}

void wait4Standby() {
  if (millis() - tempoDisplay > 5000) {
    while(girouEncoder == false && SwitchPress == false) {
      Serial.println("STANDBY");
      Menu_standby();
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
    Menu_standby();
//    flag_standby = true;
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
  if ( rotating ) delay (1);
  if ( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if ( B_set && !A_set ) {
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
