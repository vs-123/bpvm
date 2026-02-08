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

void bpvm_kbinp(bpvm_t *bpvm)
{
   bpvm->memory[0] = 0;
   bpvm->memory[1] = 0;

#define KBMAPLIST                                                                                  \
   X(ONE, 0x1)                                                                                     \
   X(TWO, 0x2)                                                                                     \
   X(THREE, 0x3)                                                                                   \
   X(FOUR, 0xC)                                                                                    \
   X(Q, 0x4)                                                                                       \
   X(W, 0x5)                                                                                       \
   X(E, 0x6)                                                                                       \
   X(R, 0xD)                                                                                       \
   X(A, 0x7)                                                                                       \
   X(S, 0x8)                                                                                       \
   X(D, 0x9)                                                                                       \
   X(F, 0xE)                                                                                       \
   X(Z, 0xA)                                                                                       \
   X(X, 0x0)                                                                                       \
   X(C, 0xB)                                                                                       \
   X(V, 0xF)

#define X(qwerty_key, bpvm_key_bit)                                                                \
   if (IsKeyDown(KEY_##qwerty_key)) {                                                              \
      if (bpvm_key_bit <= 7) {                                                                     \
         bpvm->memory[1] |= (1 << bpvm_key_bit);                                                   \
      } else {                                                                                     \
         bpvm->memory[0] |= (1 << (bpvm_key_bit - 8));                                             \
      }                                                                                            \
   }

   KBMAPLIST

#undef X
}

void bpvm_auoutp(const bpvm_t *bpvm, const AudioStream *austream) {
   assert(bpvm->memory != NULL);

   u32 aumembase = (bpvm->memory[6] << 8) | bpvm->memory[7];
   u8 *samples = &bpvm->memory[aumembase << 8];
   UpdateAudioStream(*austream, samples, 256);
}

void bpvm_frame(bpvm_t * restrict bpvm)
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

   printf("[INFO] RAYLIB SETUP TEXTURE\n");
   Image img      = GenImageColor(256, 256, BLACK);
   Texture2D txtr = LoadTextureFromImage(img);

   printf("[INFO] RAYLIB SETUP AUDIO\n");
   InitAudioDevice();
   SetAudioStreamBufferSizeDefault(256);
   AudioStream austream = LoadAudioStream(15360, 8, 1);
   PlayAudioStream(austream);

   printf("[INFO] RAYLIB START LOOP\n");
   while (!WindowShouldClose()) {
      bpvm_kbinp(&bpvm);
      if (IsAudioStreamProcessed(austream)) {
         bpvm_auoutp(&bpvm, &austream);
      }
      bpvm_frame(&bpvm);
      bpvm_render(&bpvm);

      UpdateTexture(txtr, bpvm.screen);

      BeginDrawing();
      ClearBackground(BLACK);

      DrawTextureEx(txtr, (Vector2) { 0, 0 }, 0.0f, 2.0f, WHITE);

      EndDrawing();
   }

   printf("[INFO] RAYLIB END LOOP\n");
   UnloadImage(img);
   CloseAudioDevice();

   return 0;
}
