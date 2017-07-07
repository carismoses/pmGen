; ModuleID = 'caris_zune.bc'
source_filename = "zune.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@.str = private unnamed_addr constant [16 x i8] c"year:%d day:%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define i8* @year(i8* %day) #0 {
entry:
  %retval = alloca i8*, align 8
  %day.addr = alloca i8*, align 8
  %days = alloca i32, align 4
  %year = alloca i32, align 4
  store i8* %day, i8** %day.addr, align 8
  %0 = load i8*, i8** %day.addr, align 8
  %1 = bitcast i8* %0 to i32*
  %2 = load i32, i32* %1, align 4
  store i32 %2, i32* %days, align 4
  store i32 1980, i32* %year, align 4
  br label %while.cond

while.cond:                                       ; preds = %if.end10, %entry
  %3 = load i32, i32* %days, align 4
  %cmp = icmp sgt i32 %3, 365
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %4 = load i32, i32* %year, align 4
  %rem = srem i32 %4, 4
  %cmp1 = icmp eq i32 %rem, 0
  br i1 %cmp1, label %land.lhs.true, label %lor.lhs.false

land.lhs.true:                                    ; preds = %while.body
  %5 = load i32, i32* %year, align 4
  %rem2 = srem i32 %5, 100
  %cmp3 = icmp ne i32 %rem2, 0
  br i1 %cmp3, label %if.then, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %land.lhs.true, %while.body
  %6 = load i32, i32* %year, align 4
  %rem4 = srem i32 %6, 400
  %cmp5 = icmp eq i32 %rem4, 0
  br i1 %cmp5, label %if.then, label %if.else

if.then:                                          ; preds = %lor.lhs.false, %land.lhs.true
  %7 = load i32, i32* %days, align 4
  %cmp6 = icmp sgt i32 %7, 366
  br i1 %cmp6, label %if.then7, label %if.end

if.then7:                                         ; preds = %if.then
  %8 = load i32, i32* %days, align 4
  %sub = sub nsw i32 %8, 366
  store i32 %sub, i32* %days, align 4
  %9 = load i32, i32* %year, align 4
  %add = add nsw i32 %9, 1
  store i32 %add, i32* %year, align 4
  br label %if.end

if.end:                                           ; preds = %if.then7, %if.then
  br label %if.end10

if.else:                                          ; preds = %lor.lhs.false
  %10 = load i32, i32* %days, align 4
  %sub8 = sub nsw i32 %10, 365
  store i32 %sub8, i32* %days, align 4
  %11 = load i32, i32* %year, align 4
  %add9 = add nsw i32 %11, 1
  store i32 %add9, i32* %year, align 4
  br label %if.end10

if.end10:                                         ; preds = %if.else, %if.end
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %12 = load i32, i32* %year, align 4
  %13 = load i32, i32* %days, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str, i32 0, i32 0), i32 %12, i32 %13)
  %14 = load i8*, i8** %retval, align 8
  ret i8* %14
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %y1 = alloca i64, align 8
  %y2 = alloca i64, align 8
  %y3 = alloca i64, align 8
  %i = alloca i16, align 2
  store i32 0, i32* %retval, align 4
  store i16 365, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i16, i16* %i, align 2
  %conv = sext i16 %0 to i32
  %cmp = icmp slt i32 %conv, 368
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i16, i16* %i, align 2
  %conv2 = sext i16 %1 to i64
  %2 = inttoptr i64 %conv2 to i8*
  %call = call i32 @pthread_create(i64* %y1, %union.pthread_attr_t* null, i8* (i8*)* @year, i8* %2) #3
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %3 = load i16, i16* %i, align 2
  %inc = add i16 %3, 1
  store i16 %inc, i16* %i, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
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
