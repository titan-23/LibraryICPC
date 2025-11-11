int c=(1<<k)-1;
while (c < (1<<n)) {
  // size k hoge
  int x = c&-c, y = c+x;
  c = (((c&~y)/x)>>1) | y;
}
for (int a = b; ; a = (a-1)&b) {
  // a: subset of b(descending)
  if (!a) break;
}