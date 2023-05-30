/*
 * LPC1768 dk2 board script (future microbrute)
 * Carefully written by mighty mrf himself!
 * initially for his awesome microbrute mod project
 */


  __stack_size = 0x400; /* required amount of stack */

MEMORY
{
	Flash   (rx) : ORIGIN = 0x00000000, LENGTH = 512K
	RamCCM (rwx) : ORIGIN = 0x10000000 + __stack_size, LENGTH = 32K - __stack_size
	/* RamETH (rwx) : ORIGIN = 0x2007c000, LENGTH = 16K */
	/* RamUSB (rwx) : ORIGIN = 0x20080000, LENGTH = 16K	*/
	RamAHB (rwx) : ORIGIN = 0x2007C000, LENGTH = 32K
}

REGION_ALIAS("ROM", RamAHB);
REGION_ALIAS("RAM", RamCCM);

ENTRY(ResetISR)

SECTIONS
{
	PROVIDE(_vStackTop = ORIGIN(RAM));
	PROVIDE(_VTORval = ORIGIN(ROM));
	
	.isr_crp : ALIGN(4)
	{
		FILL(0xff)
		. = ORIGIN(ROM);
		__vectors_start__ = ABSOLUTE(.) ;
		KEEP(*(.isr_vector))

		/* Global Section Table */
		. = ALIGN(4) ;
		__data_section_table = .;
		LONG(LOADADDR(.data));
		LONG(    ADDR(.data));
		LONG(  SIZEOF(.data));
		__data_section_table_end = .;
		__bss_section_table = .;
		LONG(    ADDR(.bss));
		LONG(  SIZEOF(.bss));
		__bss_section_table_end = .;
		__ramtext_section_table = .;
		LONG(LOADADDR(.ramtext));
		LONG(    ADDR(.ramtext));
		LONG(  SIZEOF(.ramtext));
		__ramtext_section_table_end = .;
		/* End of Global Section Table */

		. = ALIGN(4) ; 
		*(.after_vectors*)
		
		/* Code Read Protection data */
		. = 0x000002FC ;
		LONG(0xFFFFFFFF)
		/* PROVIDE(__CRP_WORD_START__ = .) ;                                                                                    */ 
		/* KEEP(*(.crp))                                                                                                        */ 
		/* PROVIDE(__CRP_WORD_END__ = .) ;                                                                                      */ 
		/* ASSERT(!(__CRP_WORD_START__ == __CRP_WORD_END__), "Linker CRP Enabled, but no CRP_WORD provided within application");*/ 
		/* MEMORY POINTER SCREWED AFTER THIS, NEW SECTION ?? */
	} > ROM
		
	.text : ALIGN(4)
	{
		*(.text)
		*(.text*)
		*(.glue_7)      /* glue arm to thumb code */
		*(.glue_7t)     /* glue thumb to arm code */
		*(.eh_frame)    /* some optional ABI stuff */
		
		. = ALIGN(4);
		*(.rodata)
		*(.rodata*)
		
		. = ALIGN(4);
		/* C++ constructors destructors */
		KEEP(*(.init))
		KEEP(*(.fini))
		
		/* another unknown constructors/destructors
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)
		
 		*crtbegin.o(.dtors)
 		*crtbegin?.o(.dtors)
 		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
 		*(SORT(.dtors.*))
 		*(.dtors)
		*/
		
		/* Newlib C++ constructors/Destructors, some unknown crap */
		/*
		. = ALIGN(4);
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);

		. = ALIGN(4);
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);

		. = ALIGN(4);
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array*))
		PROVIDE_HIDDEN (__fini_array_end = .);
		*/
		
	} > ROM
	
	.ARM.extab : ALIGN(4) 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > ROM
	    
	.ARM.exidx : ALIGN(4)
	{
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} > ROM
	
	_etext = .;
	_sidata = .;
	
	/* ## Create checksum value (must be present) ## */
	PROVIDE(__valid_user_code_checksum = 0 - 
	                                     (_vStackTop 
	                                     + (ResetISR + 1) 
	                                     + (NMI_Handler + 1) 
	                                     + (HardFault_Handler + 1) 
	                                     + (( DEFINED(MemManage_Handler) ? MemManage_Handler : 0 ) + 1)   /* MemManage_Handler may not be defined */
	                                     + (( DEFINED(BusFault_Handler) ? BusFault_Handler : 0 ) + 1)     /* BusFault_Handler may not be defined */
	                                     + (( DEFINED(UsageFault_Handler) ? UsageFault_Handler : 0 ) + 1) /* UsageFault_Handler may not be defined */
	                                     ) );
	
	.ramtext : ALIGN(4)
	{
		__ramtext_load = LOADADDR (.data);
    	__ramtext_start = .;
		*(.ramtext)
		*(.ramtext*)
		*(.ramfunc)
		*(.ramfunc*)
    	. = ALIGN(4);
    	__ramtext_end = .;
	} > RAM AT> ROM
	
	.data : ALIGN(4)
	{
    	__data_load = LOADADDR (.data);
    	__data_start = .;
    	*(.data)
    	*(.data*)
    	. = ALIGN(4);
    	__data_end = .;
	} > RAM AT> ROM
	
	/* MAIN BSS SECTION */
	.bss : ALIGN(4)
	{
    	__bss_start = .;
		*(.bss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
    	__bss_end = .;
	} > RAM
	
	.noinit : ALIGN(4)
	{
		*(.noinit*)
	} > RAM

	/*
	.RamETH_noinit (NOLOAD) :
	{
		. = ALIGN(4);
		*(.RamETH_noinit)
		*(.RamETH_noinit*)
	} > RamETH
	
	.RamUSB_noinit (NOLOAD) :
	{
		. = ALIGN(4);
		*(.RamUSB_noinit)
		*(.RamUSB_noinit*)
	} > RamUSB
	*/

}

