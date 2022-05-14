#include "exerciser.h"

void exercise(connection *C)
{

   query1(C, 1, 10, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2);

   query2(C, "Green");
   
   query3(C, "Duke");
   
   query4(C, "FL", "Green");
   
   query5(C, 13);
  
}
