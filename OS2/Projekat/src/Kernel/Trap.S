.global _ZN6Kernel6Events4trapEv

.set thread_offset_PC, 0x08
.set thread_offset_regs, 0x10

.align 16
.space 0x100 * 8
kernel_stack:

.align 4
_ZN6Kernel6Events4trapEv:
	csrw sscratch, t6
	ld t6, current_thread
    .irp index, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
        sd x\index, thread_offset_regs + \index * 8 (t6)
    .endr
    csrr t5, sscratch
    sd t5, thread_offset_regs + 31 * 8 (t6)
    csrr t5, sepc
    sd t5, thread_offset_PC(t6)

	la sp, kernel_stack
    call handle_trap

    ld t6, current_thread
	ld t5, thread_offset_PC(t6)
    csrw sepc, t5
    .irp index, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        ld x\index, thread_offset_regs + \index * 8 (t6)
    .endr

    sret
