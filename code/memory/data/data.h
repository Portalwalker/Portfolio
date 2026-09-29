#ifndef DATA_H
#define DATA_H


//  Genesis 1:22
//  And God blessed them, saying, “Be fruitful and multiply, and fill the waters in the seas, and let birds multiply on the earth.”


    #include "dep/unite.h"

    // TLDR using macro magic to simplify working with multi-type/mult-size arrays
    //      while working with mmap'ed memory arena's to be boundary safe and independent of libc, malloc, & free

    #define DATA_MASK_FLAGS  0b1111111100000000000000000000000000000000000000000000000000000000
    #define DATA_MASK_ABIT   0b1000000000000000000000000000000000000000000000000000000000000000
    #define DATA_MASK_TYPE   0b0000111100000000000000000000000000000000000000000000000000000000
    #define DATA_MASK_LEN    0b0000000011111111111111111111111111111111111111111111111111111111

    #define DATA_FLAG_OFFSET  56
    #define DATA_TYPE_OFFSET  DATA_FLAG_OFFSET
    #define DATA_ABIT_OFFSET (DATA_FLAG_OFFSET + 7)

    #define DATA_FLAG_BITS     8
    #define DATA_LENGTH_BITS   DATA_FLAG_OFFSET
    #define DATA_MAX_LENGTH_IN_BYTES DATA_MASK_LEN

    #define T1      0b00000000
    #define T2      0b00000001
    #define T4      0b00000010
    #define T8      0b00000011
    #define T16     0b00000100
    #define T32     0b00000101
    #define T64     0b00000110
    #define T128    0b00000111
    #define T256    0b00001000
    #define T512    0b00001001
    #define T1024   0b00001010
    #define T2048   0b00001011
    #define T4096   0b00001100
    #define T8192   0b00001101
    #define T16384  0b00001110
    #define T32768  0b00001111

    #define NOARENA 0b00000000
    #define ARENA   0b10000000

    #define DATA_DEEP_COPY     0
    #define DATA_SHALLOW_COPY  1
    #define DATA_IS_A_VALUE    0
    #define DATA_IN_ADDRESS    1

    // operation defines
    #define DATA_LEN(x)    (u64c(x) & DATA_MASK_LEN)
    #define DATA_ABIT(x)  ((u64c(x) & DATA_MASK_ABIT  ) >> DATA_ABIT_OFFSET  )
    #define DATA_TYPE(x)  ((u64c(x) & DATA_MASK_TYPE  ) >> DATA_TYPE_OFFSET  )

    // very explicit set-operation macros
    #define DATA_SANITIZE_LEN(len) DATA_LEN(len)
    #define DATA_SANITIZE_ABIT(abit) ((u64c(abit) << DATA_ABIT_OFFSET) & DATA_MASK_ABIT)
    #define DATA_SANITIZE_TYPE(type) ((u64c(type) << DATA_TYPE_OFFSET) & DATA_MASK_TYPE)
    #define DATA_SET_LEN_ABIT_TYPE(len, abit, type) ( DATA_SANITIZE_ABIT(abit) | DATA_SANITIZE_TYPE(type) | DATA_SANITIZE_LEN(len) )

    #define data_set_info(len, abit, type)      DATA_SET_LEN_ABIT_TYPE((len), (abit), (type));
    #define data_in_arena(ptr)                 (DATA_ABIT((ptr)->info) == ARENA)
    #define data_len(dataptr)                   DATA_LEN((dataptr)->info)
    #define data_type(dataptr)                  DATA_TYPE((dataptr)->info)
    #define data_index_offset(dataptr, index)   ((index) << data_type(dataptr))
    #define data_type_width(dataptr)                  (1 << data_type(dataptr))
    #define data_amt_width(amt, type)             ((amt) << (type))
    #define data_num_items(dataptr)             (data_len(dataptr) >> data_type(dataptr))
    #define data_empty(dataptr)                 (data_num_items(dataptr) == 0)

    #define data_frame(dataptr, len_in_units, type) stackstruct(data, (dataptr), datainfo((len_in_units), (type)))


    // NOTE: macros dependency => looks to data_num_items() => looks to data_len() AND data_type() => looks to [data masks] AND [info block]


    // TODO: FIX PARAMETER ORDER SO ITS MORE UNDERSTANDABLE => VALUE, INDEX, DATAPTR START HERE && ADD TYPE parameter to data_getval for code clarity
    #define data_read(var, dataptr, index, deepshallow)        data_read_index_into((var), (dataptr), (index), (deepshallow))
    #define data_write(addr, dataptr, index)                   data_write_index_from(voidptr(addr), (dataptr), (index))
    #define data_setval(value, dataptr, index)                      data_set_value_at_index(u64c(value), (dataptr), (index))
    #define data_getval(dataptr, index)                             data_get_value_from_index((dataptr), (index))
    #define data_pushval(value, dataptr)                            data_add_index(u64c(value), (dataptr), DATA_IS_A_VALUE)
    #define data_pushobj(objaddr, dataptr)                          data_add_index(u64c(value), (dataptr), DATA_IN_ADDRESS)
    #define data_popval(dataptr)                               u64c(data_sub_index(dataptr))
    #define data_popobj(dataptr, ptrtype)                      cast(data_sub_index(dataptr), ptrtype*)
    #define data_incrlen(dataptr, bytes)                            data_incr_len((bytes), (dataptr))
    #define data_decrlen(dataptr, bytes)                            data_decr_len((bytes), (dataptr))

    #define data_arena(dataptr)                            ((dataptr)->block)
    #define data_last_index(dataptr)         (data_num_items(dataptr) - 1)
    #define data_index_after_last(dataptr)    data_num_items(dataptr)

    #define data_incrunits(dataptr, num)     data_incrlen((dataptr), data_type_width(dataptr) * (num))
    #define data_decrunits(dataptr, num)     data_decrlen((dataptr), data_type_width(dataptr) * (num))
    #define data_increment(dataptr)          data_incrunits((dataptr), 1)
    #define data_decrement(dataptr)          data_decrunits((dataptr), 1)

    // NOTE: its possible to do simple arithmetic on the 'info' block because the design of the data structure/masks make it possible

    // data
    //      block => arena descriptor block
    //      info  => 8th byte (flags), other 7 bytes (length)
    typedef struct
    {
        u64 block; // NOTE: in case compiling for 32 bit: must force u64 bit type upon pointer position so math across all code works
        u64 info;
    } data;

     u64 data_max_items(data*);
     u8* data_start(data*);
    void data_add_index(u64, data*,u8);
     u64 data_sub_index(data*);
    void data_incr_len(u64, data*);
    void data_decr_len(u64, data*);
     u64 data_get_value_from_index(data*, u64);
    void data_read_index_into(void*, data*, u64, u64);
    void data_write_index_from(void*, data*, u64);
    void data_set_value_at_index(u64, data*, u64);


#endif // DATA_H
