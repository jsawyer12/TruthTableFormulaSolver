CFLAGS = -Wall -Wextra

clean:
	rm -f ttable

ttable: ttable.c
	gcc ${CFLAGS} -o ttable ttable.c
