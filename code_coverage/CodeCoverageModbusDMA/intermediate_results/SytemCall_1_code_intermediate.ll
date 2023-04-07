1|  call void @_sym_notify_call(i8 1), !dbg !66
; Function Attrs: noinline nounwind optnone
define dso_local void @SytemCall_1_code() #0 !dbg !360 {
  %1 = alloca [0 x i8], align 1, !dbg !361
  %2 = alloca [0 x i8], align 1, !dbg !361
  store [0 x i8] zeroinitializer, [0 x i8]* %1, align 1, !dbg !361
  store [0 x i8] zeroinitializer, [0 x i8]* %2, align 1, !dbg !361
  %3 = getelementptr [0 x i8], [0 x i8]* %1, i8 0, i8 0, !dbg !361
  %4 = getelementptr [0 x i8], [0 x i8]* %2, i8 0, i8 0, !dbg !361
  call void @_sym_notify_call(i8 1), !dbg !361
  %5 = call zeroext i8 @modbusSlaveHardwareInit(), !dbg !361
  call void @_sym_notify_ret(i8 1), !dbg !362
  ret void, !dbg !362
}

