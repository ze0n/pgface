#include "pgiface.h"




class PG_face
{
protected:
	
	PG_conn * pg_conn;	// db connection

public:
      
   int do_connect();
   int do_disconnect();
   int do_add_server();
   int do_sql();
   
   int do_reload_options();
      
   bool is_connected();
   
	PG_face();
	~PG_face();
   
   char * get_connection_string();
   char * get_db_name();
   
   char * get_error();
   
   PG_table * ExS(char * a);
   int ExC(char * a);
};

