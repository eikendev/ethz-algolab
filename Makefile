.PHONY: all
all: run

.PHONY: run
run:
	./algolab.sh run

.PHONY: build
build:
	./algolab.sh compile

.PHONY: clean
clean:
	find -type d -name 'build' -exec rm -rf "{}" +;

.PHONY: test
test:
	./algolab.sh test

.PHONY: debug
debug:
	./algolab.sh debug
