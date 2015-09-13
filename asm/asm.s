
.extern dudu

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
	//a dans %rdx
	mov    %rax,%rdx
	// b dans %rax
	mov    -0x10(%rbp),%rax
	//rax = rax + rdx
	add    %rdx,%rax
	//met la variable global dudu dans rdx
	mov dudu,%rdx
	//ajoute dudu a la somme
	add %rdx,%rax
	//rétabli rbp
	pop    %rbp
	//quit
	retq
