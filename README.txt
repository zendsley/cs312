Author:  Zackary Endsley
Email:  zendsle@siue.edu
Alt Email:  xzen21@gmail.com
Class Number: 20

Project Repo:  https://github.com/zendsley/cs312.git

CS 312 - OpenMP Programming Assignment
Client: Mark McKenney

The implementation seems to work about 85-95% of the time.
Some issues arise when running the program multiple times rapidly;
especially with smaller data sets, the average is not always correct when doing this.

I suppose this is an issue where using #omp parallel critical could possibly fix the problem,
but the execution was significantly slowed when using this method....
so naturally I opted for speed over safety.  It gives the right value most of the time,
so run multiple times or with fewer threads if unsure.
