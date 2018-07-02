code segment
	push offset vxx
	push 0100fh
	pop ax
	pop bx
	mov [bx], ax
	push offset vyy
	push 0f001h
	pop ax
	pop bx
	mov [bx], ax
	push offset vzz
	push vxx w
	push vyy w
	pop bx
	pop ax
	or ax, bx
	push ax
	push 0ab12h
	pop bx
	pop ax
	and ax, bx
	push ax
	pop ax
	pop bx
	mov [bx], ax
	push offset vyy
	push vxx w
	push 011h
	pop cx
	pop ax
	xor ax, cx
	push ax
	pop ax
	pop bx
	mov [bx], ax
