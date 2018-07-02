code segment
	push offset vx
	push 02018h
	pop ax
	pop bx
	mov [bx], ax
	push offset vy
	push 0a017h
	pop ax
	pop bx
	mov [bx], ax
	push offset vz
	push vx w
	push vt w
	push vy w
	push 02017h
	pop ax
	not ax
	push ax
	pop bx
	pop ax
	or ax, bx
	push ax
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
	push offset vx
	push vx w
	push 02h
	pop cx
	pop ax
	shl ax, cl
	push ax
	push vx w
	push 02h
	pop cx
	pop ax
	ror ax, cl
	push ax
	pop bx
	pop ax
	or ax, bx
	push ax
	pop ax
	pop bx
	mov [bx], ax
	push offset vy
	push vx w
	push vy w
	push vz w
	push 09876h
	pop cx
	pop ax
	xor ax, cx
	push ax
	pop cx
	pop ax
	xor ax, cx
	push ax
	pop cx
	pop ax
	xor ax, cx
	push ax
	pop ax
	pop bx
	mov [bx], ax
	push vx w
	pop ax
	call printword
	push vy w
	pop ax
	call printword
	push vz w
	pop ax
	call printword
	push vt w
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
	vt dw 0
	vx dw 0
	vy dw 0
	vz dw 0
code ends