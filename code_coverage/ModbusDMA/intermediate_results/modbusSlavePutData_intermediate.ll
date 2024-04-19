11|  %40 = call i1 @_sym_build_read_memory(i32 %39, i32 4, i8 1, i16 11), !dbg !67
10|  %34 = call i1 @_sym_build_add(i1 %33, i1 %32), !dbg !65
15|  call void @_sym_try_alternative(i1 %36, i1 %54, i16 15), !dbg !66
18|  %63 = call i1 @_sym_build_integer(i32 %50, i8 4, i16 18), !dbg !66
9|  %33 = call i1 @_sym_build_mul(i1 %26, i1 %30), !dbg !65
1|  %4 = call i1 @_sym_get_parameter_expression(i8 0)
19|  call void @_sym_try_alternative(i1 %49, i1 %63, i16 19), !dbg !66
13|  %47 = call i1 @_sym_build_zext(i1 %43, i8 16), !dbg !68
17|  call void @_sym_try_alternative(i1 %40, i1 %59, i16 17), !dbg !66
22|  %30 = call i1 @_sym_build_integer(i32 2, i8 4, i16 22), !dbg !65
2|  %5 = call i1 @_sym_get_parameter_expression(i8 1)
16|  %59 = call i1 @_sym_build_integer(i32 %58, i8 4, i16 16), !dbg !66
23|  %32 = call i1 @_sym_build_integer(i32 %31, i8 4, i16 23), !dbg !65
4|  call void @_sym_build_write_memory(i32 %16, i32 2, i1 %4, i8 1, i16 4)
14|  %54 = call i1 @_sym_build_integer(i32 %53, i8 4, i16 14), !dbg !66
5|  call void @_sym_build_write_memory(i32 %17, i32 4, i1 %5, i8 1, i16 5)
8|  %24 = call i1 @_sym_build_zext(i1 %20, i8 16), !dbg !65
12|  %43 = call i1 @_sym_build_read_memory(i32 %42, i32 2, i8 1, i16 12), !dbg !68
20|  call void @_sym_build_memcpy(i8* %38, i8* %41, i32 %50, i16 20), !dbg !66
3|  %6 = call i1 @_sym_get_parameter_expression(i8 2)
7|  %20 = call i1 @_sym_build_read_memory(i32 %19, i32 2, i8 1, i16 7), !dbg !64
6|  call void @_sym_build_write_memory(i32 %18, i32 2, i1 %6, i8 1, i16 6)
25|  %49 = phi i1 [ false, %35 ], [ %47, %46 ], !dbg !68|false,root
24|  %36 = phi i1 [ false, %25 ], [ %34, %29 ], !dbg !65|false,root
21|  %26 = phi i1 [ false, %3 ], [ %24, %23 ], !dbg !65|false,root
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
define dso_local void @modbusSlavePutData(i16 zeroext %0, i8* %1, i16 zeroext %2) #0 !dbg !74 {
  %4 = call i1 @_sym_get_parameter_expression(i8 0)
  %5 = call i1 @_sym_get_parameter_expression(i8 1)
  %6 = call i1 @_sym_get_parameter_expression(i8 2)
  %7 = alloca [0 x i8], align 1
  %8 = alloca [0 x i8], align 1
  %9 = alloca [0 x i8], align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %7, align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %8, align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %9, align 1
  %10 = getelementptr [0 x i8], [0 x i8]* %7, i8 0, i8 0
  %11 = getelementptr [0 x i8], [0 x i8]* %8, i8 0, i8 0
  %12 = getelementptr [0 x i8], [0 x i8]* %9, i8 0, i8 0
  %13 = alloca i16, align 2
  %14 = alloca i8*, align 4
  %15 = alloca i16, align 2
  %16 = ptrtoint i16* %13 to i32
  call void @_sym_build_write_memory(i32 %16, i32 2, i1 %4, i8 1, i16 4)
  store i16 %0, i16* %13, align 2
  call void @llvm.dbg.declare(metadata i16* %13, metadata !77, metadata !DIExpression()), !dbg !78
  %17 = ptrtoint i8** %14 to i32
  call void @_sym_build_write_memory(i32 %17, i32 4, i1 %5, i8 1, i16 5)
  store i8* %1, i8** %14, align 4
  call void @llvm.dbg.declare(metadata i8** %14, metadata !79, metadata !DIExpression()), !dbg !80
  %18 = ptrtoint i16* %15 to i32
  call void @_sym_build_write_memory(i32 %18, i32 2, i1 %6, i8 1, i16 6)
  store i16 %2, i16* %15, align 2
  call void @llvm.dbg.declare(metadata i16* %15, metadata !81, metadata !DIExpression()), !dbg !82
  %19 = ptrtoint i16* %13 to i32, !dbg !83
  %20 = call i1 @_sym_build_read_memory(i32 %19, i32 2, i8 1, i16 7), !dbg !83
  %21 = load i16, i16* %13, align 2, !dbg !83
  %22 = icmp eq i1 false, %20, !dbg !84
  br i1 %22, label %25, label %23, !dbg !84

23:                                               ; preds = %3
  %24 = call i1 @_sym_build_zext(i1 %20, i8 16), !dbg !84
  br label %25, !dbg !84

25:                                               ; preds = %3, %23
  %26 = phi i1 [ false, %3 ], [ %24, %23 ], !dbg !84
  %27 = zext i16 %21 to i32, !dbg !84
  %28 = icmp eq i1 false, %26, !dbg !84
  br i1 %28, label %35, label %29, !dbg !84

29:                                               ; preds = %25
  %30 = call i1 @_sym_build_integer(i32 2, i8 4, i16 22), !dbg !84
  %31 = ptrtoint [128 x i16]* @modbusMemory to i32
  %32 = call i1 @_sym_build_integer(i32 %31, i8 4, i16 23), !dbg !84
  %33 = call i1 @_sym_build_mul(i1 %26, i1 %30), !dbg !84
  %34 = call i1 @_sym_build_add(i1 %33, i1 %32), !dbg !84
  br label %35, !dbg !84

35:                                               ; preds = %25, %29
  %36 = phi i1 [ false, %25 ], [ %34, %29 ], !dbg !84
  %37 = getelementptr inbounds [128 x i16], [128 x i16]* @modbusMemory, i32 0, i32 %27, !dbg !84
  %38 = bitcast i16* %37 to i8*, !dbg !85
  %39 = ptrtoint i8** %14 to i32, !dbg !86
  %40 = call i1 @_sym_build_read_memory(i32 %39, i32 4, i8 1, i16 11), !dbg !86
  %41 = load i8*, i8** %14, align 4, !dbg !86
  %42 = ptrtoint i16* %15 to i32, !dbg !87
  %43 = call i1 @_sym_build_read_memory(i32 %42, i32 2, i8 1, i16 12), !dbg !87
  %44 = load i16, i16* %15, align 2, !dbg !87
  %45 = icmp eq i1 false, %43, !dbg !87
  br i1 %45, label %48, label %46, !dbg !87

46:                                               ; preds = %35
  %47 = call i1 @_sym_build_zext(i1 %43, i8 16), !dbg !87
  br label %48, !dbg !87

48:                                               ; preds = %35, %46
  %49 = phi i1 [ false, %35 ], [ %47, %46 ], !dbg !87
  %50 = zext i16 %44 to i32, !dbg !87
  %51 = icmp eq i1 true, %36, !dbg !85
  br i1 %51, label %52, label %55, !dbg !85

52:                                               ; preds = %48
  %53 = ptrtoint i8* %38 to i32, !dbg !85
  %54 = call i1 @_sym_build_integer(i32 %53, i8 4, i16 14), !dbg !85
  call void @_sym_try_alternative(i1 %36, i1 %54, i16 15), !dbg !85
  br label %55, !dbg !85

55:                                               ; preds = %48, %52
  %56 = icmp eq i1 true, %40, !dbg !85
  br i1 %56, label %57, label %60, !dbg !85

57:                                               ; preds = %55
  %58 = ptrtoint i8* %41 to i32, !dbg !85
  %59 = call i1 @_sym_build_integer(i32 %58, i8 4, i16 16), !dbg !85
  call void @_sym_try_alternative(i1 %40, i1 %59, i16 17), !dbg !85
  br label %60, !dbg !85

60:                                               ; preds = %55, %57
  %61 = icmp eq i1 true, %49, !dbg !85
  br i1 %61, label %62, label %64, !dbg !85

62:                                               ; preds = %60
  %63 = call i1 @_sym_build_integer(i32 %50, i8 4, i16 18), !dbg !85
  call void @_sym_try_alternative(i1 %49, i1 %63, i16 19), !dbg !85
  br label %64, !dbg !85

64:                                               ; preds = %60, %62
  call void @_sym_build_memcpy(i8* %38, i8* %41, i32 %50, i16 20), !dbg !85
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* align 2 %38, i8* align 1 %41, i32 %50, i1 false), !dbg !85
  ret void, !dbg !88
}

