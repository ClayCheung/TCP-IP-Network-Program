# this is a make file
s.c = src/file_server.c
s = bin/fserver
#c.c = src/bound_host2.c
#c = bin/host2
#all:$(s) $(c)
$(s):$(s.c)
			gcc -o $(s) $(s.c)
#$(c):$(c.c)
#			gcc -o $(c) $(c.c)
