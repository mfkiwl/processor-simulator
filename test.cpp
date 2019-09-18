#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "source/processor/model/instructions.h"
#include "source/processor/model/model.h"

unsigned int Factorial(unsigned int number) {
	return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE("arithemetic.asm", "[arithmetic]") {

	const Instructions instructions("programs/tests/arithmetic.mac");
	Model processor(instructions);

	REQUIRE(Factorial(1) == 1);
	REQUIRE(Factorial(2) == 2);
	REQUIRE(Factorial(3) == 6);
	REQUIRE(Factorial(10) == 3628800);
}