.section .rodata
.LC0:
    .string "%d\n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
add:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    movq %rbx, -8(%rbp)
    movq %r12, -16(%rbp)
    movq %r13, -24(%rbp)
    movq %r14, -32(%rbp)
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movl a(%rip), %eax
    pushq %rax
    movl b(%rip), %eax
    popq %rcx
    addl %ecx, %eax
    jmp .Lreturn3
    movq -8(%rbp), %rbx
    movq -16(%rbp), %r12
    movq -24(%rbp), %r13
    movq -32(%rbp), %r14
    leave
    ret
    .comm a, 4
    movl $1, %eax
    movl %eax, a(%rip)
    .comm b, 4
    movl $2, %eax
    movl %eax, b(%rip)
    .comm c, 4
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %rcx
    pushq %r8
    pushq %r9
    movl b(%rip), %eax
    movl %eax, %esi
    movl a(%rip), %eax
    movl %eax, %edi
    call add
    popq %r9
    popq %r8
    popq %rcx
    popq %rdx
    popq %rsi
    popq %rdi
    movl %eax, c(%rip)
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %rcx
    pushq %r8
    pushq %r9
    movl c(%rip), %eax
    movl %eax, %edi
    call print
    popq %r9
    popq %r8
    popq %rcx
    popq %rdx
    popq %rsi
    popq %rdi
    popq %rbp
    ret
