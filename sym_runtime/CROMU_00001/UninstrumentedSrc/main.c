/**
 * Main function just calls handle_menu
 * @return Always returns 0
 **/
#include "wrapped_malloc.h"
#include "test.h"
int main(void) {
	handle_menu();
	whole_free();
	return 0;
}
