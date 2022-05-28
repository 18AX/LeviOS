.global __gdt_update_segments

/** First args is gdtr address **/
__gdt_update_segments:
    pushq %rbp
    movq %rsp,%rbp

    lgdt (%rdi)

    movw $0x10,%ax
    movw %ax,%ss
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%gs

    call update_cs

    movq %rbp,%rsp
    popq %rbp
    ret

update_cs:
    popq %rax
    pushq $0x8
    pushq %rax
    lretq