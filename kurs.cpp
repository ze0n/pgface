#include "kurs.h"

int main(int argc, char *argv[])
{
   bool verbose = false;
   
   if(argc > 1)
   {
      if(!strncmp(argv[1],"-v",2))
	 verbose = true;
      else
      if(!strncmp(argv[1],"--help",6) ||!strncmp(argv[1],"-h",2)||!strncmp(argv[1],"-?",2) )
      {
	 printf("\n descr:\t Ncurses based interface for editing PGSQL DBs.\n usage:\t pgsql [-v]\n flags:\t -v\tVerbose mode - shows all the sql-queries\n\n author:\t Knyazkov Konstantin  ASTU 2007\n\t\t ze0n@inbox.ru\n");
	 return(0);
      }
   }
   
	NC_face * nc_face = NULL;  // ncurses interface

	nc_face = new NC_face(verbose);
	nc_face->initialize();
        
        
	nc_face->close();
	delete nc_face;

	return 0;
}

