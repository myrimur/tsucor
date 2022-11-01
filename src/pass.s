# pass(from, to, arg): pass execution from one coroutine to another
.global pass

.text

pass:
    # Save preserved registers to old stack
    push rdi
    push rbp
    push rbx
    push r12
    push r13
    push r14
    push r15

    mov [rdi], rsp  # Save old stack pointer
    mov rsp, [rsi]  # Load new stack pointer

    # Restore preserved regs from new stack
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    pop rdi

    mov rax, rdx  # Pass arg to new coroutine
    ret
