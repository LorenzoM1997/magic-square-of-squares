make:
	docker run --rm \
	-v ${CURDIR}:/w \
	-w /w \
	magic-squares:latest \
	g++ -Wall -Werror -o squares squares.cpp

docker:
	docker build -t magic-squares:latest -f Dockerfile .

run:
	docker run --rm \
	-v ${CURDIR}:/w \
	-w /w \
	magic-squares:latest \
	./squares

exec:
	docker run --rm \
	-v ${CURDIR}:/w \
	-w /w \
	magic-squares:latest \
