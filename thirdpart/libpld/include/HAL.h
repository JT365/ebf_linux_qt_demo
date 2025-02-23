/*
 * HAL.h -- low layer adapter for beadaPanel
 *
 * Copyright (C) 2019 Naxing Electronics
 *                    Author: Weidong Zhou <bruce.zhou2002@gmail.com>
 *
 */

#ifndef _HAL_H
#define _HAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _SYSTEMTIME {
	unsigned short wYear;
	unsigned short wMonth;
	unsigned short wDayOfWeek;
	unsigned short wDay;
	unsigned short wHour;
	unsigned short wMinute;
	unsigned short wSecond;
	unsigned short wMilliseconds;	
} __attribute__((packed)) SYSTEMTIME;

enum INT_SCREEN_MODE{BGRx, xRGB, BGR16, RGB16, none};

/* used by hal.c */
extern int initHal(const char *pBackfilePath);
extern void getHalVer(unsigned short *fwVer, unsigned char *plat, unsigned char *os);
extern unsigned int getStorageSize(void);
extern int getSerialNum(unsigned char* ps, unsigned short len);
extern int getHardwareId(unsigned char *ps, unsigned short len);
extern int getFirmwarePara(unsigned char *ps, unsigned short len);
extern int setFirmwarePara(unsigned char *ps);
extern int getPLBuffSize(void);

extern void initFB(void);
extern unsigned char * flipDualFB(unsigned char *szbuf, enum INT_SCREEN_MODE mode, int int_x, int int_y, int int_bw, int offset_x, int offset_y, unsigned int *bs);
extern int recoverDualFB(void);
extern void fillFB(unsigned char *szbuf, enum INT_SCREEN_MODE mode, int int_x, int int_y, int int_bw, int offset_x, int offset_y);
extern void setFB(unsigned int pt);
extern void getFBResolution(int *px, int *py, int *pbw);

extern void getBrightness(unsigned char *max, unsigned char *current);
extern int setBrightness(unsigned char current);	

extern void setTime(SYSTEMTIME *pTS);
extern void getTime(SYSTEMTIME *pTS);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_H */
