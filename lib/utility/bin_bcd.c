/* ********* Ѕиблиотека дл¤ преоюразовани¤ двоичных   **********************
   ********* чисел в двоично-дес¤тичные (bin-bcd)     **********************
   ********* функции                                  **********************
   ********* Electric Brothers company                **********************
   ********* v.1.0                                    **********************
   ********* 2011г.                                   **********************
   *************************************************************************/
//#include <avr/io.h>        //—тандартна¤ библиотека ввода/вывода
#include "bin_bcd.h"

#define sdvig 48

bcd32_level_t bin32_trans(const uint32_t bin) //возвращаемый в функцию параметр ¤вл¤етс¤ структурой объ¤вленной в bin_bcd.h
{
 uint32_t binary = bin;

 bcd32_level_t local;                        //local объ¤влена переменной типа bcd_level_t (структура)
 if (binary > 4294967295) binary = 4294967295;

 for (local.bill=0; binary>=1000000000; local.bill++)
 {binary = (binary - 1000000000);}
#ifdef OFFZERO_HUND
 if (local.bill == 0)
   {local.bill = ' ';}
 else local.bill = (local.bill + sdvig);
#else
 local.bill = (local.bill + sdvig);
#endif

 for (local.onh_mill=0; binary>=100000000; local.onh_mill++)
 {binary = (binary - 100000000);}
#ifdef OFFZERO_HUND
 if ((local.bill == ' ')&&(local.onh_mill == 0))
   {local.onh_mill = ' ';}
 else local.onh_mill = (local.onh_mill + sdvig);
#else
 local.onh_mill = (local.onh_mill + sdvig);
#endif

 for (local.th_mill=0; binary>=10000000; local.th_mill++)
 {binary = (binary - 10000000);}
#ifdef OFFZERO_HUND
 if ((local.bill == ' ')&&(local.onh_mill == ' ')&&(local.th_mill == 0))
   {local.th_mill = ' ';}
 else local.th_mill = (local.th_mill + sdvig);
#else
 local.th_mill = (local.th_mill + sdvig);
#endif

 for (local.mill=0; binary>=1000000; local.mill++)
 {binary = (binary - 1000000);}
#ifdef OFFZERO_HUND
 if ((local.bill == ' ')&&(local.onh_mill == ' ')&&(local.th_mill == ' ')&&(local.mill == 0))
   {local.mill = ' ';}
 else local.mill = (local.mill + sdvig);
#else
 local.mill = (local.mill + sdvig);
#endif

 for (local.onh_ths=0; binary>=100000; local.onh_ths++)
 {binary = (binary - 100000);}
#ifdef OFFZERO_HUND
 if ((local.bill == ' ')&&(local.onh_mill == ' ')&&(local.th_mill == ' ')&&(local.mill == ' ')&&(local.onh_ths == 0))
   {local.onh_ths = ' ';}
 else local.onh_ths = (local.onh_ths + sdvig);
#else
 local.onh_ths = (local.onh_ths + sdvig);
#endif


 for (local.tn_ths=0; binary>=10000; local.tn_ths++)
 {binary = (binary - 10000);}
#ifdef OFFZERO_HUND
 if ((local.bill == ' ')&&(local.onh_mill == ' ')&&(local.th_mill == ' ')&&(local.mill == ' ')&&(local.onh_ths == ' ')&&(local.tn_ths == 0))
   {local.tn_ths = ' ';}
 else local.tn_ths = (local.tn_ths + sdvig);
#else
 local.tn_ths = (local.tn_ths + sdvig);
#endif

 for (local.ths=0; binary>=1000; local.ths++)
 {binary = (binary - 1000);}
#ifdef OFFZERO_HUND
 if ((local.bill == ' ')&&(local.onh_mill == ' ')&&(local.th_mill == ' ')&&(local.mill == ' ')&&(local.onh_ths == ' ')&&(local.tn_ths == ' ')&&(local.ths == 0))
   {local.ths = ' ';}
 else local.ths = (local.ths + sdvig);
#else
 local.ths = (local.ths + sdvig);
#endif

 for (local.hund=0; binary>=100; local.hund++)
 {binary = (binary - 100);}
#ifdef OFFZERO_HUND
 if ((local.bill == ' ')&&(local.onh_mill == ' ')&&(local.th_mill == ' ')&&(local.mill == ' ')&&(local.onh_ths == ' ')&&(local.tn_ths == ' ')&&(local.ths == ' ')&&(local.hund == 0))
   {local.hund = ' ';}
 else local.hund = (local.hund + sdvig);
#else
 local.hund = (local.hund + sdvig);
#endif

 for (local.tens=0; binary>=10; local.tens++)
 {binary = (binary - 10);}
#ifdef OFFZERO
 if ((local.bill == ' ')&&(local.onh_mill == ' ')&&(local.th_mill == ' ')&&(local.mill == ' ')&&(local.onh_ths == ' ')&&(local.tn_ths == ' ')&&(local.ths == ' ')&&(local.hund == ' ')&&(local.tens == 0))
   {local.tens = ' ';}
 else local.tens = (local.tens + sdvig);
#else
 local.tens = (local.tens + sdvig);
#endif

 for (local.units=0; binary>=1; local.units++)
 {binary = (binary - 1);}
 local.units = (local.units + sdvig);

 return local;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bcd16_level_t bin16_trans(const uint16_t bin) //возвращаемый в функцию параметр ¤вл¤етс¤ структурой объ¤вленной в bin_bcd.h
{
 uint32_t binary = bin;

 bcd16_level_t local;                        //local объ¤влена переменной типа bcd_level_t (структура)
 if (binary > 65535) binary = 65535;

 for (local.tn_ths=0; binary>=10000; local.tn_ths++)
 {binary = (binary - 10000);}
#ifdef OFFZERO_HUND
 if (local.tn_ths == 0)
   {local.tn_ths = ' ';}
 else local.tn_ths = (local.tn_ths + sdvig);
#else
 local.tn_ths = (local.tn_ths + sdvig);
#endif

 for (local.ths=0; binary>=1000; local.ths++)
 {binary = (binary - 1000);}
#ifdef OFFZERO_HUND
 if ((local.tn_ths == ' ')&&(local.ths == 0))
   {local.ths = ' ';}
 else local.ths = (local.ths + sdvig);
#else
 local.ths = (local.ths + sdvig);
#endif

 for (local.hund=0; binary>=100; local.hund++)
 {binary = (binary - 100);}
#ifdef OFFZERO_HUND
 if ((local.tn_ths == ' ')&&(local.ths == ' ')&&(local.hund == 0))
   {local.hund = ' ';}
 else local.hund = (local.hund + sdvig);
#else
 local.hund = (local.hund + sdvig);
#endif

 for (local.tens=0; binary>=10; local.tens++)
 {binary = (binary - 10);}
#ifdef OFFZERO
 if ((local.tn_ths == '')&&(local.ths == ' ')&&(local.hund == ' ')&&(local.tens == 0))
   {local.tens = ' ';}
 else local.tens = (local.tens + sdvig);
#else
 local.tens = (local.tens + sdvig);
#endif

 for (local.units=0; binary>=1; local.units++)
 {binary = (binary - 1);}
 local.units = (local.units + sdvig);

 return local;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bcd8_level_t bin8_trans(const uint8_t bin) //возвращаемый в функцию параметр ¤вл¤етс¤ структурой объ¤вленной в bin_bcd.h
{
 uint32_t binary = bin;

 bcd8_level_t local;                         //local объ¤влена переменной типа bcd_level_t (структура)
 if (binary > 255) binary = 255;

 for (local.hund=0; binary>=100; local.hund++)
 {binary = (binary - 100);}
#ifdef OFFZERO
 if (local.hund == 0)
   {local.hund = ' ';}
 else local.hund = (local.hund + sdvig);
#else
 local.hund = (local.hund + sdvig);
#endif

 for (local.tens=0; binary>=10; local.tens++)
 {binary = (binary - 10);}
#ifdef OFFZERO
 if ((local.tens == ' ')&&(local.tens == 0))
   {local.tens = ' ';}
 else local.tens = (local.tens + sdvig);
#else
 local.tens = (local.tens + sdvig);
#endif

 for (local.units=0; binary>=1; local.units++)
 {binary = (binary - 1);}
 local.units = (local.units + sdvig);
 return local;
}
