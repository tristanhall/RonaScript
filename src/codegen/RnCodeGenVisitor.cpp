/*****************************************************************************
* File: RnCodeGenVisitor.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/26/22
* Version: 1
*
******************************************************************************/

#include "RnCodeGenVisitor.h"
#include "RnCodeGenerator.h"
#include "../parser/ast/StringLiteral.h"
#include "../parser/ast/ScopeNode.h"
#include "../parser/ast/ImportStmt.h"
#include "../parser/ast/ArgDecl.h"
#include "../parser/ast/AstNode.h"
#include "../parser/ast/ForLoop.h"
#include "../parser/ast/AliasDecl.h"
#include "../parser/ast/Expr.h"
#include "../parser/ast/VarDecl.h"
#include "../parser/ast/FloatLiteral.h"
#include "../parser/ast/UnaryExpr.h"
#include "../parser/ast/ReturnStmt.h"
#include "../parser/ast/FuncCall.h"
#include "../parser/ast/IndexedExpr.h"
#include "../parser/ast/ClassDecl.h"
#include "../parser/ast/WhileLoop.h"
#include "../parser/ast/BreakStmt.h"
#include "../parser/ast/IfStmt.h"
#include "../parser/ast/FuncDecl.h"
#include "../parser/ast/BinaryExpr.h"
#include "../parser/ast/AssignmentStmt.h"
#include "../parser/ast/ArrayLiteral.h"
#include "../parser/ast/ElifStmt.h"
#include "../parser/ast/IntLiteral.h"
#include "../parser/ast/ContinueStmt.h"
#include "../parser/ast/ElseStmt.h"
#include "../parser/ast/Name.h"
#include "../parser/ast/TryBlock.h"
#include "../parser/ast/CatchBlock.h"
#include "../parser/ast/BoolLiteral.h"
#include "../parser/ast/DeleteStmt.h"
#include "../parser/ast/ExitStmt.h"
#include "../parser/ast/AttributeAccess.h"

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::GeneralVisit(AstNode* node)
{
	if (!node)
	{
		return {};
	}

	switch (node->node_type)
	{
	case AST_ASSIGNMENT_STMT:
		return Visit(dynamic_cast<AssignmentStmt*>(node));
	case AST_BINARY_EXPR:
		return Visit(dynamic_cast<BinaryExpr*>(node));
	case AST_INDEXED_EXPR:
		return Visit(dynamic_cast<IndexedExpr*>(node));
	case AST_WHILE_LOOP:
		return Visit(dynamic_cast<WhileLoop*>(node));
	case AST_FOR_LOOP:
		return Visit(dynamic_cast<ForLoop*>(node));
	case AST_CLASS_DECL:
		return Visit(dynamic_cast<ClassDecl*>(node));
	case AST_EXPR:
		return Visit(dynamic_cast<Expr*>(node));
	case AST_CONTINUE_STMT:
		return Visit(dynamic_cast<ContinueStmt*>(node));
	case AST_FUNC_CALL:
		return Visit(dynamic_cast<FuncCall*>(node));
	case AST_FUNC_DECL:
		return Visit(dynamic_cast<FuncDecl*>(node));
	case AST_LIST_LITERAL:
		return Visit(dynamic_cast<ArrayLiteral*>(node));
	case AST_STRING_LITERAL:
		return Visit(dynamic_cast<StringLiteral*>(node));
	case AST_BOOL_LITERAL:
		return Visit(dynamic_cast<BoolLiteral*>(node));
	case AST_FLOAT_LITERAL:
		return Visit(dynamic_cast<FloatLiteral*>(node));
	case AST_INT_LITERAL:
		return Visit(dynamic_cast<IntLiteral*>(node));
	case AST_IMPORT:
		return Visit(dynamic_cast<ImportStmt*>(node));
	case AST_IF_STMT:
		return Visit(dynamic_cast<IfStmt*>(node));
	case AST_ELIF_STMT:
		return Visit(dynamic_cast<ElifStmt*>(node));
	case AST_ELSE_STMT:
		return Visit(dynamic_cast<ElseStmt*>(node));
	case AST_ARG_DECL:
		return Visit(dynamic_cast<ArgDecl*>(node));
	case AST_ALIAS_DECL:
		return Visit(dynamic_cast<AliasDecl*>(node));
	case AST_RETURN_STMT:
		return Visit(dynamic_cast<ReturnStmt*>(node));
	case AST_UNARY_EXPR:
		return Visit(dynamic_cast<UnaryExpr*>(node));
	case AST_VAR_DECL:
		return Visit(dynamic_cast<VarDecl*>(node));
	case AST_SCOPE:
		return Visit(dynamic_cast<ScopeNode*>(node));
	case AST_BREAK_STMT:
		return Visit(dynamic_cast<BreakStmt*>(node));
	case AST_MODULE:
		return Visit(dynamic_cast<Module*>(node));
	case AST_EXIT_STMT:
		return Visit(dynamic_cast<ExitStmt*>(node));
	case AST_DELETE_STMT:
		return Visit(dynamic_cast<DeleteStmt*>(node));
	case AST_TRY_BLOCK:
		return Visit(dynamic_cast<TryBlock*>(node));
	case AST_CATCH_BLOCK:
		return Visit(dynamic_cast<CatchBlock*>(node));
	case AST_NAME:
		return Visit(dynamic_cast<Name*>(node));
	default:
		return {};
	}
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::GeneralVisit(std::shared_ptr<AstNode> node)
{
	if (node)
	{
		return GeneralVisit(node.get());
	}
	else
	{
		return {};
	}
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(StringLiteral* node)
{
	return { new RnInstruction(OP_LOAD_STRING, RnObject::InternValue(node->data)) };
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(FloatLiteral* node)
{
	return { new RnInstruction(OP_LOAD_FLOAT, RnObject::InternValue(node->data)) };
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(IntLiteral* node)
{
	return { new RnInstruction(OP_LOAD_INT, RnObject::InternValue(node->data)) };
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(ArrayLiteral* node)
{
	InstructionBlock instructions;

	for (auto& item : node->items)
	{
		auto instruction_block = GeneralVisit(item);
		instructions.insert(instructions.begin(),
			instruction_block.begin(),
			instruction_block.end());
	}
	instructions.push_back(new RnInstruction(OP_MAKE_ARRAY, node->items.size()));
	return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(ScopeNode* node)
{
	InstructionBlock instructions;
	for (auto& child : node->children)
	{
		InstructionBlock instruction_block = GeneralVisit(child);
		instructions.insert(instructions.end(),
			instruction_block.begin(),
			instruction_block.end());
	}
	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(ForLoop* node)
{
	InstructionBlock instructions;
	InstructionBlock init = GeneralVisit(node->init);
	InstructionBlock test = GeneralVisit(node->test);
	InstructionBlock update = GeneralVisit(node->update);
	InstructionBlock scope = GeneralVisit(node->scope);
	test.push_back(new RnInstruction(OP_JUMPF_IF, scope.size() + update.size() + 2));
	scope.insert(scope.begin(), test.begin(), test.end());
	WrapContext(scope);

	instructions.reserve(scope.size() + test.size() + init.size() + update.size());

	instructions.insert(instructions.end(), init.begin(), init.end());
//	instructions.insert(instructions.end(), test.begin(), test.end());
//	instructions.emplace_back(new RnInstruction(OP_JUMPF_IF, test.size() + scope.size() + update.size() + 1));
	instructions.insert(instructions.end(), scope.begin(), scope.end());
	instructions.insert(instructions.end(), update.begin(), update.end());
	instructions.emplace_back(new RnInstruction(OP_JUMPB,
		scope.size() + update.size()));
	WrapContext(instructions);
	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(WhileLoop* node)
{
	InstructionBlock instructions;
	InstructionBlock scope = GeneralVisit(node->scope);
	WrapContext(scope);
	InstructionBlock test = GeneralVisit(node->test);
	instructions.reserve(scope.size() + test.size());
	instructions.insert(instructions.end(), test.begin(), test.end());
	instructions.emplace_back(new RnInstruction(OP_JUMPF_IF, scope.size() + 1));
//	instructions.push_back(new RnInstruction(OP_CREATE_CONTEXT));
	instructions.insert(instructions.end(), scope.begin(), scope.end());
//	instructions.push_back(new RnInstruction(OP_DESTROY_CONTEXT));
	instructions.emplace_back(new RnInstruction(OP_JUMPB,
		scope.size() + test.size() + 1));

	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(ImportStmt* node)
{
	return {};
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(Module* node)
{
	InstructionBlock instructions = GeneralVisit(node->scope);
	instructions.insert(instructions.begin(),
		new RnInstruction(OP_MAKE_MODULE,
			RnObject::InternValue(node->name->value),
			instructions.size()));
	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(FuncDecl* node)
{
	InstructionBlock instructions;
	InstructionBlock scope = GeneralVisit(node->scope);
	instructions.reserve(scope.size());
	auto make_instruction = new RnInstruction(OP_MAKE_FUNC,
		RnObject::InternValue(node->id),
		static_cast<long>(node->args.size()),
		static_cast<long>(scope.size()));
	instructions.emplace_back(make_instruction);

	for (auto& arg : node->args)
	{
		InstructionBlock arg_decl = GeneralVisit(arg);
		instructions.insert(instructions.end(), arg_decl.begin(), arg_decl.end());
	}

	instructions.insert(instructions.end(), scope.begin(), scope.end());
	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(FuncCall* node)
{
	InstructionBlock instructions;
	InstructionBlock expr = GeneralVisit(node->expr);
	for (auto& arg : node->args)
	{
		InstructionBlock arg_instructions = GeneralVisit(arg);
		instructions.insert(instructions.end(),
			arg_instructions.begin(),
			arg_instructions.end());
	}
	instructions.insert(instructions.end(), expr.begin(), expr.end());
	instructions.emplace_back(new RnInstruction(OP_CALL,
		static_cast<long>(node->args.size())));
	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(VarDecl* node)
{
	InstructionBlock instructions;
	auto interned_id = RnObject::InternValue(node->id);
	if (node->is_const)
	{
		instructions.emplace_back(new RnInstruction(OP_MAKE_CONST,
			node->type.GetType(),
			interned_id));
	}
	else
	{
		instructions.emplace_back(new RnInstruction(OP_MAKE_VAR,
			node->type.GetType(),
			interned_id));
	}
	if (node->init_value)
	{
		InstructionBlock expr = GeneralVisit(node->init_value);
		instructions.insert(instructions.end(), expr.begin(), expr.end());
		instructions.push_back(new RnInstruction(OP_LOAD_VALUE, interned_id));
		instructions.push_back(new RnInstruction(OP_STORE));
	}
	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(Name* node)
{
	return { new RnInstruction(OP_LOAD_VALUE, RnObject::InternValue(node->value)) };
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(ClassDecl* node)
{
	InstructionBlock instructions;
	InstructionBlock scope = GeneralVisit(node->scope);
	instructions.reserve(scope.size());
	instructions.emplace_back(new RnInstruction(OP_MAKE_CLASS,
		RnObject::InternValue(node->id),
		scope.size()));
	instructions.insert(instructions.end(), scope.begin(), scope.end());
	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(ExitStmt* node)
{
	return { new RnInstruction(OP_EXIT, node->exit_code->data) };
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(ReturnStmt* node)
{
	InstructionBlock instructions = GeneralVisit(node->expr);
	instructions.emplace_back(new RnInstruction(OP_RETURN));

	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(AttributeAccess* node)
{
	return {};
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(TryBlock* node)
{
	return {};
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(CatchBlock* node)
{
	return {};
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(IfStmt* node)
{
	InstructionBlock instructions;
	InstructionBlock test = GeneralVisit(node->test);
	InstructionBlock consequent = GeneralVisit(node->consequent);
	WrapContext(consequent);
	InstructionBlock alternative = GeneralVisit(node->alternative);

	instructions.insert(instructions.end(), test.begin(), test.end());
	auto jumpf = new RnInstruction(OP_JUMPF_IF, consequent.size());
	instructions.emplace_back(jumpf);
	instructions.insert(instructions.end(), consequent.begin(), consequent.end());

	if (!alternative.empty())
	{
		jumpf->_arg1++;
		instructions.emplace_back(new RnInstruction(OP_JUMPF, alternative.size()));
		instructions.insert(instructions.end(), alternative.begin(), alternative.end());
	}

	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(ElifStmt* node)
{
	InstructionBlock instructions = { new RnInstruction(OP_CREATE_CONTEXT) };
	InstructionBlock test = GeneralVisit(node->test);
	InstructionBlock consequent = GeneralVisit(node->consequent);
	WrapContext(consequent);
	InstructionBlock alternative = GeneralVisit(node->alternative);

	instructions.insert(instructions.end(), test.begin(), test.end());
	auto jumpf = new RnInstruction(OP_JUMPF_IF, consequent.size());
	instructions.emplace_back(jumpf);
	instructions.insert(instructions.end(), consequent.begin(), consequent.end());
//	instructions.emplace_back(new RnInstruction(OP_JUMPF, alternative.size()));

	if (!alternative.empty())
	{
		jumpf->_arg1++;
		instructions.emplace_back(new RnInstruction(OP_JUMPF, alternative.size()));
		instructions.insert(instructions.end(), alternative.begin(), alternative.end());
	}
	instructions.push_back(new RnInstruction(OP_DESTROY_CONTEXT));

	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(ElseStmt* node)
{
	InstructionBlock instructions;
	InstructionBlock consequent = GeneralVisit(node->consequent);
	WrapContext(consequent);
	instructions.insert(instructions.end(), consequent.begin(), consequent.end());

	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(DeleteStmt* node)
{
	size_t internvalue =
		RnObject::InternValue(dynamic_pointer_cast<Name>(node->name)->value);

	return { new RnInstruction(OP_DELETE, internvalue) };
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(BoolLiteral* node)
{
	return { new RnInstruction(OP_LOAD_BOOL, node->data) };
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(UnaryExpr* node)
{
	if (node->op == "++")
	{
		size_t internvalue =
			RnObject::InternValue(std::dynamic_pointer_cast<Name>(node->expr)->value);
		return { new RnInstruction(OP_UNARY_INCREMENT, internvalue) };
	}
	return {};
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(Expr* node)
{
	return GeneralVisit(node->expr);
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(AliasDecl* node)
{
	return { new RnInstruction(OP_MAKE_ALIAS,
		RnObject::InternValue(node->base_name->value),
		RnObject::InternValue(node->alias_name->value)) };
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(ArgDecl* node)
{
	return { new RnInstruction(OP_MAKE_ARG,
		RnType::StringToType(node->type),
		RnObject::InternValue(node->id->value)) };
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(AssignmentStmt* node)
{
	InstructionBlock instructions;
	InstructionBlock lvalue = GeneralVisit(node->lexpr);
	InstructionBlock rvalue = GeneralVisit(node->rexpr);
	instructions.insert(instructions.end(), rvalue.begin(), rvalue.end());
	instructions.insert(instructions.end(), lvalue.begin(), lvalue.end());
	instructions.push_back(new RnInstruction(OP_STORE));
	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(BinaryExpr* node)
{
	InstructionBlock instructions;

	if (node->_op == "::")
	{
		instructions.emplace_back(new RnInstruction(GetOpCodeFromOperator(node->_op),
			RnObject::InternValue(std::static_pointer_cast<Name>(node->_left)->value),
			RnObject::InternValue(std::static_pointer_cast<Name>(node->_right)->value)));
	}
	else if (node->_op == "->")
	{
		InstructionBlock left = GeneralVisit(node->_left);
		instructions.insert(instructions.end(), left.begin(), left.end());
		instructions.push_back(new RnInstruction(OP_ATTR_ACCESS,
			RnObject::InternValue(std::static_pointer_cast<Name>(node->_right)->value)));
//		instructions.emplace_back(new RnInstruction(GetOpCodeFromOperator(node->_op)));
	}
	else
	{
		InstructionBlock left = GeneralVisit(node->_left);
		InstructionBlock right = GeneralVisit(node->_right);
		instructions.insert(instructions.end(), left.begin(), left.end());
		instructions.insert(instructions.end(), right.begin(), right.end());
		instructions.emplace_back(new RnInstruction(GetOpCodeFromOperator(node->_op)));
	}

	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(ContinueStmt* node)
{
	return {};
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(IndexedExpr* node)
{
	InstructionBlock instructions;
	InstructionBlock expr = GeneralVisit(node->expr);
	InstructionBlock index = GeneralVisit(node->idx);
	instructions.insert(instructions.end(), expr.begin(), expr.end());
	instructions.insert(instructions.end(), index.begin(), index.end());
	instructions.emplace_back(new RnInstruction(OP_INDEX_ACCESS));
	return instructions;
}

/*****************************************************************************/
std::vector<RnInstruction*> RnCodeGenVisitor::Visit(BreakStmt* node)
{
	return {};
}

/*****************************************************************************/
void RnCodeGenVisitor::WrapContext(InstructionBlock& block)
{
	block.insert(block.begin(), new RnInstruction(OP_CREATE_CONTEXT));
	block.push_back(new RnInstruction(OP_DESTROY_CONTEXT));
}
