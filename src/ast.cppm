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

	template<> auto prettyFormat(const ast::FunctionCall& object)
		-> Vector<PrettyFormatElement>;

	template<> auto prettyFormat(const ast::Expression& expression)
		-> Vector<PrettyFormatElement>;
}
