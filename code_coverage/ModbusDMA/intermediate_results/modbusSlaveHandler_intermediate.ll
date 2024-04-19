15|  %36 = call i1 @_sym_get_return_expression(), !dbg !70
21|  %11 = call i1 @_sym_build_integer(i32 0, i8 1, i16 21), !dbg !58
20|  call void @_sym_notify_call(i8 8), !dbg !77
16|  %40 = call i1 @_sym_build_not_equal(i1 %36, i1 %39), !dbg !70
18|  call void @_sym_notify_call(i8 6), !dbg !73
23|  %25 = call i1 @_sym_build_integer(i32 0, i8 1, i16 23), !dbg !64
13|  call void @_sym_notify_call(i8 5), !dbg !70
14|  call void @_sym_set_return_expression(i1 false), !dbg !70
6|  call void @_sym_notify_call(i8 2), !dbg !61
12|  call void @_sym_notify_call(i8 4), !dbg !67
5|  call void @_sym_build_path_constraint(i1 %14, i1 %15, i16 5), !dbg !60
19|  call void @_sym_notify_call(i8 7), !dbg !76
9|  %22 = call i1 @_sym_get_return_expression(), !dbg !64
4|  %12 = call i1 @_sym_build_not_equal(i1 %8, i1 %11), !dbg !58
10|  %26 = call i1 @_sym_build_not_equal(i1 %22, i1 %25), !dbg !64
1|  call void @_sym_notify_call(i8 1), !dbg !58
17|  call void @_sym_build_path_constraint(i1 %42, i1 %43, i16 17), !dbg !72
25|  %39 = call i1 @_sym_build_integer(i32 0, i8 1, i16 25), !dbg !70
11|  call void @_sym_build_path_constraint(i1 %28, i1 %29, i16 11), !dbg !66
7|  call void @_sym_notify_call(i8 3), !dbg !64
3|  %8 = call i1 @_sym_get_return_expression(), !dbg !58
8|  call void @_sym_set_return_expression(i1 false), !dbg !64
2|  call void @_sym_set_return_expression(i1 false), !dbg !58
24|  %28 = phi i1 [ false, %20 ], [ %26, %24 ], !dbg !64|false,root
22|  %14 = phi i1 [ false, %0 ], [ %12, %10 ], !dbg !58|false,root
26|  %42 = phi i1 [ false, %34 ], [ %40, %38 ], !dbg !70|false,root
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
BB:->BB
; Function Attrs: noinline nounwind optnone
define dso_local void @modbusSlaveHandler() #0 !dbg !89 {
  %1 = alloca [0 x i8], align 1, !dbg !90
  %2 = alloca [0 x i8], align 1, !dbg !90
  %3 = alloca [0 x i8], align 1, !dbg !90
  store [0 x i8] zeroinitializer, [0 x i8]* %1, align 1, !dbg !90
  store [0 x i8] zeroinitializer, [0 x i8]* %2, align 1, !dbg !90
  store [0 x i8] zeroinitializer, [0 x i8]* %3, align 1, !dbg !90
  %4 = getelementptr [0 x i8], [0 x i8]* %1, i8 0, i8 0, !dbg !90
  %5 = getelementptr [0 x i8], [0 x i8]* %2, i8 0, i8 0, !dbg !90
  %6 = getelementptr [0 x i8], [0 x i8]* %3, i8 0, i8 0, !dbg !90
  call void @_sym_notify_call(i8 1), !dbg !90
  call void @_sym_set_return_expression(i1 false), !dbg !90
  %7 = call zeroext i8 @modbusSlaveCheckFrameSize(), !dbg !90
  call void @_sym_notify_ret(i8 1), !dbg !90
  %8 = call i1 @_sym_get_return_expression(), !dbg !90
  %9 = icmp eq i1 false, %8, !dbg !90
  br i1 %9, label %13, label %10, !dbg !90

10:                                               ; preds = %0
  %11 = call i1 @_sym_build_integer(i32 0, i8 1, i16 21), !dbg !90
  %12 = call i1 @_sym_build_not_equal(i1 %8, i1 %11), !dbg !90
  br label %13, !dbg !90

13:                                               ; preds = %0, %10
  %14 = phi i1 [ false, %0 ], [ %12, %10 ], !dbg !90
  %15 = icmp ne i8 %7, 0, !dbg !90
  %16 = icmp eq i1 false, %14, !dbg !92
  br i1 %16, label %18, label %17, !dbg !92

17:                                               ; preds = %13
  call void @_sym_build_path_constraint(i1 %14, i1 %15, i16 5), !dbg !92
  br label %18, !dbg !92

18:                                               ; preds = %13, %17
  br i1 %15, label %20, label %19, !dbg !92

19:                                               ; preds = %18
  call void @_sym_notify_call(i8 2), !dbg !93
  call void @modbusSlaveStartReceiving(), !dbg !93
  call void @_sym_notify_ret(i8 2), !dbg !95
  br label %50, !dbg !95

20:                                               ; preds = %18
  call void @_sym_notify_call(i8 3), !dbg !96
  call void @_sym_set_return_expression(i1 false), !dbg !96
  %21 = call zeroext i8 @modbusSlaveCheckID(), !dbg !96
  call void @_sym_notify_ret(i8 3), !dbg !96
  %22 = call i1 @_sym_get_return_expression(), !dbg !96
  %23 = icmp eq i1 false, %22, !dbg !96
  br i1 %23, label %27, label %24, !dbg !96

24:                                               ; preds = %20
  %25 = call i1 @_sym_build_integer(i32 0, i8 1, i16 23), !dbg !96
  %26 = call i1 @_sym_build_not_equal(i1 %22, i1 %25), !dbg !96
  br label %27, !dbg !96

27:                                               ; preds = %20, %24
  %28 = phi i1 [ false, %20 ], [ %26, %24 ], !dbg !96
  %29 = icmp ne i8 %21, 0, !dbg !96
  %30 = icmp eq i1 false, %28, !dbg !98
  br i1 %30, label %32, label %31, !dbg !98

31:                                               ; preds = %27
  call void @_sym_build_path_constraint(i1 %28, i1 %29, i16 11), !dbg !98
  br label %32, !dbg !98

32:                                               ; preds = %27, %31
  br i1 %29, label %34, label %33, !dbg !98

33:                                               ; preds = %32
  call void @_sym_notify_call(i8 4), !dbg !99
  call void @modbusSlaveStartReceiving(), !dbg !99
  call void @_sym_notify_ret(i8 4), !dbg !101
  br label %50, !dbg !101

34:                                               ; preds = %32
  call void @_sym_notify_call(i8 5), !dbg !102
  call void @_sym_set_return_expression(i1 false), !dbg !102
  %35 = call zeroext i8 @modbusSlaveCheckCRC(), !dbg !102
  call void @_sym_notify_ret(i8 5), !dbg !102
  %36 = call i1 @_sym_get_return_expression(), !dbg !102
  %37 = icmp eq i1 false, %36, !dbg !102
  br i1 %37, label %41, label %38, !dbg !102

38:                                               ; preds = %34
  %39 = call i1 @_sym_build_integer(i32 0, i8 1, i16 25), !dbg !102
  %40 = call i1 @_sym_build_not_equal(i1 %36, i1 %39), !dbg !102
  br label %41, !dbg !102

41:                                               ; preds = %34, %38
  %42 = phi i1 [ false, %34 ], [ %40, %38 ], !dbg !102
  %43 = icmp ne i8 %35, 0, !dbg !102
  %44 = icmp eq i1 false, %42, !dbg !104
  br i1 %44, label %46, label %45, !dbg !104

45:                                               ; preds = %41
  call void @_sym_build_path_constraint(i1 %42, i1 %43, i16 17), !dbg !104
  br label %46, !dbg !104

46:                                               ; preds = %41, %45
  br i1 %43, label %48, label %47, !dbg !104

47:                                               ; preds = %46
  call void @_sym_notify_call(i8 6), !dbg !105
  call void @modbusSlaveStartReceiving(), !dbg !105
  call void @_sym_notify_ret(i8 6), !dbg !107
  br label %50, !dbg !107

48:                                               ; preds = %46
  call void @_sym_notify_call(i8 7), !dbg !108
  %49 = call zeroext i16 @modbusSlaveParseFrame(), !dbg !108
  call void @_sym_notify_ret(i8 7), !dbg !109
  call void @_sym_notify_call(i8 8), !dbg !109
  call void @modbusSlaveStartReceiving(), !dbg !109
  call void @_sym_notify_ret(i8 8), !dbg !110
  br label %50, !dbg !110

50:                                               ; preds = %48, %47, %33, %19
  ret void, !dbg !110
}

