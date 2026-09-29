#include "dep/data.h"
#include "dep/arena.h"
#include "dep/trix.h"

// compiler trick to force 'data' to be 16 bytes
SIZECHECKER(data, STACK_ALIGNMENT);


// :::: data_max_items
// ::::::::::::::::: returns the maximum possible values that the data arena can hold
// ::::::::::::::::: if no arena is in use, the programmer is responsible to handle the data type's max size and max possible items

u64 data_max_items(data* ptr)
{
    return (DATA_ABIT(ptr->info)) ? (arena_width(ptr->block) / data_type_width(ptr))
                                  : 0;
}


// :::: data_start
// ::::::::::::::::: gets start address of data
// ::::::::::::::::: usually calls 'arena_start' which looks up the proper arena in use in the global arena grid

u8* data_start(data* ptr)
{
    return (DATA_ABIT((ptr)->info) ? arena_start((ptr)->block) : u8ptr((ptr)->block));
}



// :::: data_add_index
// ::::::::::::::::::::: adds an index to the array
// ::::::::::::::::::::: if out-of-space and using an arena => arena in use will be remapped and the array naturally extended
// ::::::::::::::::::::: if out-of-space and not using arena => responsibility is given to programmer to handle this condition in outside logic

void data_add_index(u64 val_or_addr, data* array, u8 vaflag)
{
    debug_return(array == nullptr, "array == nullptr");
    debug_warn(val_or_addr == 0 && vaflag == DATA_IN_ADDRESS, "val_or_addr == 0");
    debug_warn(vaflag != DATA_IS_A_VALUE && vaflag != DATA_IN_ADDRESS, "vaflag != DATA_IN_ADDRESS  and  vaflag != DATA_IS_A_VALUE");

    if (data_in_arena(array))
    {
        if (arena_avail(data_arena(array)) < data_type_width(array))
        {
            u64 block = arena_grow(data_arena(array)); // arena block is examined,
                                                       // the arena is remapped (expanded),
                                                       // the global arena_grid is updated,
                                                       // and a new arena descriptor block is returned and assigned to array->block
                                                       // NOTE: array->block is the arena descriptor block when data is using an arena

            if (block == ARENA_MAP_FAIL)
            {
                debug_return(block == ARENA_MAP_FAIL, "failed to grow arena in order to add a unit of data");
                return;
            }

            array->block = block;
        }
    }

    // write value to the new index
    if (vaflag == DATA_IN_ADDRESS)
    {
        data_write(val_or_addr, array, data_index_after_last(array));
    }
    else
    {
        data_setval(val_or_addr, array, data_index_after_last(array));
    }

    // increases length by: 1 * data_type_width(array)
    data_increment(array);
}



// :::: data_sub_index
// ::::::::::::::::::::: subtracts an index to the array

u64 data_sub_index(data* array)
{
    static u64 value;
    static u64 backindex;

    debug_return(array == nullptr, "array == nullptr");
    debug_warn(data_num_items(array) == 0, "data is empty already");

    if (!data_num_items(array))
    {
        return 0;
    }

    backindex = data_last_index(array);
        value = data_getval(array, backindex);

    data_setval(0, array, backindex);

    // decreases length by: 1 * data_type_width(array)
    data_decrement(array);

    return value;
}



// :::: data_incr_len
// :::::::::::::::::::: increases the length stored in block [info]
// :::::::::::::::::::: decreases number of available bytes within [start] if it is an arena decriptor block

void data_incr_len(u64 numbytes, data* ptr)
{
    debug_return(ptr == nullptr, "ptr == nullptr");
    debug_return(u64c(numbytes + data_len(ptr)) > DATA_MAX_LENGTH_IN_BYTES, "increasing data to overflow bit layout");

    ptr->info += numbytes;

    // NOTE: bit layout defined in arena.c supports such arithmetic
    //       length increase => arena availability decrease
    if (data_in_arena(ptr))
    {
        ptr->block -= numbytes;
    }
}



// :::: data_incr_len
// :::::::::::::::::::: decreases the length stored in block [info]
// :::::::::::::::::::: increases number of available bytes within [start] if it is an arena decriptor block

void data_decr_len(u64 numbytes, data* ptr)
{
    debug_return(ptr == nullptr, "ptr == nullptr");
    debug_return(numbytes > data_len(ptr), "decreasing data by more than its length");

    ptr->info -= numbytes;

    // NOTE: bit layout defined in arena.c supports such arithmetic
    //       length decrease => arena availability increase
    if (data_in_arena(ptr))
    {
        ptr->block += numbytes;
    }
}



// :::: data_get_index
// ::::::::::::::::::::: returns a value from the index of the array

u64 data_get_value_from_index(data* array, u64 index)
{
        debug_return(array == nullptr, "array == nullptr");
        debug_return(index & DATA_MASK_FLAGS, "index overflows flags");
        // debug_warn(index > data_num_items(array), "index out of bounds");

        // NOTE: data_index_offset(array, index) calculates how many bytes forward to move to find the address of the object being indexed
        //       the u8ptr cast ensures good single-byte-addition in place of (void*) which could lead to undefined behavior when adding

        switch (data_type(array))
        {
                case T16 :   //     *((u64*)(array->block + index*8))  // DATA_SHALLOW_COPY

                                    return u64c(data_start(array) + data_index_offset(array, index));


                case T8  :  //      *((u64*)(array->block + index*8))  // DATA_DEEP_COPY

                                    return dref(u64ptr(data_start(array) + data_index_offset(array, index)));


                case T1  :  //      *((u8*)(array->block + index))     // DATA_DEEP_COPY

                                    return dref(data_start(array) + data_index_offset(array, index));


                case T2  :  //      *((u16*)(array->block + index*2))  // DATA_DEEP_COPY

                                    return dref(u16ptr(data_start(array) + data_index_offset(array, index)));


                case T4  :  //      *((u32*)(array->block + index*4))  // DATA_DEEP_COPY

                                    return dref(u32ptr(data_start(array) + data_index_offset(array, index)));


                default  :  //      array->block + index*data_type_width      // DATA_SHALLOW_COPY

                                    return u64c(data_start(array) + data_index_offset(array, index));
        }
}



// :::: data_read_index_into
// ::::::::::::::::::::::::::: reads a value from the index of the array into destination address of a variable

void data_read_index_into(void* dest, data* array, u64 index, u64 deepshallow)
{
    debug_return(dest == nullptr, "dest == nullptr");
    debug_return(array == nullptr, "array == nullptr");
    debug_return(index & DATA_MASK_FLAGS, "index overflows flags");
    debug_return(deepshallow != DATA_DEEP_COPY && deepshallow != DATA_SHALLOW_COPY, "deepshallow must be DATA_DEEP_COPY | DATA_SHALLOW_COPY");
    // debug_warn(index > data_num_items(array), "index out of bounds");

    // NOTE: data_index_offset(array, index) calculates how many bytes forward to move to find the address of the object being indexed
    //       the u8ptr cast ensures good single-byte-addition in place of (void*) which could lead to undefined behavior when adding
    // NOTE: dest is assumed to be the variable being initialized/assigned a value

    switch (data_type(array))
    {

        case T16 :  //      memcpy(addr, array->block + index*sizeofitem, sizeofitem);   // DATA_DEEP_COPY
                    //      void* addr = array->block + index*sizeofitem;                // DATA_SHALLOW_COPY

                            if (deepshallow == DATA_DEEP_COPY)   {
                                                                   dref(u64ptr(dest))     = dref(u64ptr(data_start(array) + data_index_offset(array, index)));
                                                                   dref(u64ptr(dest) + 1) = dref(u64ptr(data_start(array) + data_index_offset(array, index)) + 1);
                                                                 }
                            else        /* DATA_SHALLOW_COPY */  { dref(u64ptr(dest)) = u64c(data_start(array) + data_index_offset(array, index)); }


                    return;

        case T8  :   //      u64 longval = *((u64*)(array->block + index*8));  // DATA_DEEP_COPY
                     //     u64* longval = array->block + index*8;             // DATA_SHALLOW_COPY

                            if (deepshallow == DATA_DEEP_COPY)   { dref(u64ptr(dest)) = dref(u64ptr(data_start(array) + data_index_offset(array, index))); }
                            else         /* DATA_SHALLOW_COPY */ { dref(u64ptr(dest)) = u64c(data_start(array) + data_index_offset(array, index));         }


                    return;

        case T1  :  //      u8 byteval = *((u8*)(array->block + index));  // DATA_DEEP_COPY
                    //     u8* byteval = array->block + index;            // DATA_SHALLOW_COPY


                            if (deepshallow == DATA_DEEP_COPY)   { dref(u8ptr(dest)) = dref(data_start(array) + data_index_offset(array, index));  }
                            else         /* DATA_SHALLOW_COPY */ { dref(u64ptr(dest)) = u64c(data_start(array) + data_index_offset(array, index)); }


                    return;

        case T2  :  //      u16 shortval = *((u16*)(array->block + index*2));  // DATA_DEEP_COPY
                    //     u16* shortval = array->block + index*2;             // DATA_SHALLOW_COPY


                            if (deepshallow == DATA_DEEP_COPY)    { dref(u16ptr(dest)) = dref(u16ptr(data_start(array) + data_index_offset(array, index))); }
                            else         /* DATA_SHALLOW_COPY */  { dref(u64ptr(dest)) = u64c(data_start(array) + data_index_offset(array, index));         }


                    return;

        case T4  :  //      u32 intval = *((u32*)(array->block + index*4));  // DATA_DEEP_COPY
                    //     u32* intval = array->block + index*4;             // DATA_SHALLOW_COPY


                            if (deepshallow == DATA_DEEP_COPY)   { dref(u32ptr(dest)) = dref(u32ptr(data_start(array) + data_index_offset(array, index))); }
                            else         /* DATA_SHALLOW_COPY */ { dref(u64ptr(dest)) = u64c(data_start(array) + data_index_offset(array, index));         }


                    return;

        default  :  //      memcpy(addr, array->block + index*sizeofitem, sizeofitem);   // DATA_DEEP_COPY
                    //      void* addr = array->block + index*sizeofitem;                // DATA_SHALLOW_COPY

                            if (deepshallow == DATA_DEEP_COPY)    { memcpy(dest, data_start(array) + data_index_offset(array, index), data_type_width(array)); }
                            else         /* DATA_SHALLOW_COPY */  { dref(u64ptr(dest)) = u64c(data_start(array) + data_index_offset(array, index));       }


                    return;
    }
}



// :::: data_write_index_from
// :::::::::::::::::::::::::::: writes a value from the address of a variable to the index of the array
// :::::::::::::::::::::::::::: valaddr is interpreted as a value for type T1-T8
// :::::::::::::::::::::::::::: valaddr is interpreted as a pointer to a struct for types T16 and up
// :::::::::::::::::::::::::::: e.g. T16 -> 16-byte struct

void data_write_index_from(void* valaddr, data* array, u64 index)
{
    debug_return(array == nullptr, "array == nullptr");
    debug_return(index & DATA_MASK_FLAGS, "index overflows flags");
    debug_warn(valaddr == 0 && data_type(array) >= T16, "valaddr == nullptr...zeroizing the index instead");
    // debug_warn(index > data_num_items(array), "index out of bounds");

    // NOTE: data_index_offset(array, index) calculates how many bytes forward to move to find the address of the object being indexed
    //       the u8ptr cast ensures good single-byte-addition in place of (void*) which could lead to undefined behavior when adding
    // NOTE: valaddr is assumed to be the address of the variable holding the value

    switch (data_type(array))
    {

        case T16 :   //      memcpy((array->block + index), valaddr, length_of_val);

                             if (valaddr) {
                                            dref(u64ptr(data_start(array) + data_index_offset(array, index)))     = dref(u64ptr(valaddr));
                                            dref(u64ptr(data_start(array) + data_index_offset(array, index)) + 1) = dref(u64ptr(valaddr) + 1);
                                          }
                             else         {
                                            dref(u64ptr(data_start(array) + data_index_offset(array, index)))     = 0;
                                            dref(u64ptr(data_start(array) + data_index_offset(array, index)) + 1) = 0;
                                          }
            return;

        case T8 :   //      *((u64*)(array->block + index*4) = (u64)(longval);

                            dref(u64ptr(data_start(array) + data_index_offset(array, index))) = dref(u64ptr(valaddr));

            return;

        case T1 :   //      *(array->block + index) = (u8)(byteval);

                            dref(data_start(array) + data_index_offset(array, index)) = dref(u8ptr(valaddr));

            return;

        case T2 :   //      *((u16*)(array->block + index*2) = (u16)(shortval);

                            dref(u16ptr(data_start(array) + data_index_offset(array, index))) = dref(u16ptr(valaddr));

            return;

        case T4 :   //      *((u32*)(array->block + index*4) = (u32)(intval);

                            dref(u32ptr(data_start(array) + data_index_offset(array, index))) = dref(u32ptr(valaddr));

            return;

        default :   //      memcpy((array->block + index), valaddr, length_of_val;
                    //      for BIGGER blocks of array (e.g. T16,T32,T64...etc), memcpy array to indexed address

                            if (valaddr) { memcpy(data_start(array) + data_index_offset(array, index), voidptr(valaddr), data_type_width(array)); }
                            else         { memzero(data_start(array) + data_index_offset(array, index), data_type_width(array));         }

            return;
    }
}


// :::: data_set_value_at_index
// :::::::::::::::::::::::::::::: writes a value from the value of a variable to the index of the array
// :::::::::::::::::::::::::::::: val is interpreted as a value for type T1-T8
// :::::::::::::::::::::::::::::: val is interpreted as a pointer to a struct for types T16 and up
// :::::::::::::::::::::::::::::: e.g. T16 -> 16-byte struct

void data_set_value_at_index(u64 val, data* array, u64 index)
{
    debug_return(array == nullptr, "array == nullptr");
    debug_return(index & DATA_MASK_FLAGS, "index overflows flags");
    // debug_warn(index > data_num_items(array), "index out of bounds");   // too many warnings => allow the index to be out of bounds because not all the data structures function the same

    // NOTE: data_index_offset(array, index) calculates how many bytes forward to move to find the address of the object being indexed
    //       the u8ptr cast ensures good single-byte-addition in place of (void*) which could lead to undefined behavior when adding
    // NOTE: valaddr is assumed to be the address of the variable holding the value

    switch (data_type(array))
    {

        case T16 :   //     optimized version of default

                            if (val)
                            {
                                dref(u64ptr(data_start(array) + data_index_offset(array, index)))     = dref(u64ptr(val));
                                dref(u64ptr(data_start(array) + data_index_offset(array, index)) + 1) = dref(u64ptr(val) + 1);
                            }
                            else
                            {
                                dref(u64ptr(data_start(array) + data_index_offset(array, index)))     = 0;
                                dref(u64ptr(data_start(array) + data_index_offset(array, index)) + 1) = 0;
                            }

            return;

        case T8 :   //      *((u64*)(array->block + index*4) = (u64)(longval);

                            dref(u64ptr(data_start(array) + data_index_offset(array, index))) = u64c(val);

            return;

        case T1 :   //      *(array->block + index) = (u8)(byteval);

                            dref(data_start(array) + data_index_offset(array, index)) = u8c(val);

            return;

        case T2 :   //      *((u16*)(array->block + index*2) = (u16)(shortval);

                            dref(u16ptr(data_start(array) + data_index_offset(array, index))) = u16c(val);

            return;

        case T4 :   //      *((u32*)(array->block + index*4) = (u32)(intval);

                            dref(u32ptr(data_start(array) + data_index_offset(array, index))) = u32c(val);

            return;

        default :   //      memcpy((array->block + index), val, length_of_val);
                    //      for BIGGER blocks of array (e.g. T16,T32,T64...etc), memcpy array to indexed address

                            debug_warn(val, "this had better be an address");

                            if (val)     { memcpy(data_start(array) + data_index_offset(array, index), voidptr(val), data_type_width(array)); }
                            else         { memzero(data_start(array) + data_index_offset(array, index), data_type_width(array));              }

            return;
    }
}
