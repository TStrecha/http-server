all: compile

compile:
	gcc -Isrc/ -g -o target/server src/main.c src/comm/request.c src/comm/response.c src/comm/server.c src/lib/lib.c src/lib/map.c src/lib/error.c src/lib/mem.c src/lib/default_handlers.c src/lib/option.c src/log/log.c -lws2_32 -DLOG_USE_COLOR

debug:
	gcc -Isrc/ -g -o target/server-debug src/main.c src/comm/request.c src/comm/response.c src/comm/server.c src/lib/lib.c src/lib/map.c src/lib/error.c src/lib/mem.c src/lib/default_handlers.c src/lib/option.c src/log/log.c -lws2_32 -DLOG_USE_COLOR -DMEMDEB

clean:
	del main.exe