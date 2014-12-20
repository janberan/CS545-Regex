Word-based regex matching
===========

Usage
-----------
1. Clone the repo
2. make
3. ./parser <word> <lemma> <tags>

Examples
-----------

Sample database:
eeeeaa..cc<ii<xx..zz>....>$ԱԱee....cc<....kk>$eeԱԱaabbcc$

Sample queries:
./parser A B "aa$"
./parser A A "(cc<jj || ii>) && (bb || aa) $"
