1|  %9 = call i1 @_sym_build_read_memory(i32 %8, i32 4, i8 1, i16 1), !dbg !58
2|  call void @_sym_set_return_expression(i1 %9), !dbg !59
; Function Attrs: noinline nounwind optnone
define dso_local %struct.tskTaskControlBlock* @modbusSlaveGetTaskHandke() #0 !dbg !60 {
  %1 = alloca [0 x i8], align 1, !dbg !63
  %2 = alloca [0 x i8], align 1, !dbg !63
  %3 = alloca [0 x i8], align 1, !dbg !63
  store [0 x i8] zeroinitializer, [0 x i8]* %1, align 1, !dbg !63
  store [0 x i8] zeroinitializer, [0 x i8]* %2, align 1, !dbg !63
  store [0 x i8] zeroinitializer, [0 x i8]* %3, align 1, !dbg !63
  %4 = getelementptr [0 x i8], [0 x i8]* %1, i8 0, i8 0, !dbg !63
  %5 = getelementptr [0 x i8], [0 x i8]* %2, i8 0, i8 0, !dbg !63
  %6 = getelementptr [0 x i8], [0 x i8]* %3, i8 0, i8 0, !dbg !63
  %7 = getelementptr %struct.modbus_t, %struct.modbus_t* @modbusVars, i32 0, i32 2
  %8 = ptrtoint %struct.tskTaskControlBlock** %7 to i32
  %9 = call i1 @_sym_build_read_memory(i32 %8, i32 4, i8 1, i16 1), !dbg !63
  %10 = getelementptr %struct.modbus_t, %struct.modbus_t* @modbusVars, i32 0, i32 2
  %11 = load %struct.tskTaskControlBlock*, %struct.tskTaskControlBlock** %10, align 8, !dbg !63
  call void @_sym_set_return_expression(i1 %9), !dbg !64
  ret %struct.tskTaskControlBlock* %11, !dbg !64
}

