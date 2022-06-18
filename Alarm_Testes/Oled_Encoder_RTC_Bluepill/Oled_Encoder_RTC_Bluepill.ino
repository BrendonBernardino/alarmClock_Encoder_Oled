#include <Wire.h> // must be included here so that Arduino library object file references work
#include <U8g2lib.h>
#include <RtcDS3231.h>
TwoWire WIRE2 (2,I2C_FAST_MODE);
//TwoWire WIRE1 (1,I2C_REMAP);
#define Wire WIRE2
//#define Wire WIRE1
RtcDS3231<TwoWire> Rtc(Wire);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, PB6, PB7, U8X8_PIN_NONE); 

enum PinAssignments {
  encoderPinA = PA1,   // right
  encoderPinB = PA0,   // left
  selectButton = PB9
};

// Pino interrupção
#define DS3231_SQW  PB14

// Informa se o alarme disparou ou não
bool disparouAlarme = false;

volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 1;   // change management
static boolean rotating = false;    // debounce management

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;

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

// Rotina de interrupção do alarme
void ISR_ATTR funcao_alarme()
{
    disparouAlarme = true;
    Serial.println("Alarm TRIGGERED");
}

void setup ()
{
  // Inicia a comunicação serial
  Serial.begin(115200);
  Wire.begin();
  u8g2.begin();

  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(selectButton, INPUT);

  // turn on pullup resistors
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  digitalWrite(selectButton, HIGH);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE);

  attachInterrupt(digitalPinToInterrupt(selectButton), doSelect, FALLING);
  
  // Configura o pino de interrupção
  pinMode(DS3231_SQW, INPUT);
 
  // Inicia a comunicação com o RTC
  Rtc.Begin();

  // Obtém o valor da hora quando o código foi compilado
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  // Se a data/hora não é valida
  // Pode ocorrer na primeira vez que o RTC é usado
  // Ou então se o RTC estiver sem a bateria ou ela estiver com tensão baixa
  if (!Rtc.IsDateTimeValid())
  {
    Serial.println("Data e hora inválidos!");
    Rtc.SetDateTime(compiled);
  }
  else
  {
    // Se a hora/data for válida, mas estiver desatualizada, atualiza
    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled)
    {
      Rtc.SetDateTime(compiled);
    }
  }

  // Desabilita o pino de geração da onda quadrada
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);


  // Configura alarme 1 para um dia e horário específico:
  // Dia 18 às 14:35:20
  DS3231AlarmOne alarm1(
            0,
            1,
            28,
            1,
            DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  Rtc.SetAlarmOne(alarm1);

  // Configura alarme 2 para um dia e horário específico:
  // Dia 12 às 8:40
  DS3231AlarmTwo alarm2(
            1,
            29,
            1,
            DS3231AlarmTwoControl_OncePerMinute);
  Rtc.SetAlarmTwo(alarm2);

  // Limpa os estados dos alarmes
  Rtc.LatchAlarmsTriggeredFlags();
  attachInterrupt(digitalPinToInterrupt(DS3231_SQW), funcao_alarme, FALLING);
}

void loop ()
{
  RtcDateTime now = Rtc.GetDateTime();
  Serial.println(now.Hour());
  Serial.println(now.Minute());
  rotating = true;  // reset the debouncer
  if (lastReportedPos != encoderPos) {
    Serial.print("Index:");
    Serial.println(encoderPos, DEC);
    lastReportedPos = encoderPos;
  }
  if (disparouAlarme)
  {
    disparouAlarme = false; // Limpa flag
   
    // Descobre qual alarme foi
    DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();
   
    if (flag & DS3231AlarmFlag_Alarm1)
    {
      Serial.println("Alarme 1 disparado!");
    }
    if (flag & DS3231AlarmFlag_Alarm2)
    {
      Serial.println("Alarme 2 disparado!");
    }
  }

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

// Interrupt on A changing state
void doEncoderA() {
    // debounce
  if ( rotating ) delay (1);  // wait a little until the bouncing is done

    // Test transition, did things really change?
  if ( digitalRead(encoderPinA) != A_set ) { // debounce once more
    A_set = !A_set;

    // adjust counter + if A leads B
    if ( A_set && !B_set )
      encoderPos += 1;
      Serial.println("Girou Encoder");

    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB() {
  if ( rotating ) delay (1);
  if ( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if ( B_set && !A_set )
      encoderPos -= 1;
      Serial.println("Girou Encoder");

    rotating = false;
  }
}

void doSelect() {
  Serial.println("Botao pressionado");
}
