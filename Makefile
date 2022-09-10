OUTPUT_NAME = dircnt
OBJECTS = $(OUTPUT_NAME).o

default: $(OUTPUT_NAME)

%.o: %.c
	gcc -c $< -o $@

dircnt: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(OUTPUT_NAME)
