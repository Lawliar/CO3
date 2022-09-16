; ModuleID = '/home/ubuntu/STMfirmware/SpearModbusDMASpear/Target/Src/libcgc.c'
source_filename = "/home/ubuntu/STMfirmware/SpearModbusDMASpear/Target/Src/libcgc.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "thumbv7em-none-unknown-eabi"

%struct.Symex_t = type { %struct.RingBuffer_t, i32, i32, i16, i8, i8, i8, %struct.tskTaskControlBlock*, %struct.tskTaskControlBlock*, [64 x i8], i32, i32, [7 x i8] }
%struct.RingBuffer_t = type { [1036 x i8], i32, i32, i32, i32, i8 }
%struct.tskTaskControlBlock = type opaque

@input_cur = dso_local local_unnamed_addr global i32 0, align 4
@AFLfuzzer = external dso_local local_unnamed_addr global %struct.Symex_t, align 4

; Function Attrs: norecurse nounwind optsize readnone
define dso_local arm_aapcs_vfpcc i32 @transmit(i32 %0, i8* nocapture readnone %1, i32 %2, i32* nocapture readnone %3) local_unnamed_addr #0 {
  ret i32 0
}

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc i32 @receive_cgc(i8* nocapture %0, i32 %1, i32* nocapture %2) local_unnamed_addr #1 {
  %4 = load i32, i32* getelementptr inbounds (%struct.Symex_t, %struct.Symex_t* @AFLfuzzer, i32 0, i32 0, i32 3), align 4, !tbaa !3
  %5 = add i32 %4, -4
  %6 = load i32, i32* @input_cur, align 4, !tbaa !12
  %7 = icmp ugt i32 %5, %6
  br i1 %7, label %8, label %17

8:                                                ; preds = %3
  %9 = sub i32 %5, %6
  %10 = icmp ult i32 %9, %1
  br i1 %10, label %11, label %13

11:                                               ; preds = %8
  store i32 %9, i32* %2, align 4, !tbaa !13
  %12 = getelementptr inbounds i8, i8* getelementptr inbounds (%struct.Symex_t, %struct.Symex_t* @AFLfuzzer, i32 0, i32 0, i32 0, i32 4), i32 %6
  tail call void @llvm.memcpy.p0i8.p0i8.i32(i8* align 1 %0, i8* align 1 %12, i32 %9, i1 false)
  store i32 %5, i32* @input_cur, align 4, !tbaa !12
  br label %17

13:                                               ; preds = %8
  %14 = getelementptr inbounds i8, i8* getelementptr inbounds (%struct.Symex_t, %struct.Symex_t* @AFLfuzzer, i32 0, i32 0, i32 0, i32 4), i32 %6
  tail call void @llvm.memcpy.p0i8.p0i8.i32(i8* align 1 %0, i8* align 1 %14, i32 %1, i1 false)
  %15 = load i32, i32* @input_cur, align 4, !tbaa !12
  %16 = add i32 %15, %1
  store i32 %16, i32* @input_cur, align 4, !tbaa !12
  store i32 %1, i32* %2, align 4, !tbaa !13
  br label %17

17:                                               ; preds = %3, %13, %11
  %18 = phi i32 [ 0, %11 ], [ 0, %13 ], [ 2, %3 ]
  ret i32 %18
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i32, i1 immarg) #2

; Function Attrs: nofree nounwind optsize
define dso_local arm_aapcs_vfpcc i32 @allocate_cgc(i32 %0, i32 %1, i8** nocapture %2) local_unnamed_addr #3 {
  %4 = and i32 %0, 127
  %5 = icmp eq i32 %4, 0
  %6 = sub nuw nsw i32 128, %4
  %7 = select i1 %5, i32 0, i32 %6
  %8 = add i32 %7, %0
  %9 = tail call arm_aapcs_vfpcc noalias i8* @malloc(i32 %8) #6
  %10 = ptrtoint i8* %9 to i32
  switch i32 %10, label %11 [
    i32 -1, label %12
    i32 0, label %12
  ]

11:                                               ; preds = %3
  store i8* %9, i8** %2, align 4, !tbaa !15
  br label %12

12:                                               ; preds = %3, %3, %11
  %13 = phi i32 [ 0, %11 ], [ 2, %3 ], [ 2, %3 ]
  ret i32 %13
}

; Function Attrs: nofree nounwind optsize allocsize(0)
declare dso_local arm_aapcs_vfpcc noalias i8* @malloc(i32) local_unnamed_addr #4

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc i32 @deallocate_cgc(i8* nocapture %0, i32 %1) local_unnamed_addr #1 {
  tail call arm_aapcs_vfpcc void @free(i8* %0) #7
  ret i32 0
}

; Function Attrs: nounwind optsize
declare dso_local arm_aapcs_vfpcc void @free(i8* nocapture) local_unnamed_addr #5

attributes #0 = { norecurse nounwind optsize readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind willreturn }
attributes #3 = { nofree nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nofree nounwind optsize allocsize(0) "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { optsize allocsize(0) }
attributes #7 = { optsize }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 10.0.0-4ubuntu1 "}
!3 = !{!4, !8, i64 1044}
!4 = !{!"", !5, i64 0, !8, i64 1056, !8, i64 1060, !10, i64 1064, !9, i64 1066, !9, i64 1067, !9, i64 1068, !11, i64 1072, !11, i64 1076, !6, i64 1080, !8, i64 1144, !8, i64 1148, !6, i64 1152}
!5 = !{!"", !6, i64 0, !8, i64 1036, !8, i64 1040, !8, i64 1044, !8, i64 1048, !9, i64 1052}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = !{!"int", !6, i64 0}
!9 = !{!"_Bool", !6, i64 0}
!10 = !{!"short", !6, i64 0}
!11 = !{!"any pointer", !6, i64 0}
!12 = !{!8, !8, i64 0}
!13 = !{!14, !14, i64 0}
!14 = !{!"long", !6, i64 0}
!15 = !{!11, !11, i64 0}
