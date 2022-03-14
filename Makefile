fib: 
	./build/app/app examples/fib.loli
udebug: ./build/tests/unit-tests
	cgdb ./build/tests/unit-tests

adebug: ./build/app/app
	cgdb ./build/app/app
