PK
    S°µNÇ-]¯"H  "H  ! * CSAPP : malloclab å®éªæ¥å.mdup& OýàCSAPP : malloclab å®éªæ¥å.md# CSAPP : malloclab å®éªæ¥å

å§åï¼æ½æ¥å³°

å­¦å·ï¼PB18000335


## å®éªä»ç»
--------------------
å¨è¿ä¸ªå®éªä¸­æä»¬éè¦ç¼åä¸ä¸ªå¨æåéåå­çå½æ°ï¼å®ç° `malloc` `free` `realloc` ç­åè½ï¼å¹¶ä¸åå°ç©ºé´å©ç¨çåæçä¹é´çå¹³è¡¡ã

## å®éªåå¤
--------------------

### éå¶åæ¡ä»¶

* mm_initï¼å¨ driver è°ç¨å¶å®è¿ç¨åå½æ°åï¼ä¼åè°ç¨ `mm_init` è¿è¡åå§åï¼ä¾å¦åéåå§ç©ºé´
* mm_mallocï¼å½æ°è¿åä¸ä¸ªè´è½½è³å°ä¸º `size` çå°åï¼å¹¶ä¸åéçååºå½å¨å çéåº¦åï¼å¹¶ä¸ä¸ä¼ä¸å«çåéå 
* mm_freeï¼å½æ°éæ¾ `ptr` å¤çç©ºé´ï¼å¹¶ä¸æ²¡æè¿åå¼
* mm_reallocï¼å½æ°è¿åä¸ä¸ªè³å° `size` å­èç©ºé´çå°åï¼å¹¶ä¸ç¬¦åä¸è¿°éå¶
â ptr ä¸ºç©ºç­æäº mm_malloc(size)
â size ä¸º 0 ç­æäº mm_free(ptr)
â ptr ä¸ä¸ºç©ºï¼å¹¶ä¸æ¯ mm_malloc æ mm_realloc è¿åçå°åï¼åæ¹å ptr æåçåçå¤§å°ï¼å¹¶ä¸è¿åæ°åçå°åãåºå½å¯¹å°åä¸­å­å¨çåå®¹è¿è¡å¤çï¼ä½¿å¾æ°åä¸­çåå®¹ä¸ååä¸è´ï¼åå°æªåï¼åå¤§ä¸ç¨ç®¡ï¼
* ä¸åæ¹å mm.c ä¸­çæ¥å£
* ä¸åè®¸ä½¿ç¨ä»»ä½æå³åå­ç®¡ççåºå½æ°æç³»ç»å½æ°ï¼å æ­¤ç¨åºä¸­ä½ ä¸è½ä½¿ç¨ `malloc` `calloc` `free` `realloc` `sbrk` `brk` ç­å½æ°
* ä½ ä¸è½å®ä¹ä»»ä½åæ°ç»ãç»æä½ãæ ãåè¡¨çå¨å±æéæçå¤åæ°æ®ç»æï¼ä½æ¯ä½ å¯ä»¥å®ä¹å¨å±æ éåéï¼æ¯å¦æ´å½¢ãæµ®ç¹æ°åæé
* ä¸ºäºä¸å«çåºä¿æä¸è´ï¼ä½ çåéå¨åºå½è¿åå«è¿å¶å¯¹é½çå°åï¼driver ä¼å¼ºå¶è¦æ±ä½ è¿ä¹å

### mm_check

ç¨äº debugï¼æ£æ¥å çç¸å®¹æ¡ä»¶ï¼æåªæ¯ç®åå°ææ´ä¸ªå è¾åºäºä¸ä¸ï¼msg ç±è°ç¨å½æ°æä¾ï¼è¾åºè°ç¨å½æ°ä¿¡æ¯ååæ°ã
```
	static void mm_check(char *msg)
	{
		printf("%s\n", msg);
		for (char *bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
		{
			printf("%08x %d %d\n", HDRP(bp), GET_SIZE(HDRP(bp)), GET_ALLOC(HDRP(bp)));
		}
	}
```

## å®éªè¿ç¨
--------------------

### æè·¯

ä¸ºäºè·åæ´é«çåæ°ï¼è¾¾å°é«æçåé«ç©ºé´å©ç¨çï¼å¯ä»¥èèä½¿ç¨æ¾ç¤ºç©ºé²åè¡¨ååç¦»ééçæ¹æ³ãèç®åçåç¦»éééè¦éåé¾è¡¨æ¾å°æä½³ééå¤æåº¦ä¸º $O(N)$ ï¼å¯ä»¥èèå©ç¨å¹³è¡¡æ ä¸­ç treap æ¥å¯¹é¾è¡¨è¿è¡ç»´æ¤ï¼å°å¤æåº¦ä¸éä¸º $O(logN)$ã

### éå¼ç©ºé²é¾è¡¨çå®ç°

ä»£ç åºæ¬åä¹¦æ¬ä¸ä¸è´ï¼éè¦èªå·±å®ç° realloc åè½ï¼ç±äºæ¯å¨æ¬å°è¿è¡æµè¯ï¼æä»¥æ´æ¹äºæ¥å£éçå½¢ååãç¨åºå¨æ²¡å mm_check ä¹åä¸ç´æ²¡è½éè¿æµè¯ï¼åä¸ç¥éå°åºåªä¸åå­å¨ bugï¼debug äºå¥½ä¹ï¼æååç° `memcpy` å½æ°ä¸­ï¼ä¸¤ä¸ª copy çæ°ç»ä¹é´æéå æ¶ï¼å¹¶ä¸è½è¾¾æå¤å¶çç®çï¼æåèªå·±åäºä¸ä¸ª `mm_memcpy` å½æ°ï¼å°æ°å¼ä¸ä¸ªä¸ä¸ªçè¿è¡å¤å¶ã

ä¸å¾æ¯éå¼ç©ºé²é¾è¡¨çæµè¯ç»æ

![1.jpg](https://raw.githubusercontent.com/Afool1999/CSAPP-Labs/master/malloclab/pic/1.jpg)

å¯¹ç©ºé´å©ç¨çè¾ä½çç¬¬ 7ã8ã9ã10 å·æ°æ®è§å¯ï¼ä¼åç°å¯ä»¥å¯¹æåä¸¤ä¸ªæ°æ®è¿è¡ä¸ä¸ªå°ä¼åï¼`place` çç­ç¥å¯ä»¥æ¹ä¸ºå°åä¸é¨åè®¾ä¸ºåéç©ºé´ï¼ä¸è¿æä¹å¥½åå¹¶ä¸å¤§ã

### æ¾å¼ç©ºé²é¾è¡¨+åç¦»éé+å¹³è¡¡æ 

åèèå¦ä½å¨æ°æ®åä¸­å­å¨æéè¦çä¿¡æ¯ï¼å¹¶è®¡ç®æå°åçå¤§å°ã
ç±äºæåå¹¶ç©ºé²åçæä½ï¼æä»¥éè¦å¤´é¨èé¨ä¿¡æ¯ï¼treap éè¦ç»´æ¤ `rank` å¼ä»¥åå·¦å³å¿å­ï¼ä¸ºäºæ¹ä¾¿åæç¶èç¹ä¿¡æ¯å­å¨ä¸æ¥ï¼æä»¥å«è¿å¶å¯¹é½æ¶æå°åå¤§å°åºä¸º 24 å­èå± 6 ä¸ªå­ã

åå°½åè¾ä¸è¦ï¼æ»ç®å°ç¨åºè°åºæ¥ä¹åï¼å´å°´å°¬çåç°åæ°éä½æ²¡ææåï¼åèä¸éäºã
![2.jpg](https://raw.githubusercontent.com/Afool1999/CSAPP-Labs/master/malloclab/pic/2.jpg)

ç©ºé´å©ç¨çä¸éåæ¯å¯ä»¥çè§£ï¼æ¯ç«æå°åçå¤§å°åæäº 6 ä¸ªå­ï¼ä½æçå¥½åä¹æ²¡æ¯åæ¥é«å¤å°ãä»ç»ä¸çåç°æ¶é´ä¸»è¦æµªè´¹å¨äºç¬¬ä¹ä¸ªç¹ä¸ï¼èå¶å®çç¹éåº¦åæ¯æ¯åæ¬åä¸å°ï¼äºæ¯éæ©å¯¹ç¬¬ä¹ä¸ªç¹è¿è¡éå¯¹æ§ä¼åãè§å¯æ°æ®åç°ç¨åºå¤§é¨åæ¶é´é½è±å¨äº realloc ä¸ï¼èå¶ä¸­åååå¤å¤å°è°ç¨  malloc ï¼æ ¹æ®å¯¹æ°æ®çè§å¯ï¼æä»¬åç°å¯¹äºæåä¸ä¸ªåå¯ä»¥ç´æ¥è¿è¡å»¶é¿ï¼äºæ¯ä¼åä¹åç¨åºè·åå°±ä¸å»äºãå¶æ¬¡ä¸å¼å§åéåçæ¹å¼ä¹æéè¯¯ï¼äºå®ä¸åå ä¸ªç¹æä½çç©ºé´å©ç¨çå¾ææ¾å°±æ¯ç¨åºèèä¸å¨çåå ï¼æ´æ­£ä¹åç©ºé´å©ç¨çå¾åä¹æè¾å¤§æåãä¸è¿è²ä¼¼è¿æ²¡äººå®¶ç½ä¸ä¸ç¨å¹³è¡¡æ æ¥çåé«ï¼å¯è½æ¯æç©ºé´ä½¿ç¨çå§¿å¿ä¸å¯¹å§ã

![3.jpg](https://raw.githubusercontent.com/Afool1999/CSAPP-Labs/master/malloclab/pic/3.jpg)


#### ä»£ç åæ

å®åä¹¦ä¸åºæ¬ä¿æä¸è´ã

```
	#define DEBUG
	#undef DEBUG
	
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
```

ä¸ºäºæ¹ä¾¿å¹³è¡¡æ æä½ï¼ç¨äºä¸ä¸ªç»æä½ï¼ä¸å¼å§çæ¶åç´æ¥ç¨äºæéåéï¼ä½æ¯å¯ä»¥åªç¨ int æ¥å­å¨å°åï¼è¿æ ·å¯ä»¥çä¸ä¸å°ç©ºé´ã
```
	typedef struct Node
	{
		unsigned int rank;
		int par;
		int lch;
		int rch;
	}node;
```
å¹³è¡¡æ çç¸å³å®ç°ï¼ç±äº c å¹¶ä¸æ¯æå¼ç¨ï¼æä»¥å¾å¤å½æ°ç¨äºè¿åå¼åå°å¼ç¨çææã
```
	//  éæºæ°çæ
	static unsigned int mrand()
	{
		static unsigned int seed = 12345;
		seed = seed * 482711UL % 4294967295UL;
		return seed;
	}
	//  è·åèç¹çsize
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
	
	
	
	//	rtçå·¦å­æ æå³å­æ å¿å®å­å¨
	
	//  å·¦æ
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
	//  å³æ
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
	//  æå¥èç¹
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
	//  å¨ä»¥_rtä¸ºæ ¹çæ ä¸­æ¾å°å¤§å°ä¸ºsizeçèç¹ï¼æ²¡æè¿å 0
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
	//  å é¤èç¹
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
```
å³äº malloc é¨åçå®ç°ã
```
	//  æ¾å°å¤§å°ä¸ºsizeçèç¹æå±çç±»å«
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
	//  æå¥èç¹
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
	//  å é¤èç¹
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
	//  åå§å
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
	//  æå±å çå¤§å°
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
	//  åå¹¶å
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
	//  åå¹¶åå¹¶æ è®°ä¸ºå·²å ç¨
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
	//  æ¾å¯è½å­å¨æªåéåéå¤§å°åçä½ç½®
	static int find_place(int i, size_t asize)
	{
		int cat = 0;
		int size = asize;
		int lim = 1 << (i+5);
		while (i < 8 && (size > lim || GET(heap_start + i * WSIZE) == 0))
		{
			i++;
			lim <<= 1;
		}
		if (GET(heap_start + i * WSIZE) == 0) ++i;
		return i;
	}
	//  åå²
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
	
	#ifdef DEBUG
		sprintf(ch, "-----place-----\n");
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
		int bp=0;
	
		if (size == 0)
			return NULL;
	
		if (size <= 2 * DSIZE)
			asize = 3 * DSIZE;
		else
			asize = DSIZE * ((size + (DSIZE)+(DSIZE - 1)) / DSIZE);
	
		int i = 0;
		while ((i = find_place(i, asize)) < 9)
		{
			void *cat = (unsigned int)(long long)(heap_start + i * WSIZE);
			int res = find_node(GET(cat), asize);
			if (res != 0)
			{
				bp = res;
				break;
			}
			++i;
		}
	
		if (bp != 0)
		{
			place(bp, asize);
			return (void *)bp;
		}
		
		extendsize = MAX(asize, CHUNKSIZE);
		if ((bp = (int)(long long)extend_heap(extendsize / WSIZE)) == 0)
			return NULL;
		place(bp, asize);
	
		
	#ifdef DEBUG
		sprintf(ch, "-----mm_malloc-----\n");
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
	
		
	#ifdef DEBUG
	  sprintf(ch, "-----mm_free-----\n");
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
```

## å®éªæ»ç»
------------------
è¿ä¸ªå®éªçä»£ç ä»ä¸æä¸ç´æ¸å°äºäºæï¼ä¸æ¹é¢ç¨åºæ»æ¯æ¨åéæ¥ä¸æ¯è¿éæ²¡æ³å¯¹ï¼å°±æ¯é£éåééäºï¼æé´è¿æä¸ªæ¯è¾å°æ°çé®é¢å°±æ¯å¦ä½ææéç±»åè½¬å­ä¸ºæ´åï¼ä¸è¿å¥½å¨ä¹ä¸é¾è§£å³ï¼è¿ä¹ä¸­åç©¿æäºæä¸­èï¼è½å®æå®éªç®ç´å°±æ¯ä¸å¹¸äºååãè¿æ¬¡å®éªè½ç¶ä½¿ç¨äºå¹³è¡¡æ çåæ³ï¼ä½è¿æ¯æ²¡æè¾¾å°æé«çåæ°ï¼åå å¨äºå¯¹äºä¸­é´çæ°æ®ï¼ç©ºé´ä½¿ç¨çå¹¶ä¸é«ï¼å¯ä»¥éå¯¹ç¹å®çæ°æ®åè¿è¡ä¼åï¼ä¸è¿é´äºåä»£ç å è°ç¨èè´¹äºè¿ä¹å¤çæ¶é´ç²¾åï¼å©ä¸çä¹æ²¡å¥å¿æåäºãä¸è¿è½è¯´è±çæ¶é´å¾é¿ï¼ä½ä¹è½å¤å¾å¥½çå·©åºå¨æåéåå­çç¥è¯ï¼ä¹æ¯å¾å¼å¾çï¼

## å¶å®
------------------

æ¬å®éªææèµæºå¨ [CSAPP-Labs/malloclab at master Â· Afool1999/CSAPP-Labs Â· GitHub](https://github.com/Afool1999/CSAPP-Labs/tree/master/malloclab)PK 
    S°µNÇ-]¯"H  "H  ! *               CSAPP : malloclab å®éªæ¥å.mdup& OýàCSAPP : malloclab å®éªæ¥å.mdPK      y   H    