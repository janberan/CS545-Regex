Corpus linguistics regex matching
===========

For a detailed description, please see the project report (report.pdf)

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
