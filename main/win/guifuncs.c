/***************************************************************************
                          guifuncs.c  -  description
                             -------------------
    copyright            : (C) 2003 by ShadowPrince
    email                : shadow@emulation64.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define _WIN32_IE 0x0500
#include <LuaConsole.h>

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "../guifuncs.h"
#include "main_win.h"
#include "translation.h"
#include "rombrowser.h"
#include "../../winproject/resource.h"
#include <vcr.h>

char *get_currentpath()
{
    return AppPath;
}

char *get_savespath()
{
    static char defDir[MAX_PATH];
    if (Config.DefaultSavesDir) {
        sprintf(defDir,"%sSave\\",AppPath);
        return defDir;
    }
    else return Config.SavesDir;
}

void display_loading_progress(int p)
{
   sprintf(TempMessage,"%d%%",p);
   //SendMessage( hStatus, SB_SETTEXT, 1, (LPARAM)TempMessage );  
   SendMessage( hStatusProgress, PBM_SETPOS, p+1, 0 );
}

bool warn_recording() {

    int res = 0, warnings = res; // red eared slider
    bool rec = VCR_isCapturing() || recording;
    //printf("\nRecording info:\nVCR_isRecording: %i\nVCR_isCapturing: %i", VCR_isRecording(), VCR_isCapturing());
    if (continue_vcr_on_restart_mode == 0) {
        std::string finalMessage = "";
        if (VCR_isRecording()) {
            finalMessage.append("Movie recording ");
            warnings++;
        }
        if (rec) {
            if (warnings > 0) { finalMessage.append(","); }
            finalMessage.append(" AVI capture ");
            warnings++;
        }
        if (enableTraceLog) {
            if(warnings > 0){ finalMessage.append(","); }
            finalMessage.append(" Trace logging ");
            warnings++;
        }
        finalMessage.append("is running. Are you sure you want to stop emulation?");
        //if(rec) SetWindowPos(mainHWND, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        if(warnings > 0) res = MessageBox(NULL, finalMessage.c_str(), "Stop emulation?", MB_YESNO | MB_TOPMOST | MB_ICONWARNING);
        //if (rec) SetWindowPos(mainHWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    return res == 7;
}
void internal_warnsavestate(char* messageCaption, char* message, bool modal) {
    if (!Config.savesERRORS) return;
    TranslateDefault(message, messageCaption, TempMessage);
    if (modal) 
        MessageBox(mainHWND, TempMessage, messageCaption, MB_ICONERROR);
    else 
        display_status(TempMessage);
}
void warn_savestate(char* messageCaption, char* message){internal_warnsavestate(messageCaption, message, false);}
void warn_savestate(char* messageCaption, char* message, bool modal){internal_warnsavestate(messageCaption, message, modal);}

void display_MD5calculating_progress(int p )
{
   if (romInfoHWND!=NULL) {
    if (p>0) {
        sprintf(TempMessage,"%d%%",p);                                 
        SetDlgItemText( romInfoHWND, IDC_MD5_PROGRESS, TempMessage );
        SendMessage( GetDlgItem(romInfoHWND, IDC_CURRENT_ROM_PROGRESS), PBM_SETPOS, p+1, 0 );
        SendMessage( GetDlgItem(romInfoHWND, IDC_MD5_PROGRESS_BAR), PBM_SETPOS, p+1, 0 );
        }
     else
       {
        SetDlgItemText( romInfoHWND, IDC_MD5_PROGRESS, "" );           
        SendMessage( GetDlgItem(romInfoHWND, IDC_CURRENT_ROM_PROGRESS), PBM_SETPOS, 0, 0 );
        SendMessage( GetDlgItem(romInfoHWND, IDC_MD5_PROGRESS_BAR), PBM_SETPOS, 0, 0 );
       }   
   }    
}
int ask_extension()
{
    if (!Config.alertBAD) /*technically a bad rom, right?*/return 1;

    if (MessageBox(NULL, "This rom has an invalid extension, do you want to continue? (May cause instability)", "Warning", MB_YESNO | MB_ICONWARNING) != IDNO)
        return 1;
    else
        return 0;
}
int ask_bad()
{
    if (!Config.alertBAD) return 1;
    
    if (MessageBox(NULL,"This rom is a bad dump, do you want to continue?","Warning",MB_YESNO | MB_ICONWARNING) != IDNO)
            return 1;
    else 
            return 0;            
}

int ask_hack()
{   
    if (!Config.alertHACK) return 1;
    
    if (MessageBox(NULL,"This rom is a hacked or too large dump, do you want to continue?","Question",MB_YESNO | MB_ICONQUESTION) != IDNO)
            return 1;
    else
            return 0;
}

//Shows text in the first sector of statusbar
void display_status(const char* status)
{
   SendMessage( hStatus, SB_SETTEXT, 0, (LPARAM)status ); 
}
