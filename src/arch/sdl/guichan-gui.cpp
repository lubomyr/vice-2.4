#include <algorithm>
#ifdef ANDROID
#include <android/log.h>
#endif
#include <guichan.hpp>
#include <iostream>
#include <sstream>
#include <SDL/SDL_ttf.h>
#include <guichan/sdl.hpp>
#include "sdltruetypefont.hpp"
#include <unistd.h>

#define PANDORA

#if defined(ANDROID)
#include <SDL_screenkeyboard.h>
#endif

extern "C"
{
  #include "vice.h"

#include "archdep.h"
#include "interrupt.h"
#include "ioutil.h"
#include "joy.h"
#include "lib.h"
#include "machine.h"
#include "menu_common.h"
#include "raster.h"
#include "resources.h"
#include "screenshot.h"
#include "sound.h"
#include "types.h"
#include "ui.h"
#include "uihotkey.h"
#include "uimenu.h"
#include "util.h"
#include "video.h"
#include "videoarch.h"
#include "vkbd.h"
#include "vsidui_sdl.h"
#include "vsync.h"

#include "vice_sdl.h"
}

SDL_Surface* new_screen;
int emulating;
bool running = false;
int rungame=0;
static BYTE *draw_buffer_backup = NULL;

#define GUI_WIDTH  640
#define GUI_HEIGHT 480

/* What is being loaded */
#define MENU_SELECT_FILE 1
#define MENU_ADD_DIR 2
#define MENU_SELECT_DRIVE8 3
#define MENU_SELECT_DRIVE9 4
#define MENU_SELECT_DRIVE10 5
#define MENU_SELECT_DRIVE11 6

char currentDir[300];
char launchDir[300];

namespace globals
{
    gcn::Gui* gui;
}

namespace sdl
{
  // Main objects to draw graphics and get user input
  gcn::SDLGraphics* graphics;
  gcn::SDLInput* input;
  gcn::SDLImageLoader* imageLoader;

  void init()
  {
#ifdef PANDORA
  	char layersize[20];
  	snprintf(layersize, 20, "%dx%d", GUI_WIDTH, GUI_HEIGHT);

  	char bordercut[20];
  	snprintf(bordercut, 20, "0,0,0,0");

#endif
    new_screen = SDL_SetVideoMode(GUI_WIDTH, GUI_HEIGHT, 16, SDL_SWSURFACE);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

#ifdef PANDORA
    SDL_ShowCursor(SDL_ENABLE);
#endif

    imageLoader = new gcn::SDLImageLoader();
    gcn::Image::setImageLoader(imageLoader);

    graphics = new gcn::SDLGraphics();
    graphics->setTarget(new_screen);
    
    input = new gcn::SDLInput();

    globals::gui = new gcn::Gui();
    globals::gui->setGraphics(graphics);
    globals::gui->setInput(input);
  }


  void halt()
  {
    delete globals::gui;
    delete imageLoader;
    delete input;
    delete graphics;
  }


  void run()
  {
    // The main loop
    while(running)
    {
      // Check user input
      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
    		if (event.type == SDL_QUIT)
    		{
    			running = false;
    			break;
    		}
        else if (event.type == SDL_KEYDOWN)
        {
          switch(event.key.keysym.sym)
          {
            case SDLK_ESCAPE:
	    case SDLK_F12:

              running = false;
              break;

#ifndef PANDORA
            case SDLK_f:
              if (event.key.keysym.mod & KMOD_CTRL)
              {
                // Works with X11 only
                SDL_WM_ToggleFullScreen(new_screen);
              }
              break;
#endif
          }
        }
        input->pushInput(event);
	    }
      // Now we let the Gui object perform its logic.
      globals::gui->logic();
      // Now we let the Gui object draw itself.
      globals::gui->draw();
      // Finally we update the screen.
      SDL_Flip(new_screen);
    }
  }

}


namespace widgets
{
  extern void run_menuLoad_guichan(char *curr_path, int aLoadType);
extern gcn::Window *window_load;
extern gcn::Window *window_warning;
extern gcn::Window *window_savestate;
  extern gcn::Widget* activateAfterClose;
  void menuSaveState_Init(void);
  void menuSaveState_Exit(void);
  void loadMenu_Init(void);
  void loadMenu_Exit(void);
  void menuMessage_Init(void);
  void menuMessage_Exit(void);
  std::string selectedFilePath;
  std::string selectedFilePath8;
  std::string selectedFilePath9;
  std::string selectedFilePath10;
  std::string selectedFilePath11;
  
  gcn::Color baseCol;
  gcn::Color baseColLabel;
  gcn::Container* top;
  gcn::contrib::SDLTrueTypeFont* font;

  // Presets
  
  // Main buttons
  gcn::Image *background_image;
  gcn::Icon* background;
  gcn::Button* button_start;
  gcn::Button* button_resume;
  gcn::Button* button_quit;
  gcn::Button* button_setting;
  gcn::Button* button_savestate;
  
  gcn::Button* button_df0;
  gcn::Button* button_df1;
  gcn::Button* button_df2;
  gcn::Button* button_df3;
  gcn::TextField* textField_df0;
  gcn::TextField* textField_df1;
  gcn::TextField* textField_df2;
  gcn::TextField* textField_df3;
  gcn::Button* button_ejectdf0;
  gcn::Button* button_ejectdf1;
  gcn::Button* button_ejectdf2;
  gcn::Button* button_ejectdf3;

  void show_settings();
  
extern "C" {
    extern void sdl_ui_activate();
}

  class QuitButtonActionListener : public gcn::ActionListener
  {
    public:
      void action(const gcn::ActionEvent& actionEvent)
      {
  		running = false;
		exit(0);
      }
  };
  QuitButtonActionListener* quitButtonActionListener;


  class StartButtonActionListener : public gcn::ActionListener
  {
    public:
      void action(const gcn::ActionEvent& actionEvent)
      {
		activateAfterClose = button_df0;
		run_menuLoad_guichan(currentDir, MENU_SELECT_FILE);
      }
  };
  StartButtonActionListener* startButtonActionListener;


  class ResumeButtonActionListener : public gcn::ActionListener
  {
    public:
      void action(const gcn::ActionEvent& actionEvent)
      {
    		running = false;
      }
  };
  ResumeButtonActionListener* resumeButtonActionListener;

  class SettingButtonActionListener : public gcn::ActionListener
  {
    public:
      void action(const gcn::ActionEvent& actionEvent)
      {
	    running = false;
	    sdl_ui_activate();
      }
  };
  SettingButtonActionListener* settingButtonActionListener;

 class DFxButtonActionListener : public gcn::ActionListener
  {
    public:
      void action(const gcn::ActionEvent& actionEvent)
      {
	   if (actionEvent.getSource() == button_df0)
	   {
		activateAfterClose = button_df0;
      		run_menuLoad_guichan(currentDir, MENU_SELECT_DRIVE8);
  	    }
	   if (actionEvent.getSource() == button_df1)
	   {
		activateAfterClose = button_df1;
      		run_menuLoad_guichan(currentDir, MENU_SELECT_DRIVE9);
  	    }
	   if (actionEvent.getSource() == button_df2)
	   {
		activateAfterClose = button_df2;
      		run_menuLoad_guichan(currentDir, MENU_SELECT_DRIVE10);
  	    }
	    if (actionEvent.getSource() == button_df3)
	    {
		activateAfterClose = button_df3;
      		run_menuLoad_guichan(currentDir, MENU_SELECT_DRIVE11);
  	    }
      }
  };
  DFxButtonActionListener* dfxButtonActionListener;


  class EjectButtonActionListener : public gcn::ActionListener
  {
    public:
      void action(const gcn::ActionEvent& actionEvent)
      {
		if (actionEvent.getSource() == button_ejectdf0)
		{
				selectedFilePath8="";
		}
		if (actionEvent.getSource() == button_ejectdf1)
		{
				selectedFilePath9="";
		}
		if (actionEvent.getSource() == button_ejectdf2)
		{
				selectedFilePath10="";
		}
		if (actionEvent.getSource() == button_ejectdf3)
		{
				selectedFilePath11="";
		}
    		show_settings();
      }
  };
  EjectButtonActionListener* ejectButtonActionListener;

class StateButtonActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        window_savestate->setVisible(true);
    }
};
StateButtonActionListener* stateButtonActionListener;
  
  void init()
  {
    baseCol.r = 192;
    baseCol.g = 192;
    baseCol.b = 208;
    baseColLabel.r = baseCol.r;
    baseColLabel.g = baseCol.g;
    baseColLabel.b = baseCol.b;
    baseColLabel.a = 192;
    
    top = new gcn::Container();
    top->setDimension(gcn::Rectangle(0, 0, 640, 480));
    top->setBaseColor(baseCol);
    globals::gui->setTop(top);

    TTF_Init();
    font = new gcn::contrib::SDLTrueTypeFont("data/FreeSans.ttf", 17);	
    gcn::Widget::setGlobalFont(font);
	
    background_image = gcn::Image::load("data/background.jpg");
    background = new gcn::Icon(background_image);

  	//--------------------------------------------------
  	// Create main buttons
  	//--------------------------------------------------
    
  	button_quit = new gcn::Button("Quit");
  	button_quit->setSize(90,50);
    button_quit->setBaseColor(baseCol);
    button_quit->setId("Quit");
    quitButtonActionListener = new QuitButtonActionListener();
    button_quit->addActionListener(quitButtonActionListener);
    
  	button_start = new gcn::Button("Run");
  	button_start->setSize(90,50);
    button_start->setBaseColor(baseCol);
  	button_start->setId("Reset");
    startButtonActionListener = new StartButtonActionListener();
    button_start->addActionListener(startButtonActionListener);

  	button_resume = new gcn::Button("Resume");
  	button_resume->setSize(90,50);
    button_resume->setBaseColor(baseCol);
  	button_resume->setId("Resume");
    resumeButtonActionListener = new ResumeButtonActionListener();
    button_resume->addActionListener(resumeButtonActionListener);
    
	button_setting = new gcn::Button("Setting");
	button_setting->setSize(90,50);
	button_setting->setBaseColor(baseCol);
	button_setting->setId("Setting");
	settingButtonActionListener = new SettingButtonActionListener();
	button_setting->addActionListener(settingButtonActionListener);

    button_savestate = new gcn::Button("SaveStates");
    button_savestate->setSize(100,50);
    button_savestate->setBaseColor(baseCol);
    button_savestate->setId("SaveStates");
    button_savestate->setVisible(true);
    stateButtonActionListener = new StateButtonActionListener();
    button_savestate->addActionListener(stateButtonActionListener);	
	
  	// Button and text for drives
  	button_df0 = new gcn::Button("Drive 8");
  	button_df0->setSize(64,22);
  	button_df0->setPosition(20,120);
    button_df0->setBaseColor(baseCol);
    button_df0->setId("Drive8");
  	button_df1 = new gcn::Button("Drive 9");
  	button_df1->setSize(64,22);
  	button_df1->setPosition(20,160);
    button_df1->setBaseColor(baseCol);
    button_df1->setId("Drive9");
  	button_df2 = new gcn::Button("Drive 10");
  	button_df2->setSize(64,22);
  	button_df2->setPosition(20,200);
    button_df2->setBaseColor(baseCol);
    button_df2->setId("Drive10");
  	button_df3 = new gcn::Button("Drive 11");
  	button_df3->setSize(64,22);
  	button_df3->setPosition(20,240);
    button_df3->setBaseColor(baseCol);
    button_df3->setId("Drive11");
    dfxButtonActionListener = new DFxButtonActionListener();
  	button_df0->addActionListener(dfxButtonActionListener);
  	button_df1->addActionListener(dfxButtonActionListener);
  	button_df2->addActionListener(dfxButtonActionListener);
  	button_df3->addActionListener(dfxButtonActionListener);
  	textField_df0 = new gcn::TextField("insert disk image                                                            ");
  	textField_df0->setSize(410,22);
  	textField_df0->setPosition(95,120);
  	textField_df0->setEnabled(false);
    textField_df0->setBaseColor(baseCol);
  	textField_df1 = new gcn::TextField("insert disk image                                                            ");
  	textField_df1->setSize(410,22);
  	textField_df1->setPosition(95,160);
  	textField_df1->setEnabled(false);
    textField_df1->setBaseColor(baseCol);
  	textField_df2 = new gcn::TextField("insert disk image                                                            ");
  	textField_df2->setSize(410,22);
  	textField_df2->setPosition(95,200);
  	textField_df2->setEnabled(false);
    textField_df2->setBaseColor(baseCol);
  	textField_df3 = new gcn::TextField("insert disk image                                                            ");
  	textField_df3->setSize(410,22);
  	textField_df3->setPosition(95,240);
  	textField_df3->setEnabled(false);
    textField_df3->setBaseColor(baseCol);	
	button_ejectdf0 = new gcn::Button("Eject");
	button_ejectdf0->setHeight(22);
	button_ejectdf0->setBaseColor(baseCol);
	button_ejectdf0->setPosition(515,120);
	button_ejectdf0->setId("ejectDr8");
	button_ejectdf1 = new gcn::Button("Eject");
	button_ejectdf1->setHeight(22);
	button_ejectdf1->setBaseColor(baseCol);
	button_ejectdf1->setPosition(515,160);
	button_ejectdf1->setId("ejectDr9");
	button_ejectdf2 = new gcn::Button("Eject");
	button_ejectdf2->setHeight(22);
	button_ejectdf2->setBaseColor(baseCol);
	button_ejectdf2->setPosition(515,200);
	button_ejectdf2->setId("ejectDr10");
	button_ejectdf3 = new gcn::Button("Eject");
	button_ejectdf3->setHeight(22);
	button_ejectdf3->setBaseColor(baseCol);
	button_ejectdf3->setPosition(515,240);
	button_ejectdf3->setId("ejectDr11");
	ejectButtonActionListener = new EjectButtonActionListener();
	button_ejectdf0->addActionListener(ejectButtonActionListener);
	button_ejectdf1->addActionListener(ejectButtonActionListener);
	button_ejectdf2->addActionListener(ejectButtonActionListener);
	button_ejectdf3->addActionListener(ejectButtonActionListener);
	
    menuSaveState_Init();
    menuMessage_Init();
    loadMenu_Init();	
	
  	//--------------------------------------------------
    // Place everything on main form
  	//--------------------------------------------------
    top->add(background, 0, 0);
    top->add(button_savestate, 20, 410);
    top->add(button_start, 180, 410);
    top->add(button_resume, 290, 410);
    top->add(button_setting, 400, 410);
    top->add(button_quit, 510, 410);
 
    top->add(button_df0);
    top->add(textField_df0);
    top->add(button_ejectdf0);
    top->add(button_df1);  
    top->add(textField_df1);
    top->add(button_ejectdf1);
    top->add(button_df2);  
    top->add(textField_df2);
    top->add(button_ejectdf2);
    top->add(button_df3);  
    top->add(textField_df3);
    top->add(button_ejectdf3);
  
    top->add(window_savestate);
    top->add(window_load, 120, 20);
    top->add(window_warning, 170, 220);
  }

  	//--------------------------------------------------
  	// Initialize focus handling
  	//--------------------------------------------------
//    button_start->setFocusable(true);
//    button_resume->setFocusable(true);
//    button_quit->setFocusable(true);

  void halt()
  {
    menuSaveState_Exit();
    menuMessage_Exit();
    loadMenu_Exit();
	
	top->remove(button_start);
	top->remove(button_resume);
	top->remove(button_quit);
  
    delete button_start;
    delete button_resume;
    delete button_quit;
    delete button_setting;
    delete button_savestate;
	
  	delete button_df0;
  	delete button_df1;
  	delete button_df2;
  	delete button_df3;
  	delete textField_df0;
  	delete textField_df1;
  	delete textField_df2;
  	delete textField_df3;
	delete button_ejectdf0;
	delete button_ejectdf1;
	delete button_ejectdf2;
	delete button_ejectdf3;	

    delete resumeButtonActionListener;
    delete startButtonActionListener;
    delete quitButtonActionListener;
    delete settingButtonActionListener;
    delete stateButtonActionListener;
    delete dfxButtonActionListener;
    delete ejectButtonActionListener;

    delete background;
    delete background_image;
	
    delete font;
    delete top;
    
  }

  //-----------------------------------------------
  // Start of helper functions
  //-----------------------------------------------
  void show_settings()
  {
      if (selectedFilePath8=="")
	    textField_df0->setText("insert disk image");
      else
	    textField_df0->setText(selectedFilePath8);
      if (selectedFilePath9=="")
	    textField_df1->setText("insert disk image");
      else
	    textField_df1->setText(selectedFilePath9);
      if (selectedFilePath10=="")
	    textField_df2->setText("insert disk image");
      else
	    textField_df2->setText(selectedFilePath10);
      if (selectedFilePath11=="")
	    textField_df3->setText("insert disk image");
      else
	    textField_df3->setText(selectedFilePath11);
  }
  
} 

extern "C" {
int gui_open();
}

int gui_open()
 {
    getcwd(launchDir, 250);
    strcpy(currentDir,".");
#ifdef ANDROID	  
  SDL_ANDROID_SetScreenKeyboardShown(0);
#endif
   sdl_ui_activate_pre_action();
   running = true;

    sdl::init();
    widgets::init();
    sdl::run();
    widgets::halt();
    sdl::halt();
#ifdef ANDROID	  
  SDL_ANDROID_SetScreenKeyboardShown(1);
#endif
    sdl_ui_activate_post_action();
    sdl_video_restore_size();
    return -1;
 }
