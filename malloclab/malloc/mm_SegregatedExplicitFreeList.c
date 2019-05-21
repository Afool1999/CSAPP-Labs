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
#define PUT(p, val)			(*(unsigned int *)(p) = (unsigned int)(val))
#define GET_SIZE(p)			(GET(p) & ~0x7)
#define GET_ALLOC(p)		(GET(p) &  0x1)
#define HDRP(bp)			((char *)(bp) - WSIZE)
#define FTRP(bp)			((char *)(bp) + GET_SIZE((HDRP(bp))) - DSIZE)
#define SIZE(bp)			( GET_SIZE(HDRP(bp)))
#define ALLOC(bp)			(GET_ALLOC(HDRP(bp)))
#define NEXT_BLKP(bp)		((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)		((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))


typedef struct Node
{
	unsigned int rank;
	int par;
	int lch;
	int rch;
}node;

static char *heap_listp;
static char *heap_start;



static unsigned int mrand();
static unsigned int get_size(node *rt);
static void clear_node(node *rt);
static void new_node(node *bp);

static int lturn(int _rt);
static int rturn(int _rt);
static int insert_node(int _rt, int _chd);
static int find_node(int _rt, size_t size);
static int delete_node(int _rt);


static void *get_root(size_t size);
static void push_list(void *bp);
static void erase_list(void *bp);
static void *extend_heap(size_t words);
static void *coalesce(void *bp);

static int find_place(size_t asize);
int mm_init(void);
void *mm_malloc(size_t size);



static void *mm_memcpy(void *str1, void *str2, size_t n)
{
	for (int i = 0; i < n; ++i)
		*((char *)str1 + i) = *((char *)str2 + i);
	return str1;
}

void dfs(int _rt)
{
	node *rt = (node *)_rt;
	if (_rt == 0)return;
	if (rt->lch)dfs(rt->lch);
	printf("%d\n", SIZE(_rt));
	if (rt->rch)dfs(rt->rch);
}

static void mm_check(char *msg)
{
	for (int i = 0; i < 9; ++i)
	{
		void *p = heap_start + i * WSIZE;
		dfs(GET(p));
	}
	printf("%s\n", msg);
}



static unsigned int mrand()
{
	static unsigned int seed = 12345;
	seed = seed * 482711UL % 4294967295UL;
	return seed;
}

static unsigned int get_size(node *rt)
{
	return SIZE((void *)rt);
}

static void new_node(node *bp)
{
	bp->rank = mrand();
	bp->par = bp->lch = bp->rch = 0;
}

static void clear_node(node *rt)
{
	rt->rank = 0;
	rt->par = rt->lch = rt->rch = 0;
}



//	rt的左子树或右子树必定存在
static int lturn(int _rt)
{
	node *rt = (node *)_rt;
	int _rchd = rt->rch;
	node *rchd = (node *)_rchd;

	rt->rch = rchd->lch;
	rchd->lch = _rt;

	if (rt->rch != 0)
		((node *)(rt->rch))->par = _rt;
	rchd->par = rt->par;
	rt->par = _rchd;
	if (rchd->par != 0)
	{
		node *par = (node *)(rchd->par);
		if (par->lch == _rt)
			par->lch = _rchd;
		else if (par->rch == _rt)
			par->rch = _rchd;
	}

	return _rt = _rchd;
}

static int rturn(int _rt)
{
	node *rt = (node *)_rt;
	int _lchd = rt->lch;
	node *lchd = (node *)_lchd;

	rt->lch = lchd->rch;
	lchd->rch = _rt;

	if (rt->lch != 0)
		((node *)(rt->lch))->par = _rt;
	lchd->par = rt->par;
	rt->par = _lchd;
	if (lchd->par != 0)
	{
		node *par = (node *)(lchd->par);
		if (par->lch == _rt)
			par->lch = _lchd;
		else if (par->rch == _rt)
			par->rch = _lchd;
	}

	return _rt = _lchd;
}

static int insert_node(int _rt, int _chd)
{
	if (_rt == 0)
	{
		_rt = _chd;
		return _rt;
	}

	node *rt = (node *)_rt;
	node *chd = (node *)_chd;

	if (get_size(rt) >= get_size(chd))
	{
		rt->lch = insert_node(rt->lch, _chd);
		node *suc = (node *)(rt->lch);
		suc->par = _rt;
		if (rt->rank > suc->rank)
			_rt = rturn(_rt);
	}
	else {
		rt->rch = insert_node(rt->rch, _chd);
		node *suc = (node *)(rt->rch);
		suc->par = _rt;
		if (rt->rank > suc->rank)
			_rt = lturn(_rt);
	}

	return _rt;
}

static int find_node(int _rt, size_t size)
{
	node *rt = (node *)_rt;
	if (_rt == 0) return 0;
	if (get_size(rt) == size)
		return _rt;

	int res = 0;

	if (get_size(rt) >= size)
	{
		res = find_node(rt->lch, size);
		if (res == 0)
			res = _rt;
	}
	else res = find_node(rt->rch, size);

	return res;
}

static int delete_node(int _rt)
{
	node *rt = (node *)_rt;
	if (_rt == 0)
		return 0;

	int _lchd = rt->lch;
	int _rchd = rt->rch;
	node *lchd = (node *)(_lchd);
	node *rchd = (node *)(_rchd);

	if ((lchd == NULL) || (rchd == NULL))
	{
		if (lchd == NULL)
		{
			if (rchd != NULL)
				rchd->par = rt->par;
			_rt = _rchd;
		}
		else {
			lchd->par = rt->par;
			_rt = _lchd;
		}
	}
	else {
		if (lchd->rank < rchd->rank)
		{
			_rt = rturn(_rt);
			node *temp = (node *)_rt;
			temp->rch = delete_node(temp->rch);
		}
		else {
			_rt = lturn(_rt);
			node *temp = (node *)_rt;
			temp->lch = delete_node(temp->lch);
		}
	}

	return _rt;
}




//	following is the realizing of malloc



static void *get_root(size_t size)
{
	int i = 0, lim = 32;
	while ((size > lim) && i < 8)
	{
		i++;
		lim <<= 1;
	}
	return (void *)(heap_start + i * WSIZE);
}

static void push_list(void *bp)
{
	
#ifdef DEBUG
	char ch[100];
	printf("\n=====push_list(%d)=====\n", SIZE(bp));
#endif
	

	if (bp == NULL)
		return;

	void *cat = get_root(SIZE(bp));
	int ptr = GET(cat);
	int chd = (int)(long long)bp;

	new_node((node *)chd);
	ptr = insert_node(ptr, chd);
	int root = ptr;
	PUT(cat, root);

	sprintf(ch, "-----push_list-----\n");
#ifdef DEBUG
	mm_check(ch);
#endif // DEBUG

	//	original root's parent is NULL
}

static void erase_list(void *bp)
{
	
#ifdef DEBUG
	char ch[100];
	printf("\n=====erase_list(%d)=====\n", SIZE(bp));
#endif
	

	if (bp == NULL)
		return;

	void *cat = get_root(SIZE(bp));
	int ptr = GET(cat);

	if (ptr == 0)
		return;

	int rt = (unsigned int)(long long)bp;
	int temp = rt;
	int tpar = ((node *)rt)->par;
	
	
	rt = delete_node(rt);
	clear_node(temp);


	
	if (ptr == temp)
	{
		PUT(cat, (unsigned int)(long long)rt);
	}
	else {
		if (tpar != 0)
		{
			node *par = (node *)tpar;
			if (par->lch == temp)
			{
				par->lch = rt;
				if (rt != 0)
					((node *)rt)->par = tpar;
			}
			else if (par->rch == temp)
			{
				par->rch = rt;
				if (rt != 0)
					((node *)rt)->par = tpar;
			}
		}
	}

	
#ifdef DEBUG
	sprintf(ch, "-----erase_list-----\n");
	mm_check(ch);
#endif // DEBUG

}

int mm_init(void)
{
	
#ifdef DEBUG
	char ch[100];
	printf("\n=====mm_init=====\n");
#endif
	

	if ((heap_start = mem_sbrk(12 * WSIZE)) == (void *)-1)
		return -1;

	//	generate free list links, stores pointer to treap's root
	for (int i = 0; i < 9; ++i)
		PUT(heap_start + i * WSIZE, 0);
	/*
		i = 0  size <= 32
		...
		i = 7 size <= 4096
		i = 8 size >  4096
		
	*/

	PUT(heap_start + 9  * WSIZE, PACK(DSIZE, 1));
	PUT(heap_start + 10 * WSIZE, PACK(DSIZE, 1));
	PUT(heap_start + 11 * WSIZE, PACK(0, 1));

	heap_listp = heap_start + 10 * WSIZE;

	
#ifdef DEBUG
	sprintf(ch, "-----mm_init-----\n");
	mm_check(ch);
#endif

	if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
		return -1;
	return 0;
}

static void *extend_heap(size_t words)
{
	
#ifdef DEBUG
	char ch[100];
	printf("\n=====extend_heap(%d)=====\n", words);
#endif // DEBUG

	

	char *bp;
	size_t size;

	size = words + (words & 1);
	size = size * WSIZE;
	if ((long)(bp = mem_sbrk(size)) == -1)
		return NULL;

	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));



	push_list(bp);

	
#ifdef DEBUG
	sprintf(ch, "-----extend_heap-----\n");
	mm_check(ch);
#endif // DEBUG

	return coalesce(bp);
}

static void *coalesce(void *bp)
{
	
#ifdef DEBUG
	char ch[100];
	printf("\n=====coalesce(%d)=====\n", SIZE(bp));
#endif
	

	size_t prev_alloc = ALLOC(PREV_BLKP(bp));
	size_t next_alloc = ALLOC(NEXT_BLKP(bp));
	size_t size = SIZE(bp);
		
	if (prev_alloc && next_alloc)
	{
		return bp;
	}
	else if (prev_alloc && !next_alloc)
	{
		erase_list(NEXT_BLKP(bp));
		erase_list(bp);

		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));

		push_list(bp);
	}
	else if (!prev_alloc && next_alloc)
	{
		erase_list(PREV_BLKP(bp));
		erase_list(bp);

		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);

		push_list(bp);
	}
	else {
		erase_list(PREV_BLKP(bp));
		erase_list(NEXT_BLKP(bp));
		erase_list(bp);

		size += GET_SIZE(HDRP(PREV_BLKP(bp)))
			+ GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);

		push_list(bp);
	}

	
#ifdef DEBUG
	sprintf(ch, "-----coalesce-----\n");
	mm_check(ch);
#endif // DEBUG

	return bp;
}

static void *coalesce1(void *bp)
{
	
#ifdef DEBUG
	char ch[100];
	printf("\n=====coalesce(%d)=====\n", SIZE(bp));
#endif


	size_t prev_alloc = ALLOC(PREV_BLKP(bp));
	size_t next_alloc = ALLOC(NEXT_BLKP(bp));
	size_t size = SIZE(bp);

	if (prev_alloc && next_alloc)
	{
		return bp;
	}
	else if (prev_alloc && !next_alloc)
	{
		erase_list(NEXT_BLKP(bp));

		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 1));
		PUT(FTRP(bp), PACK(size, 1));

	}
	else if (!prev_alloc && next_alloc)
	{
		erase_list(PREV_BLKP(bp));

		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp), PACK(size, 1));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 1));
		bp = PREV_BLKP(bp);

	}
	else {
		erase_list(PREV_BLKP(bp));
		erase_list(NEXT_BLKP(bp));

		size += GET_SIZE(HDRP(PREV_BLKP(bp)))
			+ GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 1));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 1));
		bp = PREV_BLKP(bp);

	}


#ifdef DEBUG
	sprintf(ch, "-----coalesce-----\n");
	mm_check(ch);
#endif // DEBUG

	return bp;
}

static int find_place(size_t asize)
{
	int cat = 0;
	int size = asize;
	int i = 0, lim = 32;
	while (i < 8 && (size > lim || GET(heap_start + i * WSIZE) == 0))
	{
		i++;
		lim <<= 1;
	}
	if (GET(heap_start + i * WSIZE) == 0) ++i;
	if (i == 9) return 0;
	else cat = (unsigned int)(long long)(void *)(heap_start + i * WSIZE);
	int res = find_node(GET(cat), asize);

	return res;
}

static void place(int bp, size_t asize)
{
	
#ifdef DEBUG
	char ch[100];
	printf("\n=====place(%d)=====\n", asize);
#endif // DEBUG

	


	size_t csize = GET_SIZE(HDRP(bp));

	if ((csize - asize) >= (3 * DSIZE))
	{
		erase_list(bp);

		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		bp = NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(csize - asize, 0));
		PUT(FTRP(bp), PACK(csize - asize, 0));

		push_list(bp);
	}
	else
	{
		erase_list(bp);

		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
	}

	sprintf(ch, "-----place-----\n");
#ifdef DEBUG
	mm_check(ch);
#endif // DEBUG
}

void *mm_malloc(size_t size)
{
	
#ifdef DEBUG
	char ch[100];
	printf("\n=====mm_malloc(%d)=====\n", size);
#endif // DEBUG

	
	

	size_t asize;
	size_t extendsize;
	int bp;

	if (size == 0)
		return NULL;

	if (size <= 2 * DSIZE)
		asize = 3 * DSIZE;
	else
		asize = DSIZE * ((size + (DSIZE)+(DSIZE - 1)) / DSIZE);

	if ((bp = find_place(asize)) != 0)
	{
		place(bp, asize);
		return (void *)bp;
	}
	
	extendsize = MAX(asize, CHUNKSIZE);
	if ((bp = (int)(long long)extend_heap(extendsize / WSIZE)) == 0)
		return NULL;
	place(bp, asize);

	sprintf(ch, "-----mm_malloc-----\n");
#ifdef DEBUG
	mm_check(ch);
#endif DEBUG

	return (void *)bp;
}

void mm_free(void *ptr)
{
	
#ifdef DEBUG
	char ch[100];
	printf("\n=====free(%d)=====\n", SIZE(ptr));
#endif // DEBUG

	

	size_t size = GET_SIZE(HDRP(ptr));

	PUT(HDRP(ptr), PACK(size, 0));
	PUT(FTRP(ptr), PACK(size, 0));

	push_list(ptr);

	coalesce(ptr);

	sprintf(ch, "-----mm_free-----\n");
#ifdef DEBUG
	mm_check(ch);
#endif // DEBUG
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{

	size_t asize;

	if (ptr == NULL)
	{
		return mm_malloc(size);
	}

	if (size == 0)
	{
		mm_free(ptr);
		return ptr;
	}

	if (size <= 2 * DSIZE)
		asize = 3 * DSIZE;
	else
		asize = DSIZE * ((size + (DSIZE)+(DSIZE - 1)) / DSIZE);

	if (asize <= SIZE(ptr))
	{
		place(ptr, asize);
		return ptr;
	}
	else
	{

		int csize = SIZE(ptr) - DSIZE;
		void *p1 = coalesce1(ptr);
		int psize = SIZE(p1);
		if (psize >= asize)
		{
			if (p1 != ptr)
				mm_memcpy(p1, ptr, csize);
			place(p1, asize);
			return p1;
		}
		else {
			if (NEXT_BLKP(ptr) == NULL)
			{
				void *res = extend_heap((asize - psize)/WSIZE);
				size += SIZE(res);
				PUT(HDRP(p1), PACK(size, 1));
				PUT(FTRP(p1), PACK(size, 1));
				if (p1 != ptr)
					mm_memcpy(p1, ptr, csize);
			}
			void *res = mm_malloc(size);
			mm_memcpy(res, ptr, csize);
			mm_free(p1);
			return res;
		}

	}
	return NULL;
}