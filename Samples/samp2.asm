code segment
	push offset vx0
	push 0349h
	pop ax
	pop bx
	mov [bx], ax
	push offset vy22
	push 01a1h
	pop ax
	pop bx
	mov [bx], ax
	push offset vx0
	push vy22 w
	push 02h
	pop cx
	pop ax
	rol ax, cl
	push ax
	push vy22 w
	pop ax
	not ax
	push ax
	pop bx
	pop ax
	or ax, bx
	push ax
	pop ax
	pop bx
	mov [bx], ax
	push offset vy22
	push vx0 w
	push 02h
	pop cx
	pop ax
	shr ax, cl
	push ax
	push vy22 w
	push vz w
	pop cx
	pop ax
	xor ax, cx
	push ax
	push vy22 w
	pop bx
	pop ax
	or ax, bx
	push ax
	pop bx
	pop ax
	or ax, bx
	push ax
	pop ax
	pop bx
	mov [bx], ax
	push offset vz
	push vy22 w
	push vx0 w
	pop bx
	pop ax
	and ax, bx
	push ax
	pop ax
	not ax
	push ax
	pop ax
	not ax
	push ax
	pop ax
	pop bx
	mov [bx], ax
	push vx0 w
	pop ax
	call printword
	push vy22 w
	pop ax
	call printword
	push vz w
	pop ax
	call printword
	int 20h
printword:	; print the hexadecimal number at ax
	mov cx, ax
	call printbyte
	mov ch, cl
	call printbyte
	call println
	ret
printbyte: 		; print the hexadecimal number at ch
	mov dl, ch
	shr dl, 4
	call printchar
	mov dl, ch
	shl dl, 4
	shr dl, 4
	call printchar
	ret
printchar:		; print 0-f at last 4 bits of dl
	mov ah, 02h
	cmp dl, 0ah
	jb printnum
	sub dl, 0ah
	add dl, 97d
	int 21h
	ret
printnum:		; print 0-9 at dl
	add dl, 48d
	int 21h
	ret
println:		; print a newline (\r\n)
	mov ah, 02h
	mov dl, 13d
	int 21h
	mov dl, 10d
	int 21h
	ret
;;;	variables	;;;
	vx0 dw 0
	vy22 dw 0
	vz dw 0
code ends