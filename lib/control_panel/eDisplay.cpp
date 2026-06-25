#include "eDisplay.h"



// распределение сегментов индикатора и светодиодов по таблице кодировки
const uint8_t SEG_A  = 0;
const uint8_t SEG_B  = 1;
const uint8_t SEG_C  = 2;
const uint8_t SEG_D  = 3;
const uint8_t SEG_E  = 4;
const uint8_t SEG_F  = 5;
const uint8_t SEG_G  = 6;
const uint8_t SEG_DP = 7;

const uint32_t constexpr SPEED_BLINK_OFF = SPEED_BLINK/2;

const uint8_t constexpr character[] = { (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<7), // -
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<7), //0
                                        (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //1
                                        (1<<SEG_A)|(1<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<7), //2
                                        (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<7), //3
                                        (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //4
                                        (1<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //5
                                        (1<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //6
                                        (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //7
                                        (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //8
                                        (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //9
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //A
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //B-
                                        (1<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<7), //C
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //D-
                                        (1<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //E
                                        (1<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //F
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //G-
                                        (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //H
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //I-
                                        (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //J
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //K-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<7), //L
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //M-
                                        (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<7), //N-
                                        (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<7), //O-
                                        (1<<SEG_A)|(1<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //P
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //Q-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //R-
                                        (1<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //S-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //T-
                                        (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<7), //U
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //V-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //W-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //X-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //Y-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //Z-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //_
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //`
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //a
                                        (0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //b
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<7), //c
                                        (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<7), //d
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //e-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //f-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //g-
                                        (0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //h
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //i-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //j-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //k-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //l-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //m-
                                        (0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<7), //n
                                        (0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<7), //o
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //p-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //q-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<7), //r
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //s-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //t
                                        (0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //u
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //v-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //w-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //x-

                                        (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<7), //y
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7), //z-
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7),
                                        (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<7)
};

inline uint8_t  bit_is_set(uint16_t reg, uint16_t bit)
{
    return reg & (1 << bit);
}

void eDisplay::display_init(void)
{

}



/***********************************************************************************************/
void eDisplay::show(const char str[], uint8_t blink)
{
    const char num_symbol = 48;
    bool comma_digit[4] = {false, false, false, false};
    char symbol_digit[4] = {'x', 'x', 'x', 'x'};

    uint8_t next_symbol = 0;
    uint8_t next_comma = 0;

    if(blink == STAT) all_digits_stat();

    for(uint8_t i = 0; i <= 8; ++i)
    {
        if((next_comma < 4) && (next_symbol < 4))
        {
            if(str[i] == '.') 
            {
                if(next_symbol > 0) next_comma = next_symbol - 1;
                comma_digit[next_comma] = true;
            }
            else
            {
                if((str[i] >= 0)&&(str[i] < 10)) symbol_digit[next_symbol] = str[i] + num_symbol;
                else symbol_digit[next_symbol] = str[i];
                ++next_symbol;
            }
        }
    }

    digits.digit_4.comma = comma_digit[0];
    digits.digit_4.symbol = symbol_digit[0];

    digits.digit_3.comma = comma_digit[1];
    digits.digit_3.symbol = symbol_digit[1];

    digits.digit_2.comma = comma_digit[2];
    digits.digit_2.symbol = symbol_digit[2];

    digits.digit_1.comma = comma_digit[3];
    digits.digit_1.symbol = symbol_digit[3];
}

void eDisplay::show(const uint32_t data)
{
    bcd32_level_t show_data;

    show_data = bin32_trans(data);

    all_digits_stat();

    digits.digit_4.symbol = show_data.ths;
    digits.digit_4.comma = false;

    digits.digit_3.symbol = show_data.hund;
    digits.digit_3.comma = false;

    digits.digit_2.symbol = show_data.tens;
    digits.digit_2.comma = false;

    digits.digit_1.symbol = show_data.units;
    digits.digit_1.comma = false;

}

void eDisplay::show_clock(const display_clock_t clock, uint8_t blink)
{
    bcd8_level_t hour, minute;

    hour = bin8_trans(clock.hour);
    minute = bin8_trans(clock.minute);
    all_digits_stat();


    digits.digit_4.symbol = hour.tens;
    digits.digit_3.symbol = hour.units;
    digits.digit_2.symbol = minute.tens;
    digits.digit_1.symbol = minute.units;
    
    digits.digit_4.comma = false;
    digits.digit_3.comma = clock.comma;
    digits.digit_2.comma = false;
    digits.digit_1.comma = false;

    switch(blink)
    {
        case BLINK_HOUR:
        {
            digit_4_mode = BLINK;
            digit_3_mode = BLINK;
        }; break;

        case BLINK_MIN:
        {
            digit_2_mode = BLINK;
            digit_1_mode = BLINK;
        }; break;

        default:
        {
            digit_4_mode = STAT;
            digit_3_mode = STAT;
            digit_2_mode = STAT;
            digit_1_mode = STAT;
        }; break;
    }
}

void eDisplay::show_clock(eDS1338& rtc, uint8_t blink)
{
    bcd8_level_t hour, minute;

    hour = bin8_trans(rtc.get_hour());
    minute = bin8_trans(rtc.get_minute());
    all_digits_stat();


    digits.digit_4.symbol = hour.tens;
    digits.digit_3.symbol = hour.units;
    digits.digit_2.symbol = minute.tens;
    digits.digit_1.symbol = minute.units;
    
    digits.digit_4.comma = false;
    digits.digit_3.comma = rtc.get_sec_comma();
    digits.digit_2.comma = false;
    digits.digit_1.comma = false;

    switch(blink)
    {
        case BLINK_HOUR:
        {
            digit_4_mode = BLINK;
            digit_3_mode = BLINK;
        }; break;

        case BLINK_MIN:
        {
            digit_2_mode = BLINK;
            digit_1_mode = BLINK;
        }; break;

        default:
        {
            digit_4_mode = STAT;
            digit_3_mode = STAT;
            digit_2_mode = STAT;
            digit_1_mode = STAT;
        }; break;
    }
}

void eDisplay::show_day(const uint8_t day, uint8_t blink)
{
    all_digits_stat();

    if(blink == BLINK)
    {
        digit_2_mode = BLINK;
        digit_1_mode = BLINK;
        if(day == ALL_DAY) digit_3_mode = BLINK;
        else digit_3_mode = STAT;
    }
    else
    {
        digit_2_mode = STAT;
        digit_1_mode = STAT;
    }

    switch(day)
    {
        case MONDAY:
        {
            char title[] = "__NH";
            show(title, BLINK);
        }; break;

        case TUESDAY:
        {
            char title[] = "__8t";
            show(title, BLINK);
        }; break;

        case WEDNESDAY:
        {
            char title[] = "__CP";
            show(title, BLINK);
        }; break;

        case THURSDAY:
        {
            char title[] = "__4t";
            show(title, BLINK);
        }; break;

        case FRIDAY:
        {
            char title[] = "__Nt";
            show(title, BLINK);
        }; break;

        case SATURDAY:
        {
            char title[] = "__Cb";
            show(title, BLINK);
        }; break;

        case SUNDAY:
        {
            char title[] = "__8C";
            show(title, BLINK);
        }; break;

        case ALL_DAY:
        {
            char title[] = "_ALL";
            show(title, BLINK);
        }; break;

        default:
        {
            char title[] = "__--";
            show(title);
        }; break;        
    }
}

void eDisplay::show_date(const uint8_t date, uint8_t blink)
{
    bcd8_level_t show_date;
    all_digits_stat();

    if(blink == BLINK)
    {
        digit_2_mode = BLINK;
        digit_1_mode = BLINK;
    }
    else
    {
        digit_2_mode = STAT;
        digit_1_mode = STAT;
    }

    show_date = bin8_trans(date);

    digits.digit_4.symbol = 'd';
    digits.digit_3.symbol = '-';
    digits.digit_2.symbol = show_date.tens;
    digits.digit_1.symbol = show_date.units;

    digits.digit_4.comma = false;
    digits.digit_3.comma = false;
    digits.digit_2.comma = false;
    digits.digit_1.comma = false;
}

void eDisplay::show_month(const uint8_t month, uint8_t blink)
{
    all_digits_stat();

    if(blink == BLINK)
    {
        digit_2_mode = BLINK;
        digit_1_mode = BLINK;
    }

    else
    {
        digit_2_mode = STAT;
        digit_1_mode = STAT;
    }

    bcd8_level_t show_month = bin8_trans(month);
    digits.digit_4.comma = false;
    digits.digit_3.comma = false;
    digits.digit_2.comma = false;
    digits.digit_1.comma = false;

    if(!month) 
    {
        char title[] = "u---";
        show(title);
        return;
    }

    digits.digit_4.symbol = 'u';
    digits.digit_3.symbol = '-';
    digits.digit_2.symbol = show_month.tens;
    digits.digit_1.symbol = show_month.units;
}

void eDisplay::show_year(const uint16_t year, uint8_t blink)
{
    bcd16_level_t show_data;
    all_digits_stat();

    if(blink == BLINK)
    {
        digit_4_mode = BLINK;
        digit_3_mode = BLINK;
        digit_2_mode = BLINK;
        digit_1_mode = BLINK;
    }
    else
    {
        digit_4_mode = STAT;
        digit_3_mode = STAT;
        digit_2_mode = STAT;
        digit_1_mode = STAT;
    }

    show_data = bin16_trans(year);

    digits.digit_4.symbol = show_data.ths;
    digits.digit_4.comma = false;

    digits.digit_3.symbol = show_data.hund;
    digits.digit_3.comma = false;

    digits.digit_2.symbol = show_data.tens;
    digits.digit_2.comma = false;

    digits.digit_1.symbol = show_data.units;
    digits.digit_1.comma = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
#define PARAM_DEG   0
#define PARAM_MIN   1
#define PARAM_SEC   2
#define PARAM_TZ    3

void eDisplay::show_location(const int16_t loc, uint8_t param, uint8_t blink)
{
    bcd16_level_t show_data;
    all_digits_stat();
    
    if(param == PARAM_DEG)
    {
        if(loc < 0)
        {
            show_data = bin16_trans(abs(loc));
            digits.digit_4.symbol = '-';
        }
        else
        {
            show_data = bin16_trans(loc);
            digits.digit_4.symbol = 'x';
        }

        digits.digit_3.symbol = show_data.hund;
        digits.digit_2.symbol = show_data.tens;
        digits.digit_1.symbol = show_data.units;
    }
    else if(param == PARAM_MIN)
    {
        show_data = bin16_trans(loc);
        digits.digit_4.symbol = 'u';
        digits.digit_3.symbol = 'x';
        digits.digit_2.symbol = show_data.tens;
        digits.digit_1.symbol = show_data.units;
    }
    else if(param == PARAM_SEC)
    {
        show_data = bin16_trans(loc);
        digits.digit_4.symbol = 'c';
        digits.digit_3.symbol = 'x';
        digits.digit_2.symbol = show_data.tens;
        digits.digit_1.symbol = show_data.units;
    }

    else if(param == PARAM_TZ)
    {
        digits.digit_4.symbol = 't';

        if(loc < 0)
        {
            show_data = bin16_trans(abs(loc));
            digits.digit_3.symbol = '-';
        }
        else
        {
            show_data = bin16_trans(loc);
            digits.digit_3.symbol = 'x';
        }

        digits.digit_2.symbol = show_data.tens;
        digits.digit_1.symbol = show_data.units;
    }

    if(blink == BLINK)
    {
        if(param == PARAM_DEG)
        {
            digit_4_mode = BLINK;
        }
        digit_3_mode = BLINK;
        digit_2_mode = BLINK;
        digit_1_mode = BLINK;
    }
    else
    {
        digit_4_mode = STAT;
        digit_3_mode = STAT;
        digit_2_mode = STAT;
        digit_1_mode = STAT;
    }

    digits.digit_4.comma = false;    
    digits.digit_3.comma = false;    
    digits.digit_2.comma = false;    
    digits.digit_1.comma = false;
}

void eDisplay::show_location_deg(const int16_t loc, uint8_t blink){ show_location(loc, PARAM_DEG, blink); }
void eDisplay::show_location_min(const int16_t loc, uint8_t blink){ show_location(loc, PARAM_MIN, blink); }
void eDisplay::show_location_sec(const int16_t loc, uint8_t blink){ show_location(loc, PARAM_SEC, blink); }
void eDisplay::show_location_tz(const int8_t time_zone, uint8_t blink){ show_location((uint16_t)time_zone, PARAM_TZ, blink); }




/***********************************************************************************************/
void eDisplay::display_char_write_pos(uint8_t value, uint8_t pos, uint8_t mode)
{
    all_digits_stat();
    
    if((value >= 0)&&(value < 10)) value = value + 48;

   switch(pos)
   {
     case 1: {
                if(mode) digit_1_mode = BLINK;
                else digit_1_mode = STAT;

                digits.digit_1.symbol = value;

			 };break;

     case 2: {
                if(mode) digit_2_mode = BLINK;
                else digit_2_mode = STAT;

                digits.digit_2.symbol = value;
			 };break;

     case 3: {
                if(mode) digit_3_mode = BLINK;
                else digit_3_mode = STAT;

                digits.digit_3.symbol = value;
			 };break;

     case 4: {
                if(mode) digit_4_mode = BLINK;
                else digit_4_mode = STAT;

                digits.digit_4.symbol = value;
			 };break;

	 default: ; break;
   }
}

/************************************************************************/
void eDisplay::display_update(void)
{

	switch(step)
	{
        case 0:
        {
            segment = 0;
            step = 1;

        };break;

        case 1:
        {
            if((digit_4_mode)&&(blink_stat)){ segment = 0; }
            else segment = character[digits.digit_4.symbol];

            if (digits.digit_4.comma) { HAL_GPIO_WritePin(DISP_SEG_DP_PORT, DISP_SEG_DP_PIN, GPIO_PIN_SET); }
            else { HAL_GPIO_WritePin(DISP_SEG_DP_PORT, DISP_SEG_DP_PIN, GPIO_PIN_RESET); }

            HAL_GPIO_WritePin(DISP_DIG_4_PORT, DISP_DIG_4_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(DISP_DIG_3_PORT, DISP_DIG_3_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(DISP_DIG_2_PORT, DISP_DIG_2_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(DISP_DIG_1_PORT, DISP_DIG_1_PIN, GPIO_PIN_RESET);

            step = 2;
        };break;

        case 2:
        {
            segment = 0;
            step = 3;
        };break;

        case 3:
        {
            if((digit_3_mode)&&(blink_stat)){ segment = 0; }
            else segment = character[digits.digit_3.symbol];

            if (digits.digit_3.comma) { HAL_GPIO_WritePin(DISP_SEG_DP_PORT, DISP_SEG_DP_PIN, GPIO_PIN_SET); }
            else { HAL_GPIO_WritePin(DISP_SEG_DP_PORT, DISP_SEG_DP_PIN, GPIO_PIN_RESET); }

            HAL_GPIO_WritePin(DISP_DIG_4_PORT, DISP_DIG_4_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(DISP_DIG_3_PORT, DISP_DIG_3_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(DISP_DIG_2_PORT, DISP_DIG_2_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(DISP_DIG_1_PORT, DISP_DIG_1_PIN, GPIO_PIN_RESET);

            step = 4;
        };break;

        case 4:
        {
            segment = 0;
            step = 5;
        };break;

        case 5:
        {
            if((digit_2_mode)&&(blink_stat)){ segment = 0; }
            else segment = character[digits.digit_2.symbol];

            if (digits.digit_2.comma) { HAL_GPIO_WritePin(DISP_SEG_DP_PORT, DISP_SEG_DP_PIN, GPIO_PIN_SET); }
            else { HAL_GPIO_WritePin(DISP_SEG_DP_PORT, DISP_SEG_DP_PIN, GPIO_PIN_RESET); }

            HAL_GPIO_WritePin(DISP_DIG_4_PORT, DISP_DIG_4_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(DISP_DIG_3_PORT, DISP_DIG_3_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(DISP_DIG_2_PORT, DISP_DIG_2_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(DISP_DIG_1_PORT, DISP_DIG_1_PIN, GPIO_PIN_RESET);

            step = 6;
        };break;

        case 6:
        {
            segment = 0;
            step = 7;
        };break;

        case 7:
        {
            if((digit_1_mode)&&(blink_stat)){ segment = 0; }
            else segment = character[digits.digit_1.symbol];

            if (digits.digit_1.comma) { HAL_GPIO_WritePin(DISP_SEG_DP_PORT, DISP_SEG_DP_PIN, GPIO_PIN_SET); }
            else { HAL_GPIO_WritePin(DISP_SEG_DP_PORT, DISP_SEG_DP_PIN, GPIO_PIN_RESET); }

            HAL_GPIO_WritePin(DISP_DIG_4_PORT, DISP_DIG_4_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(DISP_DIG_3_PORT, DISP_DIG_3_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(DISP_DIG_2_PORT, DISP_DIG_2_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(DISP_DIG_1_PORT, DISP_DIG_1_PIN, GPIO_PIN_SET);

            step = 0;

        };break;
	}

////////////////////////////////////////////////////////////////////
    if (blink < SPEED_BLINK_OFF)
    {
	  ++blink;
	  blink_stat = 1;
    }
    else if((blink >= SPEED_BLINK_OFF)&&(blink < SPEED_BLINK))
        {
          blink_stat = 0;
		  ++blink;
        }
        else blink = 0;


    if(bit_is_set(segment, SEG_A)){ HAL_GPIO_WritePin(DISP_SEG_A_PORT, DISP_SEG_A_PIN, GPIO_PIN_SET); }
    else { HAL_GPIO_WritePin(DISP_SEG_A_PORT, DISP_SEG_A_PIN, GPIO_PIN_RESET); }

    if(bit_is_set(segment, SEG_B)){ HAL_GPIO_WritePin(DISP_SEG_B_PORT, DISP_SEG_B_PIN, GPIO_PIN_SET); }
    else { HAL_GPIO_WritePin(DISP_SEG_B_PORT, DISP_SEG_B_PIN, GPIO_PIN_RESET); }

    if(bit_is_set(segment, SEG_C)){ HAL_GPIO_WritePin(DISP_SEG_C_PORT, DISP_SEG_C_PIN, GPIO_PIN_SET); }
    else { HAL_GPIO_WritePin(DISP_SEG_C_PORT, DISP_SEG_C_PIN, GPIO_PIN_RESET); }

    if(bit_is_set(segment, SEG_D)){ HAL_GPIO_WritePin(DISP_SEG_D_PORT, DISP_SEG_D_PIN, GPIO_PIN_SET); }
    else { HAL_GPIO_WritePin(DISP_SEG_D_PORT, DISP_SEG_D_PIN, GPIO_PIN_RESET); }

    if(bit_is_set(segment, SEG_E)){ HAL_GPIO_WritePin(DISP_SEG_E_PORT, DISP_SEG_E_PIN, GPIO_PIN_SET); }
    else { HAL_GPIO_WritePin(DISP_SEG_E_PORT, DISP_SEG_E_PIN, GPIO_PIN_RESET); }

    if(bit_is_set(segment, SEG_F)){ HAL_GPIO_WritePin(DISP_SEG_F_PORT, DISP_SEG_F_PIN, GPIO_PIN_SET); }
    else { HAL_GPIO_WritePin(DISP_SEG_F_PORT, DISP_SEG_F_PIN, GPIO_PIN_RESET); }

    if(bit_is_set(segment, SEG_G)){ HAL_GPIO_WritePin(DISP_SEG_G_PORT, DISP_SEG_G_PIN, GPIO_PIN_SET); }
    else { HAL_GPIO_WritePin(DISP_SEG_G_PORT, DISP_SEG_G_PIN, GPIO_PIN_RESET); }

}



/********************************************************************************************/
/** Конструктор класса  */
eDisplay::eDisplay()
{
    step = 0;
    segment = 0;
    blink = 0;
    digit_4_mode = STAT;
    digit_3_mode = STAT;
    digit_2_mode = STAT;
    digit_1_mode = STAT;
    blink_stat = false;


}


void eDisplay::all_digits_stat(void)
{
    digit_4_mode = STAT;
    digit_3_mode = STAT;
    digit_2_mode = STAT;
    digit_1_mode = STAT;
}

