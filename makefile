cc = gcc
target = main
obj = main.o fun.o console.o start_mplayer.o
cflags =
$(target):$(obj)
	$(cc) $^ -o $@ $(cflags)
%.o:%.c
	$(cc) -c $< -o $@ $(cflags)

clean:
	rm $(target) $(obj)