#include "ast_evaluator.hh"

namespace ast {

void ASTEvaluator::visit(const IntegerLiteral &literal) {
    result = literal.value;
}

void ASTEvaluator::visit(const BinaryOperator &binop) {
    binop.get_left().accept(*this);
    int32_t left = result;
    binop.get_right().accept(*this);
    int32_t right = result;

    switch (binop.op) {
        case o_plus:
            result = left + right;
            break;
        case o_minus:
            result = left - right;
            break;
        case o_times:
            result = left * right;
            break;
        case o_divide:
            if (right == 0) {
                utils::error("Division by zero.");
            }
            result = left / right;
            break;
        case o_eq:
            result = left == right;
            break;
        case o_neq:
            result = left != right;
            break;
        case o_lt:
            result = left < right;
            break;
        case o_le:
            result = left <= right;
            break;
        case o_gt:
            result = left > right;
            break;
        case o_ge:
            result = left >= right;
            break;
        default:
            utils::error("Unknown binary operator.");
    }
}

void ASTEvaluator::visit(const Sequence &seq) {
    const auto &exprs = seq.get_exprs();
    if (exprs.empty()) {
        utils::error("Evaluation error: Empty sequence.");
    }

    for (const auto &expr : exprs) {
        expr->accept(*this);
    }
}

void ASTEvaluator::visit(const IfThenElse &ite) {
    ite.get_condition().accept(*this);
    if (result) {
        ite.get_then_part().accept(*this);
    } else {
        ite.get_else_part().accept(*this);
    }
}

} // namespace ast
