#define PTR .word

	.section	.rodata

	.global	TEXT_START
TEXT_START:
	PTR	_text_start

	.global	TEXT_END
TEXT_END:
	PTR	_text_end

	.global	DATA_START
DATA_START:
	PTR	_data_start

	.global	DATA_END
DATA_END:
	PTR	_data_end

	.global	RODATA_START
RODATA_START:
	PTR	_rodata_start

	.global	RODATA_END
RODATA_END:
	PTR	_rodata_end

	.global	BSS_START
BSS_START:
	PTR	_bss_start

	.global	BSS_END
BSS_END:
	PTR	_bss_end

	.global	HEAP_START
HEAP_START:
	PTR	_heap_start

	.global	HEAP_SIZE
HEAP_SIZE:
	PTR	_heap_size
