#ifndef LOWER_H_
#define LOWER_H_
#include <llvm/IR/Function.h>

void lowerSwitch(llvm::Function &);
void breakConstantExpr(llvm::Function &);

#endif /* LOWER_H_ */
