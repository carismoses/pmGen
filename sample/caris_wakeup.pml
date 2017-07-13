TYPEDEF union.pthread_attr_t {int u0; int[48] u1;}
int ()
int
int (int)
int
int
int
int (int, union.pthread_attr_t, server, int)
int
int (int)

int _isRunning;
int _lk;
int _r_lock;
int _r_want;
int _sleep_q;
init {
chan _syn = [0] of { int };
  _isRunning = 1;
  _lk = 0;
  _r_lock = 0;
  _r_want = 0;
  _sleep_q = 0;
  run _main(_syn);
}

proctype _client(int n; chan __return) {
int retval;
int n_addr;
int v0;
bit tobool;
int conv;
bit cmp;
int v1;
bit tobool3;
int conv4;
bit cmp5;
int v2;
bit tobool9;
bit lnot;
int v3;
int currentLabel;
chan _syn = [0] of { int };

Label0:
  currentLabel = 0;
  n_addr = n;
  goto Label1;

Label1:
  currentLabel = 1;
  goto Label2;

Label2:
  currentLabel = 2;
  v0 = _lk;
  tobool =  v0;
  conv =  tobool;
  cmp = (conv != 0);
  if
    ::(cmp!= 0) -> goto Label3
    ::(cmp==0) -> goto Label4
  fi;

Label3:
  currentLabel = 3;
  goto Label2;

Label4:
  currentLabel = 4;
  _lk = 1;
  goto Label5;

Label5:
  currentLabel = 5;
  v1 = _r_lock;
  tobool3 =  v1;
  conv4 =  tobool3;
  cmp5 = (conv4 == 1);
  if
    ::(cmp5!= 0) -> goto Label6
    ::(cmp5==0) -> goto Label10
  fi;

Label6:
  currentLabel = 6;
  _r_want = 1;
  _isRunning = 0;
  _lk = 0;
  goto Label7;

Label7:
  currentLabel = 7;
  v2 = _isRunning;
  tobool9 =  v2;
  lnot = tobool9 ^ true;
  if
    ::(lnot!= 0) -> goto Label8
    ::(lnot==0) -> goto Label9
  fi;

Label8:
  currentLabel = 8;
  goto Label7;

Label9:
  currentLabel = 9;
  goto Label5;

Label10:
  currentLabel = 10;
  goto Label11;

Label11:
  currentLabel = 11;
  _r_lock = 1;
  _lk = 0;
  goto Label1;

Label12:
  currentLabel = 12;
  v3 = retval;
  __return!v3;
  goto LabelSkip;
LabelSkip:skip
}

proctype _server(int n; chan __return) {
int retval;
int n_addr;
int v0;
bit tobool;
int conv;
bit cmp;
int v1;
bit tobool2;
int v2;
bit tobool4;
int conv5;
bit cmp6;
int v3;
bit tobool11;
int conv12;
bit cmp13;
int v4;
bit tobool17;
int v5;
int currentLabel;
chan _syn = [0] of { int };

Label0:
  currentLabel = 0;
  n_addr = n;
  goto Label1;

Label1:
  currentLabel = 1;
  _r_lock = 0;
  goto Label2;

Label2:
  currentLabel = 2;
  v0 = _lk;
  tobool =  v0;
  conv =  tobool;
  cmp = (conv != 0);
  if
    ::(cmp!= 0) -> goto Label3
    ::(cmp==0) -> goto Label4
  fi;

Label3:
  currentLabel = 3;
  goto Label2;

Label4:
  currentLabel = 4;
  v1 = _r_want;
  tobool2 =  v1;
  if
    ::(tobool2!= 0) -> goto Label5
    ::(tobool2==0) -> goto Label14
  fi;

Label5:
  currentLabel = 5;
  goto Label6;

Label6:
  currentLabel = 6;
  v2 = _sleep_q;
  tobool4 =  v2;
  conv5 =  tobool4;
  cmp6 = (conv5 != 0);
  if
    ::(cmp6!= 0) -> goto Label7
    ::(cmp6==0) -> goto Label8
  fi;

Label7:
  currentLabel = 7;
  goto Label6;

Label8:
  currentLabel = 8;
  _sleep_q = 1;
  _r_want = 0;
  goto Label9;

Label9:
  currentLabel = 9;
  v3 = _lk;
  tobool11 =  v3;
  conv12 =  tobool11;
  cmp13 = (conv12 != 0);
  if
    ::(cmp13!= 0) -> goto Label10
    ::(cmp13==0) -> goto Label11
  fi;

Label10:
  currentLabel = 10;
  goto Label9;

Label11:
  currentLabel = 11;
  v4 = _isRunning;
  tobool17 =  v4;
  if
    ::(tobool17!= 0) -> goto Label13
    ::(tobool17==0) -> goto Label12
  fi;

Label12:
  currentLabel = 12;
  _isRunning = 1;
  goto Label13;

Label13:
  currentLabel = 13;
  _sleep_q = 0;
  goto Label14;

Label14:
  currentLabel = 14;
  goto Label1;

Label12:
  currentLabel = 12;
  v5 = retval;
  __return!v5;
  goto LabelSkip;
LabelSkip:skip
}

proctype _main(chan __return) {
int retval;
int p0;
int p1;
int t;
int v0;
int call;
int v1;
int call1;
int currentLabel;
chan _syn = [0] of { int };
chan _return0 = [0] of { int };
chan _return1 = [0] of { int };

Label0:
  currentLabel = 0;
  retval = 0;
  t = 0;
  v0 =  t;
  run _client(v0, _return0);
  v1 =  t;
  run _server(v1, _return1);
  __return!0;
  goto LabelSkip;
LabelSkip:skip
}
