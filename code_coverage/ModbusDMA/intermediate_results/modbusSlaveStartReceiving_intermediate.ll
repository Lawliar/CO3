2|  call void @_sym_build_write_memory(i32 %8, i32 2, i1 false, i8 1, i16 2), !dbg !59
1|  call void @_sym_notify_call(i8 1), !dbg !58
; Function Attrs: noinline nounwind optnone
define dso_local void @modbusSlaveStartReceiving() #0 !dbg !121 {
  %1 = alloca [0 x i8], align 1, !dbg !122
  %2 = alloca [0 x i8], align 1, !dbg !122
  %3 = alloca [0 x i8], align 1, !dbg !122
  store [0 x i8] zeroinitializer, [0 x i8]* %1, align 1, !dbg !122
  store [0 x i8] zeroinitializer, [0 x i8]* %2, align 1, !dbg !122
  store [0 x i8] zeroinitializer, [0 x i8]* %3, align 1, !dbg !122
  %4 = getelementptr [0 x i8], [0 x i8]* %1, i8 0, i8 0, !dbg !122
  %5 = getelementptr [0 x i8], [0 x i8]* %2, i8 0, i8 0, !dbg !122
  %6 = getelementptr [0 x i8], [0 x i8]* %3, i8 0, i8 0, !dbg !122
  call void @_sym_notify_call(i8 1), !dbg !122
  %7 = bitcast void (...)* @SytemCall_2_code to void ()*
  call void %7(), !dbg !122
  call void @_sym_notify_ret(i8 1), !dbg !123
  %8 = ptrtoint %struct.modbus_t* @modbusVars to i32
  call void @_sym_build_write_memory(i32 %8, i32 2, i1 false, i8 1, i16 2), !dbg !123
  %9 = getelementptr %struct.modbus_t, %struct.modbus_t* @modbusVars, i32 0, i32 0
  store i16 0, i16* %9, align 32, !dbg !123
  ret void, !dbg !124
}

