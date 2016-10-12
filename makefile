# this is a make file
#s.c = src/echo_server.c
#s = bin/eserver
c.c = src/op_client.c
c = bin/opclient
#all:$(s) $(c)
#$(s):$(s.c)
#			gcc -o $(s) $(s.c)
$(c):$(c.c)
			gcc -o $(c) $(c.c)
