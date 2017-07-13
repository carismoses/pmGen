TYPEDEF union.pthread_attr_t {int u0; int[48] u1;}
int
int (int)
int (int)
int (int, ...)
int ()
int[4]
int
int[2]
int (int, union.pthread_attr_t, P0, int)

int _flag[2];
int _turn;
int _test;
init {
chan _syn = [0] of { int };
  _turn = 0;
  _test = 0;
  run _main(_syn);
}

proctype _P0(int t; chan __return) {
int retval;
int t_addr;
int v0;
bit cmp;
int v1;
bit cmp1;
bit v2;
int v3;
int inc;
int v4;
int call;
int v5;
int dec;
int v6;
int currentLabel;
chan _syn = [0] of { int };

Label0:
  currentLabel = 0;
  t_addr = t;
   _flag[0] = 1;
  _turn = 1;
  goto Label1;

Label1:
  currentLabel = 1;
  v0 =  _flag[1];
  cmp = (v0 == 1);
  if
    ::(cmp!= 0) -> goto Label2
    ::(cmp==0) -> goto Label3
  fi;

Label2:
  currentLabel = 2;
  v1 = _turn;
  cmp1 = (v1 == 1);
  goto Label3;

Label3:
  if
    ::(currentLabel == 1)->v2 = false
    ::(currentLabel == 2)->v2 = cmp1
  fi;
  currentLabel = 3;
  if
    ::(v2!= 0) -> goto Label4
    ::(v2==0) -> goto Label5
  fi;

Label4:
  currentLabel = 4;
  goto Label1;

Label5:
  currentLabel = 5;
  v3 = _test;
  inc = v3 + 1;
  _test = inc;
  v4 = _test;
   printf( "%d\0A\00" , v4);
  v5 = _test;
  dec = v5 + -1;
  _test = dec;
   _flag[0] = 0;
  v6 = retval;
  __return!v6;
  goto LabelSkip;
LabelSkip:skip
}

proctype _P1(int t; chan __return) {
int retval;
int t_addr;
int v0;
bit cmp;
int v1;
bit cmp1;
bit v2;
int v3;
int inc;
int v4;
int call;
int v5;
int dec;
int v6;
int currentLabel;
chan _syn = [0] of { int };

Label0:
  currentLabel = 0;
  t_addr = t;
   _flag[1] = 1;
  _turn = 0;
  goto Label1;

Label1:
  currentLabel = 1;
  v0 =  _flag[0];
  cmp = (v0 == 1);
  if
    ::(cmp!= 0) -> goto Label2
    ::(cmp==0) -> goto Label3
  fi;

Label2:
  currentLabel = 2;
  v1 = _turn;
  cmp1 = (v1 == 0);
  goto Label3;

Label3:
  if
    ::(currentLabel == 1)->v2 = false
    ::(currentLabel == 2)->v2 = cmp1
  fi;
  currentLabel = 3;
  if
    ::(v2!= 0) -> goto Label4
    ::(v2==0) -> goto Label5
  fi;

Label4:
  currentLabel = 4;
  goto Label1;

Label5:
  currentLabel = 5;
  v3 = _test;
  inc = v3 + 1;
  _test = inc;
  v4 = _test;
   printf( "%d\0A\00" , v4);
  v5 = _test;
  dec = v5 + -1;
  _test = dec;
   _flag[1] = 0;
  v6 = retval;
  __return!v6;
  goto LabelSkip;
LabelSkip:skip
}

proctype _main(chan __return) {
int retval;
int p0;
int p1;
int t;
int v0;
int conv;
int v1;
int call;
int v2;
int conv1;
int v3;
int call2;
int currentLabel;
chan _syn = [0] of { int };
chan _return0 = [0] of { int };
chan _return1 = [0] of { int };

Label0:
  currentLabel = 0;
  retval = 0;
  t = 0;
  v0 = t;
  conv =  v0;
  v1 =  conv;
  run _P0(v1, _return0);
  v2 = t;
  conv1 =  v2;
  v3 =  conv1;
  run _P1(v3, _return1);
  __return!0;
  goto LabelSkip;
LabelSkip:skip
}
