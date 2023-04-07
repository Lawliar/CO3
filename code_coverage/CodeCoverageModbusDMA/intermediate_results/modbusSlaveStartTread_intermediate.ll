2|  call void @_sym_set_parameter_expression(i8 1, i1 false), !dbg !58
1|  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !58
3|  call void @_sym_set_parameter_expression(i8 2, i1 false), !dbg !58
4|  call void @_sym_set_parameter_expression(i8 3, i1 false), !dbg !58
5|  call void @_sym_set_parameter_expression(i8 4, i1 false), !dbg !58
6|  call void @_sym_set_parameter_expression(i8 5, i1 false), !dbg !58
7|  call void @_sym_notify_call(i8 1), !dbg !58
; Function Attrs: noinline nounwind optnone
define dso_local void @modbusSlaveStartTread() #0 !dbg !55 {
  %1 = alloca [0 x i8], align 1, !dbg !58
  %2 = alloca [0 x i8], align 1, !dbg !58
  %3 = alloca [0 x i8], align 1, !dbg !58
  store [0 x i8] zeroinitializer, [0 x i8]* %1, align 1, !dbg !58
  store [0 x i8] zeroinitializer, [0 x i8]* %2, align 1, !dbg !58
  store [0 x i8] zeroinitializer, [0 x i8]* %3, align 1, !dbg !58
  %4 = getelementptr [0 x i8], [0 x i8]* %1, i8 0, i8 0, !dbg !58
  %5 = getelementptr [0 x i8], [0 x i8]* %2, i8 0, i8 0, !dbg !58
  %6 = getelementptr [0 x i8], [0 x i8]* %3, i8 0, i8 0, !dbg !58
  call void @_sym_set_parameter_expression(i8 0, i1 false), !dbg !58
  call void @_sym_set_parameter_expression(i8 1, i1 false), !dbg !58
  call void @_sym_set_parameter_expression(i8 2, i1 false), !dbg !58
  call void @_sym_set_parameter_expression(i8 3, i1 false), !dbg !58
  call void @_sym_set_parameter_expression(i8 4, i1 false), !dbg !58
  call void @_sym_set_parameter_expression(i8 5, i1 false), !dbg !58
  call void @_sym_notify_call(i8 1), !dbg !58
  %7 = bitcast void ()* @modbusSlaveHandler to void (i8*)*
  %8 = getelementptr [10 x i8], [10 x i8]* @.str, i32 0, i32 0
  %9 = getelementptr %struct.modbus_t, %struct.modbus_t* @modbusVars, i32 0, i32 2
  %10 = call i32 @xTaskCreate(void (i8*)* %7, i8* %8, i16 zeroext 512, i8* null, i32 1, %struct.tskTaskControlBlock** %9), !dbg !58
  call void @_sym_notify_ret(i8 1), !dbg !59
  ret void, !dbg !59
}

