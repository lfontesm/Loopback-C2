#if !defined(RC4_H_)
#define RC4_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void swap(unsigned char *p1, unsigned char *p2);
void rc4_init(unsigned char *key,int key_len);
void rc4(unsigned char  *key,int key_len,char *buff,int len);

#endif // RC4_H_
