; ModuleID = 'caris_wakeup.bc'
source_filename = "wakeup.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@isRunning = global i8 1, align 1
@lk = common global i8 0, align 1
@r_lock = common global i8 0, align 1
@r_want = common global i8 0, align 1
@sleep_q = common global i8 0, align 1

; Function Attrs: noinline nounwind optnone uwtable
define i8* @client(i8* %n) #0 {
entry:
  %retval = alloca i8*, align 8
  %n.addr = alloca i8*, align 8
  store i8* %n, i8** %n.addr, align 8
  br label %sleep

sleep:                                            ; preds = %progress, %entry
  br label %while.cond

while.cond:                                       ; preds = %while.body, %sleep
  %0 = load i8, i8* @lk, align 1
  %tobool = trunc i8 %0 to i1
  %conv = zext i1 %tobool to i32
  %cmp = icmp ne i32 %conv, 0
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  store i8 1, i8* @lk, align 1
  br label %while.cond2

while.cond2:                                      ; preds = %while.end11, %while.end
  %1 = load i8, i8* @r_lock, align 1
  %tobool3 = trunc i8 %1 to i1
  %conv4 = zext i1 %tobool3 to i32
  %cmp5 = icmp eq i32 %conv4, 1
  br i1 %cmp5, label %while.body7, label %while.end12

while.body7:                                      ; preds = %while.cond2
  store i8 1, i8* @r_want, align 1
  store i8 0, i8* @isRunning, align 1
  store i8 0, i8* @lk, align 1
  br label %while.cond8

while.cond8:                                      ; preds = %while.body10, %while.body7
  %2 = load i8, i8* @isRunning, align 1
  %tobool9 = trunc i8 %2 to i1
  %lnot = xor i1 %tobool9, true
  br i1 %lnot, label %while.body10, label %while.end11

while.body10:                                     ; preds = %while.cond8
  br label %while.cond8

while.end11:                                      ; preds = %while.cond8
  br label %while.cond2

while.end12:                                      ; preds = %while.cond2
  br label %progress

progress:                                         ; preds = %while.end12
  store i8 1, i8* @r_lock, align 1
  store i8 0, i8* @lk, align 1
  br label %sleep

return:                                           ; No predecessors!
  %3 = load i8*, i8** %retval, align 8
  ret i8* %3
}

; Function Attrs: noinline nounwind optnone uwtable
define i8* @server(i8* %n) #0 {
entry:
  %retval = alloca i8*, align 8
  %n.addr = alloca i8*, align 8
  store i8* %n, i8** %n.addr, align 8
  br label %wakeup

wakeup:                                           ; preds = %if.end19, %entry
  store i8 0, i8* @r_lock, align 1
  br label %while.cond

while.cond:                                       ; preds = %while.body, %wakeup
  %0 = load i8, i8* @lk, align 1
  %tobool = trunc i8 %0 to i1
  %conv = zext i1 %tobool to i32
  %cmp = icmp ne i32 %conv, 0
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %1 = load i8, i8* @r_want, align 1
  %tobool2 = trunc i8 %1 to i1
  br i1 %tobool2, label %if.then, label %if.end19

if.then:                                          ; preds = %while.end
  br label %while.cond3

while.cond3:                                      ; preds = %while.body8, %if.then
  %2 = load i8, i8* @sleep_q, align 1
  %tobool4 = trunc i8 %2 to i1
  %conv5 = zext i1 %tobool4 to i32
  %cmp6 = icmp ne i32 %conv5, 0
  br i1 %cmp6, label %while.body8, label %while.end9

while.body8:                                      ; preds = %while.cond3
  br label %while.cond3

while.end9:                                       ; preds = %while.cond3
  store i8 1, i8* @sleep_q, align 1
  store i8 0, i8* @r_want, align 1
  br label %while.cond10

while.cond10:                                     ; preds = %while.body15, %while.end9
  %3 = load i8, i8* @lk, align 1
  %tobool11 = trunc i8 %3 to i1
  %conv12 = zext i1 %tobool11 to i32
  %cmp13 = icmp ne i32 %conv12, 0
  br i1 %cmp13, label %while.body15, label %while.end16

while.body15:                                     ; preds = %while.cond10
  br label %while.cond10

while.end16:                                      ; preds = %while.cond10
  %4 = load i8, i8* @isRunning, align 1
  %tobool17 = trunc i8 %4 to i1
  br i1 %tobool17, label %if.end, label %if.then18

if.then18:                                        ; preds = %while.end16
  store i8 1, i8* @isRunning, align 1
  br label %if.end

if.end:                                           ; preds = %if.then18, %while.end16
  store i8 0, i8* @sleep_q, align 1
  br label %if.end19

if.end19:                                         ; preds = %if.end, %while.end
  br label %wakeup

return:                                           ; No predecessors!
  %5 = load i8*, i8** %retval, align 8
  ret i8* %5
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
  %0 = bitcast i16* %t to i8*
  %call = call i32 @pthread_create(i64* %p0, %union.pthread_attr_t* null, i8* (i8*)* @client, i8* %0) #2
  %1 = bitcast i16* %t to i8*
  %call1 = call i32 @pthread_create(i64* %p1, %union.pthread_attr_t* null, i8* (i8*)* @server, i8* %1) #2
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @pthread_create(i64*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.0 (https://github.com/llvm-mirror/clang.git afd3b93fc835d07303f8880c091dbd5306f5fb9c) (https://github.com/llvm-mirror/llvm.git 59d9d429cca3ba1cbdc9a5bb3e84008f06930810)"}
