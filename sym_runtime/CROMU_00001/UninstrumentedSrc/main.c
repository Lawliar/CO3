/**
 * Main function just calls handle_menu
 * @return Always returns 0
 **/
#include "wrapped_malloc.h"
#include "test.h"
#include "runtime.h"
int main(void) {
	sockRec();
	handle_menu();
	_sym_end();
	whole_free();
	return 0;
}
