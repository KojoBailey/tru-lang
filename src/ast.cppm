module;

#include <string>

export module ast;

import util;
import pretty_printer;

using namespace core;

export namespace ast {
	class Expression; // Sum Type
		struct Identifier;
		struct ExpressionSequence; // aka Block
		struct Declaration;
		struct Assignment;
		struct Return;
		struct StringLiteral;
		struct FunctionCall;
		struct BinaryOperator;
		struct BinaryOperation;
		struct UnaryOperator;
		struct UnaryOperation;

	class Component; // Sum Type
		struct MemberDeclaration;
		struct Import;

	class Interface;
}

export {
	// Strings are printed as quoted strings rather than nodes.
	template<> auto prettyFormat(const String& object)
		-> Vector<PrettyFormatElement>;

	// Identifiers are printed as simple unquoted strings.
	template<> auto prettyFormat(const ast::Identifier& object)
		-> Vector<PrettyFormatElement>;

	template<> auto prettyFormat(const ast::StringLiteral& object)
		-> Vector<PrettyFormatElement>;

	template<> auto prettyFormat(const Vector<Pair<ast::Identifier, ast::Expression*>>& object)
		-> Vector<PrettyFormatElement>;

	template<> auto prettyFormat(const Vector<ast::Expression>& object)
		-> Vector<PrettyFormatElement>;

	template<> auto prettyFormat(const ast::ExpressionSequence& object)
		-> Vector<PrettyFormatElement>;

	template<> auto prettyFormat(const ast::FunctionCall& object)
		-> Vector<PrettyFormatElement>;

	template<> auto prettyFormat(const ast::Expression& expression)
		-> Vector<PrettyFormatElement>;
}

struct ast::Identifier {
	String name;

	auto operator==(const ast::Identifier& other) const -> Bool;
};

// aka Block
struct ast::ExpressionSequence {
	Vector<ast::Expression> expressions;
};

struct ast::Declaration {
	Maybe<ast::Identifier> newIdentifier; // May be anonymous.
	Maybe<ast::Expression*> type; // May be inferred.
};

struct ast::Assignment {
	ast::Identifier target;
	ast::Expression* value;
};

struct ast::Return {
	ast::Expression* value;
};

struct ast::StringLiteral {
	String contents;
};

struct ast::FunctionCall {
	ast::Identifier callee;
	Vector<Pair<ast::Identifier, ast::Expression*>> args; // Supports mixed positional & keyword args.
};

struct ast::BinaryOperator {};

struct ast::BinaryOperation {
	ast::BinaryOperator op; // NOTE: `operator` is a reserved C++ keyword.
	ast::Expression* leftExpression;
	ast::Expression* rightExpression;
};

struct ast::UnaryOperator {};

struct ast::UnaryOperation {
	ast::UnaryOperator op; // NOTE: `operator` is a reserved C++ keyword.
	ast::Expression* expression;
};

class ast::Expression {
public:
	Variant<
		ast::ExpressionSequence,
		ast::Declaration,
		ast::Assignment,
		ast::Return,
		ast::Identifier,
		ast::StringLiteral,
		ast::FunctionCall,
		ast::BinaryOperation,
		ast::UnaryOperation
	> node;
};

struct ast::MemberDeclaration {
	Bool isLocal;
	ast::Identifier name;
	ast::Expression* type;
	Maybe<ast::Expression*> defaultValue;
};

struct ast::Import {
	Vector<String> path;
	Bool isQualified;
	Maybe<String> qualifierAlias;
};

class ast::Component {
public:
	Variant<
		ast::MemberDeclaration,
		ast::Import
	> component;
};

class ast::Interface {
public:
	Vector<ast::Component> components;
};
