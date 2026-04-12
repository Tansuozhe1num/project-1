#include "./bitarray.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

struct bitarray {
  size_t bit_sz;
  char* buf;
};

static size_t modulo(const ssize_t n, const size_t m) {
  const ssize_t signed_m = (ssize_t)m;
  assert(signed_m > 0);
  const ssize_t result = ((n % signed_m) + signed_m) % signed_m;
  assert(result >= 0);
  return (size_t)result;
}

static char bitmask(const size_t bit_index) {
  return 1 << (bit_index % 8);
}

bitarray_t* bitarray_new(const size_t bit_sz) {
  char* const buf = calloc(1, (bit_sz+7) / 8);
  if (buf == NULL) return NULL;
  bitarray_t* const bitarray = malloc(sizeof(struct bitarray));
  if (bitarray == NULL) {
    free(buf);
    return NULL;
  }
  bitarray->buf = buf;
  bitarray->bit_sz = bit_sz;
  return bitarray;
}

void bitarray_free(bitarray_t* const bitarray) {
  if (bitarray == NULL) return;
  free(bitarray->buf);
  bitarray->buf = NULL;
  free(bitarray);
}

size_t bitarray_get_bit_sz(const bitarray_t* const bitarray) { return bitarray->bit_sz; }

bool bitarray_get(const bitarray_t* const bitarray, const size_t bit_index) {
  assert(bit_index < bitarray->bit_sz);
  return (bitarray->buf[bit_index / 8] & bitmask(bit_index)) ? true : false;
}

void bitarray_set(bitarray_t* const bitarray, const size_t bit_index, const bool value) {
  assert(bit_index < bitarray->bit_sz);
  if (value) bitarray->buf[bit_index / 8] |= bitmask(bit_index);
  else       bitarray->buf[bit_index / 8] &= ~bitmask(bit_index);
}

void bitarray_randfill(bitarray_t* const bitarray){
  int32_t *ptr = (int32_t *)bitarray->buf;
  for (int64_t i=0; i<bitarray->bit_sz/32 + 1; i++){
    ptr[i] = rand();
  }
}

static const uint8_t rev8[256] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

static inline uint64_t reverse_64(uint64_t x) {
    x = ((x & 0x5555555555555555ULL) << 1) | ((x & 0xAAAAAAAAAAAAAAAAULL) >> 1);
    x = ((x & 0x3333333333333333ULL) << 2) | ((x & 0xCCCCCCCCCCCCCCCCULL) >> 2);
    x = ((x & 0x0F0F0F0F0F0F0F0FULL) << 4) | ((x & 0xF0F0F0F0F0F0F0F0ULL) >> 4);
    return __builtin_bswap64(x);
}

static inline void bitarray_reverse(bitarray_t* const bitarray, size_t start, size_t end) {
    char* buf = bitarray->buf;
    
    // 如果整体长度太小，干脆退回纯粹的单比特逐位翻转避免边缘翻转的复杂对齐开销。
    if (end - start < 128) {
        while (start < end) {
            bool s_val = (buf[start >> 3] >> (start & 7)) & 1;
            bool e_val = (buf[end >> 3] >> (end & 7)) & 1;
            if (s_val != e_val) {
                buf[start >> 3] ^= (1 << (start & 7));
                buf[end >> 3] ^= (1 << (end & 7));
            }
            start++;
            end--;
        }
        return;
    }
    
    // 1. Bitwise peel head to align start to byte boundary
    while (start < end && (start & 7) != 0) {
        bool s_val = (buf[start >> 3] >> (start & 7)) & 1;
        bool e_val = (buf[end >> 3] >> (end & 7)) & 1;
        if (s_val != e_val) {
            buf[start >> 3] ^= (1 << (start & 7));
            buf[end >> 3] ^= (1 << (end & 7));
        }
        start++;
        end--;
    }
    
    // 2. Bitwise peel tail to align end to byte boundary (inclusive end means (end+1) % 8 == 0)
    while (start < end && ((end + 1) & 7) != 0) {
        bool s_val = (buf[start >> 3] >> (start & 7)) & 1;
        bool e_val = (buf[end >> 3] >> (end & 7)) & 1;
        if (s_val != e_val) {
            buf[start >> 3] ^= (1 << (start & 7));
            buf[end >> 3] ^= (1 << (end & 7));
        }
        start++;
        end--;
    }
    
    if (start >= end) return;
    
    size_t s_byte = start >> 3;
    size_t e_byte = end >> 3;
    
    // 3. 64-bit bulk processing via __builtin_memcpy to avoid extreme pointer unalignment issues
    while (s_byte + 7 < e_byte) {
        uint64_t s_val, e_val;
        __builtin_memcpy(&s_val, buf + s_byte, 8);
        __builtin_memcpy(&e_val, buf + e_byte - 7, 8);
        
        s_val = reverse_64(s_val);
        e_val = reverse_64(e_val);
        
        __builtin_memcpy(buf + s_byte, &e_val, 8);
        __builtin_memcpy(buf + e_byte - 7, &s_val, 8);
        
        s_byte += 8;
        e_byte -= 8;
    }
    
    // 4. 8-bit LUT fallback for leftover completely aligned inner bytes
    while (s_byte < e_byte) {
        uint8_t temp = buf[s_byte];
        buf[s_byte] = rev8[(uint8_t)buf[e_byte]];
        buf[e_byte] = rev8[temp];
        s_byte++;
        e_byte--;
    }
    
    if (s_byte == e_byte) {
        buf[s_byte] = rev8[(uint8_t)buf[s_byte]];
    }
}

void bitarray_rotate(bitarray_t* const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount) {
  assert(bit_offset + bit_length <= bitarray->bit_sz);
  if (bit_length <= 1) return;
  size_t left_amount = modulo(-bit_right_amount, bit_length);
  if (left_amount == 0) return;

  size_t a_end = bit_offset + left_amount - 1;
  size_t b_start = bit_offset + left_amount;
  size_t end = bit_offset + bit_length - 1;
  
  bitarray_reverse(bitarray, bit_offset, a_end);
  bitarray_reverse(bitarray, b_start, end);
  bitarray_reverse(bitarray, bit_offset, end);
}
