#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#define NOT_IMPL assert(false && "NOT IMPLEMENTED!")
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

/* 16 MiB RAM as per wiki */
/* 16 * 1024 * 1024 = 16777216 => 16777216 + 8 = 16777224 */
/* 8 extra bytes else 9 byte instr @ 0XFFFFFF gonna read out of bounds */
#define MEMORY_SIZE 16777224
#define SCREEN_DIMENSIONS 65536 /* 256 * 256 = 65536 */

typedef struct {
   u8 *memory;
   Color palette[256];
   Color screen[SCREEN_DIMENSIONS];
} bpvm_t;

void bpvm_init(bpvm_t *bpvm)
{
   assert(bpvm->memory == NULL);
   bpvm->memory = malloc(MEMORY_SIZE * sizeof(u8));
   for (u8 r = 0; r < 6; r++) {
      for (u8 g = 0; g < 6; g++) {
         for (u8 b = 0; b < 6; b++) {
            bpvm->palette[r * 36 + g * 6 + b] = (Color) { r * 51, g * 51, b * 51, 255 };
         }
      }
   }

   /* these are black for some reason as per specs lol */
   for (u16 i = 216; i < 256; i++) {
      bpvm->palette[i] = BLACK;
   }
}

int bpvm_load(bpvm_t *bpvm, const char *filename)
{
   assert(bpvm->memory != NULL);
   FILE *f = fopen(filename, "rb");
   if (f) {
      fread(bpvm->memory, 1, MEMORY_SIZE, f);
      fclose(f);
      return 1;
   }

   return -1;
}

void bpvm_frame(bpvm_t *bpvm)
{
   assert(bpvm->memory != NULL);
   NOT_IMPL;
}

void bpvm_render(bpvm_t *bpvm)
{
   assert(bpvm->memory != NULL);
   NOT_IMPL;
}

int main(void)
{
   bpvm_t bpvm = { 0 };
   bpvm_init(&bpvm);
   int ok = bpvm_load(&bpvm, "test.bp");

   if (ok < 0) {
      fprintf(stderr, "[ERROR] BP FILE NOT LOADED\n");
      return 1;
   }

   printf("[SUCCESS] BP FILE LOADED\n");

   /**********************************/
   /*            RAYLIB              */
   /**********************************/
   printf("[INFO] INITIALISING RAYLIB\n");
   InitWindow(512, 512, "BPVM");
   SetTargetFPS(60);

   Image img = GenImageColor(256, 256, BLACK);
   Texture2D txtr = LoadTextureFromImage(img);
   UnloadImage(img);

   return 0;
}
