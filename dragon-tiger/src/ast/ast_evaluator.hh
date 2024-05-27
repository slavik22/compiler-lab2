#ifndef AST_EVALUATOR_HH
#define AST_EVALUATOR_HH

#include "nodes.hh"
#include "../utils/errors.hh"

namespace ast {

class ASTEvaluator : public ConstASTIntVisitor {
public:
    int32_t result; // Variable to store the result of the evaluation

    ASTEvaluator() : result(0) {} // Initialize result to 0

    void visit(const IntegerLiteral &);
    void visit(const StringLiteral &);
    void visit(const BinaryOperator &);
    void visit(const Sequence &);
    void visit(const Let &);
    void visit(const Identifier &);
    void visit(const IfThenElse &);
    void visit(const VarDecl &);
    void visit(const FunDecl &);
    void visit(const FunCall &);
    void visit(const WhileLoop &);
    void visit(const ForLoop &);
    void visit(const Break &);
    void visit(const Assign &);

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
