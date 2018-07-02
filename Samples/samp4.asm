code segment
	push offset va
	push vb w
	pop ax
	pop bx
	mov [bx], ax
	push offset vb
	push 02345h
	push 01h
	pop cx
	pop ax
	shl ax, cl
	push ax
	push 0ff00h
	pop cx
	pop ax
	xor ax, cx
	push ax
	pop ax
	not ax
	push ax
	pop ax
	pop bx
	mov [bx], ax
	push offset va
	push vc w
	push vb w
	push 0cadh
	pop bx
	pop ax
	and ax, bx
	push ax
	pop bx
	pop ax
	or ax, bx
	push ax
	pop ax
	pop bx
	mov [bx], ax
	push offset vd
	push vb w
	pop ax
	not ax
	push ax
	push va w
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
	push va w
	pop ax
	call printword
	push vb w
	pop ax
	call printword
	push vc w
	pop ax
	call printword
	push vd w
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
	va dw 0
	vb dw 0
	vc dw 0
	vd dw 0
code ends