/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

#define DEBUG
#undef DEBUG

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "none",
    /* First member's full name */
    "none",
    /* First member's email address */
    "none",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};


#define WSIZE				4
#define DSIZE				8
#define CHUNKSIZE			(1 << 12)

#define MAX(x, y)			((x) > (y)? (x) : (y))

#define PACK(size, alloc)	((size) | (alloc))

#define GET(p)				(*(unsigned int *)(p))
#define PUT(p, val)			(*(unsigned int *)(p) = (val))

#define GET_SIZE(p)			(GET(p) & ~0x7)
#define GET_ALLOC(p)		(GET(p) &  0x1)

#define HDRP(bp)			((char *)(bp) - WSIZE)
#define FTRP(bp)			((char *)(bp) + GET_SIZE((HDRP(bp))) - DSIZE)

#define NEXT_BLKP(bp)		((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)		((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))




static char *heap_listp;

static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);
static void *combine(void *bp, int csize);
static void mm_check(char *msg);
static void *mm_memcpy(void *str1, void *str2, size_t n);


static void *extend_heap(size_t words)
{
	char *bp;
	size_t size;

	size = words + (words & 1);
	size = size * WSIZE;
	if ((long)(bp = mem_sbrk(size)) == -1)
		return NULL;

	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

	return coalesce(bp);
}

static void *coalesce(void *bp)
{
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if (prev_alloc && next_alloc)
	{
		return bp;
	}
	else if (prev_alloc && !next_alloc)
	{
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}
	else if (!prev_alloc && next_alloc)
	{
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}
	else {
		size += GET_SIZE(HDRP(PREV_BLKP(bp)))
			+ GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}

	return bp;
}

static void *combine(void *bp, int csize)
{
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if (prev_alloc && next_alloc)
	{
		if (size < csize) return NULL;
		return bp;
	}
	else if (prev_alloc && !next_alloc)
	{
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		if (size < csize) return NULL;
		PUT(HDRP(bp), PACK(size, 1));
		PUT(FTRP(bp), PACK(size, 1));
	}
	else if (!prev_alloc && next_alloc)
	{
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		if (size < csize) return NULL;
		PUT(FTRP(bp), PACK(size, 1));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 1));
		bp = PREV_BLKP(bp);
	}
	else {
		size += GET_SIZE(HDRP(PREV_BLKP(bp)))
			+ GET_SIZE(HDRP(NEXT_BLKP(bp)));
		if (size < csize) return NULL;
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 1));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 1));
		bp = PREV_BLKP(bp);
	}

	return bp;
}

static void *find_fit(size_t asize)
{
	void *bp;

	for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
	{
		if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp))))
			return bp;
	}
	return NULL;
}

static void place(void *bp, size_t asize)
{
	size_t csize = GET_SIZE(HDRP(bp));

	if ((csize - asize) >= (2 * DSIZE))
	{
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		bp = NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(csize - asize, 0));
		PUT(FTRP(bp), PACK(csize - asize, 0));
	}
	else
	{
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
	}
}

static void mm_check(char *msg)
{
	printf("%s\n", msg);
	for (char *bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
	{
		printf("%08x %d %d\n", HDRP(bp), GET_SIZE(HDRP(bp)), GET_ALLOC(HDRP(bp)));
	}
}

static void *mm_memcpy(void *str1, void *str2, size_t n)
{
	for (int i = 0; i < n; ++i)
		*((char *)str1 + i) = *((char *)str2 + i);
	return str1;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
		return -1;

	PUT(heap_listp, 0);
	PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp + (3 * WSIZE), PACK(0, 1));
	heap_listp += (2 * WSIZE);

	if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
		return -1;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
	char ch[100];
	sprintf(ch, "----------\nmalloc %d", size);

	size_t asize;
	size_t extendsize;
	char *bp;

	if (size == 0)
		return NULL;

	if (size <= DSIZE)
		asize = 2 * DSIZE;
	else
		asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

	if ((bp = find_fit(asize)) != NULL)
	{
		place(bp, asize);

#ifdef DEBUG
		mm_check(ch);
#endif // DEBUG

		return bp;
	}

	extendsize = MAX(asize, CHUNKSIZE);
	if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
		return NULL;
	place(bp, asize);

#ifdef DEBUG
	mm_check(ch);
#endif // DEBUG

	return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
	char ch[100];
	sprintf(ch, "----------\nfree %08x", HDRP(bp));

	size_t size = GET_SIZE(HDRP(bp));

	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));

	coalesce(bp);

#ifdef DEBUG
	mm_check(ch);
#endif // DEBUG
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *bp, size_t size)
{
	char ch[100];
	sprintf(ch, "----------\nrealloc %08x %d", HDRP(bp), size);

	size_t asize;

	if (bp == NULL)
	{
#ifdef DEBUG
		mm_check(ch);
#endif // DEBUG
		return mm_malloc(size);
	}

	if (size == 0)
	{
		mm_free(bp);
#ifdef DEBUG
		mm_check(ch);
#endif // DEBUG
		return bp;
	}

	if (size <= DSIZE)
		asize = 2 * DSIZE;
	else
		asize = DSIZE * ((size + (DSIZE)+(DSIZE - 1)) / DSIZE);

	if (asize <= GET_SIZE(HDRP(bp)))
	{
		place(bp, asize);
#ifdef DEBUG
		mm_check(ch);
#endif // DEBUG
		return bp;
	}
	else
	{

		int csize = GET_SIZE(HDRP(bp)) - DSIZE;
		char *pre = combine(bp, asize);

		if (pre == NULL)
		{
			char *nnode = mm_malloc(size);
			mm_memcpy(nnode, bp, csize);
			mm_free(bp);
#ifdef DEBUG
			mm_check(ch);
#endif // DEBUG
			return nnode;
		}
		else if (pre != bp)
		{
			mm_memcpy(pre, bp, csize);
			place(pre, asize);
#ifdef DEBUG
			mm_check(ch);
#endif // DEBUG
			return pre;
		}
		else
		{
			place(bp, asize);
#ifdef DEBUG
			mm_check(ch);
#endif // DEBUG
			return bp;
		}

	}
	return NULL;
}
