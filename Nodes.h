#ifndef NODE_H
#define NODE_H

#include <memory>

enum class Operator {
  Node,
  Leaf,
  Constant,
  EulersNumber,
  Symbol,
  UnaryNode,
  UnaryMinus,
  Logarithm,
  BinaryNode,
  Addition,
  Multiplication,
  Power
};

class Node;
using Node_ptr = std::shared_ptr<Node>;
using std::string;

class Node
{
public:
  Node();
  virtual ~Node();
  virtual Node_ptr diff(char symbol) const = 0;
  virtual string getString() const = 0;
  virtual Node_ptr simplify() = 0;
  Operator getOperatorType() const;
protected:
  virtual string getStringRepresentation() const = 0;
  Operator operator_;
};

class UnaryNode : public Node
{
public:
  UnaryNode (Node_ptr child);
  virtual ~UnaryNode ();
  string getString() const;

protected:
  Node_ptr child_;
};

class Logarithm : public UnaryNode, public std::enable_shared_from_this<Logarithm>
{
public:
  Logarithm (Node_ptr child);
  virtual ~Logarithm ();
  Node_ptr diff(char symbol) const;
  Node_ptr simplify();
private:
  string getStringRepresentation() const;
};

class UnaryMinus : public UnaryNode, public std::enable_shared_from_this<UnaryMinus>
{
public:
  UnaryMinus (Node_ptr child);
  virtual ~UnaryMinus ();
  Node_ptr diff(char symbol) const;
  Node_ptr simplify();
private:
  string getStringRepresentation() const;
};

class Leaf : public Node {
public:
  Leaf ();
  virtual ~Leaf ();
  string getString() const;
};

class Symbol : public Leaf, public std::enable_shared_from_this<Symbol> {
public:
  Symbol (char sym);
  virtual ~Symbol();
  char getChar() const;
  Node_ptr diff(char symbol) const;
  Node_ptr simplify();
private:
  char sym_;
  string getStringRepresentation() const;
};

class Constant : public Leaf, public std::enable_shared_from_this<Constant> {
public:
  Constant (double value);
  virtual ~Constant();
  double getValue() const;
  Node_ptr diff(char symbol) const;
  virtual Node_ptr simplify();
protected:
  double value_;
  virtual string getStringRepresentation() const;
};


class EulersNumber : public Constant, public std::enable_shared_from_this<EulersNumber> 
{
public:
  EulersNumber ();
  virtual ~EulersNumber();
  Node_ptr simplify();
private:
  string getStringRepresentation() const;
};


class BinaryNode : public Node {
public:
  BinaryNode (Node_ptr left_child, Node_ptr right_child);
  virtual ~BinaryNode ();
  string getString() const;
protected:
  Node_ptr left_child_;
  Node_ptr right_child_;
};

class Power : public BinaryNode, public std::enable_shared_from_this<Power> {
public:
  Power (Node_ptr left_child, Node_ptr right_child);
  virtual ~Power ();
  Node_ptr diff(char symbol) const;
  Node_ptr simplify();
protected:
  string getStringRepresentation() const;
};

class Multiplication : public BinaryNode, public std::enable_shared_from_this<Multiplication>
{
public:
  Multiplication (Node_ptr left_child, Node_ptr right_child);
  virtual ~Multiplication ();
  Node_ptr diff(char symbol) const;
  Node_ptr simplify();
protected:
  string getStringRepresentation() const;
};

class Addition : public BinaryNode, public std::enable_shared_from_this<Addition>
{
public:
  Addition (Node_ptr left_child, Node_ptr right_child);
  virtual ~Addition ();
  Node_ptr diff(char symbol) const;
  Node_ptr simplify();
protected:
  string getStringRepresentation() const;
};

#endif /* NODE_H */
