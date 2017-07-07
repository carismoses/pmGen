init {
chan _syn = [0] of { int };
  run _main(_syn);
}

proctype _year(int day; chan __return) {
int retval;
int day_addr;
int days;
int year;
int v0;
int v1;
int v2;
int v3;
bit cmp;
int v4;
int rem;
bit cmp1;
int v5;
int rem2;
bit cmp3;
int v6;
int rem4;
bit cmp5;
int v7;
bit cmp6;
int v8;
int sub;
int v9;
int add;
int v10;
int sub8;
int v11;
int add9;
int v12;
int v13;
int call;
int v14;
int currentLabel;
chan _syn = [0] of { int };

Label0:
  currentLabel = 0;
  day_addr = day;
  v0 = day_addr;
  v1 =  v0;
  v2 = v1;
  days = v2;
  year = 1980;
  goto Label1;

Label1:
  currentLabel = 1;
  v3 = days;
  cmp = (v3 > 365);
  if
    ::(cmp!= 0) -> goto Label2
    ::(cmp==0) -> goto Label10
  fi;

Label2:
  currentLabel = 2;
  v4 = year;
  rem = v4 % 4;
  cmp1 = (rem == 0);
  if
    ::(cmp1!= 0) -> goto Label3
    ::(cmp1==0) -> goto Label4
  fi;

Label3:
  currentLabel = 3;
  v5 = year;
  rem2 = v5 % 100;
  cmp3 = (rem2 != 0);
  if
    ::(cmp3!= 0) -> goto Label5
    ::(cmp3==0) -> goto Label4
  fi;

Label4:
  currentLabel = 4;
  v6 = year;
  rem4 = v6 % 400;
  cmp5 = (rem4 == 0);
  if
    ::(cmp5!= 0) -> goto Label5
    ::(cmp5==0) -> goto Label8
  fi;

Label5:
  currentLabel = 5;
  v7 = days;
  cmp6 = (v7 > 366);
  if
    ::(cmp6!= 0) -> goto Label6
    ::(cmp6==0) -> goto Label7
  fi;

Label6:
  currentLabel = 6;
  v8 = days;
  sub = v8 - 366;
  days = sub;
  v9 = year;
  add = v9 + 1;
  year = add;
  goto Label7;

Label7:
  currentLabel = 7;
  goto Label9;

Label8:
  currentLabel = 8;
  v10 = days;
  sub8 = v10 - 365;
  days = sub8;
  v11 = year;
  add9 = v11 + 1;
  year = add9;
  goto Label9;

Label9:
  currentLabel = 9;
  goto Label1;

Label10:
  currentLabel = 10;
  v12 = year;
  v13 = days;
   printf( "year:%d day:%d\0A\00" , v12, v13);
  v14 = retval;
  __return!v14;
  goto LabelSkip;
LabelSkip:skip
}

proctype _main(chan __return) {
int retval;
 y1;
 y2;
 y3;
int i;
int v0;
int conv;
bit cmp;
int v1;
 conv2;
int v2;
int call;
int v3;
int inc;
int currentLabel;
chan _syn = [0] of { int };
chan _return0 = [0] of { int };

Label0:
  currentLabel = 0;
  retval = 0;
  i = 365;
  goto Label1;

Label1:
  currentLabel = 1;
  v0 = i;
  conv =  v0;
  cmp = (conv < 368);
  if
    ::(cmp!= 0) -> goto Label2
    ::(cmp==0) -> goto Label4
  fi;

Label2:
  currentLabel = 2;
  v1 = i;
  conv2 =  v1;
  v2 =  conv2;
  run _year(v2, _return0);
  goto Label3;

Label3:
  currentLabel = 3;
  v3 = i;
  inc = v3 + 1;
  i = inc;
  goto Label1;

Label4:
  currentLabel = 4;
  __return!0;
  goto LabelSkip;
LabelSkip:skip
}
