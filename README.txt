Project 1 phase 2
name: Haopeng Song
ID: 9753089761


I created a server and client communication program using c++ programming language.
The Server.cpp is implemented for the server, it starts a server. Each Tweet#.cpp represents a instance of Tweet people.
Each Follower#.cpp represents a instance of follower.

For each Tweet#.c program, when it starts it will look for the input file namely Tweet#.txt. However this was done inside the program and does not require a command line argument.

For each Follower#.cpp program, when it starts it will look for the input file namely Follower#.txt However this was done inside the program and does not require a command line argument.

Compilation step:
1.
	I use server Viterbi-scf2.usc.edu since it is linux.
	Login to server Viterbi-scf2.usc.edu
2.
	Go to the file directory
	In the command line, use: 
	tar -zxvf ee450_haopengs_phase2.tar.gz (extract)

3.	
	type "make" to compile 

4.	Plase follow these steps:

	run the executable:
	1> in the first terminal window:
	 run server first.
	./server             

	2> in second terminal window:
		run one of the Tweet instance.
	./a         to run the TweetA instance
	or
	./b 	    to run the TweetB instance
	or
	./c	        to run the TweetC instance

	3> in the third terminal window:
		run all followers one by one.

	./f1		to run the Follower1 instance
	./f2		to run the Follower2 instance
	./f3		to run the Follower3 instance
	./f4		to run the Follower4 instance
	./f5		to run the Follower5 instance

	The results will print out the follower who like the tweet and its following people in the format below:

		<Follower#> is follwing <Tweet#> 
		<Follower#> has liked <Tweet#>


	Please provide input files inside the same folder with clients executables, namely TweetA.txt, TweetB.txt, TweetC.txt, Follower1.txt, Follower2.txt, Follower3.txt, Follower4.txt, Follower5.txt

	Please format those files with the exact same setting stated on the project manual.
	There is no need to type file names on the command line as long as there are in the same folder it’s fine.

The output messages are matched with table on the project manual.

Reused Code: I referenced Beej’s socket programming on Page 26 and 28. I referenced the way it sets up the socket and the using of the fork() function.

