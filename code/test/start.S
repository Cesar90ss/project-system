/* Start.s
 *	Assembly language assist for user programs running on top of Nachos.
 *
 *	Since we don't want to pull in the entire C library, we define
 *	what we need for a user program here, namely Start and the system
 *	calls.
 */
 #include "syscall.h"
         .text
        .align  2
 /* -------------------------------------------------------------
 * __start
 *	Initialize running a C program, by calling "main".
 *
 *  NOTE: This has to be first, so that it gets loaded at location 0.
 *	The Nachos kernel always starts a program by jumping to location 0.
 * -------------------------------------------------------------
 */
     .globl __start
    .ent	__start
__start:
    jal	main
    move	$4,$2
    jal	Exit     /* if we return from main, call exit syscall */
    .end __start
 /* -------------------------------------------------------------
 * System call stubs:
 *	Assembly language assist to make system calls to the Nachos kernel.
 *	There is one stub per system call, that places the code for the
 *	system call into register r2, and leaves the arguments to the
 *	system call alone (in other words, arg1 is in r4, arg2 is
 *	in r5, arg3 is in r6, arg4 is in r7)
 *
 *  The return value is in r2. This follows the standard C calling
 *  convention on the MIPS.
 * -------------------------------------------------------------
 */
/* ----------------------*/
    .globl Halt
    .ent	Halt
Halt:
    addiu $2,$0,SC_Halt
    syscall
    j	$31
    .end Halt
/* ----------------------*/
    .globl Exit
    .ent	Exit
Exit:
    addiu $2,$0,SC_Exit
    syscall
    j	$31
    .end Exit
/* ----------------------*/
    .globl Exec
    .ent	Exec
Exec:
    addiu $2,$0,SC_Exec
    syscall
    j	$31
    .end Exec
/* ----------------------*/
    .globl Join
    .ent	Join
Join:
    addiu $2,$0,SC_Join
    syscall
    j	$31
    .end Join
/* ----------------------*/
    .globl Create
    .ent	Create
Create:
    addiu $2,$0,SC_Create
    syscall
    j	$31
    .end Create
/* ----------------------*/
    .globl Open
    .ent	Open
Open:
    addiu $2,$0,SC_Open
    syscall
    j	$31
    .end Open
/* ----------------------*/
    .globl Read
    .ent	Read
Read:
    addiu $2,$0,SC_Read
    syscall
    j	$31
    .end Read
/* ----------------------*/
    .globl Write
    .ent	Write
Write:
    addiu $2,$0,SC_Write
    syscall
    j	$31
    .end Write
/* ----------------------*/
    .globl Close
    .ent	Close
Close:
    addiu $2,$0,SC_Close
    syscall
    j	$31
    .end Close
/* ----------------------*/
    .globl Seek
    .ent	Seek
Seek:
    addiu $2,$0,SC_Seek
    syscall
    j	$31
    .end Seek
/* ----------------------*/
    .globl Remove
    .ent	Remove
Remove:
    addiu $2,$0,SC_Remove
    syscall
    j	$31
    .end Remove
/* ----------------------*/
    .globl Yield
    .ent	Yield
Yield:
    addiu $2,$0,SC_Yield
    syscall
    j	$31
    .end Yield
/* ----------------------*/
    .globl PutChar
    .ent	PutChar
PutChar:
    addiu $2,$0,SC_PutChar
    syscall
    j	$31
    .end PutChar
/* ----------------------*/
    .globl GetChar
    .ent	GetChar
GetChar:
    addiu $2,$0,SC_GetChar
    syscall
    j	$31
    .end GetChar
/* ----------------------*/
    .globl PutString
    .ent PutString
PutString:
    addiu $2,$0,SC_PutString
    syscall
    j	$31
    .end PutString
/* ----------------------*/
    .globl GetString
    .ent	GetString
GetString:
    addiu $2,$0,SC_GetString
    syscall
    j	$31
    .end GetString
/* ----------------------*/
    .globl PutInt
    .ent PutInt
PutInt:
    addiu $2,$0,SC_PutInt
    syscall
    j	$31
    .end PutInt
/* ----------------------*/
    .globl GetInt
    .ent	GetInt
GetInt:
    addiu $2,$0,SC_GetInt
    syscall
    j	$31
    .end GetInt
/* ----------------------*/
    .globl ThreadCreateWrapper
    .ent	ThreadCreateWrapper
ThreadCreateWrapper:
    add     $sp, $sp, -32       /* Take some space on stack to fun args */
    move	$6, $4
    move    $4, $5
    jal $6
    move    $4, $2
    jal	UserThreadExit     /* if we return from user thread function, call UserThreadExit */
    .end ThreadCreateWrapper
/* ----------------------*/
    .globl UserThreadCreate
    .ent	UserThreadCreate
UserThreadCreate:
    move    $6, $5
    move    $5, $4
    la      $4, ThreadCreateWrapper
    addiu $2,$0,SC_UserThreadCreate
    syscall
    j	$31
    .end UserThreadCreate
/* ----------------------*/
    .globl UserThreadExit
    .ent	UserThreadExit
UserThreadExit:
    addiu $2,$0,SC_UserThreadExit
    syscall
    j	$31
    .end UserThreadExit
/* ----------------------*/
    .globl UserThreadJoin
    .ent	UserThreadJoin
UserThreadJoin:
    addiu $2,$0,SC_UserThreadJoin
    syscall
    j	$31
    .end UserThreadJoin
/* ----------------------*/
     .globl UserSemaphoreCreate
    .ent	UserSemaphoreCreate
UserSemaphoreCreate:
    addiu $2,$0,SC_UserSemaphoreCreate
    syscall
    j	$31
    .end UserSemaphoreCreate
/* ----------------------*/
    .globl UserSemaphoreP
    .ent	UserSemaphoreP
UserSemaphoreP:
    addiu $2,$0,SC_UserSemaphoreP
    syscall
    j	$31
    .end UserSemaphoreP
/* ----------------------*/
    .globl UserSemaphoreV
    .ent	UserSemaphoreV
UserSemaphoreV:
    addiu $2,$0,SC_UserSemaphoreV
    syscall
    j	$31
    .end UserSemaphoreV
/* ----------------------*/
     .globl UserSemaphoreDestroy
    .ent	UserSemaphoreDestroy
UserSemaphoreDestroy:
    addiu $2,$0,SC_UserSemaphoreDestroy
    syscall
    j	$31
    .end UserSemaphoreDestroy
/* ----------------------*/
    .globl ForkExec
    .ent	ForkExec
ForkExec:
    addiu $2,$0,SC_ForkExec
    syscall
    j	$31
    .end ForkExec
/* ----------------------*/
    .globl AllocPageHeap
    .ent	AllocPageHeap
AllocPageHeap:
    addiu $2,$0,SC_AllocPageHeap
    syscall
    j	$31
    .end AllocPageHeap
/* ----------------------*/
    .globl FreePageHeap
    .ent	FreePageHeap
FreePageHeap:
    addiu $2,$0,SC_FreePageHeap
    syscall
    j	$31
    .end FreePageHeap
/* ----------------------*/
    .globl Waitpid
    .ent	Waitpid
Waitpid:
    addiu $2,$0,SC_Waitpid
    syscall
    j	$31
    .end Waitpid
/* ----------------------*/
    .globl CheckEnd
    .ent	CheckEnd
CheckEnd:
    addiu $2,$0,SC_CheckEnd
    syscall
    j	$31
    .end CheckEnd
/* ----------------------*/
    .globl Listen
    .ent	Listen
Listen:
    addiu $2,$0,SC_Listen
    syscall
    j	$31
    .end Listen
/* ----------------------*/
    .globl Accept
    .ent	Accept
Accept:
    addiu $2,$0,SC_Accept
    syscall
    j	$31
    .end Accept
/* ----------------------*/
    .globl Connect
    .ent	Connect
Connect:
    addiu $2,$0,SC_Connect
    syscall
    j	$31
    .end Connect
/* ----------------------*/
    .globl Send
    .ent	Send
Send:
    addiu $2,$0,SC_Send
    syscall
    j	$31
    .end Send
/* ----------------------*/
    .globl Receive
    .ent	Receive
Receive:
    addiu $2,$0,SC_Receive
    syscall
    j	$31
    .end Receive
/* ----------------------*/
    .globl Disconnect
    .ent	Disconnect
Disconnect:
    addiu $2,$0,SC_Disconnect
    syscall
    j	$31
    .end Disconnect
/* ----------------------*/

/* ----------------------*/
    .globl GetCurrentDirectory
    .ent	GetCurrentDirectory
GetCurrentDirectory:
    addiu $2,$0,SC_GetCurrentDirectory
    syscall
    j	$31
    .end GetCurrentDirectory

/* ----------------------*/
    .globl SetCurrentDirectory
    .ent	SetCurrentDirectory
SetCurrentDirectory:
    addiu $2,$0,SC_SetCurrentDirectory
    syscall
    j	$31
    .end SetCurrentDirectory

/* dummy function to keep gcc happy */
        .globl  __main
        .ent    __main
__main:
        j       $31
        .end    __main
