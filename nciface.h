#include <cdk/cdk.h>

#include <stdio.h>
#include "pgface.h"

// defines
#define MAXSTRLEN   256


// colors
#define CPBG 		1
#define CPMSG		2
#define CPMENU		3
#define CPSTATUS	4
#define CPGREEN         5
#define CPPATH          6

// message types
#define MTERROR		1	// ok
#define MTWARNING	2	// ok cancel
#define MTINFO		3	// ok
#define MTCONF		4	// Y N

// buttons
#define BTNYES		0
#define BTNOK		0
#define BTNNO		1
#define BTNCANCEL	2

// navigations
#define NVDATABASES     0
#define NVSCHEMAS       1
#define NVTABLES        2
#define NVOBJECTS       3

// data/append
#define DEDIT		0
#define DAPPEND		1

// messge width
#define MESSAGE_WIDTH 	50

// number of tabs ~ TAB_NUM*8
#define TAB_NUM		5


class NC_table
{

   char 		*title;
   
   PG_table * data;
   
   
   CDKSCREEN *cdkscreen;
   WINDOW *cursesWin;
   
public:
   CDKMATRIX 		* matrix;
   int cols;
   int rows;
   int vrows;
   int vcols;
   char **coltitle, **rowtitle;
   int *colwidth, *colvalue;    	
  
   EDisplayType * cols_type;

   NC_table(CDKSCREEN *scr,PG_table * dt,char * titl ); // pg_face for refresh
   ~NC_table();
   int Create(int yoffset);
   char * getcell(int i,int j);
   
   int get_cur_pos();
   void set_cur_pos(int pos);
};


class NC_face
{
protected:
// service
void rebind_esc (CDKOBJS *obj);


	WINDOW		*cursesWin; // curses

	CDKLABEL	* lbstatus;
        char		** status_text; 

	CDKLABEL	* lbpath;
        char		** path_text; 

	CDKMENU* menu;	// menu
	
        
	char * menulist[MAX_MENU_ITEMS][MAX_SUB_ITEMS]; 
	

public:
   CDKSCREEN	*cdkscreen;	// screen
   
//   NC_face();
   NC_face(bool verbose);

    ~NC_face();

   int 	initialize();
   void	close();
	
   void	set_status(char *);
   void	set_path();

   int		show_message(const char * title,const char * message, int msg_type);
	
   void hide_objects();

   bool verbose;
   
// list
   CDKSCROLL*        list;
   char **list_items;

   
   // edit
 CDKENTRY ** entries;
 CDKBUTTON ** buttons;
 int nentries,nbuttons;
 int edit_append_flag;
 
  // DATA
  PG_face *         pg_face;
  NC_table	   * mx_data;
  PG_table	   * pg_table_data;

  int mx_pos;
  
   //SQL
   CDKMENTRY	* textbox;
   NC_table	* mxtb;
   PG_table	* pg_table;
   CDKLABEL	* sqlstatus;
   char		** sql_status_text; 
   CDKBUTTON    * sql_button_perform;
   CDKBUTTON    * sql_button_close;


      int navigation;
      
      char db_name[256];
      char schema_name[256];
      char table_name[256];
      char object_name[256];
         
   
   
   void erase_all();
   void show_all();
   
   //------ actions ------
      
      int do_add_server();
      int do_options();
      int do_exit();

      int do_create();
      int do_delete();
      int do_properties();
      
      int do_help();
      
      int do_connect();
      int do_disconnect();
      int do_sql();
      int do_data();
      
      int do_data_edit();
      int do_data_append();
      int do_data_delete();
      
      int do_navigate(int type);
      
      int do_update();
      int do_insert();
      
      void refresh(NC_table * table);

      //----------------------
      int choose_items_count;
      char **choose_items;            // list_choose + navigation
      
      
      void clear_strings(char *** strs,int * count);
      void add_string(char ** strs, int count,int index,char * str);
      void alloc_strings(char *** strs,int * count1,int count);
};


