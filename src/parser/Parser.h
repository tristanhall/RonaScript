/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_PARSER_H
#define RONASCRIPT_PARSER_H

#include <vector>
#include <string>
#include <unordered_set>
#include <map>
#include <map>
#include "../lexer/Token.h"
#include "../lexer/TokenType.h"
#include "ast/Ast.h"
#include "../util/RonaSequencer.h"
#include <filesystem>
#include <unordered_map>

class StringLiteral;
class ScopeNode;
class ImportStmt;
class ArgDecl;
class AstNode;
class ForLoop;
class AliasDecl;
class Expr;
class VarDecl;
class FloatLiteral;
class UnaryExpr;
class ReturnStmt;
class FuncCall;
class IndexedExpr;
class ClassDecl;
class WhileLoop;
class BreakStmt;
class IfStmt;
class FuncDecl;
class BinaryExpr;
class AssignmentStmt;
class ArrayLiteral;
class ElifStmt;
class IntLiteral;
class ContinueStmt;
class ElseStmt;
class Name;
class TryBlock;
class CatchBlock;
class BoolLiteral;
class DeleteStmt;
class ExitStmt;
class AttributeAccess;

enum Associativity_t
{
	LEFT,
	RIGHT,
	NO_ASSOCIATIVITY,
};

enum ParserState
{
	GENERAL_CONTEXT,
	CLASS_DECL_CONTEXT,
	FUNC_DECL_CONTEXT,
};

class Parser : public RonaSequencer<Token*, TokenType>
{
 public:
	Parser();
	~Parser();

	void ConditionalBufAdvance(TokenType t);
	std::shared_ptr<ImportStmt> ParseImportStmt();
	std::shared_ptr<VarDecl> ParseVarDecl(std::vector<Token*> qualifiers = {});
	std::shared_ptr<FuncDecl> ParseFuncDecl(std::vector<Token*> qualifiers = {});
	std::shared_ptr<ClassDecl> ParseClassDecl();
	std::shared_ptr<AstNode> GetExprComponent();
	std::shared_ptr<AstNode> ParseExpr(TokenType stop_token = TokenType::SEMICOLON);
	std::shared_ptr<UnaryExpr> ParseUnaryExpr(std::shared_ptr<AstNode> expr = nullptr);
	std::shared_ptr<BreakStmt> ParseBreakStmt();
	std::shared_ptr<ContinueStmt> ParseContinueStmt();
	std::shared_ptr<ReturnStmt> ParseReturnStmt();
	std::shared_ptr<DeleteStmt> ParseDeleteStmt();
	std::shared_ptr<ExitStmt> ParseExitStmt();
	std::shared_ptr<AssignmentStmt> ParseAssignmentStatement(
		const std::shared_ptr<AstNode>& rexpr = nullptr);
	std::shared_ptr<IfStmt> ParseIfStmt();
	std::shared_ptr<ElifStmt> ParseElifStmt();
	std::shared_ptr<ElseStmt> ParseElseStmt();
	std::shared_ptr<ScopeNode> ParseScope();
	std::shared_ptr<FuncCall> ParseFuncCall(
		const std::shared_ptr<AstNode>& expr = nullptr);
	std::shared_ptr<ArrayLiteral> ParseArrayLiteral();
	std::shared_ptr<WhileLoop> ParseWhileLoop();
	std::shared_ptr<ForLoop> ParseForLoop();
	std::shared_ptr<AliasDecl> ParseAliasDecl();
	std::shared_ptr<AstNode> ParseIndexedExpr(
		const std::shared_ptr<AstNode>& expr = nullptr);
	std::shared_ptr<Name> ParseName();
	std::shared_ptr<TryBlock> ParseTryBlock();
	std::shared_ptr<CatchBlock> ParseCatchBlock();
	std::shared_ptr<Module> ParseModule();
	void RevertScope();
	void ConvertScope(const std::shared_ptr<ScopeNode>& scope);
	[[nodiscard]] std::string DumpsAst() const;
	void Parse();
	std::vector<Token*> GetQualifiers();
	void LoadTokens(std::vector<Token*> t);
	std::shared_ptr<AstNode> TransformBinaryExpr(
		std::shared_ptr<BinaryExpr> binary_expr);

	/*************************************************************************/
	TokenType GetCurrentAsExpectedType() override
	{
		return Current()->token_type;
	}

	/*************************************************************************/
	size_t GetTokenCount()
	{
		return GetDataSize();
	}

	/*************************************************************************/
	std::string ItemToString(Token* token) override
	{
		return token->ToString();
	}

	/*************************************************************************/
	void HandleUnexpectedItem() override
	{
		auto msg = "Unexpected token '" + Current()->lexeme + "'";
		msg += +" in file " + Current()->file_info.ToString();
		msg += "\n\n" + Current()->file_info.GetContextualBlock();
		throw std::runtime_error(msg);
	}

 public:
	std::string working_dir = ".";
	std::filesystem::path file;
	std::vector<Token*> tokens;
	Ast* ast;

 private:
	std::shared_ptr<ScopeNode> _current_scope = nullptr;
	std::unordered_set<TokenType>
		unary_lookback_set = { TokenType::EQUAL, TokenType::R_PARAN, TokenType::COMMA };
	std::unordered_map<TokenType, std::string> _char_map;
	std::unordered_map<TokenType, int> _prec_tbl;
	std::unordered_map<TokenType, Associativity_t> associativity;
	std::unordered_map<std::string, std::string> _pragma_table;
	ParserState _previous_state = GENERAL_CONTEXT;
	ParserState _current_state = GENERAL_CONTEXT;

};

#endif //RONASCRIPT_PARSER_H