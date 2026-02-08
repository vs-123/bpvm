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

typedef enum {
   BPVM_K_1 = 1 << 0x1,
   BPVM_K_2 = 1 << 0x2,
   BPVM_K_3 = 1 << 0x3,
   BPVM_K_C = 1 << 0xC,

   BPVM_K_4 = 1 << 0x4,
   BPVM_K_5 = 1 << 0x5,
   BPVM_K_6 = 1 << 0x6,
   BPVM_K_D = 1 << 0xD,

   BPVM_K_7 = 1 << 0x7,
   BPVM_K_8 = 1 << 0x8,
   BPVM_K_9 = 1 << 0x9,
   BPVM_K_E = 1 << 0xE,

   BPVM_K_A = 1 << 0xA,
   BPVM_K_0 = 1 << 0x0,
   BPVM_K_B = 1 << 0xB,
   BPVM_K_F = 1 << 0xF,
} bpvm_key_t;

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

void bpvm_kbinp(bpvm_t *bpvm, KeyboardKey key)
{
   bpvm->memory[0] = 0;
   bpvm->memory[1] = 0;

#define KBMAPLIST \
   X(ONE, 1) \
   X(TWO, 2) \
   X(THREE, 3) \
   X(FOUR, C) \
   X(Q, 4) \
   X(W, 5) \
   X(E, 6) \
   X(R, D) \
   X(A, 7) \
   X(S, 8) \
   X(D, 9) \
   X(F, E) \
   X(Z, A) \
   X(X, 0) \
   X(C, B) \
   X(V, F) 

#define X(qwerty_key, bpvm_key) \
   case KEY_##qwerty_key: { \
      bpvm->memory[1] |= BPVM_K_##bpvm_key; \
   } \
   break;

   switch (key) {
   KBMAPLIST

   default:
      assert(false && "UNEXPECTED KEY");
      break;
   }

#undef X
}

void bpvm_frame(bpvm_t *bpvm)
{
   assert(bpvm->memory != NULL);
   printf("[INFO] BPVM_FRAME!\n");
   u8 *pc = bpvm->memory + (bpvm->memory[2] << 16 | bpvm->memory[3] << 8 | bpvm->memory[4]);

   u32 i = 65536;
   do {
      bpvm->memory[pc[3] << 16 | pc[4] << 8 | pc[5]]
          = bpvm->memory[pc[0] << 16 | pc[1] << 8 | pc[2]];
      pc = bpvm->memory + (pc[6] << 16 | pc[7] << 8 | pc[8]);
   } while (--i);
}

void bpvm_render(bpvm_t *bpvm)
{
   assert(bpvm->memory != NULL);
   u8 page  = bpvm->memory[5];
   u32 base = bpvm->memory[5] << 16;

   for (u32 i = 0; i < SCREEN_DIMENSIONS; i++) {
      u8 clridx       = bpvm->memory[base + i];
      Color clr       = bpvm->palette[clridx];
      bpvm->screen[i] = clr;
   }
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

   Image img      = GenImageColor(256, 256, BLACK);
   Texture2D txtr = LoadTextureFromImage(img);

   printf("[INFO] RAYLIB START LOOP\n");
   while (!WindowShouldClose()) {
      bpvm_frame(&bpvm);
      bpvm_render(&bpvm);

      UpdateTexture(txtr, bpvm.screen);

#define HANDLE_VALID_KEYS \
   X(ONE) \
   X(TWO) \
   X(THREE) \
   X(FOUR) \
   X(Q) \
   X(W) \
   X(E) \
   X(R) \
   X(A) \
   X(S) \
   X(D) \
   X(F) \
   X(Z) \
   X(X) \
   X(C) \
   X(V) 

#define X(key) \
   if (IsKeyDown(KEY_##key)) { \
      bpvm_kbinp(&bpvm, KEY_##key); \
   }

      HANDLE_VALID_KEYS

#undef X

      BeginDrawing();
      ClearBackground(BLACK);

      DrawTextureEx(txtr, (Vector2) { 0, 0 }, 0.0f, 2.0f, WHITE);

      EndDrawing();
   }

   printf("[INFO] RAYLIB END LOOP\n");
   UnloadImage(img);

   return 0;
}
