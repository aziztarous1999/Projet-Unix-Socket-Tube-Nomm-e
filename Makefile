myapp: interface.c
	gcc interface.c `pkg-config --cflags --libs gtk+-3.0` -o myapp

run:
	./myapp $(name)

clean:
	rm *.o myapp


