code segment
    assume cs:code,ds:data

start:
    mov ax,data      ;将LED的初始顺序输入
    mov ds,ax
    ;初始化8259
    cli                ;关中断
    mov ax,0h
    mov ds,ax           ;ds清零
    in al,210h          ;写入IRQ5
    and al,11011111b
    out 21h,al  

 
    MOV DX,203H     ;初始化8255
    MOV AL,10010000b;1001 0000 初始化命令：A端口0方式输入，B端口0方式输出
    OUT DX,AL       ;将初始化命令输入到命令状态口



lop:
    xor al,11111111     ;实现交替点亮熄灭
    out dx,al
    sti
    lp1                 ;实现数码管显示8259-A，同时也是延时程序
inte:                   ;中断函数
    push dx         ;暂时储存状态
    push ax
    push cx         
    cli             ;关中断
    mov al,20h      ;中断结束命令
    pop cx
    pop ax
    pop dx          ;恢复

LP1: 
mov  dx,211h         ;使数码管显示8
mov  al,00000000b
out  dx,al
mov  dx,210h
mov  al,7fh          ;对应段码
out  dx,al
mov  al,00100000b
out  dx,al
                       ;使数码管显示2
mov  dx,211h
mov  al,00000000b
out  dx,al
mov  dx,210h
mov  al,5bh
out  dx,al
mov  dx,211h
mov  al,00010000b
out  dx,al
mov  dx,211h
mov  al,00000000b
out  dx,al
                      ;使数码管显示5
mov  dx,210h
mov  al,6dh
out  dx,al
mov  dx,211h
mov  al,00001000b
out  dx,al
mov  dx,210h
mov  al,6dh
out  dx,al
mov  dx,211h
mov  al,00000100b
out  dx,al
                    ;使数码管显示9
mov  dx,211h
mov  al,00000000b
out  dx,al
mov  dx,210h
mov  al,6fh
out  dx,al
mov  dx,211h
mov  al,00000010b
out  dx,al
                  ;使数码管显示-
mov dx,211h
mov al,00000000b
out dx,al
mov dx,211h
mov al,00000001b
out dx,al
                   ;使数码管显示A
mov dx,211h
mov al,00000000b
out dx,al
mov dx,210h
mov al,77h
out dx,al
mov dx,211h
mov al,00000001b
out dx,al

loop lp1          ;转至lp
jmp lop           ;跳至lop
hlt               ;暂时停机，用作延时



code ends
end start