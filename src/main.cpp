import util;
import ast;
import pretty_printer;

using namespace core;
using namespace ast;

auto main() -> CInt
{
	// ExpressionSequence mainFunctionBody;
	// mainFunctionBody.expressions.emplace_back(Expression{helloWorld});
	//
	// Expression mainFunctionBodyExpr = Expression{mainFunctionBody};
	// MemberDeclaration mainFunction = MemberDeclaration{
	// 	.isLocal = false,
	// 	.name = Identifier{"run"},
	// 	.type = nullptr,
	// 	.defaultValue = &mainFunctionBodyExpr,
	// };
	// Interface program;
	// program.components.emplace_back(Component{mainFunction});

	prettyPrint(ExpressionSequence{
		.expressions = { Expression{ FunctionCall{
			.callee = Identifier{"printLine"},
			.args = {
				{
					Identifier{"0"},
					new Expression{StringLiteral{"Hello, world!"}}
				},
			},
		}}},
	});

    return 0;
}
