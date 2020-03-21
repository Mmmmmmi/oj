oj_server:oj_server.cc 
	g++ $^ -o $@ -std=c++11 -lpthread -L ./lib -l jsoncpp -l ctemplate -I ./include 
.PHONY:clean
clean:
	rm oj_server
