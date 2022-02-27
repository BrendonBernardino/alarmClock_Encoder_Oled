#include <EEPROM.h>

//uint16 DataWrite = 0;
uint16 AddressWriteA1 = 0x10;
uint16 AddressWriteA2 = 0x11;
uint16 AddressWriteA3 = 0x12;

uint16 Status;
uint16 Data;

void gravaAlarme(uint8_t _alarme_num, uint16_t _hora, uint16_t _minutos);
uint16_t leAlarme(uint8_t _alarme_num);

void gravaAlarme(uint8_t _alarme_num, uint16_t _hora, uint16_t _minutos) {
  switch(_alarme_num) {
    case 1:
      Status = EEPROM.write(AddressWriteA1, _hora);
      Status = EEPROM.write(AddressWriteA1, (_minutos << 8));
    break;
    case 2:
      Status = EEPROM.write(AddressWriteA2, _hora);
      Status = EEPROM.write(AddressWriteA2, (_minutos << 8));
    break;
    case 3:
      Status = EEPROM.write(AddressWriteA3, _hora);
      Status = EEPROM.write(AddressWriteA3, (_minutos << 8));
    break;
  }
}

uint16_t leAlarme(uint8_t _alarme_num) {
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
  return Data;
}
