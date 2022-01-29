.intel_syntax noprefix
.globl main
main:
  push 32
  push 4
  pop rdi
  pop rax
  add rax, rdi
  push rax
  push 6
  pop rdi
  pop rax
  sub rax, rdi
  push rax
  push 23
  pop rdi
  pop rax
  add rax, rdi
  push rax
  push 53
  push 5
  push 4
  pop rdi
  pop rax
  imul rax, rdi
  push rax
  pop rdi
  pop rax
  sub rax, rdi
  push rax
  pop rdi
  pop rax
	cmp rax, rdi
	setle al
	movzb rax, al
  push rax
  pop rax
  ret
