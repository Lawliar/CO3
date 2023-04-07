3|  %21 = call i1 @_sym_build_signed_greater_than(i1 %16, i1 %20), !dbg !60
2|  %14 = call i1 @_sym_build_zext(i1 %9, i8 16), !dbg !58
1|  %9 = call i1 @_sym_build_read_memory(i32 %8, i32 2, i8 1, i16 1), !dbg !58
5|  call void @_sym_build_write_memory(i32 %29, i32 1, i1 false, i8 1, i16 5), !dbg !62
10|  %20 = call i1 @_sym_build_integer(i32 5, i8 4, i16 10), !dbg !60
6|  call void @_sym_build_write_memory(i32 %31, i32 1, i1 false, i8 1, i16 6), !dbg !63
8|  call void @_sym_set_return_expression(i1 %34), !dbg !64
7|  %34 = call i1 @_sym_build_read_memory(i32 %33, i32 1, i8 1, i16 7), !dbg !64
4|  call void @_sym_build_path_constraint(i1 %23, i1 %24, i16 4), !dbg !61
11|  %23 = phi i1 [ false, %15 ], [ %21, %19 ], !dbg !60|false,root
9|  %16 = phi i1 [ false, %0 ], [ %14, %13 ], !dbg !58|false,root
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
; Function Attrs: noinline nounwind optnone
define internal zeroext i8 @modbusSlaveCheckFrameSize() #0 !dbg !111 {
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
  %8 = ptrtoint %struct.modbus_t* @modbusVars to i32
  %9 = call i1 @_sym_build_read_memory(i32 %8, i32 2, i8 1, i16 1), !dbg !114
  %10 = getelementptr %struct.modbus_t, %struct.modbus_t* @modbusVars, i32 0, i32 0
  %11 = load i16, i16* %10, align 32, !dbg !114
  %12 = icmp eq i1 false, %9, !dbg !114
  br i1 %12, label %15, label %13, !dbg !114

13:                                               ; preds = %0
  %14 = call i1 @_sym_build_zext(i1 %9, i8 16), !dbg !114
  br label %15, !dbg !114

15:                                               ; preds = %0, %13
  %16 = phi i1 [ false, %0 ], [ %14, %13 ], !dbg !114
  %17 = zext i16 %11 to i32, !dbg !114
  %18 = icmp eq i1 false, %16, !dbg !116
  br i1 %18, label %22, label %19, !dbg !116

19:                                               ; preds = %15
  %20 = call i1 @_sym_build_integer(i32 5, i8 4, i16 10), !dbg !116
  %21 = call i1 @_sym_build_signed_greater_than(i1 %16, i1 %20), !dbg !116
  br label %22, !dbg !116

22:                                               ; preds = %15, %19
  %23 = phi i1 [ false, %15 ], [ %21, %19 ], !dbg !116
  %24 = icmp sgt i32 %17, 5, !dbg !116
  %25 = icmp eq i1 false, %23, !dbg !117
  br i1 %25, label %27, label %26, !dbg !117

26:                                               ; preds = %22
  call void @_sym_build_path_constraint(i1 %23, i1 %24, i16 4), !dbg !117
  br label %27, !dbg !117

27:                                               ; preds = %22, %26
  br i1 %24, label %28, label %30, !dbg !117

28:                                               ; preds = %27
  %29 = ptrtoint i8* %7 to i32, !dbg !118
  call void @_sym_build_write_memory(i32 %29, i32 1, i1 false, i8 1, i16 5), !dbg !118
  store i8 1, i8* %7, align 1, !dbg !118
  br label %32, !dbg !118

30:                                               ; preds = %27
  %31 = ptrtoint i8* %7 to i32, !dbg !119
  call void @_sym_build_write_memory(i32 %31, i32 1, i1 false, i8 1, i16 6), !dbg !119
  store i8 0, i8* %7, align 1, !dbg !119
  br label %32, !dbg !119

32:                                               ; preds = %30, %28
  %33 = ptrtoint i8* %7 to i32, !dbg !120
  %34 = call i1 @_sym_build_read_memory(i32 %33, i32 1, i8 1, i16 7), !dbg !120
  %35 = load i8, i8* %7, align 1, !dbg !120
  call void @_sym_set_return_expression(i1 %34), !dbg !120
  ret i8 %35, !dbg !120
}

