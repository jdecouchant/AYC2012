    .globl plus
    .globl _plus
plus:
_plus:
	// %rbp est scratch
	push   %rbp
	// %rsp est le stack pointer
	mov    %rsp,%rbp
	// %rdi contient le 1er argument = a
	mov    %rdi,-0x8(%rbp)
	// %rsi contient le 2eme argument = b
	mov    %rsi,-0x10(%rbp)
	//a => %rax
	mov    -0x8(%rbp),%rax
	//les 32 premier bits de a dans edx
	mov    %eax,%edx
	// b dans %rax
	mov    -0x10(%rbp),%rax
	//additionne les 32 premiers bits des 2 et sotck le resultat dans %eax, le return register
	add    %edx,%eax
	//rétabli rbp
	pop    %rbp
	//quit
	retq
