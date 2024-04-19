7|  call void @_sym_set_return_expression(i1 %28), !dbg !63
10|  %24 = call i1 @_sym_build_integer(i32 %23, i8 4, i16 10), !dbg !61
3|  %12 = call i1 @_sym_build_read_memory(i32 %11, i32 2, i8 1, i16 3), !dbg !60
9|  %22 = call i1 @_sym_build_integer(i32 2, i8 4, i16 9), !dbg !61
6|  %26 = call i1 @_sym_build_add(i1 %25, i1 %24), !dbg !61
2|  call void @_sym_build_write_memory(i32 %10, i32 2, i1 %2, i8 1, i16 2)
5|  %25 = call i1 @_sym_build_mul(i1 %18, i1 %22), !dbg !61
4|  %16 = call i1 @_sym_build_zext(i1 %12, i8 16), !dbg !61
1|  %2 = call i1 @_sym_get_parameter_expression(i8 0)
8|  %18 = phi i1 [ false, %1 ], [ %16, %15 ], !dbg !61|false,root
11|  %28 = phi i1 [ false, %17 ], [ %26, %21 ], !dbg !61|false,root
BB:->BB
BB:->BB
BB:->BB
BB:->BB
; Function Attrs: noinline nounwind optnone
define dso_local i8* @modbusSlaveGetGata(i16 zeroext %0) #0 !dbg !65 {
  %2 = call i1 @_sym_get_parameter_expression(i8 0)
  %3 = alloca [0 x i8], align 1
  %4 = alloca [0 x i8], align 1
  %5 = alloca [0 x i8], align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %3, align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %4, align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %5, align 1
  %6 = getelementptr [0 x i8], [0 x i8]* %3, i8 0, i8 0
  %7 = getelementptr [0 x i8], [0 x i8]* %4, i8 0, i8 0
  %8 = getelementptr [0 x i8], [0 x i8]* %5, i8 0, i8 0
  %9 = alloca i16, align 2
  %10 = ptrtoint i16* %9 to i32
  call void @_sym_build_write_memory(i32 %10, i32 2, i1 %2, i8 1, i16 2)
  store i16 %0, i16* %9, align 2
  call void @llvm.dbg.declare(metadata i16* %9, metadata !68, metadata !DIExpression()), !dbg !69
  %11 = ptrtoint i16* %9 to i32, !dbg !70
  %12 = call i1 @_sym_build_read_memory(i32 %11, i32 2, i8 1, i16 3), !dbg !70
  %13 = load i16, i16* %9, align 2, !dbg !70
  %14 = icmp eq i1 false, %12, !dbg !71
  br i1 %14, label %17, label %15, !dbg !71

15:                                               ; preds = %1
  %16 = call i1 @_sym_build_zext(i1 %12, i8 16), !dbg !71
  br label %17, !dbg !71

17:                                               ; preds = %1, %15
  %18 = phi i1 [ false, %1 ], [ %16, %15 ], !dbg !71
  %19 = zext i16 %13 to i32, !dbg !71
  %20 = icmp eq i1 false, %18, !dbg !71
  br i1 %20, label %27, label %21, !dbg !71

21:                                               ; preds = %17
  %22 = call i1 @_sym_build_integer(i32 2, i8 4, i16 9), !dbg !71
  %23 = ptrtoint [128 x i16]* @modbusMemory to i32
  %24 = call i1 @_sym_build_integer(i32 %23, i8 4, i16 10), !dbg !71
  %25 = call i1 @_sym_build_mul(i1 %18, i1 %22), !dbg !71
  %26 = call i1 @_sym_build_add(i1 %25, i1 %24), !dbg !71
  br label %27, !dbg !71

27:                                               ; preds = %17, %21
  %28 = phi i1 [ false, %17 ], [ %26, %21 ], !dbg !71
  %29 = getelementptr inbounds [128 x i16], [128 x i16]* @modbusMemory, i32 0, i32 %19, !dbg !71
  %30 = bitcast i16* %29 to i8*, !dbg !72
  call void @_sym_set_return_expression(i1 %28), !dbg !73
  ret i8* %30, !dbg !73
}

