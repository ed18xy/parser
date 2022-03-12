Implementation of a recursive descent-parser in C following an EBNF (EBNF.png).
Tokenizing statements specified in the input file front.in.
statements examples:
    a = a + b
    a = a + b * c
    a = b * (c - d); e = f / g
    if (a + b) c = d
    if (a) {b = c/d; e=f-g}
    if (a) {b=c}
    while(a*b) c = d
    while(a) {c = g + e; d = f*b}
    while(b) {a = f}
    for(a=b;c;a=b+d) f = g
    for(f=g;a*b;c=e-d) {a = b+c; d=g}