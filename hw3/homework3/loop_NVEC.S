	.text
	.file	"loop.c"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               # -- Begin function main
.LCPI0_0:
	.quad	0x3e112e0be826d695              # double 1.0000000000000001E-9
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
.Lfunc_begin0:
	.file	0 "/mnt/d/MIT6.172/hw3/homework3" "loop.c" md5 0xfe27a5da90ed2d2b69e01a98f1b09add
	.loc	0 49 0                          # loop.c:49:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: main:argc <- $edi
	#DEBUG_VALUE: main:argv <- $rsi
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r12
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	subq	$12328, %rsp                    # imm = 0x3028
	.cfi_def_cfa_offset 12368
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
.Ltmp0:
	#DEBUG_VALUE: main:total <- 0
	#DEBUG_VALUE: main:seed <- 0
	.loc	0 56 18 prologue_end            # loop.c:56:18
	movl	$0, 12(%rsp)
.Ltmp1:
	#DEBUG_VALUE: main:j <- 0
	.loc	0 0 18 is_stmt 0                # loop.c:0:18
	leaq	8224(%rsp), %rdi
.Ltmp2:
	#DEBUG_VALUE: main:argc <- [DW_OP_LLVM_entry_value 1] $edi
	xorl	%ebx, %ebx
.Ltmp3:
	.loc	0 62 14 is_stmt 1               # loop.c:62:14
	movl	$4096, %edx                     # imm = 0x1000
	xorl	%esi, %esi
.Ltmp4:
	#DEBUG_VALUE: main:argv <- [DW_OP_LLVM_entry_value 1] $rsi
	callq	memset@PLT
.Ltmp5:
	.loc	0 0 14 is_stmt 0                # loop.c:0:14
	leaq	4128(%rsp), %rdi
	.loc	0 63 14 is_stmt 1               # loop.c:63:14
	movl	$4096, %edx                     # imm = 0x1000
	xorl	%esi, %esi
	callq	memset@PLT
.Ltmp6:
	.loc	0 0 14 is_stmt 0                # loop.c:0:14
	leaq	32(%rsp), %rdi
	.loc	0 64 14 is_stmt 1               # loop.c:64:14
	movl	$4096, %edx                     # imm = 0x1000
	xorl	%esi, %esi
	callq	memset@PLT
.Ltmp7:
	#DEBUG_VALUE: main:j <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
	.loc	0 0 14 is_stmt 0                # loop.c:0:14
	leaq	16(%rsp), %rsi
.Ltmp8:
	.file	1 "./." "fasttime.h" md5 0x27bd3d6b4b5f1ce284266b0c813327bc
	.loc	1 72 3 is_stmt 1                # ././fasttime.h:72:3
	movl	$1, %edi
	callq	clock_gettime@PLT
.Ltmp9:
	.loc	1 77 3                          # ././fasttime.h:77:3
	movq	16(%rsp), %r15
	movq	24(%rsp), %r14
.Ltmp10:
	#DEBUG_VALUE: main:time1 <- [DW_OP_LLVM_fragment 0 64] undef
	#DEBUG_VALUE: main:time1 <- [DW_OP_LLVM_fragment 64 64] undef
	#DEBUG_VALUE: main:i <- 0
	.p2align	4, 0x90
.LBB0_1:                                # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
	#DEBUG_VALUE: main:argc <- [DW_OP_LLVM_entry_value 1] $edi
	#DEBUG_VALUE: main:argv <- [DW_OP_LLVM_entry_value 1] $rsi
	#DEBUG_VALUE: main:total <- 0
	#DEBUG_VALUE: main:seed <- 0
	#DEBUG_VALUE: main:i <- $ebx
	#DEBUG_VALUE: main:j <- 0
	.loc	1 0 3 is_stmt 0                 # ././fasttime.h:0:3
	xorl	%eax, %eax
.Ltmp11:
	.p2align	4, 0x90
.LBB0_2:                                #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	#DEBUG_VALUE: main:argc <- [DW_OP_LLVM_entry_value 1] $edi
	#DEBUG_VALUE: main:argv <- [DW_OP_LLVM_entry_value 1] $rsi
	#DEBUG_VALUE: main:total <- 0
	#DEBUG_VALUE: main:seed <- 0
	#DEBUG_VALUE: main:i <- $ebx
	#DEBUG_VALUE: main:j <- $rax
	.loc	0 71 32 is_stmt 1               # loop.c:71:32
	movl	4128(%rsp,%rax,4), %ecx
.Ltmp12:
	#DEBUG_VALUE: main:j <- [DW_OP_constu 1, DW_OP_or, DW_OP_stack_value] $rax
	#DEBUG_VALUE: main:j <- [DW_OP_constu 1, DW_OP_or, DW_OP_stack_value] $rax
	movl	4132(%rsp,%rax,4), %edx
	.loc	0 71 25 is_stmt 0               # loop.c:71:25
	addl	8224(%rsp,%rax,4), %ecx
	addl	8228(%rsp,%rax,4), %edx
.Ltmp13:
	#DEBUG_VALUE: main:j <- [DW_OP_constu 2, DW_OP_or, DW_OP_stack_value] $rax
	#DEBUG_VALUE: main:j <- [DW_OP_constu 2, DW_OP_or, DW_OP_stack_value] $rax
	.loc	0 71 18                         # loop.c:71:18
	movl	%ecx, 32(%rsp,%rax,4)
	.loc	0 71 32                         # loop.c:71:32
	movl	4136(%rsp,%rax,4), %ecx
	.loc	0 71 25                         # loop.c:71:25
	addl	8232(%rsp,%rax,4), %ecx
.Ltmp14:
	#DEBUG_VALUE: main:j <- [DW_OP_constu 3, DW_OP_or, DW_OP_stack_value] $rax
	#DEBUG_VALUE: main:j <- [DW_OP_constu 3, DW_OP_or, DW_OP_stack_value] $rax
	.loc	0 71 18                         # loop.c:71:18
	movl	%edx, 36(%rsp,%rax,4)
	.loc	0 71 32                         # loop.c:71:32
	movl	4140(%rsp,%rax,4), %edx
	.loc	0 71 25                         # loop.c:71:25
	addl	8236(%rsp,%rax,4), %edx
	.loc	0 71 18                         # loop.c:71:18
	movl	%ecx, 40(%rsp,%rax,4)
	movl	%edx, 44(%rsp,%rax,4)
.Ltmp15:
	.loc	0 70 29 is_stmt 1               # loop.c:70:29
	addq	$4, %rax
.Ltmp16:
	#DEBUG_VALUE: main:j <- $rax
	.loc	0 70 23 is_stmt 0               # loop.c:70:23
	cmpq	$1024, %rax                     # imm = 0x400
.Ltmp17:
	.loc	0 70 9                          # loop.c:70:9
	jne	.LBB0_2
.Ltmp18:
# %bb.3:                                #   in Loop: Header=BB0_1 Depth=1
	#DEBUG_VALUE: main:argc <- [DW_OP_LLVM_entry_value 1] $edi
	#DEBUG_VALUE: main:argv <- [DW_OP_LLVM_entry_value 1] $rsi
	#DEBUG_VALUE: main:total <- 0
	#DEBUG_VALUE: main:seed <- 0
	#DEBUG_VALUE: main:j <- $rax
	#DEBUG_VALUE: main:i <- $ebx
	.loc	0 69 25 is_stmt 1               # loop.c:69:25
	addl	$1, %ebx
.Ltmp19:
	#DEBUG_VALUE: main:i <- $ebx
	.loc	0 69 19 is_stmt 0               # loop.c:69:19
	cmpl	$100000, %ebx                   # imm = 0x186A0
.Ltmp20:
	.loc	0 69 5                          # loop.c:69:5
	jne	.LBB0_1
.Ltmp21:
# %bb.4:
	#DEBUG_VALUE: main:argc <- [DW_OP_LLVM_entry_value 1] $edi
	#DEBUG_VALUE: main:argv <- [DW_OP_LLVM_entry_value 1] $rsi
	#DEBUG_VALUE: main:total <- 0
	#DEBUG_VALUE: main:seed <- 0
	#DEBUG_VALUE: main:j <- $rax
	#DEBUG_VALUE: main:i <- $ebx
	#DEBUG_VALUE: main:time1 <- [DW_OP_LLVM_fragment 64 64] $r14
	#DEBUG_VALUE: main:time1 <- [DW_OP_LLVM_fragment 0 64] $r15
	.loc	0 0 5                           # loop.c:0:5
	leaq	16(%rsp), %rsi
.Ltmp22:
	.loc	1 72 3 is_stmt 1                # ././fasttime.h:72:3
	movl	$1, %edi
	callq	clock_gettime@PLT
.Ltmp23:
	.loc	1 77 3                          # ././fasttime.h:77:3
	movq	16(%rsp), %rbx
.Ltmp24:
	movq	24(%rsp), %r12
.Ltmp25:
	#DEBUG_VALUE: main:seed <- [DW_OP_plus_uconst 12, DW_OP_deref] $rsp
	#DEBUG_VALUE: main:time2 <- [DW_OP_LLVM_fragment 64 64] $r12
	#DEBUG_VALUE: main:time2 <- [DW_OP_LLVM_fragment 0 64] $rbx
	.loc	1 0 3 is_stmt 0                 # ././fasttime.h:0:3
	leaq	12(%rsp), %rdi
.Ltmp26:
	.loc	0 78 16 is_stmt 1               # loop.c:78:16
	callq	rand_r@PLT
.Ltmp27:
                                        # kill: def $eax killed $eax def $rax
	.loc	0 78 30 is_stmt 0               # loop.c:78:30
	leal	1023(%rax), %ecx
	testl	%eax, %eax
	cmovnsl	%eax, %ecx
	andl	$-1024, %ecx                    # imm = 0xFC00
	subl	%ecx, %eax
.Ltmp28:
	#DEBUG_VALUE: main:total <- undef
	#DEBUG_VALUE: tdiff:start <- [DW_OP_LLVM_fragment 0 64] $r15
	#DEBUG_VALUE: tdiff:start <- [DW_OP_LLVM_fragment 64 64] $r14
	#DEBUG_VALUE: tdiff:end <- [DW_OP_LLVM_fragment 64 64] $r12
	#DEBUG_VALUE: tdiff:end <- [DW_OP_LLVM_fragment 0 64] $rbx
	.loc	1 83 21 is_stmt 1               # ././fasttime.h:83:21
	subq	%r15, %rbx
.Ltmp29:
	.loc	1 83 10 is_stmt 0               # ././fasttime.h:83:10
	cvtsi2sd	%rbx, %xmm1
.Ltmp30:
	.loc	0 78 14 is_stmt 1               # loop.c:78:14
	cltq
.Ltmp31:
	.loc	1 83 56                         # ././fasttime.h:83:56
	subq	%r14, %r12
.Ltmp32:
	.loc	1 83 43 is_stmt 0               # ././fasttime.h:83:43
	cvtsi2sd	%r12, %xmm0
.Ltmp33:
	.loc	0 78 14 is_stmt 1               # loop.c:78:14
	movl	32(%rsp,%rax,4), %ebx
.Ltmp34:
	#DEBUG_VALUE: main:total <- $ebx
	.loc	1 83 36                         # ././fasttime.h:83:36
	mulsd	.LCPI0_0(%rip), %xmm0
	addsd	%xmm1, %xmm0
.Ltmp35:
	#DEBUG_VALUE: main:elapsedf <- $xmm0
	.loc	0 83 5 is_stmt 0                # loop.c:83:5
	leaq	.L.str(%rip), %rdi
	leaq	.L.str.1(%rip), %rcx
	leaq	.L.str.2(%rip), %r8
	movl	$1024, %esi                     # imm = 0x400
	movl	$100000, %edx                   # imm = 0x186A0
	movb	$1, %al
	callq	printf@PLT
.Ltmp36:
	.loc	0 87 5 is_stmt 1                # loop.c:87:5
	movl	%ebx, %eax
	addq	$12328, %rsp                    # imm = 0x3028
	.cfi_def_cfa_offset 40
	popq	%rbx
.Ltmp37:
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r14
.Ltmp38:
	.cfi_def_cfa_offset 16
	popq	%r15
.Ltmp39:
	.cfi_def_cfa_offset 8
	retq
.Ltmp40:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.file	2 "/usr/include/x86_64-linux-gnu/bits" "types.h" md5 0xd108b5f93a74c50510d7d9bc0ab36df9
	.file	3 "/usr/include/x86_64-linux-gnu/bits/types" "struct_timespec.h" md5 0x55dc154df3f21a5aa944dcafba9b43f6
	.file	4 "/usr/include" "time.h" md5 0xdb37158473a25e1d89b19f8bc6892801
	.file	5 "/usr/include/x86_64-linux-gnu/bits/types" "clockid_t.h" md5 0x099a80153c2ad48bc7f5f4a188cb6d24
	.file	6 "/usr/include" "stdlib.h" md5 0x02258fad21adf111bb9df9825e61954a
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Elapsed execution time: %f sec; N: %d, I: %d, __OP__: %s, __TYPE__: %s\n"
	.size	.L.str, 72

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"+"
	.size	.L.str.1, 2

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.asciz	"uint32_t"
	.size	.L.str.2, 9

	.file	7 "/usr/include/x86_64-linux-gnu/bits" "stdint-uintn.h" md5 0x2bf2ae53c58c01b1a1b9383b5195125c
	.section	.debug_loclists,"",@progbits
	.long	.Ldebug_list_header_end0-.Ldebug_list_header_start0 # Length
.Ldebug_list_header_start0:
	.short	5                               # Version
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
	.long	11                              # Offset entry count
.Lloclists_table_base0:
	.long	.Ldebug_loc0-.Lloclists_table_base0
	.long	.Ldebug_loc1-.Lloclists_table_base0
	.long	.Ldebug_loc2-.Lloclists_table_base0
	.long	.Ldebug_loc3-.Lloclists_table_base0
	.long	.Ldebug_loc4-.Lloclists_table_base0
	.long	.Ldebug_loc5-.Lloclists_table_base0
	.long	.Ldebug_loc6-.Lloclists_table_base0
	.long	.Ldebug_loc7-.Lloclists_table_base0
	.long	.Ldebug_loc8-.Lloclists_table_base0
	.long	.Ldebug_loc9-.Lloclists_table_base0
	.long	.Ldebug_loc10-.Lloclists_table_base0
.Ldebug_loc0:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin0-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp2-.Lfunc_begin0           #   ending offset
	.byte	1                               # Loc expr size
	.byte	85                              # super-register DW_OP_reg5
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp2-.Lfunc_begin0           #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	4                               # Loc expr size
	.byte	163                             # DW_OP_entry_value
	.byte	1                               # 1
	.byte	85                              # super-register DW_OP_reg5
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc1:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Lfunc_begin0-.Lfunc_begin0    #   starting offset
	.uleb128 .Ltmp4-.Lfunc_begin0           #   ending offset
	.byte	1                               # Loc expr size
	.byte	84                              # DW_OP_reg4
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp4-.Lfunc_begin0           #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	4                               # Loc expr size
	.byte	163                             # DW_OP_entry_value
	.byte	1                               # 1
	.byte	84                              # DW_OP_reg4
	.byte	159                             # DW_OP_stack_value
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc2:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp28-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp34-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp37-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	83                              # super-register DW_OP_reg3
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc3:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp0-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp25-.Lfunc_begin0          #   ending offset
	.byte	2                               # Loc expr size
	.byte	48                              # DW_OP_lit0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp25-.Lfunc_begin0          #   starting offset
	.uleb128 .Lfunc_end0-.Lfunc_begin0      #   ending offset
	.byte	2                               # Loc expr size
	.byte	119                             # DW_OP_breg7
	.byte	12                              # 12
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc4:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp1-.Lfunc_begin0           #   starting offset
	.uleb128 .Ltmp7-.Lfunc_begin0           #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp10-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp11-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	17                              # DW_OP_consts
	.byte	0                               # 0
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp11-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp12-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	80                              # DW_OP_reg0
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp12-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp13-.Lfunc_begin0          #   ending offset
	.byte	5                               # Loc expr size
	.byte	112                             # DW_OP_breg0
	.byte	0                               # 0
	.byte	49                              # DW_OP_lit1
	.byte	33                              # DW_OP_or
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp13-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp14-.Lfunc_begin0          #   ending offset
	.byte	5                               # Loc expr size
	.byte	112                             # DW_OP_breg0
	.byte	0                               # 0
	.byte	50                              # DW_OP_lit2
	.byte	33                              # DW_OP_or
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp14-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp16-.Lfunc_begin0          #   ending offset
	.byte	5                               # Loc expr size
	.byte	112                             # DW_OP_breg0
	.byte	0                               # 0
	.byte	51                              # DW_OP_lit3
	.byte	33                              # DW_OP_or
	.byte	159                             # DW_OP_stack_value
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp16-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp23-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	80                              # DW_OP_reg0
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc5:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp21-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp38-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	95                              # DW_OP_reg15
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	94                              # DW_OP_reg14
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp38-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp39-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	95                              # DW_OP_reg15
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc6:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp10-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp24-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	83                              # super-register DW_OP_reg3
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc7:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp25-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp29-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	83                              # DW_OP_reg3
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp29-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp32-.Lfunc_begin0          #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc8:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp28-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp38-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	95                              # DW_OP_reg15
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	94                              # DW_OP_reg14
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp38-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp39-.Lfunc_begin0          #   ending offset
	.byte	3                               # Loc expr size
	.byte	95                              # DW_OP_reg15
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc9:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp28-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp29-.Lfunc_begin0          #   ending offset
	.byte	6                               # Loc expr size
	.byte	83                              # DW_OP_reg3
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp29-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp32-.Lfunc_begin0          #   ending offset
	.byte	5                               # Loc expr size
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	92                              # DW_OP_reg12
	.byte	147                             # DW_OP_piece
	.byte	8                               # 8
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_loc10:
	.byte	4                               # DW_LLE_offset_pair
	.uleb128 .Ltmp35-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp36-.Lfunc_begin0          #   ending offset
	.byte	1                               # Loc expr size
	.byte	97                              # DW_OP_reg17
	.byte	0                               # DW_LLE_end_of_list
.Ldebug_list_header_end0:
	.section	.debug_abbrev,"",@progbits
	.byte	1                               # Abbreviation Code
	.byte	17                              # DW_TAG_compile_unit
	.byte	1                               # DW_CHILDREN_yes
	.byte	37                              # DW_AT_producer
	.byte	37                              # DW_FORM_strx1
	.byte	19                              # DW_AT_language
	.byte	5                               # DW_FORM_data2
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	114                             # DW_AT_str_offsets_base
	.byte	23                              # DW_FORM_sec_offset
	.byte	16                              # DW_AT_stmt_list
	.byte	23                              # DW_FORM_sec_offset
	.byte	27                              # DW_AT_comp_dir
	.byte	37                              # DW_FORM_strx1
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	115                             # DW_AT_addr_base
	.byte	23                              # DW_FORM_sec_offset
	.byte	116                             # DW_AT_rnglists_base
	.byte	23                              # DW_FORM_sec_offset
	.ascii	"\214\001"                      # DW_AT_loclists_base
	.byte	23                              # DW_FORM_sec_offset
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	2                               # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	32                              # DW_AT_inline
	.byte	33                              # DW_FORM_implicit_const
	.byte	1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	3                               # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	4                               # Abbreviation Code
	.byte	22                              # DW_TAG_typedef
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	5                               # Abbreviation Code
	.byte	19                              # DW_TAG_structure_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	6                               # Abbreviation Code
	.byte	13                              # DW_TAG_member
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	56                              # DW_AT_data_member_location
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	7                               # Abbreviation Code
	.byte	36                              # DW_TAG_base_type
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	62                              # DW_AT_encoding
	.byte	11                              # DW_FORM_data1
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	8                               # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	9                               # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	64                              # DW_AT_frame_base
	.byte	24                              # DW_FORM_exprloc
	.byte	122                             # DW_AT_call_all_calls
	.byte	25                              # DW_FORM_flag_present
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	10                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	34                              # DW_FORM_loclistx
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	11                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	12                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	34                              # DW_FORM_loclistx
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	11                              # DW_FORM_data1
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	13                              # Abbreviation Code
	.byte	29                              # DW_TAG_inlined_subroutine
	.byte	1                               # DW_CHILDREN_yes
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	17                              # DW_AT_low_pc
	.byte	27                              # DW_FORM_addrx
	.byte	18                              # DW_AT_high_pc
	.byte	6                               # DW_FORM_data4
	.byte	88                              # DW_AT_call_file
	.byte	11                              # DW_FORM_data1
	.byte	89                              # DW_AT_call_line
	.byte	11                              # DW_FORM_data1
	.byte	87                              # DW_AT_call_column
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	14                              # Abbreviation Code
	.byte	52                              # DW_TAG_variable
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	15                              # Abbreviation Code
	.byte	29                              # DW_TAG_inlined_subroutine
	.byte	1                               # DW_CHILDREN_yes
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	85                              # DW_AT_ranges
	.byte	35                              # DW_FORM_rnglistx
	.byte	88                              # DW_AT_call_file
	.byte	11                              # DW_FORM_data1
	.byte	89                              # DW_AT_call_line
	.byte	11                              # DW_FORM_data1
	.byte	87                              # DW_AT_call_column
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	16                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	34                              # DW_FORM_loclistx
	.byte	49                              # DW_AT_abstract_origin
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	17                              # Abbreviation Code
	.byte	72                              # DW_TAG_call_site
	.byte	1                               # DW_CHILDREN_yes
	.byte	127                             # DW_AT_call_origin
	.byte	19                              # DW_FORM_ref4
	.byte	125                             # DW_AT_call_return_pc
	.byte	27                              # DW_FORM_addrx
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	18                              # Abbreviation Code
	.byte	73                              # DW_TAG_call_site_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	2                               # DW_AT_location
	.byte	24                              # DW_FORM_exprloc
	.byte	126                             # DW_AT_call_value
	.byte	24                              # DW_FORM_exprloc
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	19                              # Abbreviation Code
	.byte	46                              # DW_TAG_subprogram
	.byte	1                               # DW_CHILDREN_yes
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	58                              # DW_AT_decl_file
	.byte	11                              # DW_FORM_data1
	.byte	59                              # DW_AT_decl_line
	.byte	5                               # DW_FORM_data2
	.byte	39                              # DW_AT_prototyped
	.byte	25                              # DW_FORM_flag_present
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	60                              # DW_AT_declaration
	.byte	25                              # DW_FORM_flag_present
	.byte	63                              # DW_AT_external
	.byte	25                              # DW_FORM_flag_present
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	20                              # Abbreviation Code
	.byte	5                               # DW_TAG_formal_parameter
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	21                              # Abbreviation Code
	.byte	15                              # DW_TAG_pointer_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	22                              # Abbreviation Code
	.byte	1                               # DW_TAG_array_type
	.byte	1                               # DW_CHILDREN_yes
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	23                              # Abbreviation Code
	.byte	33                              # DW_TAG_subrange_type
	.byte	0                               # DW_CHILDREN_no
	.byte	73                              # DW_AT_type
	.byte	19                              # DW_FORM_ref4
	.byte	55                              # DW_AT_count
	.byte	5                               # DW_FORM_data2
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	24                              # Abbreviation Code
	.byte	36                              # DW_TAG_base_type
	.byte	0                               # DW_CHILDREN_no
	.byte	3                               # DW_AT_name
	.byte	37                              # DW_FORM_strx1
	.byte	11                              # DW_AT_byte_size
	.byte	11                              # DW_FORM_data1
	.byte	62                              # DW_AT_encoding
	.byte	11                              # DW_FORM_data1
	.byte	0                               # EOM(1)
	.byte	0                               # EOM(2)
	.byte	0                               # EOM(3)
	.section	.debug_info,"",@progbits
.Lcu_begin0:
	.long	.Ldebug_info_end0-.Ldebug_info_start0 # Length of Unit
.Ldebug_info_start0:
	.short	5                               # DWARF version number
	.byte	1                               # DWARF Unit Type
	.byte	8                               # Address Size (in bytes)
	.long	.debug_abbrev                   # Offset Into Abbrev. Section
	.byte	1                               # Abbrev [1] 0xc:0x1ee DW_TAG_compile_unit
	.byte	0                               # DW_AT_producer
	.short	12                              # DW_AT_language
	.byte	1                               # DW_AT_name
	.long	.Lstr_offsets_base0             # DW_AT_str_offsets_base
	.long	.Lline_table_start0             # DW_AT_stmt_list
	.byte	2                               # DW_AT_comp_dir
	.byte	0                               # DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.long	.Laddr_table_base0              # DW_AT_addr_base
	.long	.Lrnglists_table_base0          # DW_AT_rnglists_base
	.long	.Lloclists_table_base0          # DW_AT_loclists_base
	.byte	2                               # Abbrev [2] 0x2b:0x11 DW_TAG_subprogram
	.byte	3                               # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	69                              # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	60                              # DW_AT_type
                                        # DW_AT_inline
	.byte	3                               # Abbrev [3] 0x33:0x8 DW_TAG_variable
	.byte	11                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	70                              # DW_AT_decl_line
	.long	68                              # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	4                               # Abbrev [4] 0x3c:0x8 DW_TAG_typedef
	.long	68                              # DW_AT_type
	.byte	10                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	66                              # DW_AT_decl_line
	.byte	5                               # Abbrev [5] 0x44:0x18 DW_TAG_structure_type
	.byte	9                               # DW_AT_name
	.byte	16                              # DW_AT_byte_size
	.byte	3                               # DW_AT_decl_file
	.byte	11                              # DW_AT_decl_line
	.byte	6                               # Abbrev [6] 0x49:0x9 DW_TAG_member
	.byte	4                               # DW_AT_name
	.long	92                              # DW_AT_type
	.byte	3                               # DW_AT_decl_file
	.byte	16                              # DW_AT_decl_line
	.byte	0                               # DW_AT_data_member_location
	.byte	6                               # Abbrev [6] 0x52:0x9 DW_TAG_member
	.byte	7                               # DW_AT_name
	.long	104                             # DW_AT_type
	.byte	3                               # DW_AT_decl_file
	.byte	21                              # DW_AT_decl_line
	.byte	8                               # DW_AT_data_member_location
	.byte	0                               # End Of Children Mark
	.byte	4                               # Abbrev [4] 0x5c:0x8 DW_TAG_typedef
	.long	100                             # DW_AT_type
	.byte	6                               # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	160                             # DW_AT_decl_line
	.byte	7                               # Abbrev [7] 0x64:0x4 DW_TAG_base_type
	.byte	5                               # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	8                               # DW_AT_byte_size
	.byte	4                               # Abbrev [4] 0x68:0x8 DW_TAG_typedef
	.long	100                             # DW_AT_type
	.byte	8                               # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	197                             # DW_AT_decl_line
	.byte	2                               # Abbrev [2] 0x70:0x19 DW_TAG_subprogram
	.byte	12                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	82                              # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	137                             # DW_AT_type
                                        # DW_AT_inline
	.byte	8                               # Abbrev [8] 0x78:0x8 DW_TAG_formal_parameter
	.byte	14                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	82                              # DW_AT_decl_line
	.long	60                              # DW_AT_type
	.byte	8                               # Abbrev [8] 0x80:0x8 DW_TAG_formal_parameter
	.byte	15                              # DW_AT_name
	.byte	1                               # DW_AT_decl_file
	.byte	82                              # DW_AT_decl_line
	.long	60                              # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	7                               # Abbrev [7] 0x89:0x4 DW_TAG_base_type
	.byte	13                              # DW_AT_name
	.byte	4                               # DW_AT_encoding
	.byte	8                               # DW_AT_byte_size
	.byte	9                               # Abbrev [9] 0x8d:0xf8 DW_TAG_subprogram
	.byte	0                               # DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       # DW_AT_high_pc
	.byte	1                               # DW_AT_frame_base
	.byte	87
                                        # DW_AT_call_all_calls
	.byte	22                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	49                              # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	409                             # DW_AT_type
                                        # DW_AT_external
	.byte	10                              # Abbrev [10] 0x9c:0x9 DW_TAG_formal_parameter
	.byte	0                               # DW_AT_location
	.byte	29                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	49                              # DW_AT_decl_line
	.long	409                             # DW_AT_type
	.byte	10                              # Abbrev [10] 0xa5:0x9 DW_TAG_formal_parameter
	.byte	1                               # DW_AT_location
	.byte	30                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	49                              # DW_AT_decl_line
	.long	491                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xae:0xd DW_TAG_variable
	.byte	4                               # DW_AT_location
	.byte	145
	.asciz	"\240\300"
	.byte	23                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	50                              # DW_AT_decl_line
	.long	458                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xbb:0xc DW_TAG_variable
	.byte	3                               # DW_AT_location
	.byte	145
	.ascii	"\240 "
	.byte	27                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	51                              # DW_AT_decl_line
	.long	458                             # DW_AT_type
	.byte	11                              # Abbrev [11] 0xc7:0xb DW_TAG_variable
	.byte	2                               # DW_AT_location
	.byte	145
	.byte	32
	.byte	28                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	52                              # DW_AT_decl_line
	.long	458                             # DW_AT_type
	.byte	12                              # Abbrev [12] 0xd2:0x9 DW_TAG_variable
	.byte	2                               # DW_AT_location
	.byte	32                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	53                              # DW_AT_decl_line
	.long	471                             # DW_AT_type
	.byte	12                              # Abbrev [12] 0xdb:0x9 DW_TAG_variable
	.byte	3                               # DW_AT_location
	.byte	33                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	56                              # DW_AT_decl_line
	.long	454                             # DW_AT_type
	.byte	12                              # Abbrev [12] 0xe4:0x9 DW_TAG_variable
	.byte	4                               # DW_AT_location
	.byte	34                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	55                              # DW_AT_decl_line
	.long	409                             # DW_AT_type
	.byte	12                              # Abbrev [12] 0xed:0x9 DW_TAG_variable
	.byte	5                               # DW_AT_location
	.byte	35                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	67                              # DW_AT_decl_line
	.long	60                              # DW_AT_type
	.byte	12                              # Abbrev [12] 0xf6:0x9 DW_TAG_variable
	.byte	6                               # DW_AT_location
	.byte	36                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	55                              # DW_AT_decl_line
	.long	409                             # DW_AT_type
	.byte	12                              # Abbrev [12] 0xff:0x9 DW_TAG_variable
	.byte	7                               # DW_AT_location
	.byte	37                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	75                              # DW_AT_decl_line
	.long	60                              # DW_AT_type
	.byte	12                              # Abbrev [12] 0x108:0x9 DW_TAG_variable
	.byte	10                              # DW_AT_location
	.byte	38                              # DW_AT_name
	.byte	0                               # DW_AT_decl_file
	.byte	80                              # DW_AT_decl_line
	.long	137                             # DW_AT_type
	.byte	13                              # Abbrev [13] 0x111:0x16 DW_TAG_inlined_subroutine
	.long	43                              # DW_AT_abstract_origin
	.byte	1                               # DW_AT_low_pc
	.long	.Ltmp10-.Ltmp8                  # DW_AT_high_pc
	.byte	0                               # DW_AT_call_file
	.byte	67                              # DW_AT_call_line
	.byte	24                              # DW_AT_call_column
	.byte	14                              # Abbrev [14] 0x11e:0x8 DW_TAG_variable
	.byte	2                               # DW_AT_location
	.byte	145
	.byte	16
	.long	51                              # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	13                              # Abbrev [13] 0x127:0x16 DW_TAG_inlined_subroutine
	.long	43                              # DW_AT_abstract_origin
	.byte	2                               # DW_AT_low_pc
	.long	.Ltmp26-.Ltmp22                 # DW_AT_high_pc
	.byte	0                               # DW_AT_call_file
	.byte	75                              # DW_AT_call_line
	.byte	24                              # DW_AT_call_column
	.byte	14                              # Abbrev [14] 0x134:0x8 DW_TAG_variable
	.byte	2                               # DW_AT_location
	.byte	145
	.byte	16
	.long	51                              # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	15                              # Abbrev [15] 0x13d:0x16 DW_TAG_inlined_subroutine
	.long	112                             # DW_AT_abstract_origin
	.byte	0                               # DW_AT_ranges
	.byte	0                               # DW_AT_call_file
	.byte	80                              # DW_AT_call_line
	.byte	23                              # DW_AT_call_column
	.byte	16                              # Abbrev [16] 0x146:0x6 DW_TAG_formal_parameter
	.byte	8                               # DW_AT_location
	.long	120                             # DW_AT_abstract_origin
	.byte	16                              # Abbrev [16] 0x14c:0x6 DW_TAG_formal_parameter
	.byte	9                               # DW_AT_location
	.long	128                             # DW_AT_abstract_origin
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0x153:0x12 DW_TAG_call_site
	.long	389                             # DW_AT_call_origin
	.byte	3                               # DW_AT_call_return_pc
	.byte	18                              # Abbrev [18] 0x159:0x5 DW_TAG_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	85
	.byte	1                               # DW_AT_call_value
	.byte	49
	.byte	18                              # Abbrev [18] 0x15e:0x6 DW_TAG_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	84
	.byte	2                               # DW_AT_call_value
	.byte	145
	.byte	16
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0x165:0x12 DW_TAG_call_site
	.long	389                             # DW_AT_call_origin
	.byte	4                               # DW_AT_call_return_pc
	.byte	18                              # Abbrev [18] 0x16b:0x5 DW_TAG_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	85
	.byte	1                               # DW_AT_call_value
	.byte	49
	.byte	18                              # Abbrev [18] 0x170:0x6 DW_TAG_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	84
	.byte	2                               # DW_AT_call_value
	.byte	145
	.byte	16
	.byte	0                               # End Of Children Mark
	.byte	17                              # Abbrev [17] 0x177:0xd DW_TAG_call_site
	.long	434                             # DW_AT_call_origin
	.byte	5                               # DW_AT_call_return_pc
	.byte	18                              # Abbrev [18] 0x17d:0x6 DW_TAG_call_site_parameter
	.byte	1                               # DW_AT_location
	.byte	85
	.byte	2                               # DW_AT_call_value
	.byte	145
	.byte	12
	.byte	0                               # End Of Children Mark
	.byte	0                               # End Of Children Mark
	.byte	19                              # Abbrev [19] 0x185:0x14 DW_TAG_subprogram
	.byte	16                              # DW_AT_name
	.byte	4                               # DW_AT_decl_file
	.short	279                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	409                             # DW_AT_type
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	20                              # Abbrev [20] 0x18e:0x5 DW_TAG_formal_parameter
	.long	413                             # DW_AT_type
	.byte	20                              # Abbrev [20] 0x193:0x5 DW_TAG_formal_parameter
	.long	429                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	7                               # Abbrev [7] 0x199:0x4 DW_TAG_base_type
	.byte	17                              # DW_AT_name
	.byte	5                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	4                               # Abbrev [4] 0x19d:0x8 DW_TAG_typedef
	.long	421                             # DW_AT_type
	.byte	19                              # DW_AT_name
	.byte	5                               # DW_AT_decl_file
	.byte	7                               # DW_AT_decl_line
	.byte	4                               # Abbrev [4] 0x1a5:0x8 DW_TAG_typedef
	.long	409                             # DW_AT_type
	.byte	18                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	169                             # DW_AT_decl_line
	.byte	21                              # Abbrev [21] 0x1ad:0x5 DW_TAG_pointer_type
	.long	68                              # DW_AT_type
	.byte	19                              # Abbrev [19] 0x1b2:0xf DW_TAG_subprogram
	.byte	20                              # DW_AT_name
	.byte	6                               # DW_AT_decl_file
	.short	460                             # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	409                             # DW_AT_type
                                        # DW_AT_declaration
                                        # DW_AT_external
	.byte	20                              # Abbrev [20] 0x1bb:0x5 DW_TAG_formal_parameter
	.long	449                             # DW_AT_type
	.byte	0                               # End Of Children Mark
	.byte	21                              # Abbrev [21] 0x1c1:0x5 DW_TAG_pointer_type
	.long	454                             # DW_AT_type
	.byte	7                               # Abbrev [7] 0x1c6:0x4 DW_TAG_base_type
	.byte	21                              # DW_AT_name
	.byte	7                               # DW_AT_encoding
	.byte	4                               # DW_AT_byte_size
	.byte	22                              # Abbrev [22] 0x1ca:0xd DW_TAG_array_type
	.long	471                             # DW_AT_type
	.byte	23                              # Abbrev [23] 0x1cf:0x7 DW_TAG_subrange_type
	.long	487                             # DW_AT_type
	.short	1024                            # DW_AT_count
	.byte	0                               # End Of Children Mark
	.byte	4                               # Abbrev [4] 0x1d7:0x8 DW_TAG_typedef
	.long	479                             # DW_AT_type
	.byte	25                              # DW_AT_name
	.byte	7                               # DW_AT_decl_file
	.byte	26                              # DW_AT_decl_line
	.byte	4                               # Abbrev [4] 0x1df:0x8 DW_TAG_typedef
	.long	454                             # DW_AT_type
	.byte	24                              # DW_AT_name
	.byte	2                               # DW_AT_decl_file
	.byte	42                              # DW_AT_decl_line
	.byte	24                              # Abbrev [24] 0x1e7:0x4 DW_TAG_base_type
	.byte	26                              # DW_AT_name
	.byte	8                               # DW_AT_byte_size
	.byte	7                               # DW_AT_encoding
	.byte	21                              # Abbrev [21] 0x1eb:0x5 DW_TAG_pointer_type
	.long	496                             # DW_AT_type
	.byte	21                              # Abbrev [21] 0x1f0:0x5 DW_TAG_pointer_type
	.long	501                             # DW_AT_type
	.byte	7                               # Abbrev [7] 0x1f5:0x4 DW_TAG_base_type
	.byte	31                              # DW_AT_name
	.byte	6                               # DW_AT_encoding
	.byte	1                               # DW_AT_byte_size
	.byte	0                               # End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_rnglists,"",@progbits
	.long	.Ldebug_list_header_end1-.Ldebug_list_header_start1 # Length
.Ldebug_list_header_start1:
	.short	5                               # Version
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
	.long	1                               # Offset entry count
.Lrnglists_table_base0:
	.long	.Ldebug_ranges0-.Lrnglists_table_base0
.Ldebug_ranges0:
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp28-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp30-.Lfunc_begin0          #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp31-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp33-.Lfunc_begin0          #   ending offset
	.byte	4                               # DW_RLE_offset_pair
	.uleb128 .Ltmp34-.Lfunc_begin0          #   starting offset
	.uleb128 .Ltmp35-.Lfunc_begin0          #   ending offset
	.byte	0                               # DW_RLE_end_of_list
.Ldebug_list_header_end1:
	.section	.debug_str_offsets,"",@progbits
	.long	160                             # Length of String Offsets Set
	.short	5
	.short	0
.Lstr_offsets_base0:
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"Ubuntu clang version 14.0.0-1ubuntu1.1" # string offset=0
.Linfo_string1:
	.asciz	"loop.c"                        # string offset=39
.Linfo_string2:
	.asciz	"/mnt/d/MIT6.172/hw3/homework3" # string offset=46
.Linfo_string3:
	.asciz	"gettime"                       # string offset=76
.Linfo_string4:
	.asciz	"tv_sec"                        # string offset=84
.Linfo_string5:
	.asciz	"long"                          # string offset=91
.Linfo_string6:
	.asciz	"__time_t"                      # string offset=96
.Linfo_string7:
	.asciz	"tv_nsec"                       # string offset=105
.Linfo_string8:
	.asciz	"__syscall_slong_t"             # string offset=113
.Linfo_string9:
	.asciz	"timespec"                      # string offset=131
.Linfo_string10:
	.asciz	"fasttime_t"                    # string offset=140
.Linfo_string11:
	.asciz	"s"                             # string offset=151
.Linfo_string12:
	.asciz	"tdiff"                         # string offset=153
.Linfo_string13:
	.asciz	"double"                        # string offset=159
.Linfo_string14:
	.asciz	"start"                         # string offset=166
.Linfo_string15:
	.asciz	"end"                           # string offset=172
.Linfo_string16:
	.asciz	"clock_gettime"                 # string offset=176
.Linfo_string17:
	.asciz	"int"                           # string offset=190
.Linfo_string18:
	.asciz	"__clockid_t"                   # string offset=194
.Linfo_string19:
	.asciz	"clockid_t"                     # string offset=206
.Linfo_string20:
	.asciz	"rand_r"                        # string offset=216
.Linfo_string21:
	.asciz	"unsigned int"                  # string offset=223
.Linfo_string22:
	.asciz	"main"                          # string offset=236
.Linfo_string23:
	.asciz	"A"                             # string offset=241
.Linfo_string24:
	.asciz	"__uint32_t"                    # string offset=243
.Linfo_string25:
	.asciz	"uint32_t"                      # string offset=254
.Linfo_string26:
	.asciz	"__ARRAY_SIZE_TYPE__"           # string offset=263
.Linfo_string27:
	.asciz	"B"                             # string offset=283
.Linfo_string28:
	.asciz	"C"                             # string offset=285
.Linfo_string29:
	.asciz	"argc"                          # string offset=287
.Linfo_string30:
	.asciz	"argv"                          # string offset=292
.Linfo_string31:
	.asciz	"char"                          # string offset=297
.Linfo_string32:
	.asciz	"total"                         # string offset=302
.Linfo_string33:
	.asciz	"seed"                          # string offset=308
.Linfo_string34:
	.asciz	"j"                             # string offset=313
.Linfo_string35:
	.asciz	"time1"                         # string offset=315
.Linfo_string36:
	.asciz	"i"                             # string offset=321
.Linfo_string37:
	.asciz	"time2"                         # string offset=323
.Linfo_string38:
	.asciz	"elapsedf"                      # string offset=329
	.section	.debug_str_offsets,"",@progbits
	.long	.Linfo_string0
	.long	.Linfo_string1
	.long	.Linfo_string2
	.long	.Linfo_string3
	.long	.Linfo_string4
	.long	.Linfo_string5
	.long	.Linfo_string6
	.long	.Linfo_string7
	.long	.Linfo_string8
	.long	.Linfo_string9
	.long	.Linfo_string10
	.long	.Linfo_string11
	.long	.Linfo_string12
	.long	.Linfo_string13
	.long	.Linfo_string14
	.long	.Linfo_string15
	.long	.Linfo_string16
	.long	.Linfo_string17
	.long	.Linfo_string18
	.long	.Linfo_string19
	.long	.Linfo_string20
	.long	.Linfo_string21
	.long	.Linfo_string22
	.long	.Linfo_string23
	.long	.Linfo_string24
	.long	.Linfo_string25
	.long	.Linfo_string26
	.long	.Linfo_string27
	.long	.Linfo_string28
	.long	.Linfo_string29
	.long	.Linfo_string30
	.long	.Linfo_string31
	.long	.Linfo_string32
	.long	.Linfo_string33
	.long	.Linfo_string34
	.long	.Linfo_string35
	.long	.Linfo_string36
	.long	.Linfo_string37
	.long	.Linfo_string38
	.section	.debug_addr,"",@progbits
	.long	.Ldebug_addr_end0-.Ldebug_addr_start0 # Length of contribution
.Ldebug_addr_start0:
	.short	5                               # DWARF version number
	.byte	8                               # Address size
	.byte	0                               # Segment selector size
.Laddr_table_base0:
	.quad	.Lfunc_begin0
	.quad	.Ltmp8
	.quad	.Ltmp22
	.quad	.Ltmp9
	.quad	.Ltmp23
	.quad	.Ltmp27
.Ldebug_addr_end0:
	.ident	"Ubuntu clang version 14.0.0-1ubuntu1.1"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.section	.debug_line,"",@progbits
.Lline_table_start0:
