; ModuleID = '/home/ubuntu/STMfirmware/SpearSynthetic/Target/Src/printf.c'
source_filename = "/home/ubuntu/STMfirmware/SpearSynthetic/Target/Src/printf.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "thumbv7em-none-unknown-eabi"

@.str = private unnamed_addr constant [4 x i8] c"nan\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"inf\00", align 1
@.str.2 = private unnamed_addr constant [8 x i8] c"0.00000\00", align 1

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc i32 @putc(i32 %0) local_unnamed_addr #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, i32* %2, align 4, !tbaa !3
  %4 = bitcast i32* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %4) #5
  %5 = bitcast i32* %2 to i8*
  %6 = call arm_aapcs_vfpcc i32 @transmit(i32 1, i8* nonnull %5, i32 1, i32* nonnull %3) #6
  %7 = load i32, i32* %2, align 4, !tbaa !3
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %4) #5
  ret i32 %7
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @transmit(i32, i8*, i32, i32*) local_unnamed_addr #2

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc void @int_to_str(i32 %0, i8* %1) local_unnamed_addr #0 {
  %3 = alloca [32 x i8], align 1
  %4 = getelementptr inbounds [32 x i8], [32 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 32, i8* nonnull %4) #5
  %5 = icmp eq i8* %1, null
  br i1 %5, label %37, label %6

6:                                                ; preds = %2
  %7 = icmp slt i32 %0, 0
  br i1 %7, label %8, label %11

8:                                                ; preds = %6
  store i8 45, i8* %1, align 1, !tbaa !7
  %9 = getelementptr inbounds i8, i8* %1, i32 1
  %10 = sub nsw i32 0, %0
  br label %11

11:                                               ; preds = %8, %6
  %12 = phi i32 [ %10, %8 ], [ %0, %6 ]
  %13 = phi i8* [ %9, %8 ], [ %1, %6 ]
  br label %14

14:                                               ; preds = %14, %11
  %15 = phi i32 [ %12, %11 ], [ %18, %14 ]
  %16 = phi i8* [ %4, %11 ], [ %23, %14 ]
  %17 = phi i32 [ 0, %11 ], [ %24, %14 ]
  %18 = sdiv i32 %15, 10
  %19 = mul i32 %18, 10
  %20 = sub i32 %15, %19
  %21 = trunc i32 %20 to i8
  %22 = add nsw i8 %21, 48
  store i8 %22, i8* %16, align 1, !tbaa !7
  %23 = getelementptr inbounds i8, i8* %16, i32 1
  %24 = add nuw nsw i32 %17, 1
  %25 = add i32 %15, 9
  %26 = icmp ugt i32 %25, 18
  br i1 %26, label %14, label %27

27:                                               ; preds = %14, %27
  %28 = phi i32 [ %31, %27 ], [ %24, %14 ]
  %29 = phi i8* [ %32, %27 ], [ %23, %14 ]
  %30 = phi i8* [ %34, %27 ], [ %13, %14 ]
  %31 = add nsw i32 %28, -1
  %32 = getelementptr inbounds i8, i8* %29, i32 -1
  %33 = load i8, i8* %32, align 1, !tbaa !7
  store i8 %33, i8* %30, align 1, !tbaa !7
  %34 = getelementptr inbounds i8, i8* %30, i32 1
  %35 = icmp sgt i32 %28, 1
  br i1 %35, label %27, label %36

36:                                               ; preds = %27
  store i8 0, i8* %34, align 1, !tbaa !7
  br label %37

37:                                               ; preds = %2, %36
  call void @llvm.lifetime.end.p0i8(i64 32, i8* nonnull %4) #5
  ret void
}

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc void @float_to_str(double %0, i8* %1) local_unnamed_addr #0 {
  %3 = icmp eq i8* %1, null
  br i1 %3, label %87, label %4

4:                                                ; preds = %2
  %5 = tail call arm_aapcs_vfpcc i32 @isnan(double %0) #6
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %9, label %7

7:                                                ; preds = %4
  %8 = tail call arm_aapcs_vfpcc i8* @strcpy_cgc(i8* nonnull %1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0)) #6
  br label %87

9:                                                ; preds = %4
  %10 = tail call arm_aapcs_vfpcc i32 @isinf(double %0) #6
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %14, label %12

12:                                               ; preds = %9
  %13 = tail call arm_aapcs_vfpcc i8* @strcpy_cgc(i8* nonnull %1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0)) #6
  br label %87

14:                                               ; preds = %9
  %15 = fcmp oeq double %0, 0.000000e+00
  br i1 %15, label %16, label %18

16:                                               ; preds = %14
  %17 = tail call arm_aapcs_vfpcc i8* @strcpy_cgc(i8* nonnull %1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.2, i32 0, i32 0)) #6
  br label %87

18:                                               ; preds = %14
  %19 = fcmp ogt double %0, 0.000000e+00
  %20 = select i1 %19, double 5.000000e-06, double -5.000000e-06
  %21 = fadd double %20, %0
  %22 = fcmp olt double %21, 0.000000e+00
  br i1 %22, label %23, label %26

23:                                               ; preds = %18
  %24 = getelementptr inbounds i8, i8* %1, i32 1
  store i8 45, i8* %1, align 1, !tbaa !7
  %25 = fneg double %21
  br label %26

26:                                               ; preds = %23, %18
  %27 = phi i8* [ %24, %23 ], [ %1, %18 ]
  %28 = phi double [ %25, %23 ], [ %21, %18 ]
  %29 = tail call arm_aapcs_vfpcc double @log10(double %28) #6
  %30 = fptosi double %29 to i32
  %31 = icmp sgt i32 %30, 0
  %32 = select i1 %31, i32 %30, i32 0
  br label %37

33:                                               ; preds = %71
  %34 = icmp ne i32 %73, 0
  %35 = icmp slt i32 %72, 5
  %36 = and i1 %34, %35
  br i1 %36, label %79, label %85

37:                                               ; preds = %26, %71
  %38 = phi double [ %28, %26 ], [ %65, %71 ]
  %39 = phi i8* [ %27, %26 ], [ %74, %71 ]
  %40 = phi i32 [ 0, %26 ], [ %73, %71 ]
  %41 = phi i32 [ 0, %26 ], [ %72, %71 ]
  %42 = phi i32 [ %32, %26 ], [ %75, %71 ]
  %43 = sitofp i32 %42 to double
  %44 = tail call arm_aapcs_vfpcc double @pow(double 1.000000e+01, double %43) #6
  %45 = fcmp ogt double %44, 0.000000e+00
  br i1 %45, label %46, label %62

46:                                               ; preds = %37
  %47 = tail call arm_aapcs_vfpcc i32 @isinf(double %44) #6
  %48 = icmp eq i32 %47, 0
  br i1 %48, label %49, label %62

49:                                               ; preds = %46
  %50 = fdiv double %38, %44
  %51 = tail call double @llvm.floor.f64(double %50)
  %52 = fptosi double %51 to i32
  %53 = sitofp i32 %52 to double
  %54 = fmul double %44, %53
  %55 = fsub double %38, %54
  %56 = trunc i32 %52 to i8
  %57 = add i8 %56, 48
  %58 = getelementptr inbounds i8, i8* %39, i32 1
  store i8 %57, i8* %39, align 1, !tbaa !7
  %59 = icmp ne i32 %40, 0
  %60 = zext i1 %59 to i32
  %61 = add nsw i32 %41, %60
  br label %62

62:                                               ; preds = %49, %46, %37
  %63 = phi i32 [ %41, %46 ], [ %41, %37 ], [ %61, %49 ]
  %64 = phi i8* [ %39, %46 ], [ %39, %37 ], [ %58, %49 ]
  %65 = phi double [ %38, %46 ], [ %38, %37 ], [ %55, %49 ]
  %66 = icmp eq i32 %42, 0
  %67 = fcmp ogt double %65, 0.000000e+00
  %68 = and i1 %66, %67
  br i1 %68, label %69, label %71

69:                                               ; preds = %62
  %70 = getelementptr inbounds i8, i8* %64, i32 1
  store i8 46, i8* %64, align 1, !tbaa !7
  br label %71

71:                                               ; preds = %69, %62
  %72 = phi i32 [ 0, %69 ], [ %63, %62 ]
  %73 = phi i32 [ 1, %69 ], [ %40, %62 ]
  %74 = phi i8* [ %70, %69 ], [ %64, %62 ]
  %75 = add nsw i32 %42, -1
  %76 = fcmp ogt double %65, 1.000000e-05
  %77 = icmp sgt i32 %42, 0
  %78 = or i1 %77, %76
  br i1 %78, label %37, label %33

79:                                               ; preds = %33, %79
  %80 = phi i8* [ %82, %79 ], [ %74, %33 ]
  %81 = phi i32 [ %83, %79 ], [ %72, %33 ]
  %82 = getelementptr inbounds i8, i8* %80, i32 1
  store i8 48, i8* %80, align 1, !tbaa !7
  %83 = add nsw i32 %81, 1
  %84 = icmp slt i32 %81, 4
  br i1 %84, label %79, label %85

85:                                               ; preds = %79, %33
  %86 = phi i8* [ %74, %33 ], [ %82, %79 ]
  store i8 0, i8* %86, align 1, !tbaa !7
  br label %87

87:                                               ; preds = %12, %85, %16, %2, %7
  ret void
}

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @isnan(double) local_unnamed_addr #2

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i8* @strcpy_cgc(i8*, i8*) local_unnamed_addr #2

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @isinf(double) local_unnamed_addr #2

; Function Attrs: nofree nounwind optsize
declare dso_local arm_aapcs_vfpcc double @log10(double) local_unnamed_addr #3

; Function Attrs: nofree nounwind optsize
declare dso_local arm_aapcs_vfpcc double @pow(double, double) local_unnamed_addr #3

; Function Attrs: nounwind readnone speculatable willreturn
declare double @llvm.floor.f64(double) #4

attributes #0 = { nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }
attributes #2 = { optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nofree nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind readnone speculatable willreturn }
attributes #5 = { nounwind }
attributes #6 = { nounwind optsize }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 10.0.0-4ubuntu1 "}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{!5, !5, i64 0}
