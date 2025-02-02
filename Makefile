all: compile

compile:
	gcc -I. -g -o target/server main.c comm/request.c comm/response.c comm/server.c lib/lib.c lib/map.c lib/error.c lib/mem.c log/log.c -lws2_32 -DLOG_USE_COLOR

debug:
	gcc -I. -g -o target/server-debug main.c comm/request.c comm/response.c comm/server.c lib/lib.c lib/map.c lib/error.c lib/mem.c log/log.c -lws2_32 -DLOG_USE_COLOR -DMEMDEB

clean:
	del main.exe