9|  call void @_sym_build_write_memory(i32 %42, i32 1, i1 %39, i8 1, i16 9), !dbg !62
3|  %13 = call i1 @_sym_build_read_memory(i32 %12, i32 1, i8 1, i16 3), !dbg !60
12|  %24 = call i1 @_sym_build_integer(i32 128, i8 4, i16 12), !dbg !60
10|  call void @_sym_set_return_expression(i1 false), !dbg !63
8|  %39 = call i1 @_sym_build_read_memory(i32 %38, i32 1, i8 1, i16 8), !dbg !61
4|  %18 = call i1 @_sym_build_zext(i1 %13, i8 24), !dbg !60
5|  %25 = call i1 @_sym_build_or(i1 %20, i1 %24), !dbg !60
7|  call void @_sym_build_write_memory(i32 %36, i32 1, i1 %33, i8 1, i16 7), !dbg !60
2|  call void @_sym_build_write_memory(i32 %10, i32 1, i1 %2, i8 1, i16 2)
1|  %2 = call i1 @_sym_get_parameter_expression(i8 0)
6|  %31 = call i1 @_sym_build_trunc(i1 %27, i8 8), !dbg !60
11|  %20 = phi i1 [ false, %1 ], [ %18, %17 ], !dbg !60|false,root
14|  %33 = phi i1 [ false, %26 ], [ %31, %30 ], !dbg !60|false,root
13|  %27 = phi i1 [ false, %19 ], [ %25, %23 ], !dbg !60|false,root
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
; Function Attrs: noinline nounwind optnone
define internal zeroext i16 @modbusSlaveErrorSet(i8 zeroext %0) #0 !dbg !372 {
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
  %9 = alloca i8, align 1
  %10 = ptrtoint i8* %9 to i32
  call void @_sym_build_write_memory(i32 %10, i32 1, i1 %2, i8 1, i16 2)
  store i8 %0, i8* %9, align 1
  call void @llvm.dbg.declare(metadata i8* %9, metadata !375, metadata !DIExpression()), !dbg !376
  %11 = getelementptr [256 x i8], [256 x i8]* @modbusRxTxBuffer, i32 0, i32 1
  %12 = ptrtoint i8* %11 to i32
  %13 = call i1 @_sym_build_read_memory(i32 %12, i32 1, i8 1, i16 3), !dbg !377
  %14 = getelementptr [256 x i8], [256 x i8]* @modbusRxTxBuffer, i32 0, i32 1
  %15 = load i8, i8* %14, align 1, !dbg !377
  %16 = icmp eq i1 false, %13, !dbg !377
  br i1 %16, label %19, label %17, !dbg !377

17:                                               ; preds = %1
  %18 = call i1 @_sym_build_zext(i1 %13, i8 24), !dbg !377
  br label %19, !dbg !377

19:                                               ; preds = %1, %17
  %20 = phi i1 [ false, %1 ], [ %18, %17 ], !dbg !377
  %21 = zext i8 %15 to i32, !dbg !377
  %22 = icmp eq i1 false, %20, !dbg !377
  br i1 %22, label %26, label %23, !dbg !377

23:                                               ; preds = %19
  %24 = call i1 @_sym_build_integer(i32 128, i8 4, i16 12), !dbg !377
  %25 = call i1 @_sym_build_or(i1 %20, i1 %24), !dbg !377
  br label %26, !dbg !377

26:                                               ; preds = %19, %23
  %27 = phi i1 [ false, %19 ], [ %25, %23 ], !dbg !377
  %28 = or i32 %21, 128, !dbg !377
  %29 = icmp eq i1 false, %27, !dbg !377
  br i1 %29, label %32, label %30, !dbg !377

30:                                               ; preds = %26
  %31 = call i1 @_sym_build_trunc(i1 %27, i8 8), !dbg !377
  br label %32, !dbg !377

32:                                               ; preds = %26, %30
  %33 = phi i1 [ false, %26 ], [ %31, %30 ], !dbg !377
  %34 = trunc i32 %28 to i8, !dbg !377
  %35 = getelementptr [256 x i8], [256 x i8]* @modbusRxTxBuffer, i32 0, i32 1
  %36 = ptrtoint i8* %35 to i32
  call void @_sym_build_write_memory(i32 %36, i32 1, i1 %33, i8 1, i16 7), !dbg !377
  %37 = getelementptr [256 x i8], [256 x i8]* @modbusRxTxBuffer, i32 0, i32 1
  store i8 %34, i8* %37, align 1, !dbg !377
  %38 = ptrtoint i8* %9 to i32, !dbg !378
  %39 = call i1 @_sym_build_read_memory(i32 %38, i32 1, i8 1, i16 8), !dbg !378
  %40 = load i8, i8* %9, align 1, !dbg !378
  %41 = getelementptr [256 x i8], [256 x i8]* @modbusRxTxBuffer, i32 0, i32 2
  %42 = ptrtoint i8* %41 to i32
  call void @_sym_build_write_memory(i32 %42, i32 1, i1 %39, i8 1, i16 9), !dbg !379
  %43 = getelementptr [256 x i8], [256 x i8]* @modbusRxTxBuffer, i32 0, i32 2
  store i8 %40, i8* %43, align 2, !dbg !379
  call void @_sym_set_return_expression(i1 false), !dbg !380
  ret i16 5, !dbg !380
}

