#include "ast_evaluator.hh"

namespace ast {

int32_t ASTEvaluator::visit(const IntegerLiteral &literal) {
    return literal.value;
}

int32_t ASTEvaluator::visit(const BinaryOperator &binop) {
    int32_t left = binop.get_left().accept(*this);
    int32_t right = binop.get_right().accept(*this);

    switch (binop.op) {
        case o_plus:
            return left + right;
        case o_minus:
            return left - right;
        case o_times:
            return left * right;
        case o_divide:
            if (right == 0) {
                utils::error("Division by zero.");
            }
            return left / right;
        case o_eq:
            return left == right;
        case o_neq:
            return left != right;
        case o_lt:
            return left < right;
        case o_le:
            return left <= right;
        case o_gt:
            return left > right;
        case o_ge:
            return left >= right;
        default:
            utils::error("Unknown binary operator.");
            return 0; // This will never be reached
    }
}

int32_t ASTEvaluator::visit(const Sequence &seq) {
    const auto &exprs = seq.get_exprs();
    if (exprs.empty()) {
        utils::error("Evaluation error: Empty sequence.");
    }

    int32_t result = 0;
    for (const auto &expr : exprs) {
        result = expr->accept(*this);
    }
    return result;
}

int32_t ASTEvaluator::visit(const IfThenElse &ite) {
    int32_t condition = ite.get_condition().accept(*this);
    if (condition) {
        return ite.get_then_part().accept(*this);
    } else {
        return ite.get_else_part().accept(*this);
    }
}

} // namespace ast
