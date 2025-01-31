all: server

server:
	gcc -I. -g -o target/server main.c comm/request.c comm/response.c comm/server.c lib/lib.c lib/map.c lib/error.c log/log.c -lws2_32 -DLOG_USE_COLOR

clean:
	del main.exe