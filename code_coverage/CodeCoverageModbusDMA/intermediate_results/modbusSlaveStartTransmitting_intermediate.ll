3|  %12 = call i1 @_sym_build_read_memory(i32 %11, i32 2, i8 1, i16 3), !dbg !60
7|  call void @_sym_notify_call(i8 1), !dbg !61
4|  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !61
5|  call void @_sym_set_parameter_expression(i8 1, i1 false), !dbg !61
6|  call void @_sym_set_parameter_expression(i8 2, i1 %12), !dbg !61
2|  call void @_sym_build_write_memory(i32 %10, i32 2, i1 %2, i8 1, i16 2)
1|  %2 = call i1 @_sym_get_parameter_expression(i8 0)
; Function Attrs: noinline nounwind optnone
define dso_local void @modbusSlaveStartTransmitting(i16 zeroext %0) #0 !dbg !352 {
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
  call void @llvm.dbg.declare(metadata i16* %9, metadata !355, metadata !DIExpression()), !dbg !356
  %11 = ptrtoint i16* %9 to i32, !dbg !357
  %12 = call i1 @_sym_build_read_memory(i32 %11, i32 2, i8 1, i16 3), !dbg !357
  %13 = load i16, i16* %9, align 2, !dbg !357
  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !358
  call void @_sym_set_parameter_expression(i8 1, i1 false), !dbg !358
  call void @_sym_set_parameter_expression(i8 2, i1 %12), !dbg !358
  call void @_sym_notify_call(i8 1), !dbg !358
  %14 = getelementptr [256 x i8], [256 x i8]* @modbusRxTxBuffer, i32 0, i32 0
  %15 = call i32 @HAL_UART_Transmit_DMA(%struct.__UART_HandleTypeDef* @huart4, i8* %14, i16 zeroext %13), !dbg !358
  call void @_sym_notify_ret(i8 1), !dbg !359
  ret void, !dbg !359
}

