; ModuleID = '/home/ubuntu/STMfirmware/SpearModbusDMASpear/Target/Src/service.c'
source_filename = "/home/ubuntu/STMfirmware/SpearModbusDMASpear/Target/Src/service.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "thumbv7em-none-unknown-eabi"

%struct.user_manager = type { %struct.user*, i32 }
%struct.user = type { [16 x i8], %struct.message_manager*, %struct.user* }
%struct.message_manager = type { %struct.message*, i32 }
%struct.message = type { %struct.message*, i32, i32, [32 x i8] }

@.str = private unnamed_addr constant [41 x i8] c"[-] Error Failed to allocate_cgc buffer\0A\00", align 1
@.str.1 = private unnamed_addr constant [29 x i8] c"[-] Error username too long\0A\00", align 1
@.str.2 = private unnamed_addr constant [23 x i8] c"[-] Error user exists\0A\00", align 1
@.str.3 = private unnamed_addr constant [37 x i8] c"[-] Error allocating user structure\0A\00", align 1
@.str.4 = private unnamed_addr constant [28 x i8] c"[-] Error Message too long\0A\00", align 1
@.str.5 = private unnamed_addr constant [49 x i8] c"[-] Error Failed to allocate_cgc message struct\0A\00", align 1
@.str.6 = private unnamed_addr constant [29 x i8] c"[-] Message ID out of range\0A\00", align 1
@.str.7 = private unnamed_addr constant [26 x i8] c"[-] Message ID not found\0A\00", align 1
@.str.8 = private unnamed_addr constant [37 x i8] c"***********************************\0A\00", align 1
@.str.9 = private unnamed_addr constant [4 x i8] c":  \00", align 1
@.str.10 = private unnamed_addr constant [38 x i8] c"\0A***********************************\0A\00", align 1
@.str.11 = private unnamed_addr constant [18 x i8] c"Unread messages:\0A\00", align 1
@.str.12 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@.str.13 = private unnamed_addr constant [24 x i8] c"[-] Error invalid user\0A\00", align 1
@.str.14 = private unnamed_addr constant [30 x i8] c"[-] Failed to create message\0A\00", align 1
@.str.15 = private unnamed_addr constant [27 x i8] c"[-] Failed to add message\0A\00", align 1
@.str.16 = private unnamed_addr constant [17 x i8] c"1) Send Message\0A\00", align 1
@.str.17 = private unnamed_addr constant [17 x i8] c"2) Read Message\0A\00", align 1
@.str.18 = private unnamed_addr constant [18 x i8] c"3) List Messages\0A\00", align 1
@.str.19 = private unnamed_addr constant [19 x i8] c"4) Delete Message\0A\00", align 1
@.str.20 = private unnamed_addr constant [11 x i8] c"5) Logout\0A\00", align 1
@.str.21 = private unnamed_addr constant [9 x i8] c"6) Exit\0A\00", align 1
@.str.22 = private unnamed_addr constant [3 x i8] c": \00", align 1
@.str.23 = private unnamed_addr constant [24 x i8] c"[-] receive_cgc failed\0A\00", align 1
@.str.24 = private unnamed_addr constant [20 x i8] c"[-] Invalid choice\0A\00", align 1
@.str.25 = private unnamed_addr constant [12 x i8] c"Exiting...\0A\00", align 1
@.str.26 = private unnamed_addr constant [16 x i8] c"Logging out...\0A\00", align 1
@.str.27 = private unnamed_addr constant [5 x i8] c"ID: \00", align 1
@.str.28 = private unnamed_addr constant [5 x i8] c"To: \00", align 1
@.str.29 = private unnamed_addr constant [10 x i8] c"Message: \00", align 1
@.str.30 = private unnamed_addr constant [16 x i8] c"1) Create User\0A\00", align 1
@.str.31 = private unnamed_addr constant [10 x i8] c"2) Login\0A\00", align 1
@.str.32 = private unnamed_addr constant [9 x i8] c"3) Exit\0A\00", align 1
@.str.33 = private unnamed_addr constant [24 x i8] c"[-] receive_cgc Failed\0A\00", align 1
@.str.34 = private unnamed_addr constant [11 x i8] c"username: \00", align 1
@.str.35 = private unnamed_addr constant [15 x i8] c"Login Success\0A\00", align 1
@.str.36 = private unnamed_addr constant [14 x i8] c"Login Failed\0A\00", align 1

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc void @list_users(%struct.user_manager* readonly %0) local_unnamed_addr #0 {
  %2 = icmp eq %struct.user_manager* %0, null
  br i1 %2, label %13, label %3

3:                                                ; preds = %1
  %4 = getelementptr inbounds %struct.user_manager, %struct.user_manager* %0, i32 0, i32 0
  %5 = load %struct.user*, %struct.user** %4, align 4, !tbaa !3
  %6 = icmp eq %struct.user* %5, null
  br i1 %6, label %13, label %7

7:                                                ; preds = %3, %7
  %8 = phi %struct.user* [ %11, %7 ], [ %5, %3 ]
  %9 = getelementptr inbounds %struct.user, %struct.user* %8, i32 0, i32 0, i32 0
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* nonnull %9) #7
  %10 = getelementptr inbounds %struct.user, %struct.user* %8, i32 0, i32 2
  %11 = load %struct.user*, %struct.user** %10, align 4, !tbaa !3
  %12 = icmp eq %struct.user* %11, null
  br i1 %12, label %13, label %7

13:                                               ; preds = %7, %3, %1
  ret void
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc void @puts_cgc(i8*) local_unnamed_addr #2

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc %struct.user_manager* @init_users() local_unnamed_addr #0 {
  %1 = alloca %struct.user_manager*, align 4
  %2 = bitcast %struct.user_manager** %1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %2) #5
  store %struct.user_manager* null, %struct.user_manager** %1, align 4, !tbaa !3
  %3 = bitcast %struct.user_manager** %1 to i8**
  %4 = call arm_aapcs_vfpcc i32 @allocate_cgc(i32 8, i32 0, i8** nonnull %3) #7
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %8, label %6

6:                                                ; preds = %0
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str, i32 0, i32 0)) #7
  %7 = load %struct.user_manager*, %struct.user_manager** %1, align 4, !tbaa !3
  br label %13

8:                                                ; preds = %0
  %9 = load %struct.user_manager*, %struct.user_manager** %1, align 4, !tbaa !3
  %10 = getelementptr inbounds %struct.user_manager, %struct.user_manager* %9, i32 0, i32 0
  store %struct.user* null, %struct.user** %10, align 4, !tbaa !7
  %11 = load %struct.user_manager*, %struct.user_manager** %1, align 4, !tbaa !3
  %12 = getelementptr inbounds %struct.user_manager, %struct.user_manager* %11, i32 0, i32 1
  store i32 0, i32* %12, align 4, !tbaa !10
  br label %13

13:                                               ; preds = %8, %6
  %14 = phi %struct.user_manager* [ %11, %8 ], [ %7, %6 ]
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %2) #5
  ret %struct.user_manager* %14
}

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @allocate_cgc(i32, i32, i8**) local_unnamed_addr #2

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc i32 @create_user(%struct.user_manager* %0, i8* %1) local_unnamed_addr #0 {
  %3 = alloca %struct.user*, align 4
  %4 = bitcast %struct.user** %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %4) #5
  store %struct.user* null, %struct.user** %3, align 4, !tbaa !3
  %5 = icmp eq %struct.user_manager* %0, null
  %6 = icmp eq i8* %1, null
  %7 = or i1 %5, %6
  br i1 %7, label %57, label %8

8:                                                ; preds = %2
  %9 = tail call arm_aapcs_vfpcc i32 @strlen_cgc(i8* nonnull %1) #7
  %10 = icmp ugt i32 %9, 15
  br i1 %10, label %11, label %12

11:                                               ; preds = %8
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0)) #7
  br label %57

12:                                               ; preds = %8
  %13 = getelementptr inbounds %struct.user_manager, %struct.user_manager* %0, i32 0, i32 0
  %14 = load %struct.user*, %struct.user** %13, align 4, !tbaa !3
  %15 = icmp eq %struct.user* %14, null
  br i1 %15, label %26, label %20

16:                                               ; preds = %20
  %17 = getelementptr inbounds %struct.user, %struct.user* %21, i32 0, i32 2
  %18 = load %struct.user*, %struct.user** %17, align 4, !tbaa !3
  %19 = icmp eq %struct.user* %18, null
  br i1 %19, label %26, label %20

20:                                               ; preds = %12, %16
  %21 = phi %struct.user* [ %18, %16 ], [ %14, %12 ]
  %22 = getelementptr inbounds %struct.user, %struct.user* %21, i32 0, i32 0, i32 0
  %23 = tail call arm_aapcs_vfpcc i32 @strcmp(i8* nonnull %22, i8* nonnull dereferenceable(1) %1) #8
  %24 = icmp eq i32 %23, 0
  br i1 %24, label %25, label %16

25:                                               ; preds = %20
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.2, i32 0, i32 0)) #7
  br label %57

26:                                               ; preds = %16, %12
  %27 = bitcast %struct.user** %3 to i8**
  %28 = call arm_aapcs_vfpcc i32 @allocate_cgc(i32 24, i32 0, i8** nonnull %27) #7
  %29 = icmp eq i32 %28, 0
  br i1 %29, label %31, label %30

30:                                               ; preds = %26
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.3, i32 0, i32 0)) #7
  br label %57

31:                                               ; preds = %26
  %32 = load %struct.user*, %struct.user** %3, align 4, !tbaa !3
  %33 = getelementptr inbounds %struct.user, %struct.user* %32, i32 0, i32 2
  store %struct.user* null, %struct.user** %33, align 4, !tbaa !11
  %34 = getelementptr inbounds %struct.user, %struct.user* %32, i32 0, i32 0, i32 0
  store i8 0, i8* %34, align 4, !tbaa !13
  %35 = call arm_aapcs_vfpcc %struct.message_manager* @init_manager() #8
  %36 = load %struct.user*, %struct.user** %3, align 4, !tbaa !3
  %37 = getelementptr inbounds %struct.user, %struct.user* %36, i32 0, i32 1
  store %struct.message_manager* %35, %struct.message_manager** %37, align 4, !tbaa !14
  %38 = getelementptr inbounds %struct.user, %struct.user* %36, i32 0, i32 0, i32 0
  %39 = call arm_aapcs_vfpcc i8* @strncat_cgc(i8* %38, i8* %1, i32 16) #7
  %40 = load %struct.user*, %struct.user** %13, align 4, !tbaa !7
  %41 = icmp eq %struct.user* %40, null
  br i1 %41, label %42, label %49

42:                                               ; preds = %31
  %43 = bitcast %struct.user** %3 to i32*
  %44 = load i32, i32* %43, align 4, !tbaa !3
  %45 = bitcast %struct.user_manager* %0 to i32*
  store i32 %44, i32* %45, align 4, !tbaa !7
  %46 = getelementptr inbounds %struct.user_manager, %struct.user_manager* %0, i32 0, i32 1
  %47 = load i32, i32* %46, align 4, !tbaa !10
  %48 = add i32 %47, 1
  store i32 %48, i32* %46, align 4, !tbaa !10
  br label %57

49:                                               ; preds = %31
  %50 = load %struct.user*, %struct.user** %3, align 4, !tbaa !3
  %51 = getelementptr inbounds %struct.user, %struct.user* %50, i32 0, i32 2
  store %struct.user* %40, %struct.user** %51, align 4, !tbaa !11
  %52 = ptrtoint %struct.user* %50 to i32
  %53 = bitcast %struct.user_manager* %0 to i32*
  store i32 %52, i32* %53, align 4, !tbaa !7
  %54 = getelementptr inbounds %struct.user_manager, %struct.user_manager* %0, i32 0, i32 1
  %55 = load i32, i32* %54, align 4, !tbaa !10
  %56 = add i32 %55, 1
  store i32 %56, i32* %54, align 4, !tbaa !10
  br label %57

57:                                               ; preds = %2, %49, %42, %30, %25, %11
  %58 = phi i32 [ 0, %2 ], [ 0, %11 ], [ 0, %25 ], [ 0, %30 ], [ %48, %42 ], [ %56, %49 ]
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %4) #5
  ret i32 %58
}

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @strlen_cgc(i8*) local_unnamed_addr #2

; Function Attrs: nofree nounwind optsize readonly
declare dso_local arm_aapcs_vfpcc i32 @strcmp(i8* nocapture, i8* nocapture) local_unnamed_addr #3

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc %struct.message_manager* @init_manager() local_unnamed_addr #0 {
  %1 = alloca %struct.message_manager*, align 4
  %2 = bitcast %struct.message_manager** %1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %2) #5
  store %struct.message_manager* null, %struct.message_manager** %1, align 4, !tbaa !3
  %3 = bitcast %struct.message_manager** %1 to i8**
  %4 = call arm_aapcs_vfpcc i32 @allocate_cgc(i32 8, i32 0, i8** nonnull %3) #7
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %8, label %6

6:                                                ; preds = %0
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str, i32 0, i32 0)) #7
  %7 = load %struct.message_manager*, %struct.message_manager** %1, align 4, !tbaa !3
  br label %13

8:                                                ; preds = %0
  %9 = load %struct.message_manager*, %struct.message_manager** %1, align 4, !tbaa !3
  %10 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %9, i32 0, i32 0
  store %struct.message* null, %struct.message** %10, align 4, !tbaa !15
  %11 = load %struct.message_manager*, %struct.message_manager** %1, align 4, !tbaa !3
  %12 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %11, i32 0, i32 1
  store i32 0, i32* %12, align 4, !tbaa !17
  br label %13

13:                                               ; preds = %8, %6
  %14 = phi %struct.message_manager* [ %11, %8 ], [ %7, %6 ]
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %2) #5
  ret %struct.message_manager* %14
}

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i8* @strncat_cgc(i8*, i8*, i32) local_unnamed_addr #2

; Function Attrs: nofree norecurse nounwind optsize
define dso_local arm_aapcs_vfpcc i32 @add_message(%struct.message_manager* %0, %struct.message* %1) local_unnamed_addr #4 {
  %3 = icmp eq %struct.message* %1, null
  %4 = icmp eq %struct.message_manager* %0, null
  %5 = or i1 %4, %3
  br i1 %5, label %27, label %6

6:                                                ; preds = %2
  %7 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %0, i32 0, i32 0
  %8 = load %struct.message*, %struct.message** %7, align 4, !tbaa !15
  %9 = icmp eq %struct.message* %8, null
  br i1 %9, label %10, label %14

10:                                               ; preds = %6
  store %struct.message* %1, %struct.message** %7, align 4, !tbaa !15
  %11 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %0, i32 0, i32 1
  %12 = load i32, i32* %11, align 4, !tbaa !17
  %13 = add i32 %12, 1
  store i32 %13, i32* %11, align 4, !tbaa !17
  br label %24

14:                                               ; preds = %6, %14
  %15 = phi %struct.message* [ %17, %14 ], [ %8, %6 ]
  %16 = getelementptr inbounds %struct.message, %struct.message* %15, i32 0, i32 0
  %17 = load %struct.message*, %struct.message** %16, align 4, !tbaa !18
  %18 = icmp eq %struct.message* %17, null
  br i1 %18, label %19, label %14

19:                                               ; preds = %14
  %20 = getelementptr inbounds %struct.message, %struct.message* %15, i32 0, i32 0
  store %struct.message* %1, %struct.message** %20, align 4, !tbaa !18
  %21 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %0, i32 0, i32 1
  %22 = load i32, i32* %21, align 4, !tbaa !17
  %23 = add i32 %22, 1
  store i32 %23, i32* %21, align 4, !tbaa !17
  br label %24

24:                                               ; preds = %10, %19
  %25 = phi i32 [ %23, %19 ], [ %13, %10 ]
  %26 = getelementptr inbounds %struct.message, %struct.message* %1, i32 0, i32 1
  store i32 %25, i32* %26, align 4, !tbaa !20
  br label %27

27:                                               ; preds = %24, %2
  %28 = phi i32 [ 0, %2 ], [ %25, %24 ]
  ret i32 %28
}

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc %struct.message* @create_message(i8* %0) local_unnamed_addr #0 {
  %2 = alloca %struct.message*, align 4
  %3 = bitcast %struct.message** %2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %3) #5
  store %struct.message* null, %struct.message** %2, align 4, !tbaa !3
  %4 = icmp eq i8* %0, null
  br i1 %4, label %32, label %5

5:                                                ; preds = %1
  %6 = tail call arm_aapcs_vfpcc i32 @strlen_cgc(i8* nonnull %0) #7
  %7 = icmp ugt i32 %6, 31
  br i1 %7, label %8, label %9

8:                                                ; preds = %5
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.4, i32 0, i32 0)) #7
  br label %32

9:                                                ; preds = %5
  %10 = bitcast %struct.message** %2 to i8**
  %11 = call arm_aapcs_vfpcc i32 @allocate_cgc(i32 44, i32 0, i8** nonnull %10) #7
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %14, label %13

13:                                               ; preds = %9
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([49 x i8], [49 x i8]* @.str.5, i32 0, i32 0)) #7
  br label %32

14:                                               ; preds = %9
  %15 = load %struct.message*, %struct.message** %2, align 4, !tbaa !3
  %16 = getelementptr inbounds %struct.message, %struct.message* %15, i32 0, i32 0
  store %struct.message* null, %struct.message** %16, align 4, !tbaa !18
  %17 = load %struct.message*, %struct.message** %2, align 4, !tbaa !3
  %18 = getelementptr inbounds %struct.message, %struct.message* %17, i32 0, i32 1
  store i32 0, i32* %18, align 4, !tbaa !20
  %19 = getelementptr inbounds %struct.message, %struct.message* %17, i32 0, i32 2
  store i32 0, i32* %19, align 4, !tbaa !21
  %20 = getelementptr inbounds %struct.message, %struct.message* %17, i32 0, i32 3, i32 0
  call arm_aapcs_vfpcc void @bzero_cgc(i8* nonnull %20, i32 32) #7
  %21 = call arm_aapcs_vfpcc i32 @strlen_cgc(i8* nonnull %0) #7
  %22 = icmp eq i32 %21, 0
  br i1 %22, label %32, label %23

23:                                               ; preds = %14, %23
  %24 = phi i32 [ %29, %23 ], [ 0, %14 ]
  %25 = getelementptr inbounds i8, i8* %0, i32 %24
  %26 = load i8, i8* %25, align 1, !tbaa !13
  %27 = load %struct.message*, %struct.message** %2, align 4, !tbaa !3
  %28 = getelementptr inbounds %struct.message, %struct.message* %27, i32 0, i32 3, i32 %24
  store i8 %26, i8* %28, align 1, !tbaa !13
  %29 = add nuw nsw i32 %24, 1
  %30 = call arm_aapcs_vfpcc i32 @strlen_cgc(i8* nonnull %0) #7
  %31 = icmp ult i32 %29, %30
  br i1 %31, label %23, label %32

32:                                               ; preds = %23, %14, %1, %13, %8
  %33 = load %struct.message*, %struct.message** %2, align 4, !tbaa !3
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %3) #5
  ret %struct.message* %33
}

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc void @bzero_cgc(i8*, i32) local_unnamed_addr #2

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc void @delete_message(%struct.message_manager* %0, i32 %1) local_unnamed_addr #0 {
  %3 = icmp eq %struct.message_manager* %0, null
  br i1 %3, label %38, label %4

4:                                                ; preds = %2
  %5 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %0, i32 0, i32 1
  %6 = load i32, i32* %5, align 4, !tbaa !17
  %7 = icmp ult i32 %6, %1
  br i1 %7, label %8, label %9

8:                                                ; preds = %4
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.6, i32 0, i32 0)) #7
  br label %38

9:                                                ; preds = %4
  %10 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %0, i32 0, i32 0
  %11 = load %struct.message*, %struct.message** %10, align 4, !tbaa !3
  %12 = icmp eq %struct.message* %11, null
  br i1 %12, label %37, label %13

13:                                               ; preds = %9
  %14 = getelementptr inbounds %struct.message, %struct.message* %11, i32 0, i32 1
  %15 = load i32, i32* %14, align 4, !tbaa !20
  %16 = icmp eq i32 %15, %1
  br i1 %16, label %21, label %32

17:                                               ; preds = %32
  %18 = getelementptr inbounds %struct.message, %struct.message* %35, i32 0, i32 1
  %19 = load i32, i32* %18, align 4, !tbaa !20
  %20 = icmp eq i32 %19, %1
  br i1 %20, label %21, label %32

21:                                               ; preds = %17, %13
  %22 = phi %struct.message* [ %11, %13 ], [ %35, %17 ]
  %23 = phi %struct.message* [ null, %13 ], [ %33, %17 ]
  %24 = icmp eq %struct.message* %23, null
  %25 = bitcast %struct.message* %22 to i32*
  %26 = load i32, i32* %25, align 4, !tbaa !18
  %27 = bitcast %struct.message* %23 to i32*
  %28 = bitcast %struct.message_manager* %0 to i32*
  %29 = select i1 %24, i32* %28, i32* %27
  store i32 %26, i32* %29, align 4, !tbaa !3
  %30 = bitcast %struct.message* %22 to i8*
  %31 = tail call arm_aapcs_vfpcc i32 @deallocate_cgc(i8* nonnull %30, i32 44) #7
  br label %38

32:                                               ; preds = %13, %17
  %33 = phi %struct.message* [ %35, %17 ], [ %11, %13 ]
  %34 = getelementptr inbounds %struct.message, %struct.message* %33, i32 0, i32 0
  %35 = load %struct.message*, %struct.message** %34, align 4, !tbaa !3
  %36 = icmp eq %struct.message* %35, null
  br i1 %36, label %37, label %17

37:                                               ; preds = %32, %9
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.7, i32 0, i32 0)) #7
  br label %38

38:                                               ; preds = %37, %2, %21, %8
  ret void
}

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @deallocate_cgc(i8*, i32) local_unnamed_addr #2

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc void @read_message(%struct.message_manager* readonly %0, i32 %1) local_unnamed_addr #0 {
  %3 = alloca [256 x i8], align 1
  %4 = getelementptr inbounds [256 x i8], [256 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 256, i8* nonnull %4) #5
  %5 = icmp eq %struct.message_manager* %0, null
  br i1 %5, label %36, label %6

6:                                                ; preds = %2
  %7 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %0, i32 0, i32 1
  %8 = load i32, i32* %7, align 4, !tbaa !17
  %9 = icmp ult i32 %8, %1
  br i1 %9, label %10, label %11

10:                                               ; preds = %6
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.6, i32 0, i32 0)) #7
  br label %36

11:                                               ; preds = %6
  call arm_aapcs_vfpcc void @bzero_cgc(i8* nonnull %4, i32 256) #7
  %12 = call arm_aapcs_vfpcc i8* @strncat_cgc(i8* nonnull %4, i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.8, i32 0, i32 0), i32 256) #7
  %13 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %0, i32 0, i32 0
  %14 = load %struct.message*, %struct.message** %13, align 4, !tbaa !3
  %15 = icmp eq %struct.message* %14, null
  br i1 %15, label %35, label %16

16:                                               ; preds = %11, %31
  %17 = phi %struct.message* [ %33, %31 ], [ %14, %11 ]
  %18 = getelementptr inbounds %struct.message, %struct.message* %17, i32 0, i32 1
  %19 = load i32, i32* %18, align 4, !tbaa !20
  %20 = icmp eq i32 %19, %1
  br i1 %20, label %21, label %31

21:                                               ; preds = %16
  %22 = call arm_aapcs_vfpcc i32 @strlen_cgc(i8* nonnull %4) #7
  %23 = getelementptr inbounds [256 x i8], [256 x i8]* %3, i32 0, i32 %22
  %24 = sub i32 256, %22
  %25 = call arm_aapcs_vfpcc i32 @itoa(i8* nonnull %23, i32 %1, i32 %24) #7
  %26 = call arm_aapcs_vfpcc i8* @strncat_cgc(i8* nonnull %4, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.9, i32 0, i32 0), i32 256) #7
  %27 = getelementptr inbounds %struct.message, %struct.message* %17, i32 0, i32 3, i32 0
  %28 = call arm_aapcs_vfpcc i8* @strncat_cgc(i8* nonnull %4, i8* nonnull %27, i32 256) #7
  %29 = call arm_aapcs_vfpcc i8* @strncat_cgc(i8* nonnull %4, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.10, i32 0, i32 0), i32 256) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* nonnull %4) #7
  %30 = getelementptr inbounds %struct.message, %struct.message* %17, i32 0, i32 2
  store i32 1, i32* %30, align 4, !tbaa !21
  br label %36

31:                                               ; preds = %16
  %32 = getelementptr inbounds %struct.message, %struct.message* %17, i32 0, i32 0
  %33 = load %struct.message*, %struct.message** %32, align 4, !tbaa !3
  %34 = icmp eq %struct.message* %33, null
  br i1 %34, label %35, label %16

35:                                               ; preds = %31, %11
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.7, i32 0, i32 0)) #7
  br label %36

36:                                               ; preds = %35, %2, %21, %10
  call void @llvm.lifetime.end.p0i8(i64 256, i8* nonnull %4) #5
  ret void
}

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @itoa(i8*, i32, i32) local_unnamed_addr #2

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc void @list_unread_messages(%struct.message_manager* readonly %0) local_unnamed_addr #0 {
  %2 = icmp eq %struct.message_manager* %0, null
  br i1 %2, label %49, label %3

3:                                                ; preds = %1
  %4 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %0, i32 0, i32 0
  %5 = load %struct.message*, %struct.message** %4, align 4, !tbaa !3
  %6 = icmp eq %struct.message* %5, null
  br i1 %6, label %49, label %7

7:                                                ; preds = %3, %7
  %8 = phi %struct.message* [ %16, %7 ], [ %5, %3 ]
  %9 = phi i8 [ %14, %7 ], [ 0, %3 ]
  %10 = getelementptr inbounds %struct.message, %struct.message* %8, i32 0, i32 2
  %11 = load i32, i32* %10, align 4, !tbaa !21
  %12 = icmp eq i32 %11, 0
  %13 = zext i1 %12 to i8
  %14 = add i8 %9, %13
  %15 = getelementptr inbounds %struct.message, %struct.message* %8, i32 0, i32 0
  %16 = load %struct.message*, %struct.message** %15, align 4, !tbaa !3
  %17 = icmp eq %struct.message* %16, null
  br i1 %17, label %18, label %7

18:                                               ; preds = %7
  %19 = icmp eq i8 %14, 0
  br i1 %19, label %49, label %20

20:                                               ; preds = %18
  %21 = zext i8 %14 to i32
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.11, i32 0, i32 0)) #7
  %22 = mul nuw nsw i32 %21, 112
  %23 = tail call i8* @llvm.stacksave()
  %24 = alloca i8, i32 %22, align 1
  call arm_aapcs_vfpcc void @bzero_cgc(i8* nonnull %24, i32 %22) #7
  %25 = load %struct.message*, %struct.message** %4, align 4, !tbaa !3
  %26 = icmp eq %struct.message* %25, null
  br i1 %26, label %48, label %27

27:                                               ; preds = %20, %44
  %28 = phi %struct.message* [ %46, %44 ], [ %25, %20 ]
  %29 = getelementptr inbounds %struct.message, %struct.message* %28, i32 0, i32 2
  %30 = load i32, i32* %29, align 4, !tbaa !21
  %31 = icmp eq i32 %30, 0
  br i1 %31, label %32, label %44

32:                                               ; preds = %27
  store i32 1, i32* %29, align 4, !tbaa !21
  %33 = call arm_aapcs_vfpcc i32 @strcat_cgc(i8* nonnull %24, i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.8, i32 0, i32 0)) #7
  %34 = call arm_aapcs_vfpcc i32 @strlen_cgc(i8* nonnull %24) #7
  %35 = getelementptr inbounds i8, i8* %24, i32 %34
  %36 = getelementptr inbounds %struct.message, %struct.message* %28, i32 0, i32 1
  %37 = load i32, i32* %36, align 4, !tbaa !20
  %38 = call arm_aapcs_vfpcc i32 @itoa(i8* nonnull %35, i32 %37, i32 4) #7
  %39 = call arm_aapcs_vfpcc i32 @strcat_cgc(i8* nonnull %24, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.9, i32 0, i32 0)) #7
  %40 = getelementptr inbounds %struct.message, %struct.message* %28, i32 0, i32 3, i32 0
  %41 = call arm_aapcs_vfpcc i32 @strcat_cgc(i8* nonnull %24, i8* nonnull %40) #7
  %42 = call arm_aapcs_vfpcc i32 @strcat_cgc(i8* nonnull %24, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.12, i32 0, i32 0)) #7
  %43 = call arm_aapcs_vfpcc i32 @strcat_cgc(i8* nonnull %24, i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.8, i32 0, i32 0)) #7
  br label %44

44:                                               ; preds = %32, %27
  %45 = getelementptr inbounds %struct.message, %struct.message* %28, i32 0, i32 0
  %46 = load %struct.message*, %struct.message** %45, align 4, !tbaa !3
  %47 = icmp eq %struct.message* %46, null
  br i1 %47, label %48, label %27

48:                                               ; preds = %44, %20
  call arm_aapcs_vfpcc void @puts_cgc(i8* nonnull %24) #7
  call void @llvm.stackrestore(i8* %23)
  br label %49

49:                                               ; preds = %3, %18, %1, %48
  ret void
}

; Function Attrs: nounwind
declare i8* @llvm.stacksave() #5

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @strcat_cgc(i8*, i8*) local_unnamed_addr #2

; Function Attrs: nounwind
declare void @llvm.stackrestore(i8*) #5

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc void @list_messages(%struct.message_manager* readonly %0) local_unnamed_addr #0 {
  %2 = icmp eq %struct.message_manager* %0, null
  br i1 %2, label %14, label %3

3:                                                ; preds = %1
  %4 = getelementptr inbounds %struct.message_manager, %struct.message_manager* %0, i32 0, i32 0
  %5 = load %struct.message*, %struct.message** %4, align 4, !tbaa !3
  %6 = icmp eq %struct.message* %5, null
  br i1 %6, label %14, label %7

7:                                                ; preds = %3, %7
  %8 = phi %struct.message* [ %12, %7 ], [ %5, %3 ]
  %9 = getelementptr inbounds %struct.message, %struct.message* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 4, !tbaa !20
  tail call arm_aapcs_vfpcc void @read_message(%struct.message_manager* nonnull %0, i32 %10) #8
  %11 = getelementptr inbounds %struct.message, %struct.message* %8, i32 0, i32 0
  %12 = load %struct.message*, %struct.message** %11, align 4, !tbaa !3
  %13 = icmp eq %struct.message* %12, null
  br i1 %13, label %14, label %7

14:                                               ; preds = %7, %3, %1
  ret void
}

; Function Attrs: nounwind optsize readonly
define dso_local arm_aapcs_vfpcc %struct.user* @get_user(%struct.user_manager* readonly %0, i8* readonly %1) local_unnamed_addr #6 {
  %3 = icmp eq %struct.user_manager* %0, null
  %4 = icmp eq i8* %1, null
  %5 = or i1 %3, %4
  br i1 %5, label %19, label %6

6:                                                ; preds = %2
  %7 = getelementptr inbounds %struct.user_manager, %struct.user_manager* %0, i32 0, i32 0
  %8 = load %struct.user*, %struct.user** %7, align 4, !tbaa !3
  %9 = icmp eq %struct.user* %8, null
  br i1 %9, label %19, label %10

10:                                               ; preds = %6, %15
  %11 = phi %struct.user* [ %17, %15 ], [ %8, %6 ]
  %12 = getelementptr inbounds %struct.user, %struct.user* %11, i32 0, i32 0, i32 0
  %13 = tail call arm_aapcs_vfpcc i32 @strcmp(i8* nonnull %12, i8* nonnull dereferenceable(1) %1) #8
  %14 = icmp eq i32 %13, 0
  br i1 %14, label %19, label %15

15:                                               ; preds = %10
  %16 = getelementptr inbounds %struct.user, %struct.user* %11, i32 0, i32 2
  %17 = load %struct.user*, %struct.user** %16, align 4, !tbaa !3
  %18 = icmp eq %struct.user* %17, null
  br i1 %18, label %19, label %10

19:                                               ; preds = %10, %15, %6, %2
  %20 = phi %struct.user* [ null, %2 ], [ null, %6 ], [ %11, %10 ], [ null, %15 ]
  ret %struct.user* %20
}

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc i32 @send_user_message(%struct.user_manager* readonly %0, i8* readonly %1, i8* %2) local_unnamed_addr #0 {
  %4 = icmp eq %struct.user_manager* %0, null
  %5 = icmp eq i8* %1, null
  %6 = or i1 %4, %5
  %7 = icmp eq i8* %2, null
  %8 = or i1 %6, %7
  br i1 %8, label %23, label %9

9:                                                ; preds = %3
  %10 = tail call arm_aapcs_vfpcc %struct.user* @get_user(%struct.user_manager* nonnull %0, i8* nonnull %1) #8
  %11 = icmp eq %struct.user* %10, null
  br i1 %11, label %12, label %13

12:                                               ; preds = %9
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.13, i32 0, i32 0)) #7
  br label %23

13:                                               ; preds = %9
  %14 = tail call arm_aapcs_vfpcc %struct.message* @create_message(i8* nonnull %2) #8
  %15 = icmp eq %struct.message* %14, null
  br i1 %15, label %16, label %17

16:                                               ; preds = %13
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str.14, i32 0, i32 0)) #7
  br label %23

17:                                               ; preds = %13
  %18 = getelementptr inbounds %struct.user, %struct.user* %10, i32 0, i32 1
  %19 = load %struct.message_manager*, %struct.message_manager** %18, align 4, !tbaa !14
  %20 = tail call arm_aapcs_vfpcc i32 @add_message(%struct.message_manager* %19, %struct.message* nonnull %14) #8
  %21 = icmp eq i32 %20, 0
  br i1 %21, label %22, label %23

22:                                               ; preds = %17
  tail call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.15, i32 0, i32 0)) #7
  br label %23

23:                                               ; preds = %17, %3, %22, %16, %12
  %24 = phi i32 [ 0, %3 ], [ 0, %12 ], [ 0, %16 ], [ 0, %22 ], [ %20, %17 ]
  ret i32 %24
}

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc void @handle_loggedin(%struct.user_manager* readonly %0, %struct.user* readonly %1) local_unnamed_addr #0 {
  %3 = alloca i32, align 4
  %4 = alloca [32 x i8], align 1
  %5 = alloca [16 x i8], align 1
  %6 = bitcast i32* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %6) #5
  store i32 0, i32* %3, align 4, !tbaa !22
  %7 = getelementptr inbounds [32 x i8], [32 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 32, i8* nonnull %7) #5
  %8 = getelementptr inbounds [16 x i8], [16 x i8]* %5, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 16, i8* nonnull %8) #5
  %9 = icmp eq %struct.user_manager* %0, null
  %10 = icmp eq %struct.user* %1, null
  %11 = or i1 %9, %10
  br i1 %11, label %44, label %12

12:                                               ; preds = %2
  %13 = getelementptr inbounds %struct.user, %struct.user* %1, i32 0, i32 1
  br label %14

14:                                               ; preds = %29, %12
  %15 = load %struct.message_manager*, %struct.message_manager** %13, align 4, !tbaa !14
  call arm_aapcs_vfpcc void @list_unread_messages(%struct.message_manager* %15) #8
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.16, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.17, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.18, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.19, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.20, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.21, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.22, i32 0, i32 0)) #7
  store i32 0, i32* %3, align 4, !tbaa !22
  %16 = call arm_aapcs_vfpcc i32 @receive_cgc_until(i8* nonnull %6, i8 zeroext 10, i32 2) #7
  %17 = icmp eq i32 %16, 0
  br i1 %17, label %40, label %18

18:                                               ; preds = %14
  %19 = load i32, i32* %3, align 4, !tbaa !22
  %20 = add i32 %19, -48
  store i32 %20, i32* %3, align 4, !tbaa !22
  %21 = add i32 %19, -49
  %22 = icmp ugt i32 %21, 5
  br i1 %22, label %23, label %24

23:                                               ; preds = %18
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.24, i32 0, i32 0)) #7
  br label %29

24:                                               ; preds = %18
  switch i32 %19, label %29 [
    i32 54, label %40
    i32 53, label %42
    i32 52, label %25
    i32 51, label %30
    i32 50, label %32
    i32 49, label %36
  ]

25:                                               ; preds = %24
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.27, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @bzero_cgc(i8* nonnull %7, i32 32) #7
  %26 = call arm_aapcs_vfpcc i32 @receive_cgc_until(i8* nonnull %7, i8 zeroext 10, i32 4) #7
  %27 = call arm_aapcs_vfpcc i32 @atoi(i8* nonnull %7) #8
  store i32 %27, i32* %3, align 4, !tbaa !22
  %28 = load %struct.message_manager*, %struct.message_manager** %13, align 4, !tbaa !14
  call arm_aapcs_vfpcc void @delete_message(%struct.message_manager* %28, i32 %27) #8
  br label %29

29:                                               ; preds = %25, %32, %36, %30, %24, %23
  br label %14

30:                                               ; preds = %24
  %31 = load %struct.message_manager*, %struct.message_manager** %13, align 4, !tbaa !14
  call arm_aapcs_vfpcc void @list_messages(%struct.message_manager* %31) #8
  br label %29

32:                                               ; preds = %24
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.27, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @bzero_cgc(i8* nonnull %7, i32 32) #7
  %33 = call arm_aapcs_vfpcc i32 @receive_cgc_until(i8* nonnull %7, i8 zeroext 10, i32 4) #7
  %34 = call arm_aapcs_vfpcc i32 @atoi(i8* nonnull %7) #8
  store i32 %34, i32* %3, align 4, !tbaa !22
  %35 = load %struct.message_manager*, %struct.message_manager** %13, align 4, !tbaa !14
  call arm_aapcs_vfpcc void @read_message(%struct.message_manager* %35, i32 %34) #8
  br label %29

36:                                               ; preds = %24
  call arm_aapcs_vfpcc void @bzero_cgc(i8* nonnull %7, i32 32) #7
  call arm_aapcs_vfpcc void @bzero_cgc(i8* nonnull %8, i32 16) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.28, i32 0, i32 0)) #7
  %37 = call arm_aapcs_vfpcc i32 @receive_cgc_until(i8* nonnull %8, i8 zeroext 10, i32 15) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.29, i32 0, i32 0)) #7
  %38 = call arm_aapcs_vfpcc i32 @receive_cgc_until(i8* nonnull %7, i8 zeroext 10, i32 31) #7
  %39 = call arm_aapcs_vfpcc i32 @send_user_message(%struct.user_manager* %0, i8* nonnull %8, i8* nonnull %7) #8
  br label %29

40:                                               ; preds = %14, %24
  %41 = phi i8* [ getelementptr inbounds ([12 x i8], [12 x i8]* @.str.25, i32 0, i32 0), %24 ], [ getelementptr inbounds ([24 x i8], [24 x i8]* @.str.23, i32 0, i32 0), %14 ]
  br label %42

42:                                               ; preds = %24, %40
  %43 = phi i8* [ %41, %40 ], [ getelementptr inbounds ([16 x i8], [16 x i8]* @.str.26, i32 0, i32 0), %24 ]
  call arm_aapcs_vfpcc void @puts_cgc(i8* %43) #7
  br label %44

44:                                               ; preds = %42, %2
  call void @llvm.lifetime.end.p0i8(i64 16, i8* nonnull %8) #5
  call void @llvm.lifetime.end.p0i8(i64 32, i8* nonnull %7) #5
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %6) #5
  ret void
}

; Function Attrs: optsize
declare dso_local arm_aapcs_vfpcc i32 @receive_cgc_until(i8*, i8 zeroext, i32) local_unnamed_addr #2

; Function Attrs: nofree nounwind optsize readonly
declare dso_local arm_aapcs_vfpcc i32 @atoi(i8* nocapture) local_unnamed_addr #3

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc void @handle_menu() local_unnamed_addr #0 {
  %1 = alloca i32, align 4
  %2 = alloca [16 x i8], align 1
  %3 = bitcast i32* %1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %3) #5
  store i32 0, i32* %1, align 4, !tbaa !22
  %4 = getelementptr inbounds [16 x i8], [16 x i8]* %2, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 16, i8* nonnull %4) #5
  %5 = tail call arm_aapcs_vfpcc %struct.user_manager* @init_users() #8
  %6 = icmp eq %struct.user_manager* %5, null
  br i1 %6, label %43, label %7

7:                                                ; preds = %0
  store i32 0, i32* %1, align 4, !tbaa !22
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.30, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.31, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.32, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.22, i32 0, i32 0)) #7
  %8 = call arm_aapcs_vfpcc i32 @receive_cgc_until(i8* nonnull %3, i8 zeroext 10, i32 2) #7
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %41, label %10

10:                                               ; preds = %7
  %11 = getelementptr inbounds %struct.user_manager, %struct.user_manager* %5, i32 0, i32 0
  br label %12

12:                                               ; preds = %10, %18
  %13 = load i32, i32* %1, align 4, !tbaa !22
  %14 = add i32 %13, -48
  store i32 %14, i32* %1, align 4, !tbaa !22
  %15 = add i32 %13, -49
  %16 = icmp ugt i32 %15, 2
  br i1 %16, label %17, label %21

17:                                               ; preds = %12
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.24, i32 0, i32 0)) #7
  br label %18

18:                                               ; preds = %40, %39, %23, %21, %17
  store i32 0, i32* %1, align 4, !tbaa !22
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.30, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.31, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.32, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.22, i32 0, i32 0)) #7
  %19 = call arm_aapcs_vfpcc i32 @receive_cgc_until(i8* nonnull %3, i8 zeroext 10, i32 2) #7
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %41, label %12

21:                                               ; preds = %12
  call arm_aapcs_vfpcc void @bzero_cgc(i8* nonnull %4, i32 16) #7
  %22 = load i32, i32* %1, align 4, !tbaa !22
  switch i32 %22, label %18 [
    i32 3, label %41
    i32 1, label %23
    i32 2, label %26
  ]

23:                                               ; preds = %21
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.34, i32 0, i32 0)) #7
  %24 = call arm_aapcs_vfpcc i32 @receive_cgc_until(i8* nonnull %4, i8 zeroext 10, i32 15) #7
  %25 = call arm_aapcs_vfpcc i32 @create_user(%struct.user_manager* nonnull %5, i8* nonnull %4) #8
  br label %18

26:                                               ; preds = %21
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.34, i32 0, i32 0)) #7
  %27 = call arm_aapcs_vfpcc i32 @receive_cgc_until(i8* nonnull %4, i8 zeroext 10, i32 15) #7
  %28 = load %struct.user*, %struct.user** %11, align 4, !tbaa !3
  %29 = icmp eq %struct.user* %28, null
  br i1 %29, label %40, label %30

30:                                               ; preds = %26, %35
  %31 = phi %struct.user* [ %37, %35 ], [ %28, %26 ]
  %32 = getelementptr inbounds %struct.user, %struct.user* %31, i32 0, i32 0, i32 0
  %33 = call arm_aapcs_vfpcc i32 @strcmp(i8* nonnull %32, i8* nonnull dereferenceable(1) %4) #7
  %34 = icmp eq i32 %33, 0
  br i1 %34, label %39, label %35

35:                                               ; preds = %30
  %36 = getelementptr inbounds %struct.user, %struct.user* %31, i32 0, i32 2
  %37 = load %struct.user*, %struct.user** %36, align 4, !tbaa !3
  %38 = icmp eq %struct.user* %37, null
  br i1 %38, label %40, label %30

39:                                               ; preds = %30
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.35, i32 0, i32 0)) #7
  call arm_aapcs_vfpcc void @handle_loggedin(%struct.user_manager* nonnull %5, %struct.user* nonnull %31) #8
  br label %18

40:                                               ; preds = %35, %26
  call arm_aapcs_vfpcc void @puts_cgc(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.36, i32 0, i32 0)) #7
  br label %18

41:                                               ; preds = %21, %18, %7
  %42 = phi i8* [ getelementptr inbounds ([24 x i8], [24 x i8]* @.str.33, i32 0, i32 0), %7 ], [ getelementptr inbounds ([12 x i8], [12 x i8]* @.str.25, i32 0, i32 0), %21 ], [ getelementptr inbounds ([24 x i8], [24 x i8]* @.str.33, i32 0, i32 0), %18 ]
  call arm_aapcs_vfpcc void @puts_cgc(i8* %42) #7
  br label %43

43:                                               ; preds = %41, %0
  call void @llvm.lifetime.end.p0i8(i64 16, i8* nonnull %4) #5
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %3) #5
  ret void
}

; Function Attrs: nounwind optsize
define dso_local arm_aapcs_vfpcc i32 @test() local_unnamed_addr #0 {
  tail call arm_aapcs_vfpcc void @handle_menu() #8
  ret i32 0
}

attributes #0 = { nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }
attributes #2 = { optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nofree nounwind optsize readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nofree norecurse nounwind optsize "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind }
attributes #6 = { nounwind optsize readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-m7" "target-features"="+armv7e-m,+dsp,+fp-armv8d16,+fp-armv8d16sp,+fp16,+fp64,+hwdiv,+strict-align,+thumb-mode,+vfp2,+vfp2sp,+vfp3d16,+vfp3d16sp,+vfp4d16,+vfp4d16sp,-aes,-crc,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8sp,-fp16fml,-fullfp16,-hwdiv-arm,-lob,-mve,-mve.fp,-neon,-ras,-sb,-sha2,-vfp3,-vfp3sp,-vfp4,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { nounwind optsize }
attributes #8 = { optsize }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 10.0.0-4ubuntu1 "}
!3 = !{!4, !4, i64 0}
!4 = !{!"any pointer", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{!8, !4, i64 0}
!8 = !{!"user_manager", !4, i64 0, !9, i64 4}
!9 = !{!"int", !5, i64 0}
!10 = !{!8, !9, i64 4}
!11 = !{!12, !4, i64 20}
!12 = !{!"user", !5, i64 0, !4, i64 16, !4, i64 20}
!13 = !{!5, !5, i64 0}
!14 = !{!12, !4, i64 16}
!15 = !{!16, !4, i64 0}
!16 = !{!"message_manager", !4, i64 0, !9, i64 4}
!17 = !{!16, !9, i64 4}
!18 = !{!19, !4, i64 0}
!19 = !{!"message", !4, i64 0, !9, i64 4, !9, i64 8, !5, i64 12}
!20 = !{!19, !9, i64 4}
!21 = !{!19, !9, i64 8}
!22 = !{!23, !23, i64 0}
!23 = !{!"long", !5, i64 0}
