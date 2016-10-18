# this is a make file
s.c = src/file_server.c
s = bin/fserver
c.c = src/file_client.c
c = bin/fclient
all:$(s) $(c)
$(s):$(s.c)
			gcc -o $(s) $(s.c)
$(c):$(c.c)
			gcc -o $(c) $(c.c)
