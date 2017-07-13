#include <sstream>
#include "Nodes.h"

Node::Node()
{ 
  operator_ = Operator::Node;
}

Node::~Node()
{ ; }

Operator Node::getOperatorType() const
{
  return operator_;
}

UnaryNode::UnaryNode (Node_ptr child)
  : child_(child)
{ 
  operator_ = Operator::UnaryNode; 
}

UnaryNode::~UnaryNode()
{ ; }

string UnaryNode::getString() const
{
  return getStringRepresentation();
}

UnaryMinus::UnaryMinus(Node_ptr child)
  : UnaryNode(child)
{ 
  operator_ = Operator::UnaryMinus; 
}

UnaryMinus::~UnaryMinus()
{ ; }

Node_ptr UnaryMinus::diff(char symbol) const
{
  return std::make_shared<UnaryMinus>(child_->diff(symbol));
}

Node_ptr UnaryMinus::simplify()
{
  if (child_->getOperatorType() == Operator::Constant) {
    return std::make_shared<Constant>(-1*std::dynamic_pointer_cast<Constant>(child_)->getValue());
  }
  else {
    return shared_from_this();
  }
}

string UnaryMinus::getStringRepresentation() const
{
  std::stringstream ss;
  ss << "(-" << child_->getString() << ")";
  return ss.str();
}

Logarithm::Logarithm(Node_ptr child)
  : UnaryNode(child)
{ 
  operator_ = Operator::Logarithm; 
}

Logarithm::~Logarithm()
{ ; }

Node_ptr Logarithm::diff(char symbol) const
{
  return std::make_shared<Multiplication>(
            std::make_shared<Power>(
                child_,
                std::make_shared<Constant>(-1)
            ),
            child_->diff(symbol)
          );
}

Node_ptr Logarithm::simplify()
{
  child_ = child_->simplify();
  if (child_->getOperatorType() == Operator::Constant 
      && std::dynamic_pointer_cast<EulersNumber>(child_)->getValue() == EulersNumber().getValue()) {
    return std::make_shared<Constant>(1);
  }
  else if ( child_->getOperatorType() == Operator::Constant 
      && std::dynamic_pointer_cast<Constant>(child_)->getValue() == 1){
    return std::make_shared<Constant>(0);
  }
  else {
    return shared_from_this();
  }
}

string Logarithm::getStringRepresentation() const
{
  std::stringstream ss;
  ss << "ln(" << child_->getString() << ")";
  return ss.str();
}

Leaf::Leaf()
{ 
  operator_ = Operator::Leaf; 
}
Leaf::~Leaf()
{ ; }

string Leaf::getString() const
{
  return this->getStringRepresentation();
}

Symbol::Symbol (char sym)
  : sym_(sym)
{ 
  operator_ = Operator::Symbol; 
}

Symbol::~Symbol()
{ ; }

Node_ptr Symbol::simplify()
{
  return shared_from_this();
}

Node_ptr Symbol::diff (char symbol) const
{
  if (symbol != this->sym_) {
    return std::make_shared<Constant>(0);
  }
  else {
    return std::make_shared<Constant>(1);
  }
}

string Symbol::getStringRepresentation() const
{
  return string(1,sym_);
}

Constant::Constant (double value)
  : value_(value)
{
  operator_ = Operator::Constant;
}

Constant::~Constant()
{ ; }

Node_ptr Constant::simplify()
{
  return shared_from_this();
}

string Constant::getStringRepresentation() const
{
  std::stringstream ss;
  ss << value_;
  return ss.str();
}

Node_ptr Constant::diff (char symbol) const
{
  return std::make_shared<Constant>(0);
}

double Constant::getValue() const
{
  return value_;
}

EulersNumber::EulersNumber()
  : Constant(2.71)
{
  operator_ = Operator::Constant;
}

EulersNumber::~EulersNumber()
{ ; }

Node_ptr EulersNumber::simplify()
{
  return std::enable_shared_from_this<EulersNumber>::shared_from_this();
}

string EulersNumber::getStringRepresentation() const
{
  return "e";
}

BinaryNode::BinaryNode(Node_ptr left_child, Node_ptr right_child)
  : left_child_(left_child),
    right_child_(right_child)
{
  operator_ = Operator::BinaryNode; 
}

BinaryNode::~BinaryNode()
{ ; }

string BinaryNode::getString() const
{
  return getStringRepresentation();
}

Power::Power(Node_ptr left_child, Node_ptr right_child)
  : BinaryNode(left_child, right_child)
{
  operator_ = Operator::Power; 
}

Power::~Power()
{ ; }

Node_ptr Power::diff(char symbol) const
{
  return 
    std::make_shared<Multiplication>(
      std::make_shared<Power>(
        left_child_,
        std::make_shared<Addition>(
          right_child_,
          std::make_shared<UnaryMinus>(
            std::make_shared<Constant>(1)
            )
          )
        ),
      std::make_shared<Addition>(
        std::make_shared<Multiplication>(
          right_child_,
          left_child_->diff(symbol)
          ),
        std::make_shared<Multiplication>(
          std::make_shared<Multiplication>(
            left_child_,
            right_child_->diff(symbol)
            ),
          std::make_shared<Logarithm>(
            left_child_
            )
          )
        )
   );
}

Node_ptr Power::simplify()
{
  left_child_ = left_child_->simplify();
  right_child_ = right_child_->simplify();

  if (left_child_->getOperatorType() == Operator::Constant
    && std::dynamic_pointer_cast<Constant>(left_child_)->getValue() == 0) {
        return std::make_shared<Constant>(0);
  }
  else if (right_child_->getOperatorType() == Operator::Constant
    && std::dynamic_pointer_cast<Constant>(right_child_)->getValue() == 0) {
      return std::make_shared<Constant>(1);
  }
  else if (right_child_->getOperatorType() == Operator::Constant
    && std::dynamic_pointer_cast<Constant>(right_child_)->getValue() == 1) {
    return left_child_;
  }
  else {
    return shared_from_this();
  }
}

string Power::getStringRepresentation() const
{
  std::stringstream ss;
  ss << "(" << left_child_->getString() << ")^(" << right_child_->getString() << ")";
  return ss.str();
}

Multiplication::Multiplication(Node_ptr left_child, Node_ptr right_child)
  : BinaryNode(left_child, right_child)
{
  operator_ = Operator::Multiplication; 
}

Multiplication::~Multiplication()
{ ; }

Node_ptr Multiplication::diff(char symbol) const
{
  return std::make_shared<Addition>(
            std::make_shared<Multiplication>(
              left_child_->diff(symbol), 
              right_child_
            ),
            std::make_shared<Multiplication>(
              left_child_, 
              right_child_->diff(symbol)
            )
         );
}

Node_ptr Multiplication::simplify()
{
  left_child_ = left_child_->simplify();
  right_child_ = right_child_->simplify();

  if (left_child_->getOperatorType() == Operator::Constant 
      && std::dynamic_pointer_cast<Constant>(left_child_)->getValue() == 0) {
    return std::make_shared<Constant>(0);
  }
  else if (right_child_->getOperatorType() == Operator::Constant 
      && std::dynamic_pointer_cast<Constant>(right_child_)->getValue() == 0) {
    return std::make_shared<Constant>(0);
  }
  else if (left_child_->getOperatorType() == Operator::Constant 
      && std::dynamic_pointer_cast<Constant>(left_child_)->getValue() == 1) {
    return right_child_;
  }
  else if (right_child_->getOperatorType() == Operator::Constant 
      && std::dynamic_pointer_cast<Constant>(right_child_)->getValue() == 1) {
    return left_child_;
  }
  else {
    return shared_from_this();
  }
}

string Multiplication::getStringRepresentation() const
{
  std::stringstream ss;
  ss << left_child_->getString() << "*"
     << right_child_->getString();
  return ss.str();
}

Addition::Addition(Node_ptr left_child, Node_ptr right_child)
  : BinaryNode(left_child, right_child)
{
  operator_ = Operator::Addition; 
}

Addition::~Addition()
{ ; }

Node_ptr Addition::diff(char symbol) const
{
  return std::make_shared<Addition>(
           left_child_->diff(symbol),
           right_child_->diff(symbol)
         );
}

Node_ptr Addition::simplify()
{
  left_child_ = left_child_->simplify();
  right_child_ = right_child_->simplify();

  if ( left_child_->getOperatorType() == Operator::Constant 
      && std::dynamic_pointer_cast<Constant>(left_child_)->getValue() == 0) {
    return right_child_;
  }
  else if ( right_child_->getOperatorType() == Operator::Constant 
      && std::dynamic_pointer_cast<Constant>(right_child_)->getValue() == 0) {
    return left_child_;
  }
  else if (left_child_->getOperatorType() == Operator::Constant
      && right_child_->getOperatorType() == Operator::Constant) {
    return std::make_shared<Constant>(
        std::dynamic_pointer_cast<Constant>(left_child_)->getValue()
        + std::dynamic_pointer_cast<Constant>(right_child_)->getValue()
        );
  }
  else {
    return shared_from_this();
  }
}

string Addition::getStringRepresentation() const
{
  std::stringstream ss;
  ss << left_child_->getString() << "+" << right_child_->getString();
  return ss.str();
}
