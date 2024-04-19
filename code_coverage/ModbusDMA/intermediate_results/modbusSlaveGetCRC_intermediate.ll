2|  %4 = call i1 @_sym_get_parameter_expression(i8 1)
4|  call void @_sym_build_write_memory(i32 %15, i32 2, i1 %4, i8 1, i16 4)
3|  call void @_sym_build_write_memory(i32 %14, i32 4, i1 %3, i8 1, i16 3)
1|  %3 = call i1 @_sym_get_parameter_expression(i8 0)
6|  %18 = call i1 @_sym_build_read_memory(i32 %17, i32 2, i8 1, i16 6), !dbg !65
7|  call void @_sym_set_return_expression(i1 %18), !dbg !66
5|  call void @_sym_build_write_memory(i32 %16, i32 2, i1 false, i8 1, i16 5), !dbg !64
; Function Attrs: noinline nounwind optnone
define internal zeroext i16 @modbusSlaveGetCRC(i8* %0, i16 zeroext %1) #0 !dbg !360 {
  %3 = call i1 @_sym_get_parameter_expression(i8 0)
  %4 = call i1 @_sym_get_parameter_expression(i8 1)
  %5 = alloca [0 x i8], align 1
  %6 = alloca [0 x i8], align 1
  %7 = alloca [0 x i8], align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %5, align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %6, align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %7, align 1
  %8 = getelementptr [0 x i8], [0 x i8]* %5, i8 0, i8 0
  %9 = getelementptr [0 x i8], [0 x i8]* %6, i8 0, i8 0
  %10 = getelementptr [0 x i8], [0 x i8]* %7, i8 0, i8 0
  %11 = alloca i8*, align 4
  %12 = alloca i16, align 2
  %13 = alloca i16, align 2
  %14 = ptrtoint i8** %11 to i32
  call void @_sym_build_write_memory(i32 %14, i32 4, i1 %3, i8 1, i16 3)
  store i8* %0, i8** %11, align 4
  call void @llvm.dbg.declare(metadata i8** %11, metadata !364, metadata !DIExpression()), !dbg !365
  %15 = ptrtoint i16* %12 to i32
  call void @_sym_build_write_memory(i32 %15, i32 2, i1 %4, i8 1, i16 4)
  store i16 %1, i16* %12, align 2
  call void @llvm.dbg.declare(metadata i16* %12, metadata !366, metadata !DIExpression()), !dbg !367
  call void @llvm.dbg.declare(metadata i16* %13, metadata !368, metadata !DIExpression()), !dbg !369
  %16 = ptrtoint i16* %13 to i32, !dbg !369
  call void @_sym_build_write_memory(i32 %16, i32 2, i1 false, i8 1, i16 5), !dbg !369
  store i16 -1, i16* %13, align 2, !dbg !369
  %17 = ptrtoint i16* %13 to i32, !dbg !370
  %18 = call i1 @_sym_build_read_memory(i32 %17, i32 2, i8 1, i16 6), !dbg !370
  %19 = load i16, i16* %13, align 2, !dbg !370
  call void @_sym_set_return_expression(i1 %18), !dbg !371
  ret i16 %19, !dbg !371
}

