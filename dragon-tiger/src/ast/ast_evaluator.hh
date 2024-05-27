#ifndef AST_EVALUATOR_HH
#define AST_EVALUATOR_HH

#include "nodes.hh"
#include "../utils/errors.hh"

namespace ast {

class ASTEvaluator : public ConstASTIntVisitor {
public:
    int32_t result; // Variable to store the result of the evaluation

    ASTEvaluator() : result(0) {} // Initialize result to 0

    int32_t visit(const IntegerLiteral &);
    // int32_t visit(const StringLiteral &);
    int32_t visit(const BinaryOperator &);
    int32_t visit(const Sequence &);
    // int32_t visit(const Let &);
    // int32_t visit(const Identifier &);
    int32_t visit(const IfThenElse &);
    // int32_t visit(const VarDecl &);
    // int32_t visit(const FunDecl &);
    // int32_t visit(const FunCall &);
    // int32_t visit(const WhileLoop &);
    // int32_t visit(const ForLoop &);
    // int32_t visit(const Break &);
    // int32_t visit(const Assign &);

    // Default handler for other nodes
    void visit(const Node &) override {
        utils::error("Evaluation error: Unsupported AST node.");
    }

    int32_t evaluate(const Expr &expr) {
        expr.accept(*this);
        return result;
    }
};

} // namespace ast

#endif // AST_EVALUATOR_HH
