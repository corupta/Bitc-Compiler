code segment
	push offset vx
	push 01a0fh
	pop ax
	pop bx
	mov [bx], ax
	push offset vy
	push 0bbh
	pop ax
	pop bx
	mov [bx], ax
	push offset vx
	push vx w
	push vy w
	pop ax
	not ax
	push ax
	pop bx
	pop ax
	and ax, bx
	push ax
	pop ax
	pop bx
	mov [bx], ax
	push offset vy
	push vx w
	push 01h
	pop cx
	pop ax
	shr ax, cl
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
	vx dw 0
	vy dw 0
code ends