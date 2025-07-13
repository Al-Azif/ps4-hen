OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

PHDRS
{
	payload_code_seg PT_LOAD FLAGS(5);  /* R+X */
	payload_data_seg PT_LOAD FLAGS(6);  /* R+W */
	bad_seg PT_LOAD FLAGS(6);           /* R+W */
}

SECTIONS
{
	. = 0;
	.payload_header : {
		*(.payload_header)
		. = ALIGN(8);
	} : payload_code_seg

	.payload_code : {
		*(.payload_code)
		. = ALIGN(8);
	} : payload_code_seg

	.payload_rdata : {
		*(.payload_rdata .rodata*)
		. = ALIGN(8);
	} : payload_data_seg

	.payload_data : {
		*(.payload_data)
		. = ALIGN(8);
	} : payload_data_seg

	.payload_bss : {
		*(.payload_bss)
		. = ALIGN(8);
	} : payload_data_seg

	. = 0x100000;
	.data : {
		*(.data)
		. = ALIGN(8);
	} : bad_seg

	.bss : {
		*(.bss)
		. = ALIGN(8);
	} : bad_seg

	/DISCARD/ : { *(*) }
}
