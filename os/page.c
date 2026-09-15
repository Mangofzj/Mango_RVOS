#include "kernel.h"

/* 以下全局变量定义在 mem.S 中 */
extern ptr_t TEXT_START;
extern ptr_t TEXT_END;
extern ptr_t DATA_START;
extern ptr_t DATA_END;
extern ptr_t RODATA_START;
extern ptr_t RODATA_END;
extern ptr_t BSS_START;
extern ptr_t BSS_END;
extern ptr_t HEAP_START;
extern ptr_t HEAP_SIZE;

static ptr_t    alloc_start = 0;  /* 堆池的实际起始地址 */
static ptr_t    alloc_end   = 0;  /* 堆池的实际结束地址 */
static uint32_t nr_pages    = 0;  /* 实际可分配的最大页数 */

#define PAGE_SHIFT 12
#define PAGE_SIZE  (1UL << PAGE_SHIFT)

#define PAGE_TAKEN (1u << 0)
#define PAGE_LAST  (1u << 1)

/*
 * 页描述符
 * flags 各位含义：
 * - bit 0: 该页是否已被占用
 * - bit 1: 该页是否为所分配内存块的最后一页
 */
struct page {
	uint8_t flags;
};

static inline void page_clear_flags(struct page *page)
{
	page->flags = 0;
}

static inline void page_set_flags(struct page *page, uint8_t flags)
{
	page->flags |= flags;
}

static inline int page_is_free(struct page *page)
{
	if (page->flags & PAGE_TAKEN) {
		return 0;
	} else {
		return 1;
	}
}

static inline int page_is_last(struct page *page)
{
	if (page->flags & PAGE_LAST) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * @brief 将地址对齐到页边界
 * @note 无
 * @param addr 待对齐的地址
 * @return 向上对齐到页边界后的地址
 */
static inline ptr_t page_align_up(ptr_t addr)
{
	ptr_t mask = PAGE_SIZE - 1;
	return (addr + mask) & ~mask;
}

/**
 * @brief 初始化页分配器：划分堆池并建立页描述符数组
 * @note 无
 * @param 无
 * @return 无
 */
void page_init(void)
{
	ptr_t heap_start_aligned = page_align_up(HEAP_START);

	/*
	 * 预留若干页用于存放页描述符数组。预留页数取决于 RAM_SIZE
	 * 为简单起见，这里预留的空间是个粗略估算，假定它能容纳最大的 RAM_SIZE
	 * 我们假定 RAM_SIZE 不会太小，理想情况下不小于 16M（即 PAGE_SIZE * PAGE_SIZE）
	 */
	uint32_t nr_reserved_pages = RAM_SIZE / (PAGE_SIZE * PAGE_SIZE);
	nr_pages = (HEAP_SIZE - (heap_start_aligned - HEAP_START)) / PAGE_SIZE - nr_reserved_pages;
	printf("HEAP_START = %p (aligned to %p), HEAP_SIZE = 0x%lx\n"
		"reserved pages = %d, allocatable pages = %d\n",
		HEAP_START, heap_start_aligned, HEAP_SIZE,
		nr_reserved_pages, nr_pages);

	/* 这里用 HEAP_START 而非 heap_start_aligned 作为 struct page 数组的起始地址，因为 struct page 的位置不要求对齐 */
	struct page *page = (struct page *)HEAP_START;
	for (int i = 0; i < nr_pages; i++) {
		page_clear_flags(page);
		page++;
	}

	alloc_start = heap_start_aligned + nr_reserved_pages * PAGE_SIZE;
	alloc_end = alloc_start + (PAGE_SIZE * nr_pages);

	printf("TEXT:   %p -> %p\n", TEXT_START, TEXT_END);
	printf("RODATA: %p -> %p\n", RODATA_START, RODATA_END);
	printf("DATA:   %p -> %p\n", DATA_START, DATA_END);
	printf("BSS:    %p -> %p\n", BSS_START, BSS_END);
	printf("HEAP:   %p -> %p\n", alloc_start, alloc_end);
}

/**
 * @brief 分配一块由连续物理页组成的内存
 * @note 无
 * @param npages 要分配的页数
 * @return 成功返回内存块起始地址，失败返回 NULL
 */
void *page_alloc(int npages)
{
	/* 注意这里是在页描述符数组上顺序查找 */
	int found = 0;
	struct page *candidate = (struct page *)HEAP_START;
	for (int i = 0; i <= (nr_pages - npages); i++) {
		if (page_is_free(candidate)) {
			found = 1;
			/* 找到一个空闲页，继续检查其后 (npages - 1) 页是否也未被分配 */
			struct page *scan = candidate + 1;
			for (int j = i + 1; j < (i + npages); j++) {
				if (!page_is_free(scan)) {
					found = 0;
					break;
				}
				scan++;
			}
			/* 找到足够大的内存块，标记占用，然后返回该内存块第一页的实际起始地址 */
			if (found) {
				struct page *mark = candidate;
				for (int k = i; k < (i + npages); k++) {
					page_set_flags(mark, PAGE_TAKEN);
					mark++;
				}
				mark--;
				page_set_flags(mark, PAGE_LAST);
				return (void *)(alloc_start + i * PAGE_SIZE);
			}
		}
		candidate++;
	}
	return NULL;
}

/**
 * @brief 释放内存块
 * @note 无
 * @param addr 内存块的起始地址
 * @return 无
 */
void page_free(void *addr)
{
	/* addr 非法时应断言（待补） */
	if (!addr || (ptr_t)addr >= alloc_end) {
		return;
	}

	/* 取该内存块第一个页的描述符 */
	struct page *page = (struct page *)HEAP_START;
	page += ((ptr_t)addr - alloc_start) / PAGE_SIZE;
	/* 循环清空该内存块所有页的描述符 */
	while (!page_is_free(page)) {
		if (page_is_last(page)) {
			page_clear_flags(page);
			break;
		} else {
			page_clear_flags(page);
			page++;
		}
	}
}

void page_test(void)
{
	void *p = page_alloc(2);
	printf("p = %p\n", p);

	void *p2 = page_alloc(7);
	printf("p2 = %p\n", p2);
	page_free(p2);

	void *p3 = page_alloc(4);
	printf("p3 = %p\n", p3);
}
