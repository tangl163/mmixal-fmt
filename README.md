## mmixal-fmt is a tool for formatting MMIXAL source code.

### Installation

```bash
make
make install
```

By default, the `mmixal-fmt` will be installed in `/usr/local/bin/`

### Usage

`mmixal-fmt` will read **MMIXAL** ([**MMIX**](https://mmix.cs.hm.edu/index.html) Assemble Language)
from `stdin` and output to the `stdout`. In fact, this tool is intended to integrate with this [vim plugin](https://github.com/tangl163/vim-mmixal-fmt).


Assuming below is what `mmix.mms` contains

```mmix
 % Example program to format
L IS 500        This is comment
t  IS $255   This is another comment
n GREG      % Comment
r GREG  // Comment
jj GREG 0        Comment
   PBN mm,2B
 LDA t,NewLn; TRAP 0,Fputs,StdOut
 CMP t,mm,2*(L/10-1A) ; PBNZ t,3B;       TRAP 0,Halt,0
```

After executing `cat mmix.mms | mmixal-fmt`, the output will be

```mmix
% Example program to format
L   IS    500               This is comment
t   IS    $255              This is another comment
n   GREG                    % Comment
r   GREG                    // Comment
jj  GREG  0                 Comment
    PBN   mm,2B
    LDA   t,NewLn
    TRAP  0,Fputs,StdOut
    CMP   t,mm,2*(L/10-1A)
    PBNZ  t,3B
    TRAP  0,Halt,0
```

> Everyone with more than a casual interest  in computers will probably get to know
at least one machine language sooner or later.
Machine language helps programmers understand what really goes on inside their computers.
And once one machine language has been learned, the characteristics
of another are easy to assimilate.
Computer science is largely concerned with an understanding of how low-level
details make it possible to achieve high-level goals.

By [Donald Knuth](https://en.wikipedia.org/wiki/Donald_Knuth)