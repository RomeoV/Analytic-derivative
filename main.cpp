#include <iostream>
#include <iomanip>
#include "Nodes.h"

int main(int argc, char *argv[])
{

  Node_ptr expression(
      std::make_shared<Power>(
        std::make_shared<Symbol>('x'),
        std::make_shared<Symbol>('x')
        )
      );

  /*
   * Node_ptr expression(
   *     std::make_shared<Logarithm>(
   *       std::make_shared<Power>(
   *         std::make_shared<Symbol>('x'),
   *         std::make_shared<Constant>(2)
   *         )
   *       )
   *     );
   */

   /*
    * Node_ptr expression(
    *     std::make_shared<Addition>(
    *       std::make_shared<Multiplication>(
    *         std::make_shared<Constant>(5),
    *         std::make_shared<Symbol>('x')
    *       ),
    *       std::make_shared<EulersNumber>()
    *     )
    * );
    */
   

  /*
   * Node_ptr expression(
   *       std::make_shared<Multiplication>(
   *         std::make_shared<Symbol>('x'),
   *         std::make_shared<Constant>(2)
   *       )
   *     );
   */

  Node_ptr diff = expression->diff('x');

  std::cout << "Expression:" << std::endl
            << expression->getString() 
            << std::endl << std::endl;

  std::cout << "Derivative:" << std::endl
            << diff->getString() 
            << std::endl << std::endl;;

  std::cout << "Derivative simplified:" << std::endl
            << diff->simplify()->getString()
            << std::endl;
  
  return 0;
}
