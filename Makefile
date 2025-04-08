
install:
	clib install --dev

test:
	@$(CC) $(CFLAGS) test.c -I src -I deps -I deps/greatest -o $@
	@./$@

.PHONY: install test
