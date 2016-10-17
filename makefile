# this is a make file
s.c = src/bound_host1.c
s = bin/host1
c.c = src/bound_host2.c
c = bin/host2
all:$(s) $(c)
$(s):$(s.c)
			gcc -o $(s) $(s.c)
$(c):$(c.c)
			gcc -o $(c) $(c.c)
