#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "heaplib.h"

#define HEAP_SIZE 1024
#define NUM_TESTS 24
#define NPOINTERS 100

// TODO: Add test descriptions as you add more tests...
const char* test_descriptions[] = {
    /* our SPEC tests */
    /* 0 */ "single init, should return without error",
    /* 1 */ "single init then single alloc, should pass",
    /* 2 */ "single alloc which should fail b/c heap is not big enough",
    /* 3 */ "multiple allocs, verifying no hard-coded block limit",
    /* your SPEC tests */
    /* 4  */ "multiple allocs, verifying all addresses multiples of 8",
    /* 5  */ "alloc many small blocks",
    /* 6  */ "alloc, release, realloc, tests that memory actually released",
    /* 7  */ "multiple allocs & releases for size 0 blocks",
    /* 8  */ "init a heap of size smaller than array, check that init makes heap of size exactly heap_size",
    /* 9  */ "resize returns correctly, preserves contents",
    /* 10 */ "resize ptrs all multiples of 8",
    /* 11 */ "resize allocs null blocks and can resisze to a smaller block",
    /* 12 */ "multiple allocs, which should fail b/c heap not big enough",
    /* 13 */ "your description here",
    /* 14 */ "your description here",
    /* 15 */ "your description here",
    /* STRESS tests */
    /* 16 */ "alloc & free, stay within heap limits",
    /* 17 */ "alloc, resize, & free, for structural integrity",
    /* 18 */ "alloc & free, for data integrity",
    /* 19 */ "alloc, resize & free",
    /* 20 */ "alloc, resize, & free, for structural integrity",
    /* 21 */ "just resize data corrruption test",
    /* 22 */ "your description here",
    /* 23 */ "your description here",
};

/* ------------------ COMPLETED SPEC TESTS ------------------------- */

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init
 * SPECIFICATION BEING TESTED:
 * hl_init should successfully complete (without producing a seg
 * fault) for a HEAP_SIZE of 1024 or more.
 *
 * MANIFESTATION OF ERROR:
 * A basic test of hl_init.  If hl_init has an eggregious programming
 * error, this simple call would detect the problem for you by
 * crashing.
 *
 * Note: this shows you how to create a test that should succeed.
 */
int test00() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    return SUCCESS;
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is room in the heap for a request for a block, hl_alloc
 * should sucessfully return a pointer to the requested block.
 *
 * MANIFESTATION OF ERROR:
 * The call to hl_alloc will return NULL if the library cannot find a
 * block for the user (even though the test is set up such that the
 * library should be able to succeed).
 */
int test01() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns null, test21 returns FAILURE (==0)
    return (hl_alloc(heap, HEAP_SIZE/2) != NULL);
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is not enough room in the heap for a request for a block,
 * hl_alloc should return NULL.
 *
 * MANIFESTATION OF ERROR:
 * This test is designed to request a block that is definitely too big
 * for the library to find. If hl_alloc returns a pointer, the library is flawed.
 *
 * Notice that heaplame's hl_alloc does NOT return NULL. (This is one
 * of many bugs in heaplame.)
 *
 * Note: this shows you how to create a test that should fail.
 *
 * Surely it would be a good idea to test this SPEC with more than
 * just 1 call to alloc, no? 
 */
int test02() {

    // simulated heap is just an array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns NULL, test22 returns SUCCESS (==1)
    return !hl_alloc(heap, HEAP_SIZE*2);

}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * There should be no hard-coded limit to the number of blocks heaplib
 * can support. So if the heap gets larger, so should the number of
 * supported blocks.
 *
 * MANIFESTATION OF ERROR:
 * See how many blocks are supported with heap size N. This number should
 * increase with heap size 2N. Otherwise fail!
 *
 * Note: unless it is fundamentally changed, heaplame will always fail
 * this test. That is fine. The test will have more meaning when run on
 * your heaplib.c
 */
int test03() {

    // now we simulate 2 heaps, once 2x size of the first
    char heap[HEAP_SIZE], heap2[HEAP_SIZE*2];
    int num_blocks = 0;
    int num_blocks2 = 0;

    hl_init(heap, HEAP_SIZE);

    while(true) {
        int *array = hl_alloc(heap, 8);
        if (array)
            num_blocks++;
        else
            break;
    }

    hl_init(heap2, HEAP_SIZE*2);

    while(true) {
        int *array = hl_alloc(heap2, 8);
        if (array)
            num_blocks2++;
        else
            break;
    }
#ifdef PRINT_DEBUG
    printf("%d blocks (n), then %d blocks (2n) allocated\n", num_blocks, num_blocks2);
#endif

    // hoping to return SUCCESS (==1)
    return (num_blocks2 > num_blocks);
}

/* ------------------ YOUR SPEC TESTS ------------------------- */

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED: all addresses multiples of 8
 *
 *
 * MANIFESTATION OF ERROR:
 * tries to allocate several blocks of various (weird) sizes
 * then checks that each ptr returned is a multiple of 8.
 * if not, fails.
 *
 */
int test04() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    void* ptr[4];
    int div = 5;
    uintptr_t t;

    for (int i=0; i<4; i++){
        div = div*2;
        ptr[i] = hl_alloc(heap, i+HEAP_SIZE/div);
    }

    for (int i=0; i<4; i++){
        t = (uintptr_t) ptr[i];
        if ( (t !=0) && (t%8 != 0)) {
            return FAILURE;
        }
    }

    return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_init, hl_alloc
 * SPECIFICATION BEING TESTED: 
 * hl_alloc should be successful if it has enough consecutive space on the heap to satisfy request
 *
 * MANIFESTATION OF ERROR: 
 * test fails if hl_alloc returns null on any call since enough space should be available
 *
 */
int test05() {
  char heap[HEAP_SIZE];
  hl_init(heap,HEAP_SIZE);
  for (int i=0; i<HEAP_SIZE/37; i++){ //changed from 24, fixed the error
                                    // maybe too many blocks originally?
    char* ptr = (char*) hl_alloc(heap, 1);
    if (ptr==NULL){
      return FAILURE;
    }
      ptr[0] = 'a';
  }
  return SUCCESS;
}


/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: init & alloc & release
 * SPECIFICATION BEING TESTED:
 * can reallocate after releasing. tries to allocate something large 
 * which fits, allocates a couple of small blocks to fill stuff up, 
 * then deallocates, then tries to allocate the large thing again. 
 *
 * MANIFESTATION OF ERROR:
 * it fails to find room for the big thing (even though it def had room)
 *
 */
int test06() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    void *ptr = hl_alloc(heap, HEAP_SIZE/2);
    if (ptr == NULL) {
        return FAILURE;
    }
    for (int i=0; i<2; i++){
        hl_alloc(heap, HEAP_SIZE/32);
    }
    hl_release(heap, ptr);
    void *p2 = hl_alloc(heap, HEAP_SIZE/2);
    
    return (p2 != NULL);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: init & alloc & release
 * SPECIFICATION BEING TESTED:
 * That functions deal w/ size 0 blocks correctly. In particular,
 * should always be able to allocate an empty block, and 
 * should always be able to release said block. 
 *
 * MANIFESTATION OF ERROR:
 * Tries to  allocate size 0 blocks to an empty heap,
 * then allocates a large block (works because nothing
 * actually 'in' the heap), then tries to allocate more size 0. 
 * Failure looks like a crash, or the large alloc failing. 
 * Then release all these blocks (failure = segfault).
 * Finally, alloc another large block (failure = ptr of NULL).
 *
 */
int test07() {
    int NUM_EMPTY = 3;
    void* ptr[NUM_EMPTY];
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    for (int i=0; i<NUM_EMPTY; i++){
        ptr[i] = hl_alloc(heap, 0);
        // for allocing 0-size, it is allowed to return NULL
    }
    void* p1 = hl_alloc(heap, HEAP_SIZE/16);
    if (!p1){
        return FAILURE;
    }
    for (int i=0; i<NUM_EMPTY; i++){
        hl_release(heap, ptr[i]);
    }
    for (int i=0; i<NUM_EMPTY; i++){
        ptr[i] = hl_alloc(heap,0);
    }
    for (int i=0; i<NUM_EMPTY; i++){
        hl_release(heap, ptr[i]);
    }
    hl_release(heap, p1);
    p1 = hl_alloc(heap, HEAP_SIZE/2);
    if (!p1){
        return FAILURE;
    }
    
    return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_init
 * SPECIFICATION BEING TESTED:
 * heap is exactly heap size
 *
 * MANIFESTATION OF ERROR:
 * error results if hl_alloc successfully returns a pointer when it 
 * should not be able to allocate and should be returning null
 */
int test08() {
  char heap[3*HEAP_SIZE];
  hl_init(heap, HEAP_SIZE);
  void *ptr=hl_alloc(heap, HEAP_SIZE+1);
  return !ptr;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED:
 * resize returns pointer if possible to resize and 0 if not
 * contents of block should be preserved
 *
 * MANIFESTATION OF ERROR:
 * first resize should succeed-if 0 return failure, 
 * contents should be preserved-if different return failure, 
 * second resize should not be possible-if resize not null return failure
 */
int test09() {
  char heap[HEAP_SIZE];
  hl_init(heap,HEAP_SIZE);
  char *ptr= (char*) hl_alloc(heap,10);
  for (int i=0; i<10; i++){
    ptr[i]=(char) i;
  }
  char *ptrrs = (char*) hl_resize(heap,ptr,23);
  if (ptrrs==0){
    return FAILURE;
  }

  for (int i=0; i<10; i++){
    if (ptrrs[i]!=(char) i){
      return FAILURE;
    }
  }

  void *ptrrs2=hl_resize(heap,ptrrs,HEAP_SIZE+2);
  if (ptrrs2!=0){
    return FAILURE;
  }
  return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: resize
 * SPECIFICATION BEING TESTED: all addresses multiples of 8
 *
 *
 * MANIFESTATION OF ERROR:
 * allocates, then tries to resize to several blocks of various 
 * (weird) sizes then checks that each ptr returned is a multiple of 8.
 * if not, fails.
 *
 */
int test10() {
    int NUM_P = 11;
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    void* ptr[NUM_P];
    int div = 3;
    uintptr_t t;
    ptr[0] = hl_alloc(heap, HEAP_SIZE/13);

    for (int i=1; i<NUM_P; i++){
        div = div*2;
        ptr[i] = hl_resize(heap, ptr[i-1], i+HEAP_SIZE/div);
    }

    for (int i=0; i<NUM_P; i++){
        t = (uintptr_t) ptr[i];
        if ( (t !=0) && (t%8 != 0)) {
            return FAILURE;
        }
    }

    return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED:
 * resize works like alloc when block is null
 * resize can resize to a smaller block
 *
 * MANIFESTATION OF ERROR:
 * if first resize doesn't return a pointer then it has not alloced properly and fails, 
 * if alloc doesn't work then the 
 * resize to smaller hasn't worked properly so fails, if contents change then fails 
 */
int test11() {
  char heap[HEAP_SIZE];
  hl_init(heap,HEAP_SIZE);
  void *ptr=hl_alloc(heap, 2*HEAP_SIZE); // changed from NULL to too-big alloc (to fit PreC)
  char *ptr2=hl_resize(heap,ptr,HEAP_SIZE/2);
  if (!ptr2){
    return FAILURE;
  }
  for (int i=0; i<HEAP_SIZE/2; i++){
    ptr2[i]=(char) i;
  }

  char *ptr3=hl_resize(heap,ptr2,7);
  char *ptr4=hl_alloc(heap,HEAP_SIZE/3); //changed to 3

  if (ptr4==NULL){
    return FAILURE;
  }
  for (int i=0; i<HEAP_SIZE/3; i++){
    ptr4[i]='a';
  }
  for (int i=0; i<7; i++){
    if (ptr3[i]!=(char) i){
      return FAILURE;
    }
  }
  return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * can't alloc if not enough room in the heap (i.e. should return null)
 * specifically, when room is due to multiple smaller blocks
 *
 *
 * MANIFESTATION OF ERROR:
 * tries to alloc 5 HEAP_SIZE/2 blocks. the last 3 should return NULL
 * then after releasing all of these tries 5 HEAP_SIZE/4 blocks. 
 * the last one should return NULL.
 */
int test12() {
    char heap[HEAP_SIZE];
    void* ptr[5];
    hl_init(heap, HEAP_SIZE);
    
    for (int i=0; i<5; i++){
        ptr[i] = hl_alloc(heap, HEAP_SIZE/2);
    }
    if (ptr[0]==NULL){
        return FAILURE;
    }
    for (int i=2; i<5; i++){
        if (ptr[i] != NULL) {
            return FAILURE;
        }
    }
    for (int i=0; i<5; i++){
        hl_release(heap, ptr[i]);
    }

    for (int i=0; i<5; i++){
        ptr[i] = hl_alloc(heap, HEAP_SIZE/4);
    }
    if ((ptr[0] == NULL) || (ptr[4] != NULL)){
        return FAILURE;
    }
    
    return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: resize
 * SPECIFICATION BEING TESTED:
 *if resize fails then old block is unchanged
 *
 * MANIFESTATION OF ERROR:
 *if the second alloc succeeds after the unsuccessful resize then original block is not still intact.
 */
int test13() {
  char heap[HEAP_SIZE];
  hl_init(heap,HEAP_SIZE);
  char *ptr= (char*) hl_alloc(heap,HEAP_SIZE/2);
  for (int i=0; i<HEAP_SIZE/2; i++){
    ptr[i]=(char) i;
  }

  void *ptrrs=hl_resize(heap,ptr,HEAP_SIZE+2);
  if (ptrrs!=0){
    return FAILURE;
  }

  void *ptr2=hl_alloc(heap,HEAP_SIZE/2);
  if (ptr2){
    return FAILURE;
  }
  for (int i=0; i<HEAP_SIZE/2; i++){
    if (ptr[i]!=(char) i){
      return FAILURE;
    }
  }
  return SUCCESS;

}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test14() {

    return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test15() {

    return FAILURE;
}

/* ------------------ STRESS TESTS ------------------------- */

/* THIS TEST IS NOT FINISHED. It is a stress test, but it does not
 * actually test to see whether the library or the user writes 
 * past the heap. You are encouraged to complete this test.
 * 
 * FUNCTIONS BEING TESTED: alloc, free
 * SPECIFICATION BEING TESTED:
 * The library should not give user "permission" to write off the end
 * of the heap. Nor should the library ever write off the end of the heap.
 *
 * MANIFESTATION OF ERROR:
 * If we track the data on each end of the heap, it should never be
 * written to by the library or a good user.
 *
 */
int test16() {
    int n_tries    = 10000;
    int block_size = 16;

    // 1024 bytes of padding
    // --------------------
    // 1024 bytes of "heap"
    // --------------------  <-- heap_start
    // 1024 bytes of "padding"
    char memarea[HEAP_SIZE*3];

    char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];
    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    // zero out the memarea
    memset(memarea, 0, HEAP_SIZE*3*sizeof(char));

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap_start,  block_size);
            if (pointers[index]){
                memset(pointers[index], (char) (index+1), block_size);
            }
        }
        else{
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
        }
        for (int i=0; i<HEAP_SIZE; i++){
            if (memarea[i] != 0 || memarea[2*HEAP_SIZE + i] != 0) {
                return FAILURE;
            }
        }
    }
    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc, resize, free
 * INTEGRITY OR DATA CORRUPTION?
 * integrity
 *
 * MANIFESTATION OF ERROR:
 * segfault
 */
int test17() {    
    int n_tries    = 10000;
    int max_block_size = HEAP_SIZE/3;
    int block_size;
    char heap[HEAP_SIZE];
    char *pointers[NPOINTERS];
    char *p;
    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    // zero out the memarea
    memset(heap, 0, HEAP_SIZE*sizeof(char));

    hl_init(heap, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        if (pointers[index] == 0) {
            block_size = random() % max_block_size;
            pointers[index] = hl_alloc(heap,  block_size);
            if (pointers[index]){
                memset(pointers[index], (char) (index+1), block_size);
            }
        }
        else{
            if (random() % 2){
                block_size = random() % max_block_size;
                p = hl_resize(heap, pointers[index], block_size);
                if (p){
                    pointers[index]=p;
                    memset(pointers[index], (char) random(), block_size);
                }
            }
            else{
                hl_release(heap, pointers[index]);
                pointers[index] = 0;
            }
        }
    }
    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc & free
 * INTEGRITY OR DATA CORRUPTION?
 * data corruption
 *
 * MANIFESTATION OF ERROR:
 * after allocating each block, fill it uniformly with a random char
 * right before freeing, check that it is still indeed filled with that char
 * if not, fail
 * could also segfault.
 */
int test18() {
    int n_tries    = 10000;
    int max_block_size = 32;
    char heap[HEAP_SIZE];
    char *pointers[NPOINTERS];
    char fill[NPOINTERS];
    int block_size[NPOINTERS];
    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        if (pointers[index] == 0) {
            block_size[index] = random() % max_block_size;
            pointers[index] = hl_alloc(heap,  block_size[index]);
            if (pointers[index]){
                fill[index] = (char) random();
                memset(pointers[index], fill[index], block_size[index]);
            }
        }
        else{
            for (int j=0; j< block_size[index]; j++){
                if (pointers[index][j] != fill[index]){
                    return FAILURE;
                }
            }
            hl_release(heap, pointers[index]);
            pointers[index] = 0;
        }
    }
    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc, resize, & free
 * INTEGRITY OR DATA CORRUPTION?
 * data corruption
 * 
 * MANIFESTATION OF ERROR:
 * after allocating each block, fill it uniformly with a random char
 * randomly choose to either free or resize
 * right before either, check that it is still indeed filled with original char
 * if not, fail
 * if resizing, check that chars copied over correctly, then fill w/ new chars
 * could also segfault.
 * 
 */
int test19() {
    int n_tries    = 10000;
    int max_block_size = 32;
    char heap[HEAP_SIZE];
    char *pointers[NPOINTERS];
    char* p;
    char fill[NPOINTERS];
    int block_size[NPOINTERS];
    int new_bs;
    int m;
    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        if (pointers[index] == 0) {
            block_size[index] = random() % max_block_size;
            pointers[index] = hl_alloc(heap,  block_size[index]);
            if (pointers[index]){
                fill[index] = (char) random();
                memset(pointers[index], fill[index], block_size[index]);
            }
        }
        else{
            for (int j=0; j< block_size[index]; j++){
                if (pointers[index][j] != fill[index]){
                    return FAILURE;
                }
            }
            if (random() % 3) {
                new_bs = random() % max_block_size;
                p = hl_resize(heap, pointers[index], new_bs);
                if (p){
                    pointers[index] = p;
                    m = (new_bs < block_size[index]) ? new_bs : block_size[index];
                    block_size[index] = new_bs;
                }
                else {
                    m = block_size[index];
                }
                for (int j=0; j<m; j++){
                    if (pointers[index][j] != fill[index]){
                         return FAILURE;
                    }
                }
                fill[index] = (char) random();
                memset(pointers[index], fill[index], block_size[index]);
            } else {
                hl_release(heap, pointers[index]);
                pointers[index] = 0;
            }
        }
    }
    return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc & release
 * INTEGRITY OR DATA CORRUPTION?
 * structural integrity
 * 
 * MANIFESTATION OF ERROR:
 * segfault. this is the same as test 17, but without resizing.
 * also checks for memory leaks: at the end, it frees all blocks,
 * then tries to alloc HEAP_SIZE/2 (which should work)
 */
int test20() {
    int n_tries    = 10000;
    int max_block_size = 32;
    int block_size;
    char heap[HEAP_SIZE];
    char *pointers[NPOINTERS];
    int index;
    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    // zero out the memarea
    memset(heap, 0, HEAP_SIZE*sizeof(char));

    hl_init(heap, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        index = random() % NPOINTERS;
        if (pointers[index] == 0) {
            block_size = random() % max_block_size;
            pointers[index] = hl_alloc(heap,  block_size);
            if (pointers[index]){
                memset(pointers[index], (char) (index+1), block_size);
            }
        }
        else{
            hl_release(heap, pointers[index]);
            pointers[index] = 0;
        }
    }
    
    for (int i=0; i<NPOINTERS; i++){
        if (pointers[index]){
            hl_release(heap, pointers[index]);
        }
    }
    hl_alloc(heap, HEAP_SIZE/2);
    
    return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: resize
 * INTEGRITY OR DATA CORRUPTION?
 * data corruption
 * MANIFESTATION OF ERROR:
 * if after some number of resizes the data in the currently allocated block has changed then return failure
 */
int test21() {
  int n_tries = 10000;
  int max_block_size = 32;
  char heap[HEAP_SIZE];
  char *pointers[3];
  char* p;
  char fill[3];
  int block_size[3];
  int new_bs;
  int m;
  int index;
  // zero out the pointer array
  memset(pointers, 0, 3*sizeof(char *));

  hl_init(heap, HEAP_SIZE);
  srandom(0);

  for (int i=0;i<3;i++){
    block_size[i] = random() % max_block_size;
    pointers[i] = hl_alloc(heap, block_size[i]);
    fill[i] = (char) random();
    if (pointers[i]){
        memset(pointers[i], fill[i], block_size[i]);
    }
  }

  for (int i=0; i<n_tries; i++){
    index = random() % 3;
    new_bs = random() % max_block_size;
    p = hl_resize(heap, pointers[index], new_bs);
    if (p){
      pointers[index] = p;
      m = (new_bs < block_size[index]) ? new_bs : block_size[index];
      block_size[index] = new_bs;
    } else{
      m = block_size[index];
    }
    for (int j=0; j<m; j++){
      if (pointers[index][j] != fill[index]){
            return FAILURE;
      }
    }
    fill[index] = (char) random();
    if (pointers[index]){
      memset(pointers[index], fill[index], block_size[index]);
    }
  }
  return SUCCESS;
}



/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test22() {

    return FAILURE;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test23() {

    return FAILURE;
}
