#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

/* 16 MiB RAM as per wiki */
/* 16 * 1024 * 1024 = 16777216 */
/* 16777216 + 8 = 16777224 */
/* we add 8 extra bytes cuz otherwise a 9 byte instr @ 0XFFFFFF is gonna read out of bounds */
#define MEMORY_SIZE 16777224

typedef struct {
   uint8_t *memory;
} bpvm_t;

void bpvm_init(bpvm_t *bpvm)
{
   assert(bpvm->memory == NULL);
   bpvm->memory = malloc(MEMORY_SIZE * sizeof(uint8_t));
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
   assert(false && "NOT IMPLEMENTED");
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

   return 0;
}
