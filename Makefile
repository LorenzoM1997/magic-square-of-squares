$THREADS ?= "4"

# provide a help dialog for the makefile
help: Makefile
	@sed -n 's/^##//p' $<

## make: build the code
build: docker
	docker run --rm \
	-v ${CURDIR}:/w \
	-w /w \
	magic-squares:latest \
	g++ -Wall -Werror -o squares squares.cpp

## docker: build the docker image
docker:
	docker build -t magic-squares:latest -f Dockerfile .

## run: run the executable
## the executable can accept a single argument, the number of threads to use
run: build
	docker run --rm \
	-v ${CURDIR}:/w \
	-w /w \
	magic-squares:latest \
	./squares ${THREADS}

## exec: start an interactive shell in the docker container
exec: docker
	docker run -it --rm \
	-v ${CURDIR}:/w \
	-w /w \
	magic-squares:latest