; ModuleID = '/home/ubuntu/STMfirmware/SpearModbusDMASpear/Target/Src/mymath.c'
source_filename = "/home/ubuntu/STMfirmware/SpearModbusDMASpear/Target/Src/mymath.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "thumbv7em-none-unknown-eabi"

; Function Attrs: nounwind optsize readnone
define dso_local arm_aapcs_vfpcc double @floor(double %0) local_unnamed_addr #0 {
  %2 = fcmp ogt double %0, 0.000000e+00
  br i1 %2, label %3, label %7

3:                                                ; preds = %1
  %4 = fadd double %0, 5.000000e-01
  %5 = tail call double @llvm.rint.f64(double %4)
  %6 = fadd double %5, -1.000000e+00
  br label %13

7:                                                ; preds = %1
  %8 = fcmp olt double %0, 0.000000e+00
  br i1 %8, label %9, label %13

9:                                                ; preds = %7
  %10 = fadd double %0, -5.000000e-01
  %11 = tail call double @llvm.rint.f64(double %10)
  %12 = fadd double %11, 1.000000e+00
  br label %13

13:                                               ; preds = %7, %9, %3
  %14 = phi double [ %6, %3 ], [ %12, %9 ], [ 0.000000e+00, %7 ]
  ret double %14
}

; Function Attrs: nounwind readnone speculatable willreturn
declare double @llvm.rint.f64(double) #1

; Function Attrs: nounwind optsize readnone
define dso_local arm_aapcs_vfpcc double @round_away_from_zero(double %0) local_unnamed_addr #0 {
  %2 = fcmp ogt double %0, 0.000000e+00
  br i1 %2, label %3, label %6

3:                                                ; preds = %1
  %4 = fadd double %0, 5.000000e-01
  %5 = tail call double @llvm.rint.f64(double %4)
  br label %11

6:                                                ; preds = %1
  %7 = fcmp olt double %0, 0.000000e+00
  br i1 %7, label %8, label %11

8:                                                ; preds = %6
  %9 = fadd double %0, -5.000000e-01
  %10 = tail call double @llvm.rint.f64(double %9)
  br label %11

11:                                               ; preds = %6, %8, %3
  %12 = phi double [ %5, %3 ], [ %10, %8 ], [ 0.000000e+00, %6 ]
  ret double %12
}

; Function Attrs: nofree nounwind optsize
define dso_local arm_aapcs_vfpcc double @round_cgc(double %0, double %1) local_unnamed_addr #2 {
  %3 = tail call double @llvm.rint.f64(double %1)
  %4 = tail call arm_aapcs_vfpcc double @pow(double 1.000000e+01, double %3) #4
  %5 = fneg double %3
  %6 = tail call arm_aapcs_vfpcc double @pow(double 1.000000e+01, double %5) #4
  %7 = fmul double %4, %0
  %8 = fcmp ogt double %7, 0.000000e+00
  br i1 %8, label %9, label %12

9:                                                ; preds = %2
  %10 = fadd double %7, 5.000000e-01
  %11 = tail call double @llvm.rint.f64(double %10) #5
  br label %17

12:                                               ; preds = %2
  %13 = fcmp olt double %7, 0.000000e+00
  br i1 %13, label %14, label %17

14:                                               ; preds = %12
  %15 = fadd double %7, -5.000000e-01
  %16 = tail call double @llvm.rint.f64(double %15) #5
  br label %17

17:                                               ; preds = %9, %12, %14
  %18 = phi double [ %11, %9 ], [ %16, %14 ], [ 0.000000e+00, %12 ]
  %19 = fmul double %6, %18
  ret double %19
}

; Function Attrs: nofree nounwind optsize
declare dso_local arm_aapcs_vfpcc double @pow(double, double) local_unnamed_addr #3

attributes #0 = { nounwind optsize readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable willreturn }
attributes #2 = { nofree nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nofree nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind optsize }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 10.0.0-4ubuntu1 "}
