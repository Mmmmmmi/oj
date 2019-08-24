oj_server:oj_server.cc compile.hpp
	g++ oj_server.cc -o oj_server -std=c++11 -lpthread -L ./lib -l jsoncpp -l ctemplate
.PHONY:clean
clean:
	rm oj_server
