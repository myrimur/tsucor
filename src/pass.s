# pass(from, to): pass execution from one coroutine to another
.text
.global pass
pass:

# Save preserved registers to old stack
push rdi
push rbp
push rbx
push r12
push r13
push r14
push r15

# Save old stack pointer
mov [rdi], rsp
# Load new stack pointer
mov rsp, [rsi]

# Restore preserved regs from new stack
pop r15
pop r14
pop r13
pop r12
pop rbx
pop rbp
pop rdi
ret
