; ModuleID = 'caris_peterson.bc'
source_filename = "peterson.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@flag = common global [2 x i32] zeroinitializer, align 4
@turn = common global i32 0, align 4
@test = common global i32 0, align 4
@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define i8* @P0(i8* %t) #0 {
entry:
  %retval = alloca i8*, align 8
  %t.addr = alloca i8*, align 8
  store i8* %t, i8** %t.addr, align 8
  store i32 1, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @flag, i64 0, i64 0), align 4
  store i32 1, i32* @turn, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @flag, i64 0, i64 1), align 4
  %cmp = icmp eq i32 %0, 1
  br i1 %cmp, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %while.cond
  %1 = load i32, i32* @turn, align 4
  %cmp1 = icmp eq i32 %1, 1
  br label %land.end

land.end:                                         ; preds = %land.rhs, %while.cond
  %2 = phi i1 [ false, %while.cond ], [ %cmp1, %land.rhs ]
  br i1 %2, label %while.body, label %while.end

while.body:                                       ; preds = %land.end
  br label %while.cond

while.end:                                        ; preds = %land.end
  %3 = load i32, i32* @test, align 4
  %inc = add nsw i32 %3, 1
  store i32 %inc, i32* @test, align 4
  %4 = load i32, i32* @test, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %4)
  %5 = load i32, i32* @test, align 4
  %dec = add nsw i32 %5, -1
  store i32 %dec, i32* @test, align 4
  store i32 0, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @flag, i64 0, i64 0), align 4
  %6 = load i8*, i8** %retval, align 8
  ret i8* %6
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define i8* @P1(i8* %t) #0 {
entry:
  %retval = alloca i8*, align 8
  %t.addr = alloca i8*, align 8
  store i8* %t, i8** %t.addr, align 8
  store i32 1, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @flag, i64 0, i64 1), align 4
  store i32 0, i32* @turn, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @flag, i64 0, i64 0), align 4
  %cmp = icmp eq i32 %0, 1
  br i1 %cmp, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %while.cond
  %1 = load i32, i32* @turn, align 4
  %cmp1 = icmp eq i32 %1, 0
  br label %land.end

land.end:                                         ; preds = %land.rhs, %while.cond
  %2 = phi i1 [ false, %while.cond ], [ %cmp1, %land.rhs ]
  br i1 %2, label %while.body, label %while.end

while.body:                                       ; preds = %land.end
  br label %while.cond

while.end:                                        ; preds = %land.end
  %3 = load i32, i32* @test, align 4
  %inc = add nsw i32 %3, 1
  store i32 %inc, i32* @test, align 4
  %4 = load i32, i32* @test, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %4)
  %5 = load i32, i32* @test, align 4
  %dec = add nsw i32 %5, -1
  store i32 %dec, i32* @test, align 4
  store i32 0, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @flag, i64 0, i64 1), align 4
  %6 = load i8*, i8** %retval, align 8
  ret i8* %6
}

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %p0 = alloca i64, align 8
  %p1 = alloca i64, align 8
  %t = alloca i16, align 2
  store i32 0, i32* %retval, align 4
  store i16 0, i16* %t, align 2
  %0 = load i16, i16* %t, align 2
  %conv = sext i16 %0 to i64
  %1 = inttoptr i64 %conv to i8*
  %call = call i32 @pthread_create(i64* %p0, %union.pthread_attr_t* null, i8* (i8*)* @P0, i8* %1) #3
  %2 = load i16, i16* %t, align 2
  %conv1 = sext i16 %2 to i64
  %3 = inttoptr i64 %conv1 to i8*
  %call2 = call i32 @pthread_create(i64* %p1, %union.pthread_attr_t* null, i8* (i8*)* @P1, i8* %3) #3
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @pthread_create(i64*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #2

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.0 (https://github.com/llvm-mirror/clang.git afd3b93fc835d07303f8880c091dbd5306f5fb9c) (https://github.com/llvm-mirror/llvm.git 59d9d429cca3ba1cbdc9a5bb3e84008f06930810)"}
