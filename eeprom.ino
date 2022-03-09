#include <EEPROM.h>

//uint16 DataWrite = 0;
uint16 AddressWriteA1 = 0x10;
uint16 AddressWriteA1_2 = 0x11;
uint16 AddressWriteA2 = 0x12;
uint16 AddressWriteA2_2 = 0x13;
uint16 AddressWriteA3 = 0x14;
uint16 AddressWriteA3_2 = 0x15;

uint16 AddressWriteFlag = 0x16;
uint16 AddressWriteONOFF1 = 0x17;
uint16 AddressWriteONOFF2 = 0x18;

int Status;

void gravaOnOff(uint8_t _selection, uint8_t _onoff);
int leOnOff(uint8_t _selection);
void gravaAlarme(uint8_t _alarme_num, uint16_t _hora, uint16_t _minutos);
int leAlarme(uint8_t _alarme_num, uint8_t _hora_min);

void gravaOnOff(uint8_t _selection, uint8_t _onoff) {
  switch(_selection) {
    case 1: //A1
      Status = EEPROM.write(AddressWriteONOFF1, _onoff);
    break;
    case 2: //A2
      Status = EEPROM.write(AddressWriteONOFF2, _onoff);
    break;
  }
}

int leOnOff(uint8_t _selection) {
  uint16_t DataONOFF;
  switch(_selection) {
    case 1: //A1
      Status = EEPROM.read(AddressWriteONOFF1, &DataONOFF);
    break;
    case 2: //A2
      Status = EEPROM.read(AddressWriteONOFF2, &DataONOFF);
    break;
  }
  return (int)DataONOFF;
}

void gravaAlarme(uint8_t _alarme_num, uint16_t _hora, uint16_t _minutos) {
  switch(_alarme_num) {
    case 1:
      Status = EEPROM.write(AddressWriteA1, _hora);
      Status = EEPROM.write(AddressWriteA1_2, _minutos);
    break;
    case 2:
      Status = EEPROM.write(AddressWriteA2, _hora);
      Status = EEPROM.write(AddressWriteA2_2, _minutos);
    break;
    case 3:
      Status = EEPROM.write(AddressWriteA3, _hora);
      Status = EEPROM.write(AddressWriteA3_2, _minutos);
    break;
  }
}

int leAlarme(uint8_t _alarme_num, uint8_t _hora_min) {
  uint16_t Data;

  switch(_hora_min) {
    case 1: //hora
      switch(_alarme_num) {
        case 1:
          Status = EEPROM.read(AddressWriteA1, &Data);
        break;
        case 2:
          Status = EEPROM.read(AddressWriteA2, &Data);
        break;
        case 3:
          Status = EEPROM.read(AddressWriteA3, &Data);
        break;
      }
    break;
    case 2: //minuto
      switch(_alarme_num) {
        case 1:
          Status = EEPROM.read(AddressWriteA1_2, &Data);
        break;
        case 2:
          Status = EEPROM.read(AddressWriteA2_2, &Data);
        break;
        case 3:
          Status = EEPROM.read(AddressWriteA3_2, &Data);
        break;
      }
    break;
  }
  
  return (int)Data;
}

//int leAlarme2(uint8_t _alarme_num) {
//  uint16_t Data;
//  switch(_alarme_num) {
//    case 1:
//      Status = EEPROM.read(AddressWriteA1_2, &Data);
//    break;
//    case 2:
//      Status = EEPROM.read(AddressWriteA2_2, &Data);
//    break;
//    case 3:
//      Status = EEPROM.read(AddressWriteA3_2, &Data);
//    break;
//  }
//  return (int)Data;
//}
