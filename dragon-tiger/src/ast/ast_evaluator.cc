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

int32_t ASTEvaluator::visit(const BinaryOperator &binop) {
    *ostream << '(';
    binop.get_left().accept(*this);
    *ostream << operator_name[binop.op];
    binop.get_right().accept(*this);
    *ostream << ')';
    return result;
}

int32_t ASTEvaluator::visit(const StringLiteral &literal) {
  *ostream << '"';
  for (auto &c : static_cast<std::string>(literal.value)) {
    switch (c) {
    case '"':
      *ostream << "\\\"";
      break;
    case '\\':
      *ostream << "\\\\";
      break;
    case '\a':
      *ostream << "\\a";
      break;
    case '\b':
      *ostream << "\\b";
      break;
    case '\t':
      *ostream << "\\t";
      break;
    case '\n':
      *ostream << "\\n";
      break;
    case '\v':
      *ostream << "\\v";
      break;
    case '\f':
      *ostream << "\\f";
      break;
    case '\r':
      *ostream << "\\r";
      break;
    default:
      *ostream << c;
    }
  }
  *ostream << '"';
}

int32_t ASTEvaluator::visit(const Let &let) {
  *ostream << "let";
  inc();
  for (auto decl : let.get_decls()) {
    nl();
    decl->accept(*this);
  }
  dnl();
  *ostream << "in";
  inc();
  const auto exprs = let.get_sequence().get_exprs();
  for (auto expr = exprs.cbegin(); expr != exprs.cend(); expr++) {
    if (expr != exprs.cbegin())
      *ostream << ';';
    nl();
    (*expr)->accept(*this);
  }
  dnl();
  *ostream << "end";
  return result;
}

int32_t ASTEvaluator::visit(const Identifier &id) {
  *ostream << id.name;
  if (verbose)
    if (auto decl = id.get_decl()) {
      *ostream << "/*" << "decl:" << decl.get().loc;

      if (int depth_diff = id.get_depth() - decl->get_depth())
        *ostream << " depth_diff:" << depth_diff;
      *ostream << "*/";
    }
    return result;
}

int32_t ASTEvaluator::visit(const IfThenElse &ite) {
  *ostream << "if ";
  inl();
  ite.get_condition().accept(*this);
  dnl();
  *ostream << " then ";
  inl();
  ite.get_then_part().accept(*this);
  dnl();
  *ostream << " else ";
  inl();
  ite.get_else_part().accept(*this);
  dec();
    return result;
}

int32_t ASTEvaluator::visit(const VarDecl &decl) {
  if (decl.get_expr())
    *ostream << "var ";
  *ostream << decl.name;
  if (verbose && decl.get_escapes())
    *ostream << "/*e*/";
  if (decl.type_name)
    *ostream << ": " << *decl.type_name;
  else {
    auto t = decl.get_type();
    if (t != t_undef && t != t_void)
      *ostream << ": " << get_type_name(t);
  }
  if (auto expr = decl.get_expr()) {
    *ostream << " := ";
    expr->accept(*this);
  }
    return result;
  
}

int32_t ASTEvaluator::visit(const FunDecl &decl) {
  *ostream << "function " << decl.name;
  if (verbose && decl.name != decl.get_external_name())
    *ostream << "/*" << decl.get_external_name() << "*/";
  *ostream << '(';
  auto params = decl.get_params();
  for (auto param = params.cbegin(); param != params.cend(); param++) {
    if (param != params.cbegin())
      *ostream << ", ";
    (*param)->accept(*this);
  }
  *ostream << ")";
  if (decl.type_name)
    *ostream << ": " << decl.type_name.get();
  *ostream << " = ";
  inl();
  decl.get_expr()->accept(*this);
  dec();
    return result;

}

int32_t ASTEvaluator::visit(const FunCall &call) {
  *ostream << call.func_name;
  if (verbose)
    if (auto decl = call.get_decl())
      *ostream << "/*" << "decl:" << decl.get().loc << "*/";

  *ostream << "(";

  auto args = call.get_args();
  for (auto arg = args.cbegin(); arg != args.cend(); arg++) {
    if (arg != args.cbegin())
      *ostream << ", ";
    (*arg)->accept(*this);
  }
  *ostream << ')';
    return result;
  
}

int32_t ASTEvaluator::visit(const WhileLoop &loop) {
  *ostream << "while ";
  loop.get_condition().accept(*this);
  *ostream << " do";
  inl();
  loop.get_body().accept(*this);
  dec();
      return result;

}

int32_t ASTEvaluator::visit(const ForLoop &loop) {
  *ostream << "for " << loop.get_variable().name;
  if (verbose && loop.get_variable().get_escapes())
    *ostream << "/*e*/";
  *ostream << " := ";
  loop.get_variable().get_expr()->accept(*this);
  *ostream << " to ";
  loop.get_high().accept(*this);
  *ostream << " do";
  inl();
  loop.get_body().accept(*this);
  dec();
      return result;


}

int32_t ASTEvaluator::visit(const Break &brk) {
  *ostream << "break";
  if (verbose && brk.get_loop())
    *ostream << "/*loop:" << brk.get_loop().get().loc << "*/";
     return result;


}

int32_t ASTEvaluator::visit(const Assign &assign) {
  assign.get_lhs().accept(*this);
  *ostream << " := ";
  assign.get_rhs().accept(*this);
    return result;
  
}

} // namespace ast
