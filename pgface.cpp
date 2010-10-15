#include "pgface.h"
//#include <stdio.h>

PG_face::PG_face()
{
   pg_conn = new PG_conn();
}

PG_face::~PG_face()
{
   delete pg_conn;
}

int PG_face::do_reload_options()
{
   return(pg_conn->do_reload_options());
}

bool PG_face::is_connected()
{
   return(pg_conn->is_connected());
}


int PG_face::do_connect()
{
   return(this->pg_conn->Connect());
}

int PG_face::do_disconnect()
{
   return(pg_conn->Close());
}


// Select
PG_table * PG_face::ExS(char * str)
{
   PG_table * ret;
   pg_conn->ExBegin();
   
   if(pg_conn->Ex(str)!=0)
      {
         pg_conn->ExRollback();
         ret = NULL;
      }
   else
      {
         ret = new PG_table(pg_conn->pop_res());
         pg_conn->ExCommit();
      }
   return(ret);
}

// Cmd
int PG_face::ExC(char * str)
{
   int ret;
   pg_conn->ExBegin();
   
   if(pg_conn->Ex(str)!=0)
      {
         pg_conn->ExRollback();
         ret = -1;
      }
   else
      {
         ret = 0;
	 pg_conn->ExCommit();
      }
   return(ret);
}

char * PG_face::get_connection_string()
{
   return (pg_conn->get_connection_string());
}
char * PG_face::get_db_name()
{
   return (pg_conn->get_db_name());
}

char * PG_face::get_error()
{
   return(pg_conn->get_error());
}
