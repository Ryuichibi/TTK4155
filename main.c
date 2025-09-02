#include <avr/io.h>

int main() {

	DDRA |= (1 << PA0);

	PORTA |= (1 << PA0);

	while (1) {

	}

	return 0;

}
