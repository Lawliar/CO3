; ModuleID = '/home/ubuntu/STMfirmware/SpearSynthetic/Target/Src/stdlib.c'
source_filename = "/home/ubuntu/STMfirmware/SpearSynthetic/Target/Src/stdlib.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "thumbv7em-none-unknown-eabi"

; Function Attrs: norecurse nounwind optsize readnone
define dso_local arm_aapcs_vfpcc i32 @isspace(i32 %0) local_unnamed_addr #0 {
  switch i32 %0, label %2 [
    i32 32, label %3
    i32 13, label %3
    i32 12, label %3
    i32 11, label %3
    i32 10, label %3
    i32 9, label %3
  ]

2:                                                ; preds = %1
  br label %3

3:                                                ; preds = %1, %1, %1, %1, %1, %1, %2
  %4 = phi i32 [ 0, %2 ], [ 1, %1 ], [ 1, %1 ], [ 1, %1 ], [ 1, %1 ], [ 1, %1 ], [ 1, %1 ]
  ret i32 %4
}

; Function Attrs: norecurse nounwind optsize readnone
define dso_local arm_aapcs_vfpcc i32 @isdigit(i32 %0) local_unnamed_addr #0 {
  %2 = add i32 %0, -48
  %3 = icmp ult i32 %2, 10
  %4 = zext i1 %3 to i32
  ret i32 %4
}

; Function Attrs: norecurse nounwind optsize readnone
define dso_local arm_aapcs_vfpcc i32 @isnan(double %0) local_unnamed_addr #0 {
  %2 = fcmp uno double %0, 0.000000e+00
  %3 = zext i1 %2 to i32
  ret i32 %3
}

; Function Attrs: nounwind optsize readnone
define dso_local arm_aapcs_vfpcc i32 @isinf(double %0) local_unnamed_addr #1 {
  %2 = tail call double @llvm.fabs.f64(double %0) #9
  %3 = fcmp oeq double %2, 0x7FF0000000000000
  %4 = zext i1 %3 to i32
  ret i32 %4
}

; Function Attrs: nounwind readnone speculatable willreturn
declare double @llvm.fabs.f64(double) #2

; Function Attrs: norecurse nounwind optsize readonly
define dso_local arm_aapcs_vfpcc double @atof(i8* readonly %0) local_unnamed_addr #3 {
  %2 = icmp eq i8* %0, null
  br i1 %2, label %46, label %3

3:                                                ; preds = %1, %6
  %4 = phi i8* [ %7, %6 ], [ %0, %1 ]
  %5 = load i8, i8* %4, align 1, !tbaa !3
  switch i8 %5, label %8 [
    i8 32, label %6
    i8 13, label %6
    i8 12, label %6
    i8 11, label %6
    i8 10, label %6
    i8 9, label %6
  ]

6:                                                ; preds = %3, %3, %3, %3, %3, %3
  %7 = getelementptr inbounds i8, i8* %4, i32 1
  br label %3

8:                                                ; preds = %3, %36
  %9 = phi i8 [ %42, %36 ], [ %5, %3 ]
  %10 = phi i8* [ %41, %36 ], [ %4, %3 ]
  %11 = phi double [ %37, %36 ], [ 0.000000e+00, %3 ]
  %12 = phi double [ %38, %36 ], [ 1.000000e-01, %3 ]
  %13 = phi i32 [ %39, %36 ], [ 1, %3 ]
  %14 = phi i32 [ %40, %36 ], [ 0, %3 ]
  switch i8 %9, label %21 [
    i8 0, label %43
    i8 45, label %15
    i8 43, label %17
    i8 46, label %19
  ]

15:                                               ; preds = %8
  %16 = icmp eq i32 %14, 0
  br i1 %16, label %36, label %46

17:                                               ; preds = %8
  %18 = icmp eq i32 %14, 0
  br i1 %18, label %36, label %46

19:                                               ; preds = %8
  %20 = icmp ult i32 %14, 2
  br i1 %20, label %36, label %46

21:                                               ; preds = %8
  %22 = zext i8 %9 to i32
  %23 = add nsw i32 %22, -48
  %24 = icmp ult i32 %23, 10
  br i1 %24, label %25, label %43

25:                                               ; preds = %21
  %26 = icmp ult i32 %14, 2
  br i1 %26, label %27, label %31

27:                                               ; preds = %25
  %28 = fmul double %11, 1.000000e+01
  %29 = sitofp i32 %23 to double
  %30 = fadd double %28, %29
  br label %36

31:                                               ; preds = %25
  %32 = sitofp i32 %23 to double
  %33 = fmul double %12, %32
  %34 = fadd double %11, %33
  %35 = fdiv double %12, 1.000000e+01
  br label %36

36:                                               ; preds = %19, %17, %15, %31, %27
  %37 = phi double [ %30, %27 ], [ %34, %31 ], [ %11, %15 ], [ %11, %17 ], [ %11, %19 ]
  %38 = phi double [ %12, %27 ], [ %35, %31 ], [ %12, %15 ], [ %12, %17 ], [ %12, %19 ]
  %39 = phi i32 [ %13, %27 ], [ %13, %31 ], [ -1, %15 ], [ %13, %17 ], [ %13, %19 ]
  %40 = phi i32 [ 1, %27 ], [ 2, %31 ], [ 1, %15 ], [ 1, %17 ], [ 2, %19 ]
  %41 = getelementptr inbounds i8, i8* %10, i32 1
  %42 = load i8, i8* %41, align 1, !tbaa !3
  br label %8

43:                                               ; preds = %8, %21
  %44 = sitofp i32 %13 to double
  %45 = fmul double %11, %44
  br label %46

46:                                               ; preds = %15, %17, %19, %43, %1
  %47 = phi double [ 0.000000e+00, %1 ], [ %45, %43 ], [ 0.000000e+00, %19 ], [ 0.000000e+00, %17 ], [ 0.000000e+00, %15 ]
  ret double %47
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #4

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #4

; Function Attrs: norecurse nounwind optsize readonly
define dso_local arm_aapcs_vfpcc i32 @atoi(i8* readonly %0) local_unnamed_addr #3 {
  %2 = icmp eq i8* %0, null
  br i1 %2, label %37, label %3

3:                                                ; preds = %1, %6
  %4 = phi i8* [ %7, %6 ], [ %0, %1 ]
  %5 = load i8, i8* %4, align 1, !tbaa !3
  switch i8 %5, label %8 [
    i8 32, label %6
    i8 13, label %6
    i8 12, label %6
    i8 11, label %6
    i8 10, label %6
    i8 9, label %6
  ]

6:                                                ; preds = %3, %3, %3, %3, %3, %3
  %7 = getelementptr inbounds i8, i8* %4, i32 1
  br label %3

8:                                                ; preds = %3, %28
  %9 = phi i8 [ %33, %28 ], [ %5, %3 ]
  %10 = phi i8* [ %32, %28 ], [ %4, %3 ]
  %11 = phi i32 [ %29, %28 ], [ 0, %3 ]
  %12 = phi i32 [ %30, %28 ], [ 1, %3 ]
  %13 = phi i32 [ 1, %28 ], [ 0, %3 ]
  %14 = phi i32 [ %31, %28 ], [ 0, %3 ]
  switch i8 %9, label %19 [
    i8 0, label %34
    i8 45, label %15
    i8 43, label %17
  ]

15:                                               ; preds = %8
  %16 = icmp eq i32 %13, 0
  br i1 %16, label %28, label %37

17:                                               ; preds = %8
  %18 = icmp eq i32 %13, 0
  br i1 %18, label %28, label %37

19:                                               ; preds = %8
  %20 = zext i8 %9 to i32
  %21 = add nsw i32 %20, -48
  %22 = icmp ult i32 %21, 10
  br i1 %22, label %23, label %34

23:                                               ; preds = %19
  %24 = mul nsw i32 %11, 10
  %25 = add nsw i32 %24, %21
  %26 = add nsw i32 %14, 1
  %27 = icmp eq i32 %26, 9
  br i1 %27, label %34, label %28

28:                                               ; preds = %17, %15, %23
  %29 = phi i32 [ %25, %23 ], [ %11, %15 ], [ %11, %17 ]
  %30 = phi i32 [ %12, %23 ], [ -1, %15 ], [ %12, %17 ]
  %31 = phi i32 [ %26, %23 ], [ %14, %15 ], [ %14, %17 ]
  %32 = getelementptr inbounds i8, i8* %10, i32 1
  %33 = load i8, i8* %32, align 1, !tbaa !3
  br label %8

34:                                               ; preds = %8, %19, %23
  %35 = phi i32 [ %25, %23 ], [ %11, %19 ], [ %11, %8 ]
  %36 = mul nsw i32 %35, %12
  br label %37

37:                                               ; preds = %15, %17, %34, %1
  %38 = phi i32 [ 0, %1 ], [ %36, %34 ], [ 0, %17 ], [ 0, %15 ]
  ret i32 %38
}

; Function Attrs: nofree norecurse nounwind optsize
define dso_local arm_aapcs_vfpcc i8* @strcpy_cgc(i8* returned %0, i8* nocapture readonly %1) local_unnamed_addr #5 {
  %3 = load i8, i8* %1, align 1, !tbaa !3
  %4 = icmp eq i8 %3, 0
  br i1 %4, label %14, label %5

5:                                                ; preds = %2, %5
  %6 = phi i8* [ %13, %5 ], [ %0, %2 ]
  %7 = phi i8 [ %11, %5 ], [ %3, %2 ]
  %8 = phi i32 [ %9, %5 ], [ 0, %2 ]
  store i8 %7, i8* %6, align 1, !tbaa !3
  %9 = add i32 %8, 1
  %10 = getelementptr inbounds i8, i8* %1, i32 %9
  %11 = load i8, i8* %10, align 1, !tbaa !3
  %12 = icmp eq i8 %11, 0
  %13 = getelementptr inbounds i8, i8* %0, i32 %9
  br i1 %12, label %14, label %5

14:                                               ; preds = %5, %2
  %15 = phi i8* [ %0, %2 ], [ %13, %5 ]
  store i8 0, i8* %15, align 1, !tbaa !3
  ret i8* %0
}

; Function Attrs: nofree norecurse nounwind optsize writeonly
define dso_local arm_aapcs_vfpcc void @bzero_cgc(i8* %0, i32 %1) local_unnamed_addr #6 {
  %3 = icmp ne i8* %0, null
  %4 = icmp ne i32 %1, 0
  %5 = and i1 %4, %3
  br i1 %5, label %6, label %7

6:                                                ; preds = %2
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 %0, i8 0, i32 %1, i1 false)
  br label %7

7:                                                ; preds = %6, %2
  ret void
}

; Function Attrs: norecurse nounwind optsize readonly
define dso_local arm_aapcs_vfpcc i32 @strcmp(i8* nocapture readonly %0, i8* nocapture readonly %1) local_unnamed_addr #3 {
  %3 = load i8, i8* %0, align 1, !tbaa !3
  %4 = icmp eq i8 %3, 0
  br i1 %4, label %16, label %5

5:                                                ; preds = %2, %11
  %6 = phi i8 [ %14, %11 ], [ %3, %2 ]
  %7 = phi i8* [ %13, %11 ], [ %1, %2 ]
  %8 = phi i8* [ %12, %11 ], [ %0, %2 ]
  %9 = load i8, i8* %7, align 1, !tbaa !3
  %10 = icmp eq i8 %6, %9
  br i1 %10, label %11, label %16

11:                                               ; preds = %5
  %12 = getelementptr inbounds i8, i8* %8, i32 1
  %13 = getelementptr inbounds i8, i8* %7, i32 1
  %14 = load i8, i8* %12, align 1, !tbaa !3
  %15 = icmp eq i8 %14, 0
  br i1 %15, label %16, label %5

16:                                               ; preds = %5, %11, %2
  %17 = phi i8* [ %1, %2 ], [ %13, %11 ], [ %7, %5 ]
  %18 = phi i8 [ 0, %2 ], [ 0, %11 ], [ %6, %5 ]
  %19 = zext i8 %18 to i32
  %20 = load i8, i8* %17, align 1, !tbaa !3
  %21 = zext i8 %20 to i32
  %22 = sub nsw i32 %19, %21
  ret i32 %22
}

; Function Attrs: nofree norecurse nounwind optsize
define dso_local arm_aapcs_vfpcc i8* @strncat_cgc(i8* returned %0, i8* readonly %1, i32 %2) local_unnamed_addr #5 {
  %4 = icmp eq i8* %0, null
  br i1 %4, label %29, label %5

5:                                                ; preds = %3, %5
  %6 = phi i32 [ %10, %5 ], [ 0, %3 ]
  %7 = getelementptr inbounds i8, i8* %0, i32 %6
  %8 = load i8, i8* %7, align 1, !tbaa !3
  %9 = icmp eq i8 %8, 0
  %10 = add i32 %6, 1
  br i1 %9, label %11, label %5

11:                                               ; preds = %5
  %12 = icmp eq i8* %1, null
  br i1 %12, label %29, label %13

13:                                               ; preds = %11
  %14 = icmp eq i32 %2, 0
  br i1 %14, label %25, label %15

15:                                               ; preds = %13, %20
  %16 = phi i32 [ %23, %20 ], [ 0, %13 ]
  %17 = getelementptr inbounds i8, i8* %1, i32 %16
  %18 = load i8, i8* %17, align 1, !tbaa !3
  %19 = icmp eq i8 %18, 0
  br i1 %19, label %25, label %20

20:                                               ; preds = %15
  %21 = add i32 %16, %6
  %22 = getelementptr inbounds i8, i8* %0, i32 %21
  store i8 %18, i8* %22, align 1, !tbaa !3
  %23 = add nuw i32 %16, 1
  %24 = icmp eq i32 %23, %2
  br i1 %24, label %25, label %15

25:                                               ; preds = %15, %20, %13
  %26 = phi i32 [ 0, %13 ], [ %16, %15 ], [ %2, %20 ]
  %27 = add i32 %26, %6
  %28 = getelementptr inbounds i8, i8* %0, i32 %27
  store i8 0, i8* %28, align 1, !tbaa !3
  br label %29

29:                                               ; preds = %3, %11, %25
  ret i8* %0
}

; Function Attrs: norecurse nounwind optsize readonly
define dso_local arm_aapcs_vfpcc i32 @strlen_cgc(i8* readonly %0) local_unnamed_addr #3 {
  %2 = icmp eq i8* %0, null
  br i1 %2, label %9, label %3

3:                                                ; preds = %1, %3
  %4 = phi i32 [ %8, %3 ], [ 0, %1 ]
  %5 = getelementptr inbounds i8, i8* %0, i32 %4
  %6 = load i8, i8* %5, align 1, !tbaa !3
  %7 = icmp eq i8 %6, 0
  %8 = add i32 %4, 1
  br i1 %7, label %9, label %3

9:                                                ; preds = %3, %1
  %10 = phi i32 [ 0, %1 ], [ %4, %3 ]
  ret i32 %10
}

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc i32 @receive_cgc_until(i8* nocapture %0, i8 zeroext %1, i32 %2) local_unnamed_addr #7 {
  %4 = alloca i32, align 4
  %5 = alloca i8, align 1
  %6 = bitcast i32* %4 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %6) #10
  store i32 0, i32* %4, align 4, !tbaa !6
  call void @llvm.lifetime.start.p0i8(i64 1, i8* nonnull %5) #10
  store i8 0, i8* %5, align 1, !tbaa !3
  %7 = icmp eq i32 %2, 0
  br i1 %7, label %19, label %8

8:                                                ; preds = %3, %16
  %9 = phi i32 [ %17, %16 ], [ 0, %3 ]
  %10 = getelementptr inbounds i8, i8* %0, i32 %9
  store i8 0, i8* %10, align 1, !tbaa !3
  %11 = call arm_aapcs_vfpcc i32 @receive_cgc(i8* nonnull %5, i32 1, i32* nonnull %4) #11
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %13, label %19

13:                                               ; preds = %8
  %14 = load i8, i8* %5, align 1, !tbaa !3
  %15 = icmp eq i8 %14, %1
  br i1 %15, label %19, label %16

16:                                               ; preds = %13
  store i8 %14, i8* %10, align 1, !tbaa !3
  %17 = add nuw i32 %9, 1
  %18 = icmp eq i32 %17, %2
  br i1 %18, label %19, label %8

19:                                               ; preds = %13, %8, %16, %3
  %20 = phi i32 [ 0, %3 ], [ 0, %8 ], [ %2, %16 ], [ %9, %13 ]
  call void @llvm.lifetime.end.p0i8(i64 1, i8* nonnull %5) #10
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %6) #10
  ret i32 %20
}

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @receive_cgc(i8*, i32, i32*) local_unnamed_addr #8

; Function Attrs: nofree norecurse nounwind optsize
define dso_local arm_aapcs_vfpcc i32 @strcat_cgc(i8* %0, i8* readonly %1) local_unnamed_addr #5 {
  %3 = icmp eq i8* %0, null
  %4 = icmp eq i8* %1, null
  %5 = or i1 %3, %4
  br i1 %5, label %25, label %6

6:                                                ; preds = %2, %6
  %7 = phi i32 [ %11, %6 ], [ 0, %2 ]
  %8 = getelementptr inbounds i8, i8* %0, i32 %7
  %9 = load i8, i8* %8, align 1, !tbaa !3
  %10 = icmp eq i8 %9, 0
  %11 = add i32 %7, 1
  br i1 %10, label %12, label %6

12:                                               ; preds = %6
  %13 = load i8, i8* %1, align 1, !tbaa !3
  %14 = icmp eq i8 %13, 0
  br i1 %14, label %25, label %15

15:                                               ; preds = %12, %15
  %16 = phi i8 [ %23, %15 ], [ %13, %12 ]
  %17 = phi i32 [ %20, %15 ], [ %7, %12 ]
  %18 = phi i32 [ %21, %15 ], [ 0, %12 ]
  %19 = getelementptr inbounds i8, i8* %0, i32 %17
  store i8 %16, i8* %19, align 1, !tbaa !3
  %20 = add i32 %17, 1
  %21 = add i32 %18, 1
  %22 = getelementptr inbounds i8, i8* %1, i32 %21
  %23 = load i8, i8* %22, align 1, !tbaa !3
  %24 = icmp eq i8 %23, 0
  br i1 %24, label %25, label %15

25:                                               ; preds = %15, %12, %2
  %26 = phi i32 [ 0, %2 ], [ %7, %12 ], [ %20, %15 ]
  ret i32 %26
}

; Function Attrs: nofree norecurse nounwind optsize writeonly
define dso_local arm_aapcs_vfpcc i32 @itoa(i8* %0, i32 %1, i32 %2) local_unnamed_addr #6 {
  %4 = icmp eq i8* %0, null
  br i1 %4, label %26, label %5

5:                                                ; preds = %3, %5
  %6 = phi i32 [ %8, %5 ], [ 0, %3 ]
  %7 = phi i32 [ %9, %5 ], [ %1, %3 ]
  %8 = add i32 %6, 1
  %9 = udiv i32 %7, 10
  %10 = icmp ugt i32 %7, 9
  br i1 %10, label %5, label %11

11:                                               ; preds = %5
  %12 = icmp ult i32 %8, %2
  br i1 %12, label %13, label %26

13:                                               ; preds = %11, %13
  %14 = phi i32 [ %16, %13 ], [ %1, %11 ]
  %15 = phi i32 [ %22, %13 ], [ %6, %11 ]
  %16 = udiv i32 %14, 10
  %17 = mul i32 %16, 10
  %18 = sub i32 %14, %17
  %19 = trunc i32 %18 to i8
  %20 = or i8 %19, 48
  %21 = getelementptr inbounds i8, i8* %0, i32 %15
  store i8 %20, i8* %21, align 1, !tbaa !3
  %22 = add i32 %15, -1
  %23 = icmp ugt i32 %14, 9
  br i1 %23, label %13, label %24

24:                                               ; preds = %13
  %25 = getelementptr inbounds i8, i8* %0, i32 %8
  store i8 0, i8* %25, align 1, !tbaa !3
  br label %26

26:                                               ; preds = %11, %3, %24
  %27 = phi i32 [ 0, %3 ], [ 0, %11 ], [ %8, %24 ]
  ret i32 %27
}

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc void @puts_cgc(i8* %0) local_unnamed_addr #7 {
  %2 = alloca i32, align 4
  %3 = bitcast i32* %2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %3) #10
  %4 = icmp eq i8* %0, null
  br i1 %4, label %11, label %5

5:                                                ; preds = %1, %5
  %6 = phi i32 [ %10, %5 ], [ 0, %1 ]
  %7 = getelementptr inbounds i8, i8* %0, i32 %6
  %8 = load i8, i8* %7, align 1, !tbaa !3
  %9 = icmp eq i8 %8, 0
  %10 = add i32 %6, 1
  br i1 %9, label %11, label %5

11:                                               ; preds = %5, %1
  %12 = phi i32 [ 0, %1 ], [ %6, %5 ]
  %13 = call arm_aapcs_vfpcc i32 @transmit(i32 1, i8* %0, i32 %12, i32* nonnull %2) #11
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %3) #10
  ret void
}

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @transmit(i32, i8*, i32, i32*) local_unnamed_addr #8

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i1 immarg) #4

attributes #0 = { norecurse nounwind optsize readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind optsize readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone speculatable willreturn }
attributes #3 = { norecurse nounwind optsize readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { argmemonly nounwind willreturn }
attributes #5 = { nofree norecurse nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { nofree norecurse nounwind optsize writeonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { readnone }
attributes #10 = { nounwind }
attributes #11 = { nounwind optsize }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 10.0.0-4ubuntu1 "}
!3 = !{!4, !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!7, !7, i64 0}
!7 = !{!"long", !4, i64 0}
