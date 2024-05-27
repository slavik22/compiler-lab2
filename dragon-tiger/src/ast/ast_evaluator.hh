#ifndef AST_EVALUATOR_HH
#define AST_EVALUATOR_HH

#include "nodes.hh"
#include "../utils/errors.hh"

namespace ast {

class ASTEvaluator : public ConstASTIntVisitor {
public:
    int32_t result; // Variable to store the result of the evaluation

    ASTEvaluator() : result(0) {} // Initialize result to 0

    virtual void visit(const IntegerLiteral &) override;
    virtual void visit(const BinaryOperator &) override;
    virtual void visit(const Sequence &) override;
    virtual void visit(const IfThenElse &) override;

    // Default handler for other nodes
    virtual void visit(const Node &) override {
        utils::error("Evaluation error: Unsupported AST node.");
    }

    int32_t evaluate(const Expr &expr) {
        expr.accept(*this);
        return result;
    }
};

} // namespace ast

#endif // AST_EVALUATOR_HH
