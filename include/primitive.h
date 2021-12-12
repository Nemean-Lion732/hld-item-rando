/*!
 *  @file primitive.h
 *
 *  These are structures for getting more information from GameMaker Variables
 * 
 *  AERLocals are these primitives, so we can cast their pointer into this type
 * 
 *  Many thanks to Foxbud for sharing these structs on discord 
 */

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef enum PrimitiveType {
    PRIMITIVE_REAL = 0x0,
    PRIMITIVE_STRING = 0x1,
    PRIMITIVE_ARRAY = 0x2,
    PRIMITIVE_PTR = 0x3,
    PRIMITIVE_VEC3 = 0x4,
    PRIMITIVE_UNDEFINED = 0x5,
    PRIMITIVE_OBJECT = 0x6,
    PRIMITIVE_INT32 = 0x7,
    PRIMITIVE_VEC4 = 0x8,
    PRIMITIVE_MATRIX = 0x9,
    PRIMITIVE_INT64 = 0xA,
    PRIMITIVE_ACCESSOR = 0xB,
    PRIMITIVE_NULL = 0xC,
    PRIMITIVE_BOOL = 0xD,
    PRIMITIVE_ITERATOR = 0xE,
} PrimitiveType;

typedef struct PrimitiveString {
    char* chars;
    size_t refs;
    size_t length;
} PrimitiveString;

typedef union __attribute__((aligned(4))) PrimitiveValue {
    uint8_t raw[12];
    double r;
    PrimitiveString* s;
    void* p;
    int32_t i32;
    int64_t i64;
    bool b;
} PrimitiveValue;

typedef struct Primitive {
    PrimitiveValue value;
    PrimitiveType type;
} Primitive;

typedef struct __attribute__((aligned(4))) HLDPrimitiveArray {
    size_t refs;
    struct 
    {
        size_t size; 
        Primitive* elements;
    }* subArrays;
    void* field_8;
    uint32_t field_C;
    size_t numSubArrays;
} HLDPrimitiveArray;

#endif /*   PRIMITIVE_H     */