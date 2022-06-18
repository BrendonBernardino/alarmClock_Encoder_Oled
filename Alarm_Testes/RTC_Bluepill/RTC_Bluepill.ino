#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
TwoWire WIRE2 (2,I2C_FAST_MODE);
#define Wire WIRE2
//TwoWire WIRE1 (1,I2C_REMAP);
//#define Wire WIRE1
RtcDS3231<TwoWire> Rtc(Wire);

// Pino interrupção
#define DS3231_SQW  PB14

// Informa se o alarme disparou ou não
bool disparouAlarme = false;

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
  delay(5000);
}
