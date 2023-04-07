7|  %11 = call i1 @_sym_build_not_equal(i1 %8, i1 %10), !dbg !68
10|  call void @_sym_notify_call(i8 2), !dbg !69
11|  %10 = call i1 @_sym_build_integer(i32 0, i8 4, i16 11), !dbg !68
1|  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !67
4|  call void @_sym_notify_call(i8 1), !dbg !67
5|  call void @_sym_set_return_expression(i1 false), !dbg !67
9|  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !69
8|  call void @_sym_build_path_constraint(i1 %12, i1 %13, i16 8), !dbg !66
6|  %8 = call i1 @_sym_get_return_expression(), !dbg !68
3|  call void @_sym_set_parameter_expression(i8 2, i1 false), !dbg !67
2|  call void @_sym_set_parameter_expression(i8 1, i1 false), !dbg !67
12|  %12 = phi i1 [ false, %5 ], [ %11, %.split ], !dbg !68|false,root
BB:.split.split.split.split->BB.split.split
BB:.split.split.split->BB.split.split
BB:.split.split->BB
BB:.split->BB
; Function Attrs: noinline nounwind optnone
define dso_local void @SytemCall_2_code() #0 !dbg !380 {
  %1 = alloca [1 x i8], align 1, !dbg !381
  %2 = alloca [0 x i8], align 1, !dbg !381
  store [1 x i8] zeroinitializer, [1 x i8]* %1, align 1, !dbg !381
  store [0 x i8] zeroinitializer, [0 x i8]* %2, align 1, !dbg !381
  %3 = getelementptr [1 x i8], [1 x i8]* %1, i8 0, i8 0, !dbg !381
  %4 = getelementptr [0 x i8], [0 x i8]* %2, i8 0, i8 0, !dbg !381
  br label %5, !dbg !381

5:                                                ; preds = %15, %0
  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !382
  call void @_sym_set_parameter_expression(i8 1, i1 false), !dbg !382
  call void @_sym_set_parameter_expression(i8 2, i1 false), !dbg !382
  call void @_sym_notify_call(i8 1), !dbg !382
  call void @_sym_set_return_expression(i1 false), !dbg !382
  %6 = getelementptr [256 x i8], [256 x i8]* @modbusRxTxBuffer, i32 0, i32 0
  %7 = call i32 @HAL_UARTEx_ReceiveToIdle_DMA(%struct.__UART_HandleTypeDef* @huart4, i8* %6, i16 zeroext 256), !dbg !382
  call void @_sym_notify_ret(i8 1), !dbg !383
  %8 = call i1 @_sym_get_return_expression(), !dbg !383
  %9 = icmp eq i1 false, %8, !dbg !383
  br i1 %9, label %.split.split, label %.split, !dbg !383

.split:                                           ; preds = %5
  %10 = call i1 @_sym_build_integer(i32 0, i8 4, i16 11), !dbg !383
  %11 = call i1 @_sym_build_not_equal(i1 %8, i1 %10), !dbg !383
  br label %.split.split, !dbg !383

.split.split:                                     ; preds = %5, %.split
  %12 = phi i1 [ false, %5 ], [ %11, %.split ], !dbg !383
  %13 = icmp ne i32 %7, 0, !dbg !383
  %14 = icmp eq i1 false, %12, !dbg !381
  br i1 %14, label %.split.split.split.split, label %.split.split.split, !dbg !381

.split.split.split:                               ; preds = %.split.split
  call void @_sym_build_path_constraint(i1 %12, i1 %13, i16 8), !dbg !381
  br label %.split.split.split.split, !dbg !381

.split.split.split.split:                         ; preds = %.split.split, %.split.split.split
  br i1 %13, label %15, label %17, !dbg !381

15:                                               ; preds = %.split.split.split.split
  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !384
  call void @_sym_notify_call(i8 2), !dbg !384
  %16 = call i32 @HAL_UART_DMAStop(%struct.__UART_HandleTypeDef* @huart4), !dbg !384
  call void @_sym_notify_ret(i8 2), !dbg !381
  br label %5, !dbg !381, !llvm.loop !386

17:                                               ; preds = %.split.split.split.split
  ret void, !dbg !388
}

