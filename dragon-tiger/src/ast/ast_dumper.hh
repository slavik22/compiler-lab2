#ifndef AST_DUMPER_HH
#define AST_DUMPER_HH

#include <ostream>

#include "nodes.hh"

namespace ast {

class ASTDumper : public ConstASTVisitor {
  std::ostream *ostream;
  bool verbose;
  unsigned indent_level = 0;
  void inc() { indent_level++; }
  void inl() {
    inc();
    nl();
  };
  void dec() { indent_level--; }
  void dnl() {
    dec();
    nl();
  };

public:
  ASTDumper(std::ostream *_ostream, bool _verbose)
      : ostream(_ostream), verbose(_verbose) {}
  void nl() {
    *ostream << std::endl;
    for (unsigned i = 0; i < indent_level; i++)
      *ostream << "  ";
  };
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
};

} // namespace ast

#endif // _AST_DUMPER_HH
