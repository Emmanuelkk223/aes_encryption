/* xsc256.h */
#pragma once
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>


typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;
typedef unsigned _BitInt(128) int128;
typedef unsigned _BitInt(256) int256;

#define packed __attribute__((packed))
#define alloc(x)    malloc((x))

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $16 (int128)
#define $32 (int256)
#define $i (int)
#define $c (char *)
#define $v (void *)

struct s_int192 {
    int256 x:192;
};
typedef struct s_int192 int192;

struct s_roundkey;
typedef struct s_roundkey roundkey;

struct s_roundkey {
    int8 id:4;
    int128 subkey;
    int64 rc;
    roundkey *next;
} packed;

struct s_state {
    int8 w[16];
    roundkey *subkey;
};
typedef struct s_state state;

struct s_semiwords {
    int8 x:4;
    int8 y:4;
    int8 z:4;
} packed;
typedef struct s_semiwords semiword;

#define show(x)     _Generic((x),               \
    int128: showint128,                          \
    int192: showint192,                           \
    roundkey*: showroundkey                        \
)($1 # x, (x))

void showint128(int8*, int128);
void showint192(int8*, int192);
void showroundkey(int8*, roundkey*);

void zero(int8*,int16);
int128 mksubkey(int8,int128,int64);
roundkey *mkroundkey(int8,int128,int64);
roundkey *gensubkeys(int128);
int192 xbox(int128);
int192 sboxes(int192);
semiword sbox(semiword);
int128 cbox(int192);
int128 f(int128);

state *xsc256int(int256);
 int main(void);
