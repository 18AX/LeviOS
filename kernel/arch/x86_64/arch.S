.global __ctx_set
__ctx_set:
    xor %rax,%rax
    movq 168(%rdi),%rax
    movw %ax, %ds
    movw %ax, %es
    
    movq (%rdi),%rax
    movq 8(%rdi),%rbx
    movq 16(%rdi),%rcx
    movq 24(%rdi),%rdx
    movq 32(%rdi),%rsi
    /** We skip rdi **/
    movq 48(%rdi),%r8
    movq 56(%rdi),%r9
    movq 64(%rdi),%r10
    movq 72(%rdi),%r11
    movq 80(%rdi),%r12
    movq 88(%rdi),%r13
    movq 96(%rdi),%r14
    movq 104(%rdi),%r15
    movq 112(%rdi),%rbp

    pushq 168(%rdi) /** ss **/
    pushq 160(%rdi) /** rsp **/
    pushq 152(%rdi) /** rflags **/
    pushq 144(%rdi) /** cs **/
    pushq 136(%rdi) /** rip **/

    movq 40(%rdi),%rdi

    xchgw %bx, %bx

    iretq
