server: Server.cpp TweetA.cpp TweetB.cpp TweetC.cpp Follower1.cpp Follower2.cpp Follower3.cpp Follower4.cpp Follower5.cpp
	g++ -std=c++11 -o server Server.cpp -lnsl
	g++ -std=c++11 -o a TweetA.cpp -lnsl
	g++ -std=c++11 -o b TweetB.cpp -lnsl
	g++ -std=c++11 -o c TweetC.cpp -lnsl
	g++ -std=c++11 -o f1 Follower1.cpp -lnsl
	g++ -std=c++11 -o f2 Follower2.cpp -lnsl
	g++ -std=c++11 -o f3 Follower3.cpp -lnsl
	g++ -std=c++11 -o f4 Follower4.cpp -lnsl
	g++ -std=c++11 -o f5 Follower5.cpp -lnsl


