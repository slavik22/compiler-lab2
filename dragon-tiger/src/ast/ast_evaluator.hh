#ifndef AST_EVALUATOR_HH
#define AST_EVALUATOR_HH

#include "nodes.hh"
#include "../utils/errors.hh"

namespace ast {

class ASTEvaluator : public ConstASTIntVisitor {
public:
    int32_t result; // Variable to store the result of the evaluation

    ASTEvaluator() : result(0) {} // Initialize result to 0

    virtual int32_t visit(const IntegerLiteral &);
    virtual int32_t visit(const BinaryOperator &);
    virtual int32_t visit(const Sequence &);
    virtual int32_t visit(const IfThenElse &);

    virtual int32_t visit(const StringLiteral &);
    virtual int32_t visit(const Let &);
    virtual int32_t visit(const Identifier &);
    virtual int32_t visit(const VarDecl &);
    virtual int32_t visit(const FunDecl &);
    virtual int32_t visit(const FunCall &);
    virtual int32_t visit(const WhileLoop &);
    virtual int32_t visit(const ForLoop &);
    virtual int32_t visit(const Break &);
    virtual int32_t visit(const Assign &);

    int32_t evaluate(const Expr &expr) {
        expr.accept(*this);
        return result;
    }
};

} // namespace ast

#endif // AST_EVALUATOR_HH
