/*****************************************************************************
* File: RnCodeGenerator.h
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNCODEGENERATOR_H
#define RONASCRIPT_RNCODEGENERATOR_H

#include "RnCodeGenVisitor.h"
#include "../parser/ast/Ast.h"
#include "../vm/RnInternment.h"

class RnCodeGenerator
{
 public:
	RnCodeGenerator();
	~RnCodeGenerator();
	void Generate(Ast* ast);
	void Optimize();

	InstructionBlock GetInstructions()
	{
		return instructions;
	}

 private:
	InstructionBlock instructions;
	RnCodeGenVisitor visitor;
	static RnInternment<std::string> _string_internment;
	static RnInternment<double> _float_internment;
	static RnInternment<long> _int_internment;
};

#endif //RONASCRIPT_RNCODEGENERATOR_H