all:
	$(MAKE) -C libds3wiibt	install
	$(MAKE) -C sample

run:
	$(MAKE) -C libds3wiibt	install
	$(MAKE) -C sample		run

clean:
	$(MAKE) -C libds3wiibt	clean
	$(MAKE) -C sample		clean
