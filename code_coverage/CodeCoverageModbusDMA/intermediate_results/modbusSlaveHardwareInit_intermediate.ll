2|  call void @_sym_build_write_memory(i32 %7, i32 2, i1 false, i8 1, i16 2), !dbg !68
7|  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !71
20|  %27 = call i1 @_sym_build_read_memory(i32 %26, i32 1, i8 1, i16 20), !dbg !81
14|  %15 = call i1 @_sym_get_return_expression(), !dbg !74
6|  call void @_sym_notify_call(i8 2), !dbg !70
9|  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !73
11|  call void @_sym_set_parameter_expression(i8 2, i1 false), !dbg !73
16|  call void @_sym_build_path_constraint(i1 %19, i1 %20, i16 16), !dbg !72
18|  call void @_sym_notify_call(i8 5), !dbg !75
17|  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !75
13|  call void @_sym_set_return_expression(i1 false), !dbg !73
19|  call void @_sym_build_write_memory(i32 %25, i32 1, i1 false, i8 1, i16 19), !dbg !80
12|  call void @_sym_notify_call(i8 4), !dbg !73
4|  call void @_sym_notify_call(i8 1), !dbg !69
8|  call void @_sym_notify_call(i8 3), !dbg !71
22|  %17 = call i1 @_sym_build_integer(i32 0, i8 4, i16 22), !dbg !74
5|  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !70
15|  %18 = call i1 @_sym_build_not_equal(i1 %15, i1 %17), !dbg !74
3|  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !69
1|  call void @_sym_build_write_memory(i32 %6, i32 1, i1 false, i8 1, i16 1), !dbg !67
10|  call void @_sym_set_parameter_expression(i8 1, i1 false), !dbg !73
21|  call void @_sym_set_return_expression(i1 %27), !dbg !82
23|  %19 = phi i1 [ false, %12 ], [ %18, %.split ], !dbg !74|false,root
BB:.split.split->BB
BB:.split->BB
BB:.split.split.split->BB.split.split
BB:.split.split.split.split->BB.split.split
; Function Attrs: noinline nounwind optnone
define dso_local zeroext i8 @modbusSlaveHardwareInit() #0 !dbg !363 {
  %1 = alloca [1 x i8], align 1
  %2 = alloca [0 x i8], align 1
  store [1 x i8] zeroinitializer, [1 x i8]* %1, align 1
  store [0 x i8] zeroinitializer, [0 x i8]* %2, align 1
  %3 = getelementptr [1 x i8], [1 x i8]* %1, i8 0, i8 0
  %4 = getelementptr [0 x i8], [0 x i8]* %2, i8 0, i8 0
  %5 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %5, metadata !364, metadata !DIExpression()), !dbg !365
  %6 = ptrtoint i8* %5 to i32, !dbg !365
  call void @_sym_build_write_memory(i32 %6, i32 1, i1 false, i8 1, i16 1), !dbg !365
  store i8 0, i8* %5, align 1, !dbg !365
  %7 = ptrtoint %struct.modbus_t* @modbusVars to i32
  call void @_sym_build_write_memory(i32 %7, i32 2, i1 false, i8 1, i16 2), !dbg !366
  %8 = getelementptr %struct.modbus_t, %struct.modbus_t* @modbusVars, i32 0, i32 0
  store i16 0, i16* %8, align 32, !dbg !366
  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !367
  call void @_sym_notify_call(i8 1), !dbg !367
  %9 = call i32 @HAL_UART_Abort(%struct.__UART_HandleTypeDef* @huart4), !dbg !367
  call void @_sym_notify_ret(i8 1), !dbg !368
  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !368
  call void @_sym_notify_call(i8 2), !dbg !368
  %10 = call i32 @HAL_UART_DeInit(%struct.__UART_HandleTypeDef* @huart4), !dbg !368
  call void @_sym_notify_ret(i8 2), !dbg !369
  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !369
  call void @_sym_notify_call(i8 3), !dbg !369
  %11 = call i32 @HAL_UART_Init(%struct.__UART_HandleTypeDef* @huart4), !dbg !369
  call void @_sym_notify_ret(i8 3), !dbg !370
  br label %12, !dbg !370

12:                                               ; preds = %22, %0
  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !371
  call void @_sym_set_parameter_expression(i8 1, i1 false), !dbg !371
  call void @_sym_set_parameter_expression(i8 2, i1 false), !dbg !371
  call void @_sym_notify_call(i8 4), !dbg !371
  call void @_sym_set_return_expression(i1 false), !dbg !371
  %13 = getelementptr [256 x i8], [256 x i8]* @modbusRxTxBuffer, i32 0, i32 0
  %14 = call i32 @HAL_UARTEx_ReceiveToIdle_DMA(%struct.__UART_HandleTypeDef* @huart4, i8* %13, i16 zeroext 256), !dbg !371
  call void @_sym_notify_ret(i8 4), !dbg !372
  %15 = call i1 @_sym_get_return_expression(), !dbg !372
  %16 = icmp eq i1 false, %15, !dbg !372
  br i1 %16, label %.split.split, label %.split, !dbg !372

.split:                                           ; preds = %12
  %17 = call i1 @_sym_build_integer(i32 0, i8 4, i16 22), !dbg !372
  %18 = call i1 @_sym_build_not_equal(i1 %15, i1 %17), !dbg !372
  br label %.split.split, !dbg !372

.split.split:                                     ; preds = %12, %.split
  %19 = phi i1 [ false, %12 ], [ %18, %.split ], !dbg !372
  %20 = icmp ne i32 %14, 0, !dbg !372
  %21 = icmp eq i1 false, %19, !dbg !370
  br i1 %21, label %.split.split.split.split, label %.split.split.split, !dbg !370

.split.split.split:                               ; preds = %.split.split
  call void @_sym_build_path_constraint(i1 %19, i1 %20, i16 16), !dbg !370
  br label %.split.split.split.split, !dbg !370

.split.split.split.split:                         ; preds = %.split.split, %.split.split.split
  br i1 %20, label %22, label %24, !dbg !370

22:                                               ; preds = %.split.split.split.split
  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !373
  call void @_sym_notify_call(i8 5), !dbg !373
  %23 = call i32 @HAL_UART_DMAStop(%struct.__UART_HandleTypeDef* @huart4), !dbg !373
  call void @_sym_notify_ret(i8 5), !dbg !370
  br label %12, !dbg !370, !llvm.loop !375

24:                                               ; preds = %.split.split.split.split
  %25 = ptrtoint i8* %5 to i32, !dbg !377
  call void @_sym_build_write_memory(i32 %25, i32 1, i1 false, i8 1, i16 19), !dbg !377
  store i8 1, i8* %5, align 1, !dbg !377
  %26 = ptrtoint i8* %5 to i32, !dbg !378
  %27 = call i1 @_sym_build_read_memory(i32 %26, i32 1, i8 1, i16 20), !dbg !378
  %28 = load i8, i8* %5, align 1, !dbg !378
  call void @_sym_set_return_expression(i1 %27), !dbg !379
  ret i8 %28, !dbg !379
}

