#include "pgiface.h"
//===========================================================================================

//===========================================================================================
PG_options::PG_options()
{
   pghost = NULL;	
   pghostaddr = NULL;
   pgport = NULL;
   pgoptions = NULL;
   pgtty = NULL;
   dbname = NULL;
   user = NULL;	
   password = NULL;
   connect_timeout = NULL;
   requiressl = NULL;
   
   path = NULL;
}
//---------------------------------------------------------------
PG_options::~PG_options()
{
   delar(pghost);
   delar(pghostaddr);
   delar(pgport);
   delar(pgoptions);
   delar(pgtty);
   delar(dbname);
   delar(user);
   delar(password);
   delar(connect_timeout);
   delar(requiressl);
   delar(path);
}
int PG_options::do_reload_options()
{
      FILE * fin;
      char buf[MAXSTRLEN+1];
      int len = 0;
      
      fin = fopen(this->path,"rt");
      if(fin)
      {
         
        while(!feof(fin))
	{
	 fgets(buf,MAXSTRLEN,fin);
        
	 if(buf[0]=='#')
	 {
	 }
	 else
	 if(!strncmp("hostaddr = ",buf,strlen("hostaddr = ")))
	 {
	    len = strlen("hostaddr = ");
	    if(!pghostaddr)
	       pghostaddr = new char[MAXSTRLEN+1];
	    strcpy(pghostaddr,buf+len);
	    
	    pghostaddr[strlen(pghostaddr)-1]='\0';
	 }
 	 else
	 if(!strncmp("host = ",buf,strlen("host = ")))
	 {
	    len = strlen("host = ");
	    if(!pghost)
	       pghost = new char[MAXSTRLEN+1];
	    strcpy(pghost,buf+len);

	    pghost[strlen(pghost)-1]='\0';
	 }
	 else
	 if(!strncmp("port = ",buf,strlen("port = ")))
	 {
	    len = strlen("port = ");
	    if(!pgport)
	       pgport = new char[MAXSTRLEN+1];
	    strcpy(pgport,buf+len);

   	    pgport[strlen(pgport)-1]='\0';
	 }
	 else
	 if(!strncmp("dbname = ",buf,strlen("dbname = ")))
	 {
	    len = strlen("dbname = ");
	    if(!dbname)
	       dbname = new char[MAXSTRLEN+1];
	    strcpy(dbname,buf+len);
	    
    	    dbname[strlen(dbname)-1]='\0';
	 }
	 else
	 if(!strncmp("user = ",buf,strlen("user = ")))
	 {
	    len = strlen("user = ");
	    if(!user)
	       user = new char[MAXSTRLEN+1];
	    strcpy(user,buf+len);
	    
    	    user[strlen(user)-1]='\0';
	 }
	 else
	 if(!strncmp("password = ",buf,strlen("password = ")))
	 {
	    len = strlen("password = ");
	    if(!password)
	       password = new char[MAXSTRLEN+1];
	    strcpy(password,buf+len);
	    
    	    password[strlen(password)-1]='\0';
	 }
        }
	 
        fclose(fin);
      }
      else
         return(-1);
   
   if(pghost || pghostaddr)
      return(0);    
   else
      return(-1);
      
}
char * PG_options::get_str()
{
   char * connection_string = new char[MAX_CONN_STRING_LEN+1];
   connection_string[0]='\0';
   
   if(this->pghostaddr)
   {
      strncat(connection_string," hostaddr = ",MAX_CONN_STRING_LEN-strlen(connection_string)); 
      strncat(connection_string,pghostaddr,MAX_CONN_STRING_LEN-strlen(connection_string)); 
   }
   else if(this->pghost)
   {
      strncat(connection_string," host = ",MAX_CONN_STRING_LEN-strlen(connection_string)); 
      strncat(connection_string,pghost,MAX_CONN_STRING_LEN-strlen(connection_string)); 
   }
   else 
   {
      delete [] connection_string;
      return(NULL);
   }
   
   if(this->pgport)
   {
      strncat(connection_string," port = ",MAX_CONN_STRING_LEN-strlen(connection_string)); 
      strncat(connection_string,pgport,MAX_CONN_STRING_LEN-strlen(connection_string)); 
   }
   
   if(this->dbname)
   {
      strncat(connection_string," dbname = ",MAX_CONN_STRING_LEN-strlen(connection_string)); 
      strncat(connection_string,dbname,MAX_CONN_STRING_LEN-strlen(connection_string)); 
   }

   if(this->user)
   {
      strncat(connection_string," user = ",MAX_CONN_STRING_LEN-strlen(connection_string)); 
      strncat(connection_string,user,MAX_CONN_STRING_LEN-strlen(connection_string)); 
   }

   if(this->password)
   {
      strncat(connection_string," password = ",MAX_CONN_STRING_LEN-strlen(connection_string)); 
      strncat(connection_string,password,MAX_CONN_STRING_LEN-strlen(connection_string)); 
   }

   if(this->connect_timeout)
   {
      strncat(connection_string," connect_timeout = ",MAX_CONN_STRING_LEN-strlen(connection_string)); 
      strncat(connection_string,connect_timeout,MAX_CONN_STRING_LEN-strlen(connection_string)); 
   }

   if(this->pgtty)
   {
      strncat(connection_string," tty = ",MAX_CONN_STRING_LEN-strlen(connection_string)); 
      strncat(connection_string,pgtty,MAX_CONN_STRING_LEN-strlen(connection_string)); 
   }
   
   if(this->pgoptions)
   {
      strncat(connection_string," options = ",MAX_CONN_STRING_LEN-strlen(connection_string)); 
      strncat(connection_string,pgoptions,MAX_CONN_STRING_LEN-strlen(connection_string)); 
   }
   
   if(this->requiressl)
   {
      strncat(connection_string," requiressl = ",MAX_CONN_STRING_LEN-strlen(connection_string)); 
      strncat(connection_string,requiressl,MAX_CONN_STRING_LEN-strlen(connection_string)); 
   }

   return(connection_string);
}

char * PG_options::get_db_name()
{
   char * db_name = new char[MAX_CONN_STRING_LEN+1];
   db_name[0]='\0';
   
   strncat(db_name,dbname,MAX_CONN_STRING_LEN); 
   return(db_name);
}
//===========================================================================================
PG_table::PG_table(PGresult * dat)
{
   data = dat;
   
   rowcount = PQntuples(data);
   colcount = PQnfields(data);
   
}
//-----------------------------------------------------
PG_table::~PG_table()
{
   if(data)
      PQclear(data);

}
//-----------------------------------------------------
int PG_table::get_col_index_by_name(char * field_name)
{
   return(PQfnumber(data,field_name));
}
//-----------------------------------------------------
char * PG_table::get_col_name_by_index(int field_index)
{
   return(PQfname(data,field_index));
}
//-----------------------------------------------------
char * PG_table::getcell(int i,int j)
{
   char *a =PQgetvalue(data,i,j);
   if(a)
      return(a);
   else
      return("");
}
//-----------------------------------------------------
bool PG_table::isnull(int i,int j)
{
   return(PQgetisnull(data,i,j));
}
//-----------------------------------------------------
Oid PG_table::get_field_type(int index)
{
   return(PQftype(data,index));
}
//-----------------------------------------------------
int PG_table::get_mod_data(int field_index)
{
   return(PQfmod(data,field_index));
}
//-----------------------------------------------------
int PG_table::get_field_size(int field_index)
{
   return(PQfsize(data,field_index));
}
//-----------------------------------------------------
int PG_table::get_field_length(int i,int j)
{
   return(PQgetlength(data,i,j));
}
//-----------------------------------------------------

//===========================================================================================
PG_conn::PG_conn()
{
   is_connected_f = false;
   conn = NULL;
   res = NULL;
   
   
   pg_options = new PG_options();
   
   pg_options->path = new char[strlen(CONFIG_FILE)+10];
   strcpy(pg_options->path,CONFIG_FILE);
   
   pg_options->do_reload_options();

}

PG_conn::~PG_conn()
{

   this->Close();
   delete pg_options;
   
}
int PG_conn::do_reload_options()
{
   this->pg_options->do_reload_options();
}

//------------------------------------------------------
int PG_conn::Connect()
{
   
//    !!!!!! WARNING work with strings !!!!
   char * connection_string = pg_options->get_str();
   if(!connection_string)
      return(-1);
      
   conn = PQconnectdb(connection_string);
   
   if (PQstatus(conn) != CONNECTION_OK)
   {
       set_error(PQerrorMessage(conn));
       return(-1);
   }
   
   delar(connection_string);

   return(0);      
}
//----------------------------------------------------------------------------------
int PG_conn::Close()
{
   if(res)
      PQclear(res);
   res = NULL;

   if(conn)
      PQfinish(conn);
   conn = NULL;
   
   return(0);
}
//----------------------------------------------------------------------------------
int PG_conn::Reset()
{
   if(conn)
      PQreset(conn);
   return(0);
}
//----------------------------------------------------------------------------------
// command
int PG_conn::Ex(char * str)
{

#ifdef ALWAYS_CHECK_CONN
   if(PQstatus(conn) != CONNECTION_OK)
   {
       set_error(PQerrorMessage(conn));
       return(-1);
   }
#endif
   
   if(res)
      PQclear(res);
   
//   PQescapeString(buf,str,length);
      
   res = PQexec(conn, str);
   if(!res || (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK))
   {
       set_error(PQresultErrorMessage(res));
       return(-1);
   }
   
//   PQclear(res);
//  res = NULL;
   
   return(0);
}
//----------------------------------------------------------------------------------
// start of transaction
int PG_conn::ExBegin()
{
   return(Ex("BEGIN"));
}
//----------------------------------------------------------------------------------
// commiting transaction
int PG_conn::ExCommit()
{
   return(Ex("COMMIT"));
}
//----------------------------------------------------------------------------------
// rolling back transaction
int PG_conn::ExRollback()
{
   return(Ex("ROLLBACK"));
}
//----------------------------------------------------------------------------------
void PG_conn::set_error(char * a)
{
   if(a)
   {
      strncpy(error,a,MAX_ERR_STRING_LEN);
      error[MAX_ERR_STRING_LEN] = '\0';
   }
}
//----------------------------------------------------------------------------------
bool PG_conn::is_connected()
{
   if(PQstatus(conn) != CONNECTION_OK)
   {
       //set_error(PQerrorMessage(conn));
       return(false);
   }
   return(true);
}
//-----------------------------------------------------
PGresult * PG_conn::pop_res()
{
   PGresult * ret = res;
   res = NULL;
   return(ret);
}
//-----------------------------------------------------
char * PG_conn::get_connection_string()
{
   return(pg_options->get_str());
}
//-----------------------------------------------------
char * PG_conn::get_db_name()
{
   return(pg_options->get_db_name());
}
//-----------------------------------------------------
char * PG_conn::get_error()
{
   return(strdup(error));
}



