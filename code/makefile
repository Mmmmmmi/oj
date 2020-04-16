oj_server:oj_server.cc 
	g++ $^ -o $@ -std=c++11 -lpthread -L ./lib -l jsoncpp -l ctemplate -I ./include -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include `mysql_config --cflags --libs`
.PHONY:clean
clean:
	rm oj_server
