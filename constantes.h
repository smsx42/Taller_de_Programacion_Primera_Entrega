#ifndef CONSTANTES_H
#define CONSTANTES_H
#include <string>

const char ACCION_JUMP[] = "JUMP";
const char ACCION_RIGHT[] = "RIGHT";
const char ACCION_LEFT[] = "LEFT";
const char ACCION_DUCK[] = "DUCK";
const char ACCION_HIT[] = "HIT";
const char ACCION_UPPERCUT[] = "UPPERCUT";
const char ACCION_SIDEKICK[] = "SIDEKICK";
const char ACCION_HIGHKICK[] = "HIGHKICK";
const char ACCION_NOP[] = "";

const uint8_t BYTE_FIN = 0x00;
const uint8_t BYTE_JUMP = 0x01;
const uint8_t BYTE_RIGHT = 0x02;
const uint8_t BYTE_LEFT = 0x03;
const uint8_t BYTE_DUCK = 0x04;
const uint8_t BYTE_HIT = 0x05;

const uint8_t BYTE_UPPERCUT = 0x06;
const uint8_t BYTE_SIDEKICK = 0x07;
const uint8_t BYTE_HIGHKICK = 0x08;

const int TOPE_UPPERCUT = 2;
const int TOPE_SIDEKICK = 2;
const int TOPE_HIGHKICK = 3;

#endif  // CONSTANTE_H
