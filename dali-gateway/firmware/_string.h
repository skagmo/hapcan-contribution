#ifndef STRING_H
#define STRING_H

#include "stdint.h"

uint8_t* str_toupper(uint8_t *c);
uint8_t str_utoap(void (*put)(uint8_t), uint32_t n);
uint8_t str_itoap(void (*put)(uint8_t), int32_t n);
uint8_t str_cpyp(void (*put)(uint8_t), uint8_t *src);
uint8_t str_cpycp(void (*put)(uint8_t), const uint8_t *src);
void str_htoap(void (*put)(uint8_t), uint32_t n, uint8_t n_len);
void str_cpy(uint8_t* dest, const uint8_t *src);
void str_cpycl(uint8_t* d, uint16_t* d_len, uint16_t d_size, const uint8_t *src);
void str_cpyll(uint8_t* d, uint16_t* d_len, uint16_t d_size, uint8_t *src, uint16_t src_len);
void str_utoal(uint8_t* d, uint16_t* d_len, uint16_t d_size, uint32_t n);
void str_itoal(uint8_t* d, uint16_t* d_len, uint16_t d_size, int32_t n);
void str_ditoal(uint8_t* d, uint16_t* d_len, uint16_t d_size, int32_t n, uint8_t np);
uint8_t str_atoul(uint8_t* s, uint16_t s_len, uint32_t* n);
uint8_t str_atoil(uint8_t* s, uint16_t s_len, int32_t* n);
void str_htoal(uint8_t* s, uint16_t* s_len, uint32_t n, uint8_t n_len);
void str_parser(uint8_t* in, uint8_t* out, uint16_t* out_len, uint16_t out_size);
uint8_t str_time(uint8_t* out, uint16_t out_size, uint32_t sec);
#endif