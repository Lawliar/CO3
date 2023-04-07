3|  %21 = call i1 @_sym_build_equal(i1 %16, i1 %20), !dbg !60
6|  call void @_sym_build_write_memory(i32 %31, i32 1, i1 false, i8 1, i16 6), !dbg !63
10|  %20 = call i1 @_sym_build_integer(i32 1, i8 4, i16 10), !dbg !60
1|  %9 = call i1 @_sym_build_read_memory(i32 %8, i32 1, i8 1, i16 1), !dbg !58
4|  call void @_sym_build_path_constraint(i1 %23, i1 %24, i16 4), !dbg !61
8|  call void @_sym_set_return_expression(i1 %34), !dbg !64
2|  %14 = call i1 @_sym_build_zext(i1 %9, i8 24), !dbg !58
7|  %34 = call i1 @_sym_build_read_memory(i32 %33, i32 1, i8 1, i16 7), !dbg !64
5|  call void @_sym_build_write_memory(i32 %29, i32 1, i1 false, i8 1, i16 5), !dbg !62
9|  %16 = phi i1 [ false, %0 ], [ %14, %13 ], !dbg !58|false,root
11|  %23 = phi i1 [ false, %15 ], [ %21, %19 ], !dbg !60|false,root
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
; Function Attrs: noinline nounwind optnone
define internal zeroext i8 @modbusSlaveCheckID() #0 !dbg !125 {
  %1 = alloca [0 x i8], align 1
  %2 = alloca [0 x i8], align 1
  %3 = alloca [0 x i8], align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %1, align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %2, align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %3, align 1
  %4 = getelementptr [0 x i8], [0 x i8]* %1, i8 0, i8 0
  %5 = getelementptr [0 x i8], [0 x i8]* %2, i8 0, i8 0
  %6 = getelementptr [0 x i8], [0 x i8]* %3, i8 0, i8 0
  %7 = alloca i8, align 1
  %8 = ptrtoint [256 x i8]* @modbusRxTxBuffer to i32
  %9 = call i1 @_sym_build_read_memory(i32 %8, i32 1, i8 1, i16 1), !dbg !126
  %10 = getelementptr [256 x i8], [256 x i8]* @modbusRxTxBuffer, i32 0, i32 0
  %11 = load i8, i8* %10, align 256, !dbg !126
  %12 = icmp eq i1 false, %9, !dbg !126
  br i1 %12, label %15, label %13, !dbg !126

13:                                               ; preds = %0
  %14 = call i1 @_sym_build_zext(i1 %9, i8 24), !dbg !126
  br label %15, !dbg !126

15:                                               ; preds = %0, %13
  %16 = phi i1 [ false, %0 ], [ %14, %13 ], !dbg !126
  %17 = zext i8 %11 to i32, !dbg !126
  %18 = icmp eq i1 false, %16, !dbg !128
  br i1 %18, label %22, label %19, !dbg !128

19:                                               ; preds = %15
  %20 = call i1 @_sym_build_integer(i32 1, i8 4, i16 10), !dbg !128
  %21 = call i1 @_sym_build_equal(i1 %16, i1 %20), !dbg !128
  br label %22, !dbg !128

22:                                               ; preds = %15, %19
  %23 = phi i1 [ false, %15 ], [ %21, %19 ], !dbg !128
  %24 = icmp eq i32 %17, 1, !dbg !128
  %25 = icmp eq i1 false, %23, !dbg !129
  br i1 %25, label %27, label %26, !dbg !129

26:                                               ; preds = %22
  call void @_sym_build_path_constraint(i1 %23, i1 %24, i16 4), !dbg !129
  br label %27, !dbg !129

27:                                               ; preds = %22, %26
  br i1 %24, label %28, label %30, !dbg !129

28:                                               ; preds = %27
  %29 = ptrtoint i8* %7 to i32, !dbg !130
  call void @_sym_build_write_memory(i32 %29, i32 1, i1 false, i8 1, i16 5), !dbg !130
  store i8 1, i8* %7, align 1, !dbg !130
  br label %32, !dbg !130

30:                                               ; preds = %27
  %31 = ptrtoint i8* %7 to i32, !dbg !131
  call void @_sym_build_write_memory(i32 %31, i32 1, i1 false, i8 1, i16 6), !dbg !131
  store i8 0, i8* %7, align 1, !dbg !131
  br label %32, !dbg !131

32:                                               ; preds = %30, %28
  %33 = ptrtoint i8* %7 to i32, !dbg !132
  %34 = call i1 @_sym_build_read_memory(i32 %33, i32 1, i8 1, i16 7), !dbg !132
  %35 = load i8, i8* %7, align 1, !dbg !132
  call void @_sym_set_return_expression(i1 %34), !dbg !132
  ret i8 %35, !dbg !132
}

