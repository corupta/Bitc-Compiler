code segment
	push offset vx11
	push 01a0fh
	pop ax
	pop bx
	mov [bx], ax
	push offset vy22
	push 0bbh
	pop ax
	pop bx
	mov [bx], ax
	push offset vx11
	pop ax
	pop bx
	mov [bx], ax
