#include "nciface.h"

int displayPostCallback(EObjectType cdktype, /*GCC_UNUSED,*/ void *object, void *clientData, chtype input /*,GCC_UNUSED*/);
int displayPreCallback(EObjectType cdktype, /*GCC_UNUSED,*/ void *object, void *clientData, chtype input /*,GCC_UNUSED*/);
int list_choose (EObjectType cdktype,void *object,void *clientData,chtype key);
int sql_perform (EObjectType cdktype,void *object,void *clientData,chtype key);
int data_action (EObjectType cdktype,void *object,void *clientData,chtype key);

int _action_button_confirm(EObjectType cdktype,void *object,void *clientData,chtype key);
int _action_button_cancel(EObjectType cdktype,void *object,void *clientData,chtype key);

int _action_sql_button_perform(EObjectType cdktype,void *object,void *clientData,chtype key);
int _action_sql_button_close(EObjectType cdktype,void *object,void *clientData,chtype key);

int _action_up(EObjectType cdktype,void *object,void *clientData,chtype key);

void action_btn_confirm (SButton *object);
void action_btn_cancel (SButton *object);

NC_face::NC_face(bool verb)
{
	this->status_text = new char * [1];
	this->status_text[0] = new char[MAXSTRLEN+1];
        status_text[0][0] = 0;
        
	this->path_text = new char * [1];
	this->path_text[0] = new char[MAXSTRLEN+1];
        path_text[0][0] = 0;

        db_name[0] = '\0';
        schema_name[0] = '\0';
        table_name[0] = '\0';
        object_name[0] = '\0';
        
        list_items = NULL;
        choose_items = NULL;
        choose_items_count = 0;
	
	textbox = NULL;
	
	mxtb = NULL;
	mx_data = NULL;
	pg_table = NULL;
	pg_table_data = NULL;
        pg_face = NULL;
        navigation = -1;
        
        list = 0;
	
	verbose = verb;
}


NC_face::~NC_face()
{
}

void NC_face::rebind_esc (CDKOBJS *obj)
{
   bindCDKObject (ObjTypeOf (obj), obj, KEY_F (1), getcCDKBind, (void *)KEY_ESC);
}
//----------------------------------------------------------------------------------------------
int NC_face::initialize()
{
   /* Set up CDK. */
   cursesWin = initscr();
   
//   SQLWin = newwin (LINES-4, COLS-10, 2, 5); 
//   SQLscreen = initCDKScreen (SQLWin);
   
   cdkscreen = initCDKScreen (cursesWin);

// init color pairs

   /* Start CDK Colors. */
   initCDKColor();


   init_pair(0,COLOR_WHITE,COLOR_BLACK);
   init_pair(CPBG,COLOR_YELLOW,COLOR_BLACK);
   init_pair(CPMSG,COLOR_WHITE,COLOR_BLUE);
   init_pair(CPMENU,COLOR_YELLOW,COLOR_BLACK);
//   init_pair(CPSTATUS,COLOR_WHITE,COLOR_BLUE);
   init_pair(CPGREEN,COLOR_GREEN,COLOR_BLACK);
   init_pair(CPPATH,COLOR_CYAN,COLOR_BLACK);
   
   
   
   
  //--------------------------------------------------------
   list = 0;
   mx_data = 0;
   textbox = 0;
   mxtb =0;
   sqlstatus = 0;
   sql_button_close = NULL;
   sql_button_perform = NULL;
   pg_table_data = NULL;
   sql_button_close = NULL;
   sql_button_perform = NULL;
   
   mx_pos = 0;

   entries = NULL;
   buttons = NULL;
   nentries = nbuttons = 0;
   edit_append_flag = DEDIT;
 
   pg_face = NULL;

   pg_table = NULL;
   sqlstatus = NULL;
   sql_status_text = NULL; 

   choose_items = NULL;
   choose_items_count = 0;
  //--------------------------------------------------------

   
   
   
   
   
   /*STATUS*/
   
   sprintf(status_text[0],"                                                                                                    ");

   lbstatus = newCDKLabel (cdkscreen, LEFT,BOTTOM, status_text, 1, false, false);
   
   if (lbstatus == 0)
   {
	   return(-1);
   }

   setCDKLabelBackgroundColor (lbstatus, "</16>");

   /*END STATUS*/
   
   /*PATH*/
   
   sprintf(path_text[0],"                                                                                                    ");
   
   lbpath = newCDKLabel (cdkscreen, LEFT,1, path_text, 1, false, false);

   if (lbpath == 0)
   {
	   return(-1);
   }

   setCDKLabelBackgroundColor (lbpath, "</6>");

   /*END PATH*/
   

   /* MENU */
   int submenusize[3], menuloc[3];

   /* Set up the menu. */
   
//   menulist[0][0] = "</B>File<!B>" ;          menulist[1][0] = "</B>Edit<!B>";          menulist[2][0] = "</B>Tools<!B>";
//   menulist[0][1] = "</B>Add Server...<!B>" ; menulist[1][1] = "</B>Create...<!B> ";    menulist[2][1] = "</B>Connect <!B>";
//   menulist[0][2] = "</B>Options...<!B>" ;    menulist[1][2] = "</B>Delete/Drop<!B>";   menulist[2][2] = "</B>Disconnect <!B>";
//   menulist[0][3] = "</B>Exit<!B>" ;          menulist[1][3] = "</B>Properties...<!B>"; menulist[2][3] = "</B>Qery tool<!B>";
//   menulist[0][4] = "" ;                      menulist[1][4] = "";                      menulist[2][4] = "</B>Data tool<!B>";
   
   menulist[0][0] = "</B>File<!B>" ;          	menulist[1][0] = "</B>Tools<!B>";		menulist[2][0] = "</B>Help<!B>";
   menulist[0][1] = "</B>Options...<!B>" ;	menulist[1][1] = "</B>Connect <!B>";		menulist[2][1] = "</B>Help<!B>";
   menulist[0][2] = "</B>Exit<!B>" ;    	menulist[1][2] = "</B>Disconnect <!B>";		menulist[2][2] = "";
   menulist[0][3] = "" ;          		menulist[1][3] = "</B>Qery tool<!B>";		menulist[2][3] = "";
   menulist[0][4] = "" ;                      	menulist[1][4] = "";				menulist[2][4] = "";

   submenusize[0] = 3;	menuloc[0] = LEFT;
   submenusize[1] = 4;	menuloc[1] = LEFT;
   submenusize[2] = 2;	menuloc[2] = RIGHT;

   /* Create the menu. */
   menu = newCDKMenu (cdkscreen, menulist, 3, submenusize, menuloc, TOP, A_UNDERLINE|A_BOLD|COLOR_PAIR(CPMENU), A_REVERSE);

   if (menu == 0)
   {
	   return(-1);
   }
   rebind_esc (ObjOf (menu));

   /* Create the post process function. */
   setCDKMenuPostProcess (menu, displayPostCallback, (void*)this);

   /* Create the pre process function. */
   setCDKMenuPreProcess (menu, displayPreCallback, (void *)this);

   setCDKMenuBackgroundColor(menu,"</3>");
   
   
   //--------------------------------------------------------
   // Creating objects
   this->set_status("Starting PG engine...");
   refreshCDKScreen (cdkscreen);
   this->pg_face = new PG_face();
   this->set_status("Starting PG engine... OK");
   refreshCDKScreen (cdkscreen);
   //--------------------------------------------------------
   // Loading options
   this->set_status("Loading options...");
   
   refreshCDKScreen (cdkscreen);
   
   db_name[0] = '\0';
   schema_name[0] = '\0';
   table_name[0] = '\0';
   object_name[0] = '\0';
   
   if(this->pg_face->do_reload_options()==0)
   {
      char * dn;
      this->set_status("Loading options... OK");
      dn = pg_face->get_db_name();
      strcpy(db_name,dn);
      delar(dn);
   }
   else
   {
      show_message("Error while loading options","Try to check \"pgface.conf\" file..",MTINFO);
      this->set_status("Loading options... Failed");
      this->close();
   }
   // Navigate
   set_path();
   
   this->do_navigate(NVSCHEMAS);

   //--------------------------------------------------------

   refreshCDKScreen (cdkscreen);

   traverseCDKScreen(cdkscreen);

   return(0);
}
//----------------------------------------------------------------------------------------------
void NC_face::hide_objects()
{
      //SQL
	if(textbox)
	 unregisterCDKObject(ObjTypeOf (textbox),textbox);
	if(mxtb)
	 unregisterCDKObject(ObjTypeOf (mxtb->matrix),mxtb->matrix);
      
      //LIST
        if(list)
	 unregisterCDKObject(ObjTypeOf (list),list);
	 
	 refreshCDKScreen(cdkscreen);
   
}
//----------------------------------------------------------------------------------------------
void NC_face::close()
{
        clear_strings(&choose_items,&choose_items_count);
 
        exitCancelCDKScreen(cdkscreen);
	 
	del(mxtb);
	del(mx_data);
        del(pg_table_data);
	
        // PG		// Disconnect
        del(pg_face);

	// MENU
	destroyCDKMenu (menu);

	// STATUS
	destroyCDKLabel(lbstatus);

	// PATH
	destroyCDKLabel(lbpath);

	//other objects
        if(this->list)
        {
//*****
// check
//*****
	   CDKfreeStrings(list_items);
           destroyCDKScroll(list);
        }
         
	 
      int i;
      for(i=0;i<nbuttons;i++)
      {
	 
	 if(buttons[i])
	    destroyCDKButton (buttons[i]);
	 buttons[i] = NULL;
      }
      
      for(i=0;i<nentries;i++)
      {
	 if(entries[i])
	 destroyCDKEntry (entries[i]);
	 entries[i] = NULL;
      }

      delar(buttons);
      delar(entries);

      nbuttons=0;
      nentries=0;
      
      if(textbox)
	 destroyCDKMentry(textbox);
      textbox = NULL;
 
      if(sql_button_perform) destroyCDKButton(sql_button_perform);
      if(sql_button_close) destroyCDKButton(sql_button_close);
      sql_button_close = NULL;
      sql_button_perform = NULL;
	 
	/* Clean up. */
	destroyCDKScreen (cdkscreen);
	endCDK();
	exit(0);
}
//----------------------------------------------------------------------------------------------
void string_check(char * tmp)
{
    int 			str_len;
//   убираем \n
      str_len = strlen(tmp);
	for(int j=0;j<str_len;j++)
	 if(tmp[j]=='\n')
	  tmp[j]=' ';
}
//----------------------------------------------------------------------------------------------
int NC_face::show_message(const char * title,const char * message, int msg_type)
{
	CDKDIALOG		*dialog;
        char			**mesg; 
        char			**buttons;
	int			length = 30;
	int			nbuttons =0 ;
	int 			selection=0;
        int 			string_number=0;
        int 			string_number1=0;
        char 			tmp[MESSAGE_WIDTH+1];
	char 			*error_message;
   
	strncpy(tmp,title,MESSAGE_WIDTH);
	string_check(tmp);
	
	length = strlen(message);
        
        string_number = 1 + length/MESSAGE_WIDTH;
	
	if(msg_type==MTERROR)
	{
	   error_message = pg_face->get_error();
	   
	   string_number1 = 1 + strlen(error_message)/MESSAGE_WIDTH;
	}
	 
      
	if(msg_type!=MTERROR)
	{
	   mesg = new char*[1+string_number];
	   for(int i=0;i<string_number+1;i++)
	       mesg[i] = new char[MAXSTRLEN+1];
        }
        else
	{
	   mesg = new char*[1+string_number+string_number1+1];
	   for(int i=0;i<1+string_number+string_number1+1;i++)
	       mesg[i] = new char[MAXSTRLEN+1];
	}
      
	 
	switch(msg_type)
	{
	case MTERROR:
	case MTINFO:
		nbuttons = 1;
		buttons = new char*[1];
		buttons[0]	= "</B>Ok";
		
		sprintf(mesg[0],"%s%s","</B></2>",tmp);
		
	        for(int i=1;i<string_number+1;i++)
		{
		   strncpy(tmp,message+(i-1)*MESSAGE_WIDTH,MESSAGE_WIDTH);
		   tmp[MESSAGE_WIDTH]='\0';
		   string_check(tmp);
		   sprintf(mesg[i],"%s",tmp);
	        }
		
	       if(msg_type==MTERROR)
	       {
      		sprintf(mesg[string_number+1],"</U>Error message:");

	        for(int i=string_number+2;i<string_number+1+string_number1+1;i++)
		{
		   strncpy(tmp,error_message+(i-string_number-2)*MESSAGE_WIDTH,MESSAGE_WIDTH);
		   tmp[MESSAGE_WIDTH]='\0';
		   string_check(tmp);
		   sprintf(mesg[i],"%s",tmp);
	        }
	       }	
		break;
         case MTCONF:
                 nbuttons = 2;
		 buttons = new char*[2];
                 buttons[0]     ="</B>Yes";
                 buttons[1]     ="</B>No";
		
	         sprintf(mesg[0],"</B>%s%s","</2> ",tmp);
		
	         for(int i=1;i<string_number+1;i++)
	         {
		   strncpy(tmp,message+(i-1)*MESSAGE_WIDTH,MESSAGE_WIDTH);
		   tmp[MESSAGE_WIDTH]='\0';
		   string_check(tmp);
		   sprintf(mesg[i],"%s",tmp);
	         }

		break;

	}

   if(msg_type!=MTERROR)
   {

   /* Create the dialog box. */
   dialog = newCDKDialog (cdkscreen,
			    CENTER,
			    CENTER,
			    mesg, 1+string_number, buttons, nbuttons,
			    COLOR_PAIR(CPMSG)|A_REVERSE|A_BOLD,
			    true,
			    true,
			    false);
   }
   else
   {
      dialog = newCDKDialog (cdkscreen,
			    CENTER,
			    CENTER,
			    mesg, 1+string_number+1+string_number1, buttons, nbuttons,
			    COLOR_PAIR(CPMSG)|A_REVERSE|A_BOLD,
			    true,
			    true,
			    false);
   
   }
   if (dialog == 0)
   {
		return(-1);	
   }
   
   setCDKDialogBackgroundColor(dialog,"</2>");

   /* Activate the dialog box. */
   selection = activateCDKDialog (dialog, 0);

   //this->rebind_esc((CDKOBJS*)dialog);
   
   /* Tell them what was selected. */
   if (dialog->exitType == vESCAPE_HIT)
   {
		selection = BTNCANCEL;
   }
   else if (dialog->exitType == vNORMAL)
   {
		
   }

   /* Clean up. */
   destroyCDKDialog (dialog);
   
   refreshCDKScreen (cdkscreen);
	
   delar(buttons);	
	
   for(int i=0;i<string_number+1;i++)
	delete[] mesg[i];
   delete[] mesg;

   return(selection);
}

void NC_face::set_status(char * str)
{

	// log writing


	strncpy(status_text[0],str,MAXSTRLEN);
	status_text[0][MAXSTRLEN]=0;
	
	setCDKLabel(lbstatus,status_text,1,false);

}
void NC_face::set_path()
{
      path_text[0][0]='/';
      path_text[0][1]='\0';
        
      if(pg_face)
      {

        if(pg_face->is_connected())
	if(db_name[0])
        {
           strncat(path_text[0],this->db_name,MAXSTRLEN);
           strcat(path_text[0],"/");
        
           if(schema_name[0])
           {
              strncat(path_text[0],this->schema_name,MAXSTRLEN-strlen(path_text[0]));
              strcat(path_text[0],"/");
            }
	    
           if(table_name[0])
           {
              strncat(path_text[0],this->table_name,MAXSTRLEN-strlen(path_text[0]));
              strcat(path_text[0],"/");
           }

	    if(object_name[0])
           {
              strncat(path_text[0],this->object_name,MAXSTRLEN-strlen(path_text[0]));
              strcat(path_text[0],"/");
           }

	   strncat(path_text[0],"                         ",MAXSTRLEN-strlen(path_text[0]));
	   	   
         }
        
        status_text[0][MAXSTRLEN]=0;
     }
	
	if(lbpath)
	 setCDKLabel(lbpath,path_text,1,false);
     
}

//------------------------------------
int NC_face::do_add_server()
{
      return(0);
}
//------------------------------------
int NC_face::do_help()
{
      this->show_message("Hotkeys","\"e\" - Edit; \"a\" - Append; \"x\" - Close; \"d\" - Delete;",MTINFO);
      return(0);
}
//------------------------------------
int NC_face::do_options()
{
      char * buf;
//      char * buf1 = new char[40+1];
      
      buf = pg_face->get_connection_string();
      
//      strncpy(buf1,buf,40);
      
//      if(strlen(buf)>40)
//      show_message(buf1,buf+40,MTINFO);
//      else
      show_message("Options:",buf,MTINFO);
      

//      delete []buf1;
      delete []buf;
      return(0);
}
//------------------------------------
int NC_face::do_exit()
{
      if(show_message("Confirm exit","Exit?",MTCONF)==BTNYES)
      {
         this->close();         
      }
      return(0);
}
//------------------------------------
int NC_face::do_create()
{
      return(0);
}
//------------------------------------
int NC_face::do_delete()
{
      return(0);
}
//------------------------------------
int NC_face::do_properties()
{
      return(0);
}
//------------------------------------
int NC_face::do_connect()
{
      if(pg_face->is_connected())
      {
         show_message("Error","It's already connected.",MTINFO);         
      }
      else
      {
         set_status("Connecting...");
            if(!this->pg_face->do_connect())
              {
                 set_status("Connecting...  OK");
                 char *tmp = pg_face->get_db_name();
                 strncpy(db_name,tmp,MAX_CONN_STRING_LEN);
                 delete []tmp;
                 
                 set_path();
              }
              else
	      {
		    set_status("Connecting...  Failed");
		    show_message("Error","Connection failed.",MTERROR);
		    return -1;
	      }
      }
      
      return(0);
}
//------------------------------------
int NC_face::do_disconnect()
{
      if(!pg_face->is_connected())
      {
         show_message("Error","It's already disconnected.",MTINFO);
      }
      else
      {
            this->pg_face->do_disconnect();
            
/////////   CLEAR   /////////////////
	    del(mx_data);
	    del(mxtb);
	    del(pg_table_data);
	    del(pg_table);

//  коммент НЕ трогать!
//	    schema_name[0] = '\0';
//	    table_name[0] = '\0';
//	    object_name[0] = '\0';
	 
	    clear_strings(&choose_items,&choose_items_count);
	    choose_items = NULL;
	    choose_items_count = 0;
	    
	    if(list)		
	    {
	       destroyCDKScroll(list);
	       CDKfreeStrings(list_items);
	    }
	    list = NULL;
	    if(textbox)		destroyCDKMentry(textbox);
	    textbox = NULL;
	    if(sqlstatus)	destroyCDKLabel(sqlstatus);
	    sqlstatus = NULL;
	    
	    if(sql_button_perform) destroyCDKButton(sql_button_perform);
	    if(sql_button_close) destroyCDKButton(sql_button_close);
	    sql_button_close = NULL;
	    sql_button_perform = NULL;
			
		  
	    if(nbuttons)
	    {
	       for(int i=0;i<nbuttons;i++)
	       {
		  destroyCDKButton (buttons[i]);
		  buttons[i] = NULL;
	       }
                 
	       for(int i=0;i<nentries;i++)
	       {
		  destroyCDKEntry (entries[i]);
		  entries[i] = NULL;
	       }
	       delar(buttons);
	       delar(entries);
      
	       nbuttons=0;
	       nentries=0;
	    }
////////////////////////////////////		    
	    set_path();
	    set_status("DISCONNECTED");
	    do_navigate(NVDATABASES);
      }
      return(0);
}
//------------------------------------
int NC_face::do_sql()
{
  char * buf=NULL;
  
  erase_all();
  
  textbox = newCDKMentry(cdkscreen,LEFT,2,"","SQL: ", A_BOLD, '_', vMIXED,80,5,19, 0,TRUE,FALSE); 
   if (textbox==0)
   {
      show_message("Error","Couldn't create textbox",MTINFO);
      return(-1);
   }

   bindCDKObject (vMENTRY, textbox, 10, sql_perform, this);
   
   sql_button_perform = newCDKButton(
                      cdkscreen,
                      5,
                      9,
                      "Perform",
                      action_btn_confirm,
                      TRUE,
                      FALSE);
   
   bindCDKObject (vBUTTON, sql_button_perform, 10, _action_sql_button_perform, this);
   bindCDKObject (vBUTTON, sql_button_perform, ' ', _action_sql_button_perform, this);
   
   sql_button_close = newCDKButton(
                      cdkscreen,
                      25,
                      9,
                      "Close",
                      action_btn_cancel,
                      TRUE,
                      FALSE);
   
   bindCDKObject (vBUTTON, sql_button_close, 10, _action_sql_button_close, this);
   bindCDKObject (vBUTTON, sql_button_close, ' ', _action_sql_button_close, this);

   
   
   setCDKFocusCurrent(cdkscreen,(CDKOBJS*)textbox);
   
   refreshCDKScreen (cdkscreen);

//   activateCDKMentry (textbox, 0);

//   sql_perform (vMENTRY,(void *)textbox,(void *)this,NULL);

//   destroyCDKMentry(textbox); 
//   textbox = 0;
   
//   if(mxtb)
      delete mxtb;
   
   delar(buf);
   
  return(0);
}
//------------------------------------
int NC_face::do_update()
{
      int i;
      char buf[MAX_QUERY_STRING_LEN+1],temp[MAX_QUERY_STRING_LEN+1];
      char tmp[MAX_QUERY_STRING_LEN+1];
      
      if(schema_name)
	 sprintf(buf,"UPDATE %s.%s SET ",schema_name,table_name);
      else
	 sprintf(buf,"UPDATE %s SET ",table_name);
      
      
      for(i=0;i<mx_data->cols;i++,strncat(buf,",",MAX_QUERY_STRING_LEN-strlen(buf)))
      {
	 sprintf(tmp,"%s",entries[i]->info);

	 if(tmp[0] == '\0')
	 {
	    sprintf(tmp,"NULL");
	    sprintf(temp,"\"%s\" = %s",mx_data->coltitle[i+1],tmp);
	 }
	 else
	    sprintf(temp,"\"%s\" = '%s'",mx_data->coltitle[i+1],tmp);
	 strncat(buf,temp,MAX_QUERY_STRING_LEN-strlen(buf));
      }
      buf[strlen(buf)-1]='\0';
      
      strncat(buf,"\n WHERE ",MAX_QUERY_STRING_LEN-strlen(buf));

      // ЛУЧШЕ БЫ ПРЕВИЧНЫЙ КЛЮЧ
      
      for(i=0;i<mx_data->cols;i++,strncat(buf," and ",MAX_QUERY_STRING_LEN-strlen(buf)))
      {      
	 sprintf(tmp,"%s",mx_data->getcell(getCDKMatrixRow(mx_data->matrix)-1,i));

	 if(tmp[0] == '\0')
	 {
	    sprintf(tmp,"true");	// true вместо NULL чтоб не нарушать логики
	    sprintf(temp,"%s",tmp);
	 }
	 else
	    sprintf(temp,"\"%s\" = '%s'",mx_data->coltitle[i+1],tmp);

	 strncat(buf,temp,MAX_QUERY_STRING_LEN-strlen(buf));
      }
      buf[strlen(buf)-4]='\0';
      
      strncat(buf,";",MAX_QUERY_STRING_LEN-strlen(buf));
   
      if(verbose)
	 show_message("Update string",buf,MTINFO);
   
      set_status("Table updating...");
      if(pg_face->ExC(buf))
      {
	 set_status("Table updating... Failed");
	 
	 show_message("Error","While updating table",MTERROR);
	 
	 return(1);
      }
      set_status("Table updating... OK");
   return(0);
}
//------------------------------------
int NC_face::do_insert()
{
      int i;
      char buf[MAX_QUERY_STRING_LEN+1],temp[MAX_QUERY_STRING_LEN+1];
      char tmp[MAX_QUERY_STRING_LEN+1];
      
      if(schema_name)
	 sprintf(buf,"INSERT INTO %s.%s (",schema_name,table_name);
      else
	 sprintf(buf,"INSERT INTO %s (",table_name);
      
      
      for(i=0;i<mx_data->cols;i++,strncat(buf,",",MAX_QUERY_STRING_LEN-strlen(buf)))
      {   
      	 sprintf(temp," \"%s\" ",mx_data->coltitle[i+1]);
	 strncat(buf,temp,MAX_QUERY_STRING_LEN-strlen(buf));
      }
      buf[strlen(buf)-1]='\0';
      
      strncat(buf,") values (",MAX_QUERY_STRING_LEN-strlen(buf));
      
      for(i=0;i<mx_data->cols;i++,strncat(buf,",",MAX_QUERY_STRING_LEN-strlen(buf)))
      {   
 	 sprintf(tmp,"%s",entries[i]->info);

	 if(tmp[0] == '\0')
	 {
	    sprintf(tmp,"NULL");
	    sprintf(temp," %s ",tmp);
	 }
	 else
	    sprintf(temp," '%s' ",tmp);
	 strncat(buf,temp,MAX_QUERY_STRING_LEN-strlen(buf));
      }
      buf[strlen(buf)-1]='\0';
      strncat(buf,");",MAX_QUERY_STRING_LEN-strlen(buf));
      
      if(verbose)
	 show_message("Insert string",buf,MTINFO);
      
      set_status("Table inserting...");
      if(pg_face->ExC(buf))
      {
	 set_status("Table inserting... Failed");
	 
	 show_message("Error","While inserting in table",MTERROR);
	 
	 return(1);
      }
      set_status("Table inserting... OK");
   return(0);
}
//------------------------------------
int NC_face::do_data()
{
   char * buf;
  
   buf = new char [MAX_QUERY_STRING_LEN+1];
   
   if(!schema_name)
      sprintf(buf,"SELECT * FROM %s;",this->table_name);
   else
      sprintf(buf,"SELECT * FROM %s.%s;",schema_name,this->table_name);
   
   del(pg_table_data);

   set_status("Performing query...");
   
      pg_table_data = pg_face->ExS(buf);
   
      if(!pg_table_data)
      {
      //++++++++++++++++++ ERROR MESSAGE
      set_status("Query...  Failed");
	 show_message("Error","Query failed",MTERROR);
	 
	 delar(buf);
	 del(pg_table_data);
	 
	 mx_pos = 0;
	 
	 return(0);
      }
      
   set_status("Performing query...  OK");
   
   del(mx_data);
   
   mx_data = new NC_table(cdkscreen,pg_table_data,table_name);
   int a = mx_data->Create(0);
   
   mx_data->set_cur_pos(mx_pos);
   
   bindCDKObject (vMATRIX,mx_data->matrix,'a', data_action, this);
   bindCDKObject (vMATRIX,mx_data->matrix,'i', data_action, this);
   
   bindCDKObject (vMATRIX,mx_data->matrix,'d', data_action, this);
   bindCDKObject (vMATRIX,mx_data->matrix,'e', data_action, this);
   bindCDKObject (vMATRIX,mx_data->matrix,'x', data_action, this);
   
   bindCDKObject (vMATRIX,mx_data->matrix,KEY_ESC, data_action, this);
   
   
   refreshCDKScreen(cdkscreen);

   //del(pg_table_data);
   pg_table_data = NULL;
   delar(buf);
   mx_pos = 0;
   
  return(1);
}
//------------------------------------
void NC_face::erase_all()
{
  if(list)		unregisterCDKObject(vSCROLL,list);
  if(mx_data)		unregisterCDKObject(vMATRIX,mx_data->matrix);
  if(textbox)		unregisterCDKObject(vMENTRY,textbox);
  if(mxtb)		unregisterCDKObject(vMATRIX,mxtb->matrix);
  if(sqlstatus)		unregisterCDKObject(vLABEL,sqlstatus);
}
void NC_face::show_all()
{
  if(list)		registerCDKObject(cdkscreen,vSCROLL,list);
  if(mx_data)		registerCDKObject(cdkscreen,vMATRIX,mx_data->matrix);
  if(textbox)		registerCDKObject(cdkscreen,vMENTRY,textbox);
  if(mxtb)		registerCDKObject(cdkscreen,vMATRIX,mxtb->matrix);
  if(sqlstatus)		registerCDKObject(cdkscreen,vLABEL,sqlstatus);
}
//------------------------------------
int NC_face::do_data_append()
{
   EDisplayType * type;
   int i;
   char temp[MAXSTRLEN];
   
   edit_append_flag = DAPPEND;
   
   erase_all();
   
   nentries = mx_data->cols;
   entries = new CDKENTRY*[mx_data->cols];
   nbuttons = 2;
   buttons = new CDKBUTTON*[nbuttons];
   
   for(i=0;i<mx_data->cols;i++)
   {   
      
      sprintf(temp,"%s: ",mx_data->coltitle[i+1]);
      
      entries[i] = newCDKEntry (cdkscreen,
                5,
                5+i,
                "", temp, A_NORMAL, '_', vMIXED,
                40, 0, MAXSTRLEN,
                FALSE,
                FALSE);

      if (entries[i] == 0)
      {
	 show_message("Error","Couldn't create entrybox",MTINFO);
	 // все удалить
	 
	 
	 show_all();
	 
	 return(-1);
      }	
      bindCDKObject (vENTRY, entries[i], 10, _action_button_confirm, this);
      bindCDKObject (vENTRY, entries[i], KEY_ESC, _action_button_cancel, this);

//      unbindCDKObject (vENTRY,entries[i],KEY_UP);
//      bindCDKObject (vENTRY, entries[i], KEY_UP, _action_up, this);
      
      

      //bindCDKObject (vENTRY, entries[i], CDK_NEXT, getcCDKBind, (void *)ACS_DARROW);
 
   }

/*   int maxx,maxy;
   getmaxyx(this->cdkscreen->window,maxy,maxx);   
                   '');
*/
   
   buttons[0] = newCDKButton(
                      cdkscreen,
                      5,
                      5+i+1,
                      "Confirm",
                      action_btn_confirm,
                      TRUE,
                      FALSE);
   bindCDKObject (vBUTTON, buttons[0], 10, _action_button_confirm, this);
   bindCDKObject (vBUTTON, buttons[0], ' ', _action_button_confirm, this);

   buttons[1] = newCDKButton(
                      cdkscreen,
                      24,
                      5+i+1,
                      "Cancel",
                      action_btn_cancel,
                      TRUE,
                      FALSE);
   bindCDKObject (vBUTTON, buttons[1], 10, _action_button_cancel, this);
   bindCDKObject (vBUTTON, buttons[1], ' ', _action_button_cancel, this);
   
/*
   for(i=0;i<mx_data->cols;i++)
   {   
      setCDKEntryValue (entries[i],mx_data->getcell(getCDKMatrixRow(mx_data->matrix)-1,i));
   }
*/
   refreshCDKScreen(cdkscreen);
//   show_all();   
}
//------------------------------------
int NC_face::do_data_edit()
{
   EDisplayType * type;
   int i;
   char temp[MAXSTRLEN];
   
   edit_append_flag = DEDIT;

   
   erase_all();
   
   nentries = mx_data->cols;
   entries = new CDKENTRY*[mx_data->cols];
   nbuttons = 2;
   buttons = new CDKBUTTON*[nbuttons];
   
   for(i=0;i<mx_data->cols;i++)
   {   
      
      sprintf(temp,"%s: ",mx_data->coltitle[i+1]);
      
      entries[i] = newCDKEntry (cdkscreen,
                5,
                5+i,
                "", temp, A_NORMAL, '_', vMIXED,
                40, 0, MAXSTRLEN,
                FALSE,
                FALSE);
		
      if (entries[i] == 0)
      {
	 show_message("Error","Couldn't create entrybox",MTINFO);
	 // все удалить
	 
	 
	 show_all();
	 
	 return(-1);
      }	
      bindCDKObject (vENTRY, entries[i], 10, _action_button_confirm, this);
      bindCDKObject (vENTRY, entries[i], KEY_ESC, _action_button_cancel, this);
      
   }

/*   int maxx,maxy;
   getmaxyx(this->cdkscreen->window,maxy,maxx);   
                   '');
*/
   
   buttons[0] = newCDKButton(
                      cdkscreen,
                      5,
                      5+i+1,
                      "Confirm",
                      action_btn_confirm,
                      TRUE,
                      FALSE);
   bindCDKObject (vBUTTON, buttons[0], 10, _action_button_confirm, this);
   bindCDKObject (vBUTTON, buttons[0], ' ', _action_button_confirm, this);

   buttons[1] = newCDKButton(
                      cdkscreen,
                      24,
                      5+i+1,
                      "Cancel",
                      action_btn_cancel,
                      TRUE,
                      FALSE);
   bindCDKObject (vBUTTON, buttons[1], 10, _action_button_cancel, this);
   bindCDKObject (vBUTTON, buttons[1], ' ', _action_button_cancel, this);
   

   for(i=0;i<mx_data->cols;i++)
   {   
      setCDKEntryValue (entries[i],mx_data->getcell(getCDKMatrixRow(mx_data->matrix)-1,i));
   }
   
   refreshCDKScreen(cdkscreen);
//   show_all();
}
//------------------------------------
int NC_face::do_data_delete()
{
      char tmp[MAX_QUERY_STRING_LEN+1];

      if(show_message("Confirm deleting","Delete record?",MTCONF)!=BTNYES)
      {
	 return 0;
      }
      else
      {	
	 int i;
	 char buf[MAX_QUERY_STRING_LEN+1],temp[MAX_QUERY_STRING_LEN+1];
	 
	 if(schema_name)
	    sprintf(buf,"DELETE FROM %s.%s ",schema_name,table_name);
	 else
	    sprintf(buf,"DELETE FROM %s ",table_name);
      
	 strncat(buf,"\n WHERE ",MAX_QUERY_STRING_LEN-strlen(buf));

	 // ЛУЧШЕ БЫ ПРЕВИЧНЫЙ КЛЮЧ
      
	 for(i=0;i<mx_data->cols;i++,strncat(buf," and ",MAX_QUERY_STRING_LEN-strlen(buf)))
	 {
	    sprintf(tmp,"%s",mx_data->getcell(getCDKMatrixRow(mx_data->matrix)-1,i));

	    if(tmp[0] == '\0')
	    {
	       sprintf(tmp,"true");
	       sprintf(temp,"%s",tmp);
	    }
	    else
	    {
	       sprintf(temp,"\"%s\" = '%s'",mx_data->coltitle[i+1],tmp);
	    }
	    strncat(buf,temp,MAX_QUERY_STRING_LEN-strlen(buf));
	 }
	 buf[strlen(buf)-4]='\0';
      
	 strncat(buf,";",MAX_QUERY_STRING_LEN-strlen(buf));
	 
	 if(verbose)
	    show_message("Deleting string",buf,MTINFO);
	 
	 set_status("Record deleting...");
	 if(pg_face->ExC(buf))
	 {
	    set_status("Record deleting... Failed");
	    
	    show_message("Error","While deleting record",MTERROR);
	 
	    return(1);
	 }
	 set_status("Record deleting... OK");
	 
	 refresh(mx_data);

	 return(1);        
      }  
}
//------------------------------------

//-------- WARNING -------------- LOW LEVEL STRINGS ----------            
void NC_face::clear_strings(char *** strs,int * count)
{
   char ** str1 = *strs;
if((str1)&&(*count))
{
//   char temp[128];
//   sprintf(temp,"%p %d %p %p %p %d",str1,*count,count,strs,choose_items,choose_items_count);
//   show_message(temp,"",MTINFO);
         for(int i=0;i<(*count);i++)
            delete [](*strs)[i];
         delete [](*strs);
	 *strs = NULL;
	 *count = 0;
}  
}

//-------- WARNING -------------- LOW LEVEL STRINGS ----------            
void NC_face::add_string(char ** strs, int count,int index,char * str)
{
if(strs&&count)
{
   strs[index] = strdup(str);
}
}

void NC_face::alloc_strings(char *** strs,int * count1,int count)
{
   *strs = new char*[count];   
   *count1 = count;
}
//----------------------------------------------------------------
int NC_face::do_navigate(int type)
{
   char buf[MAX_QUERY_STRING_LEN+1];
   buf[0]=0;
   PG_table * table;
   
   // tabulation
   char * tabs;
   char * tmpstr;
   int tabnum=0;

   // проверка на наличие подключения
   if(type!=NVDATABASES)
   {
   if(!pg_face->is_connected())
   {
      if(show_message("It's not connected","Connect?",MTCONF)==BTNYES)
      {
         this->do_connect();
      }
      else
      {
	 type = NVDATABASES;
	 set_status("DISCONNECTED");
///	 return(-1);
      }
   }
   
   if(!pg_face->is_connected())
   {
      type = NVDATABASES;
      set_status("DISCONNECTED");
//      return(-1);
   }
   }

   switch(type)
   {
//--------------------------- CASE------------------------------------------------
      case NVDATABASES:
         set_status("DISCONNECTED");
         
         if(list)
	 {
	    destroyCDKScroll(list);
	    list = NULL;
	 }
	  
         CDKfreeStrings(list_items);
         
         //WARNING NEEDS HARD DEBUG 
         list = newCDKScroll(cdkscreen,LEFT,2,RIGHT,20,80,"<C></B/5>[ Databases ]\n",list_items,0,FALSE,A_REVERSE,true,false);
         
         if(list==0)
         {
            show_message("Error","Couldn't make listbox",MTINFO);
            return(-1);
	 }
         rebind_esc((CDKOBJS*)list);
         
         bindCDKObject (vSCROLL, list, 10, list_choose, this);
         
         navigation = NVDATABASES;

	 clear_strings(&choose_items,&choose_items_count);
	 alloc_strings(&choose_items,&choose_items_count,1);

	 add_string(choose_items,choose_items_count,0,this->db_name);    
         sprintf(buf," %s",db_name);
         addCDKScrollItem(list,buf);

	 setCDKFocusCurrent (cdkscreen, (CDKOBJS*)list);
                  
	 break;
	 
/**********************************************************************************
 ********      Здесь получение списка баз с сервера / в функционал пока не входит /	 
 **********************************************************************************
       case NVDATABASES:

	 sprintf(buf,"SELECT d.datname,r.rolname,pg_encoding_to_char(d.encoding) FROM pg_database d JOIN pg_roles r ON d.datdba = r.oid;");

*/         
//--------------------------- CASE------------------------------------------------
      case NVSCHEMAS:
         
         sprintf(buf,"SELECT n.nspname,r.rolname FROM pg_namespace n JOIN pg_roles r ON n.nspowner = r.oid;");
         
         set_status("Geting list of namespaces...");
         table = this->pg_face->ExS(buf);
         
         if(!table)
         {
            //++++++++++++++++++ ERROR MESSAGE
            set_status("Geting list of namespaces...  Failed");
            show_message("Error","Couldn't fetch list of namespaces",MTERROR);
            return(-1);
         }
         set_status("Geting list of namespaces...  OK");
         
         if(list)
	 {
	    destroyCDKScroll(list);
	    list = NULL;
	 }
         CDKfreeStrings(list_items);
         
         //WARNING NEEDS HARD DEBUG 
         list = newCDKScroll(cdkscreen,LEFT,2,RIGHT,20,80,"<C></B/5>[ Schemas ]\n",list_items,0,FALSE,A_REVERSE,true,false);
         
         if(list==0)
         {
            show_message("Error","Couldn't make listbox",MTINFO);
            return(-1);
         }
         rebind_esc((CDKOBJS*)list);
         
         bindCDKObject (vSCROLL, list, 10, list_choose, this);
         
         navigation = NVSCHEMAS;
	 
        schema_name[0] = '\0';
        table_name[0] = '\0';
        object_name[0] = '\0';
      
      clear_strings(&choose_items,&choose_items_count);
      alloc_strings(&choose_items,&choose_items_count,table->rowcount);
      
      add_string(choose_items,choose_items_count,0,"..");
      addCDKScrollItem(list," ..");
      
      
      for(int i=0;i<table->rowcount;i++)
         {
	    tmpstr = table->getcell(i,0);
	    add_string(choose_items,choose_items_count,i+1,tmpstr);
	    
	    tabnum = TAB_NUM - (strlen(tmpstr)/8);
	    tabs = new char[tabnum+1];
	    for(int j=0;j<tabnum;j++)
	       tabs[j] = '\t';
	    tabs[tabnum]='\0';
	    	    
            sprintf(buf," %.18s %s %18s",tmpstr,tabs,table->getcell(i,1));
            addCDKScrollItem(list,buf);
	    
	    delete []tabs;
         }

	 setCDKFocusCurrent (cdkscreen, (CDKOBJS*)list);

         delete table;
         
         break;
//--------------------------- CASE------------------------------------------------
      case NVTABLES:
         
         sprintf(buf,"SELECT c.relname,r.rolname \n"
			  "FROM pg_class c JOIN pg_roles r ON r.oid = c.relowner LEFT JOIN pg_namespace n ON n.oid=c.relnamespace\n"
			  "WHERE c.relkind = 'r' AND n.nspname = '%s';",schema_name);// NOT IN ('pg_catalog', 'pg_toast','information_schema');");

         set_status("Geting list of tables...");
         table = this->pg_face->ExS(buf);
         
         if(!table)
         {
            //++++++++++++++++++ ERROR MESSAGE
            set_status("Geting list of tables...  Failed");
            show_message("Error","Couldn't fetch list of tables",MTERROR);
            return(-1);
         }
	 
	 
         set_status("Geting list of tables...  OK");
	 
         if(list)
	 {
	    destroyCDKScroll(list);
	    list = NULL;
	 }
         CDKfreeStrings(list_items);
         
         //WARNING NEEDS HARD DEBUG 
         list = newCDKScroll(cdkscreen,LEFT,2,RIGHT,20,80,"<C></B/5>[ Tables ]\n",list_items,0,FALSE,A_REVERSE,true,false);
         
         if(list==0)
         {
            show_message("Error","Couldn't make listbox",MTINFO);
            return(-1);
         }
         rebind_esc((CDKOBJS*)list);
         
         bindCDKObject (vSCROLL, list, 10, list_choose, this);
         
         navigation = NVTABLES;
      
      clear_strings(&choose_items,&choose_items_count);
      alloc_strings(&choose_items,&choose_items_count,table->rowcount+1);
      
      add_string(choose_items,choose_items_count,0,"..");
      addCDKScrollItem(list," ..");
      
      for(int i=0;i<table->rowcount;i++)
         {
	    tmpstr = table->getcell(i,0);
	    add_string(choose_items,choose_items_count,i+1,tmpstr);
	    
	    tabnum = TAB_NUM - (strlen(tmpstr)/8);
	    tabs = new char[tabnum+1];
	    for(int j=0;j<tabnum;j++)
	       tabs[j] = '\t';
	    tabs[tabnum]='\0';
	    	    
            sprintf(buf," %.18s %s %18s",tmpstr,tabs,table->getcell(i,1));
            addCDKScrollItem(list,buf);
	    
	    delete []tabs;
         }
	 setCDKFocusCurrent (cdkscreen, (CDKOBJS*)list);
	 refreshCDKScreen(cdkscreen);
	 
         delete table;
         
         break;
//--------------------------- CASE------------------------------------------------
   }
   
   return(0);
}

//=========================================================
NC_table::NC_table(CDKSCREEN	*scr,PG_table * dat,char * titl)
{
   matrix			= 0;
   title			= 0;
   rows				= 0;
   cols				= 0;
   vrows			= 0;
   vcols			= 0;
   coltitle = rowtitle = NULL;
   colwidth = colvalue = 0; 
   
   cdkscreen = scr;
   data = dat;
   title = new char[MAXSTRLEN];
   sprintf(title,"%s",titl);
   
}

NC_table::~NC_table()
{
   
  if(matrix)
  {
     destroyCDKMatrix(matrix);
     matrix = NULL;
  }

  delar(colwidth);
  delar(colvalue);
   for(int i=1;i<cols+1;i++)
   {
      delar(coltitle[i]);
   }
   delar(coltitle);
   
   delar(rowtitle);
  delar(title);
  
  del(data);

}

int NC_table::Create(int yoffset)
{
   int maxx,maxy;
   
   if(yoffset>20||yoffset<0)
    yoffset = 0;
   
   vrows = 5; // how many rows will be shown
   vcols = 5;
   
   getmaxyx(this->cdkscreen->window,maxy,maxx);
   vcols = (maxx-8)/20;
   vrows = (maxy-8-yoffset)/2;
   
   rows = data->rowcount;
   cols = data->colcount;

   vrows = (vrows > rows ? rows+1 : vrows);
   vcols = (vcols > cols ? cols : vcols);
   
   
   // WARN
   
   rowtitle = new char *[rows+1+1];
   for(int i=1;i<rows+1+1;i++)
      rowtitle[i] = "";
   
   coltitle = new char*[cols+1];
   for(int i=1;i<cols+1;i++)
   {
      coltitle[i] = new char[256];
//      if(i%2)
//	 sprintf(coltitle[i],"</5>%s<!5>",data->get_col_name_by_index(i-1));  // ПРОВЕРИТЬ
//      else
	 sprintf(coltitle[i],"%s",data->get_col_name_by_index(i-1));  // ПРОВЕРИТЬ
   }
   
   
   colwidth = new int[cols+1];
   for(int i=1;i<cols+1;i++)
      colwidth[i] = 20;
   
   colvalue = new int[cols+1];
   for(int i=1;i<cols+1;i++)
      colvalue[i] = vVIEWONLY;
   
   char title1[MAXSTRLEN];
   sprintf(title1,"<C></B/5>%s<!5><!B>",title);

     
   matrix = newCDKMatrix  (cdkscreen,
			      0,
			      2+yoffset,
			      rows+1, cols, vrows, vcols,
			     // vrows, vcols,rows, cols,
			     title1, rowtitle, coltitle,
			      colwidth, colvalue,
			      -1, -1, ' ',
			      COL, TRUE,
			      TRUE,
			      FALSE); 
   if (matrix == 0)
   {
      return(-1);
   }
   
   //bindCDKObject (vMATRIX, matrix, KEY_F (1), getcCDKBind, (void *)KEY_ESC);
   
   
   for(int i=0;i<rows;i++)
    for(int j=0;j<cols;j++)
    {
       setCDKMatrixCell (matrix,i+1,j+1,data->getcell(i,j));
    }
    
 
   setCDKFocusCurrent (cdkscreen, (CDKOBJS*)matrix);
 

 //  activateCDKMatrix(matrix,0);

    return(0);
}

void NC_face::refresh(NC_table * table)
{
   mx_pos = mx_data->get_cur_pos();
   del(pg_table_data);
   do_data();
}

char * NC_table::getcell(int i,int j)
{
      return(data->getcell(i,j));
}

int NC_table::get_cur_pos()
{
   if(this->matrix)
      return(getCDKMatrixRow (matrix));
   return(0);
}

void NC_table::set_cur_pos(int pos)
{
   if(pos>=0)
   if(matrix)
   {
      if(pos < this->rows)
	 moveToCDKMatrixCell (matrix,pos,1);
      else
	 moveToCDKMatrixCell (matrix,this->rows-1,1);
   }
}
//=========================================================
// handlers
int _action_up(EObjectType cdktype,void *object,void *clientData,chtype key)
{
   NC_face * nc_face    = (NC_face *)clientData;
 
   setCDKFocusPrevious(nc_face->cdkscreen);
   refreshCDKScreen(nc_face->cdkscreen);
    
   return(1);
   
}

//SQL---------------------------
int _action_sql_button_perform(EObjectType cdktype,void *object,void *clientData,chtype key)
{
   CDKBUTTON * button = (CDKBUTTON *)object;
   NC_face * nc_face    = (NC_face *)clientData;
   
   return(sql_perform(cdktype,(void*)nc_face->textbox,clientData,key));
}
int _action_sql_button_close(EObjectType cdktype,void *object,void *clientData,chtype key)
{
   CDKBUTTON * button = (CDKBUTTON *)object;
   NC_face * nc_face    = (NC_face *)clientData;

  
  destroyCDKMentry(nc_face->textbox);
  nc_face->textbox = NULL;
 
  destroyCDKButton(nc_face->sql_button_perform);
  destroyCDKButton(nc_face->sql_button_close);
  nc_face->sql_button_perform = NULL;
  nc_face->sql_button_close = NULL;
  
  del(nc_face->mxtb);
   
  nc_face->show_all();
 
  setCDKFocusNext(nc_face->cdkscreen);

  refreshCDKScreen(nc_face->cdkscreen);
  
  return(1);
}
//---------------------------
int _action_button_confirm(EObjectType cdktype,void *object,void *clientData,chtype key)
{
   CDKBUTTON * button = (CDKBUTTON *)object;
   NC_face * nc_face    = (NC_face *)clientData;
   int i;
   
   if(nc_face->edit_append_flag==DEDIT)
   {
   //----------------
   if(!nc_face->do_update())
   {
      //nc_face->show_message("table info","UPDATED",MTINFO);
      
      //clear
      
      
      for(i=0;i<nc_face->nbuttons;i++)
      {
	 destroyCDKButton (nc_face->buttons[i]);
	 nc_face->buttons[i] = NULL;
      }
      
            
      for(i=0;i<nc_face->nentries;i++)
      {
	 destroyCDKEntry (nc_face->entries[i]);
	 nc_face->entries[i] = NULL;
      }
      
      delar(nc_face->buttons);
      delar(nc_face->entries);

      nc_face->nbuttons=0;
      nc_face->nentries=0;
      
      nc_face->show_all();
      nc_face->refresh(nc_face->mx_data);
	 
   }
   else
   {
      //nc_face->show_message("Error", "while updating table.",MTERROR);
   }
   //----------------
   }
   else
   {
   //----------------
   if(!nc_face->do_insert())
   {
      //nc_face->show_message("table info","INSERTED",MTINFO);
      
      //clear
      
      
      for(i=0;i<nc_face->nbuttons;i++)
      {
	 destroyCDKButton (nc_face->buttons[i]);
	 nc_face->buttons[i] = NULL;
      }
      
            
      for(i=0;i<nc_face->nentries;i++)
      {
	 destroyCDKEntry (nc_face->entries[i]);
	 nc_face->entries[i] = NULL;
      }
      
      delar(nc_face->buttons);
      delar(nc_face->entries);
      
      nc_face->nbuttons=0;
      nc_face->nentries=0;
      
      nc_face->show_all();
      nc_face->refresh(nc_face->mx_data);
	 
   }
   else
   {
      //nc_face->show_message("Error", "while inserting in table.",MTERROR);
   }
   //----------------
   }

   setCDKFocusCurrent(nc_face->cdkscreen,(CDKOBJS*)nc_face->mx_data->matrix);
   refreshCDKScreen(nc_face->cdkscreen);
   return(1);
}

int _action_button_cancel(EObjectType cdktype,void *object,void *clientData,chtype key)
{
   CDKBUTTON * button = (CDKBUTTON *)object;
   NC_face * nc_face    = (NC_face *)clientData;
   int i;
      for(i=0;i<nc_face->nbuttons;i++)
      {
	 destroyCDKButton (nc_face->buttons[i]);
	 nc_face->buttons[i] = NULL;
      }
      
      for(i=0;i<nc_face->nentries;i++)
      {
	 destroyCDKEntry (nc_face->entries[i]);
	 nc_face->entries[i] = NULL;
      }
      
      delar(nc_face->buttons);
      delar(nc_face->entries);
      
      nc_face->nbuttons=0;
      nc_face->nentries=0;
      
      nc_face->show_all();
      nc_face->refresh(nc_face->mx_data);

      setCDKFocusCurrent(nc_face->cdkscreen,(CDKOBJS*)nc_face->mx_data->matrix);
      refreshCDKScreen(nc_face->cdkscreen);

   return(1);
}

void action_btn_confirm (SButton *object)
{
   return;
}

void action_btn_cancel (SButton *object)
{
   return;
}

int data_action (EObjectType cdktype,void *object,void *clientData,chtype key)
{
   CDKMATRIX * matrix = (CDKMATRIX *)object;
   NC_face * nc_face    = (NC_face *)clientData;
   
   switch(key)
   {
	 case  KEY_F(4):
	 case  'e':
	    if(getCDKMatrixRow (matrix)!=matrix->rows)
	    {
	       nc_face->do_data_edit();
	       setCDKFocusNext(nc_face->cdkscreen);
	    }
	 break;  
	 
	 case	'a':
	 case 	'i':
	    nc_face->do_data_append();
	    setCDKFocusNext(nc_face->cdkscreen);
	    break;
	 
	 case	'd':
	    if(getCDKMatrixRow (matrix)!=matrix->rows)
	    nc_face->do_data_delete();
	    break;
	 
	 case 	'x':
	    del(nc_face->mx_data);
	    nc_face->mx_pos = 0;
	    setCDKFocusNext(nc_face->cdkscreen);
	    break;

	 case KEY_ESC:
	    nc_face->mx_pos = 0;
	    del(nc_face->mx_data);
	    break;
	 default:
	 break;      
   }

   refreshCDKScreen(nc_face->cdkscreen);
   return 1;
   
}



int list_choose (EObjectType cdktype,void *object,void *clientData,chtype key)
{
   CDKSCROLL *list	= (CDKSCROLL *)object;
   NC_face * nc_face    = (NC_face *)clientData;
   char * buf;
   
   char * pch = nc_face->choose_items[getCDKScrollCurrent(list)];
//   nc_face->show_message(pch,"",MTINFO);
 
   switch(nc_face->navigation)
   {
//------------------------------------------------------------
      case NVDATABASES:
	 nc_face->do_navigate(NVSCHEMAS);
      break;
//------------------------------------------------------------
     case NVSCHEMAS:
      if(!strncmp(pch,"..",2))
      {
	 if(nc_face->show_message("Confirm disconnect.","Disconnect?",MTCONF)==BTNYES)
	 {
	    nc_face->do_disconnect();
	 }
	 else
	 {
	 }

      }
      else
      {
      buf = new char[MAX_QUERY_STRING_LEN+1];
      sprintf(buf,"SET search_path TO %s;",pch);
      
      nc_face->set_status("Changing of tablespace...");
      if(nc_face->pg_face->ExC(buf))
      {
	 nc_face->set_status("Changing of tablespace... Failed");
	 nc_face->show_message("Error","While changing of tablespace",MTERROR);
	 delar(buf);
	 return(1);
      }
      nc_face->set_status("Changing of tablespace... OK");
      sprintf(nc_face->schema_name,"%s",pch);
      delar(buf);
      
//      nc_face->navigation = NVTABLES;
      nc_face->set_path();
      nc_face->do_navigate(NVTABLES);
      }
      break;
//------------------------------------------------------------
      case NVTABLES:
      if(!strncmp(pch,"..",2))
      {
	 nc_face->schema_name[0]='\0';
	 nc_face->table_name[0]='\0';
	 nc_face->set_path();
	 nc_face->do_navigate(NVSCHEMAS);
      }
      else
      {
	 sprintf(nc_face->table_name,"%s",pch);
	    
	 if(nc_face->do_data()==0)
	    nc_face->set_path();
      }

      break;
   }
   
   refreshCDKScreen(nc_face->cdkscreen);

   return 1;
} 
// SQL perform
int sql_perform (EObjectType cdktype,void *object,void *clientData,chtype key)
{
   CDKMENTRY *textbox	= (CDKMENTRY *)object;
   NC_face * nc_face    = (NC_face *)clientData;
   char * buf;
  
   buf = strdup(textbox->info);
   
   del(nc_face->pg_table);

   nc_face->set_status("Performing query...");
   
   if(!strncasecmp(buf,"select",strlen("select")))
   {
      nc_face->pg_table = nc_face->pg_face->ExS(buf);
      if(!nc_face->pg_table)
      {
      //++++++++++++++++++ ERROR MESSAGE
	 nc_face->set_status("Query...  Failed");
	 nc_face->show_message("Error","Query failed",MTERROR);
      
	 delar(buf);
	 del(nc_face->pg_table);
	 del(nc_face->pg_table);
      
	 return(0);
      }
   }
   else
   {
      if(nc_face->pg_face->ExC(buf))
      {
	 // error
	 nc_face->set_status("Query...  Failed");
	 nc_face->show_message("Error","while performing SQL-query..",MTERROR);
	 delar(buf);
	 return(0);
	 
      }
      else
      {
	 nc_face->set_status("Query...  OK");
	 nc_face->show_message("Result - OK","The command was successfully executed.",MTINFO);
	 delar(buf);
	 return(0);
      }
   }

   nc_face->set_status("Performing query...  OK");
   
   del(nc_face->mxtb);
   
   nc_face->mxtb = new NC_table(nc_face->cdkscreen,nc_face->pg_table,"");
   int a = nc_face->mxtb->Create(10);
   
   setCDKFocusCurrent(nc_face->cdkscreen,(CDKOBJS*)nc_face->mxtb->matrix);
   
   refreshCDKScreen(nc_face->cdkscreen);

   //del(nc_face->pg_table);
   nc_face->pg_table = NULL;
   
   delar(buf);
   
  return(1);
} 
int displayPostCallback(EObjectType cdktype, /*GCC_UNUSED,*/ void *object, void *clientData, chtype input /*,GCC_UNUSED*/)
{
   CDKMENU *menu	= (CDKMENU *)object;
   NC_face * nc_face    = (NC_face *)clientData;
   int mp,sp;
   
   return 1;
}

int displayPreCallback(EObjectType cdktype, /*GCC_UNUSED,*/ void *object, void *clientData, chtype input /*,GCC_UNUSED*/)
{
   CDKMENU *menu	= (CDKMENU *)object;
   NC_face * nc_face    = (NC_face *)clientData;
   int mp,sp;

   switch (input)
   {
		case KEY_ENTER:
			getCDKMenuCurrentItem((CDKMENU *)object, &mp, &sp);	

			if(mp == 0)
			{
				switch(sp)
				{
				// File
				// Add Server
//				case 1:
//                                  nc_face->do_add_server();
//                                break;
//				break;
				// Options
				case 0:
                                    nc_face->do_options();
                                    break;
				// Exit
				case 1:
                                    nc_face->do_exit();
                                    break;
				case 2:
				case 3:
				case 4:
				       break;
				}
			}
/*			else
			if(mp == 1)
			{
				switch(sp)
				{
				// Edit
				// Create
				case 0:
                                    nc_face->do_create();
                                    break;
				// Delete
				case 1:
                                    nc_face->do_delete();
                                    break;
				// Properties
				case 2:
                                    nc_face->do_properties();
                                    break;
				case 3:
					break;
				}
			}*/
			else
			if(mp == 1)
			{
				switch(sp)
				{
				// Tools
				// Connect
				case 0:
			            if(nc_face->pg_face->is_connected())
				    {
				       nc_face->show_message("Error","It's already connected.",MTINFO);         
				    }
				    else
				    {
				       nc_face->do_navigate(NVSCHEMAS);
				    }
                                    break;
				// Disconnect
				case 1:
                                    nc_face->do_disconnect();
                                    break;
				// Query			
				case 2:
				if(!nc_face->nbuttons)
				{
				    if(nc_face->pg_face->is_connected())
				    {
				       nc_face->do_sql();
				    }
				    else
				    {
				       nc_face->show_message("Error","There is no connection.",MTINFO);         
				    }
				 }
				 else
				 {
				    nc_face->show_message("Info","At first you should finish editing the data.",MTINFO);
				 }
                                    break;
				// Data
//				case 3:
//                                    nc_face->do_data();
//                                    break;
				}
			}
			else
			if(mp == 2)
			{
			   switch(sp)
			   {
				// Help
				case 0:
                                    nc_face->do_help();
                                    break;
			   }
			}
		break;
   }

   return 1;
}
//=========================================================



