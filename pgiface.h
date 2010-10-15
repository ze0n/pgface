#include <libpq-fe.h>
#include <string.h>
#include <stdio.h>

//----- DEFINES -------------------

#define MAX_CONN_STRING_LEN 1024
#define MAX_ERR_STRING_LEN 512

#define MAX_QUERY_STRING_LEN 2048

#define ALWAYS_CHECK_CONN 1

#define ERR_STATUS_STR_LEN 64
#define MAXSTRLEN   256

#define CONFIG_FILE "./pgface.conf"

// MACRO
#define del(x) \
if(x) \
   delete x; \
 x = NULL;

#define delar(x) \
if(x) \
   delete []x; \
 x = NULL;
 

//==============================================================================

class PG_options
{
protected:

public:
   	char	* pghost,		// DN
                * pghostaddr,	        // IP
		* pgport,		// port
		* pgoptions,	        // options
		* pgtty,		// debug term
		* dbname,		// DBname
		* user,			// username
		* password,		// password
                * connect_timeout,      // timeout
                * requiressl;           // ssl

         char * path;
         
        int do_reload_options();
        
        
	PG_options();
	~PG_options();
        
        char * get_str();
        char * get_db_name();

};

//==============================================================================
class PG_table
{
protected:
      
      PGresult * data;
      

public:
   PG_table(PGresult *dat);
   ~PG_table();
   
   char * get_col_name_by_index(int field_index);
   int get_col_index_by_name(char * field_name);
   
   char * getcell(int i,int j);
   
   bool isnull(int i,int j);
   Oid get_field_type(int index);
   int get_mod_data(int field_index);
   int get_field_size(int field_index);
   int get_field_length(int i,int j);

   int rowcount;
   int colcount;
};

//==============================================================================

class PG_conn
{
protected:
	PGconn * conn;
	PGresult * res;
        PG_options * pg_options;
        
        bool is_connected_f;

                           
	char    error[MAX_ERR_STRING_LEN+1];
        
        void    set_error(char *);
        
public:
	PG_conn();
	virtual ~PG_conn();
        
        PGresult * pop_res();
        char * get_connection_string();
        char * get_db_name();
        
        char * get_error();
        bool is_connected();
        int do_reload_options();
        
        // start & end of session
	int Connect();	                // performing connection
	int Close();                    // closing connection
	int Reset();                    // re setting connection
        
	// operations for performing transactions
	int ExBegin();
	int ExCommit();
	int ExRollback();

        int Ex(char *);

};

