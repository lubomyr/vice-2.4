#include <algorithm>
#ifdef ANDROID
#include <android/log.h>
#include <SDL_screenkeyboard.h>
#endif
#include <guichan.hpp>
#include <iostream>
#include <sstream>
#include <SDL/SDL_ttf.h>
#include <guichan/sdl.hpp>
#include "sdltruetypefont.hpp"
#include "rw_config.h"
#include <sys/types.h>
#include <dirent.h>

#if defined(WIN32)
#define realpath(N,R) _fullpath((R),(N),_MAX_PATH)
#endif

#ifndef PATH_MAX
#define PATH_MAX 256
#endif

#define extterms files[q].size()>=4 && files[q].substr(files[q].size()-4)
#define fileterms extterms!=".d64" && extterms!=".D64" && extterms!=".t64" && extterms!=".T64" && extterms!=".g64" && extterms!=".G64" && extterms!=".p64" && extterms!=".P64" && extterms!=".x64" && extterms!=".X64" && extterms!=".bin" && extterms!=".Bin" && extterms!=".BIN" && extterms!=".crt" && extterms!=".Crt" && extterms!=".CRT" && extterms!=".tap" && extterms!=".Tap" && extterms!=".TAP" && extterms!=".prg" && extterms!=".Prg" && extterms!=".PRG" && extterms!=".zip" && extterms!=".Zip" && extterms!=".ZIP"

extern bool running;
bool confirmselection = false;
int menuLoad_extfilter=1;

#define _XOPEN_SOURCE 500
#if defined(AROS)
#include <limits.h>

#if defined(__GP2X__) || defined(__WIZ__) || defined(__CAANOO__) || defined(__amigaos__)
// This is a random default value ...
#define PATH_MAX 32768
#endif

static char *sep(char *path)
{
    char *tmp, c;

    tmp = strrchr(path, '/');
    if(tmp) {
        c = tmp[1];
        tmp[1] = 0;
        if (chdir(path)) {
            return NULL;
        }
        tmp[1] = c;

        return tmp + 1;
    }
    return path;
}

char *realpath(const char *_path, char *resolved_path)
{
    int fd = open(".", O_RDONLY), l;
    char current_dir_path[PATH_MAX];
    char path[PATH_MAX], lnk[PATH_MAX], *tmp = (char *)"";

    if (fd < 0) {
        return NULL;
    }
    getcwd(current_dir_path,PATH_MAX);
    strncpy(path, _path, PATH_MAX);

    if (chdir(path)) {
        if (errno == ENOTDIR) {
#if defined(__WIN32__) || defined(__MORPHOS__) || defined(__amigaos__)
            // No symbolic links and no readlink()
            l = -1;
#else
            l = readlink(path, lnk, PATH_MAX);
#endif
            if (!(tmp = sep(path))) {
                resolved_path = NULL;
                goto abort;
            }
            if (l < 0) {
                if (errno != EINVAL) {
                    resolved_path = NULL;
                    goto abort;
                }
            } else {
                lnk[l] = 0;
                if (!(tmp = sep(lnk))) {
                    resolved_path = NULL;
                    goto abort;
                }
            }
        } else {
            resolved_path = NULL;
            goto abort;
        }
    }

    if(resolved_path==NULL) // if we called realpath with null as a 2nd arg
        resolved_path = (char*) malloc( PATH_MAX );

    if (!getcwd(resolved_path, PATH_MAX)) {
        resolved_path = NULL;
        goto abort;
    }

    if(strcmp(resolved_path, "..") && *tmp) {
        strcat(resolved_path, "..");
    }

    strcat(resolved_path, tmp);
abort:
    chdir(current_dir_path);
    close(fd);
    return resolved_path;
}

#endif

/* What is being loaded, floppy/hd dir/hdf */
int menu_load_type;

namespace widgets
{
void show_settings(void);
static void unraise_loadMenu_guichan();
static void checkfilename (char *currentfilename);
extern std::string selectedFilePath;
extern std::string selectedFilePath8;
extern std::string selectedFilePath9;
extern std::string selectedFilePath10;
extern std::string selectedFilePath11;

extern gcn::Color baseCol;
gcn::Window *window_load;
gcn::Button* button_ok;
gcn::Button* button_select;
gcn::Button* button_open;
gcn::Button* button_cancel;
#ifdef ANDROID
gcn::CheckBox* checkBox_extfilter;
#endif
gcn::ListBox* listBox;
gcn::ScrollArea* listBoxScrollArea;

gcn::Widget* activateAfterClose = NULL;

int lastSelectedIndex = 0;


class DirListModel : public gcn::ListModel
{
    std::vector<std::string> dirs, files;

public:
    DirListModel(const char * path) {
        changeDir(path);
    }

    int getNumberOfElements() {
        if (menu_load_type != MENU_ADD_DIR)
            return dirs.size() + files.size();
        else
            return dirs.size();
    }

    std::string getElementAt(int i) {
        if(i >= dirs.size() + files.size() || i < 0)
            return "---";
        if(i < dirs.size())
            return dirs[i];
        if (menu_load_type != MENU_ADD_DIR)
            return files[i - dirs.size()];
    }

    void changeDir(const char * path) {
        dirs.clear();
        files.clear();
        DIR *dir;
        struct dirent *dent;
        dir = opendir(path);
        if(dir != NULL) {
            while((dent=readdir(dir))!=NULL) {
                if(dent->d_type == DT_DIR)
                    dirs.push_back(dent->d_name);
                else if (menu_load_type != MENU_ADD_DIR)
                    files.push_back(dent->d_name);
            }

            if(dirs.size() > 0 && dirs[0] == ".")
                dirs.erase(dirs.begin());

            closedir(dir);
        }

        if(dirs.size() == 0)
            dirs.push_back("..");

        std::sort(dirs.begin(), dirs.end());
        if (menu_load_type != MENU_ADD_DIR)
            std::sort(files.begin(), files.end());
#ifdef ANDROID
        if (menuLoad_extfilter==1)
#endif
            for (int q=0; q<files.size(); q++) {
                if (fileterms) {
                    files.erase(files.begin()+q);
                    q--;
                }
            }
    }

    bool isDir(int i) {
        if(i < dirs.size())
            return true;
        return false;
    }
};
DirListModel dirList(".");


class OkButtonActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        int selected_item;
        char filename[256]="";

        selected_item = listBox->getSelected();
        lastSelectedIndex = selected_item;
        strcpy(filename, "");
        strcat(filename, currentDir);
        strcat(filename, "/");
        strcat(filename, dirList.getElementAt(selected_item).c_str());
        confirmselection=true;
        checkfilename(filename);
    }
};
OkButtonActionListener* okButtonActionListener;


class SelectButtonActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        int selected_item;
        char filename[256]="";

        selected_item = listBox->getSelected();
        lastSelectedIndex = selected_item;
        strcpy(filename, "");
        strcat(filename, currentDir);
        strcat(filename, "/");
        strcat(filename, dirList.getElementAt(selected_item).c_str());
        if (menu_load_type == MENU_ADD_DIR) {
//		      strcpy(uae4all_hard_dir, filename);
        }
        unraise_loadMenu_guichan();
    }
};
SelectButtonActionListener* selectButtonActionListener;


class OpenButtonActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        int selected_item;
        char filename[256]="";

        selected_item = listBox->getSelected();
        lastSelectedIndex = selected_item;
        strcpy(filename, "");
        strcat(filename, currentDir);
        strcat(filename, "/");
        strcat(filename, dirList.getElementAt(selected_item).c_str());
        checkfilename(filename);
    }
};
OpenButtonActionListener* openButtonActionListener;


class CancelButtonActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        unraise_loadMenu_guichan();
    }
};
CancelButtonActionListener* cancelButtonActionListener;


class ListBoxActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        confirmselection=false;
#if defined(WIN32) || defined(ANDROID) || defined(AROS) || defined(RASPBERRY)
        if (menu_load_type != MENU_ADD_DIR) {
#endif

            int selected_item;
            char filename[256]="";

            selected_item = listBox->getSelected();
            lastSelectedIndex = selected_item;
            strcpy(filename, "");
            strcat(filename, currentDir);
            strcat(filename, "/");
            strcat(filename, dirList.getElementAt(selected_item).c_str());
            checkfilename(filename);
#if defined(WIN32) || defined(ANDROID) || defined(AROS) || defined(RASPBERRY)
        }
#endif
    }
};
ListBoxActionListener* listBoxActionListener;


class ListBoxKeyListener : public gcn::KeyListener
{
public:
    void keyPressed(gcn::KeyEvent& keyEvent) {
        bool bHandled = false;

        Uint8 *keystate;
        gcn::Key key = keyEvent.getKey();
        if (key.getValue() == gcn::Key::UP) {
            keystate = SDL_GetKeyState(NULL);
            if(keystate[SDLK_RSHIFT]) {
                int selected = listBox->getSelected() - 10;
                if(selected < 0)
                    selected = 0;
                listBox->setSelected(selected);
                bHandled = true;
            }
        }
        if (key.getValue() == gcn::Key::DOWN) {
            keystate = SDL_GetKeyState(NULL);
            if(keystate[SDLK_RSHIFT]) {
                int selected = listBox->getSelected() + 10;
                if(selected >= dirList.getNumberOfElements())
                    selected = dirList.getNumberOfElements() - 1;
                listBox->setSelected(selected);
                bHandled = true;
            }
        }

        if(!bHandled)
            listBox->keyPressed(keyEvent);
    }
};
ListBoxKeyListener* listBoxKeyListener;

#ifdef ANDROID
class ExtfilterActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        if (actionEvent.getSource() == checkBox_extfilter) {
            if (checkBox_extfilter->isSelected())
                menuLoad_extfilter=1;
            else
                menuLoad_extfilter=0;
        }
        dirList=currentDir;
    }
};
ExtfilterActionListener* extfilterActionListener;
#endif

void loadMenu_Init()
{
    window_load = new gcn::Window("Load");
    window_load->setSize(400,370);
    window_load->setBaseColor(baseCol);

    button_ok = new gcn::Button("Ok");
    button_ok->setPosition(231,318);
    button_ok->setSize(70, 26);
    button_ok->setBaseColor(baseCol);
    okButtonActionListener = new OkButtonActionListener();
    button_ok->addActionListener(okButtonActionListener);
    button_select = new gcn::Button("Select");
    button_select->setPosition(145,318);
    button_select->setSize(70, 26);
    button_select->setBaseColor(baseCol);
    selectButtonActionListener = new SelectButtonActionListener();
    button_select->addActionListener(selectButtonActionListener);
    button_open = new gcn::Button("Open");
    button_open->setPosition(231, 318);
    button_open->setSize(70, 26);
    button_open->setBaseColor(baseCol);
    openButtonActionListener = new OpenButtonActionListener();
    button_open->addActionListener(openButtonActionListener);
    button_cancel = new gcn::Button("Cancel");
    button_cancel->setPosition(316,318);
    button_cancel->setSize(70, 26);
    button_cancel->setBaseColor(baseCol);
    cancelButtonActionListener = new CancelButtonActionListener();
    button_cancel->addActionListener(cancelButtonActionListener);

    listBox = new gcn::ListBox(&dirList);
    listBox->setSize(650,220);
    listBox->setBaseColor(baseCol);
    listBox->setWrappingEnabled(true);
    listBoxScrollArea = new gcn::ScrollArea(listBox);
    listBoxScrollArea->setFrameSize(1);
    listBoxScrollArea->setPosition(10,10);
    listBoxScrollArea->setSize(376,298);
    listBoxScrollArea->setScrollbarWidth(20);
    listBoxScrollArea->setBaseColor(baseCol);
    listBoxActionListener = new ListBoxActionListener();
    listBox->addActionListener(listBoxActionListener);
    listBoxKeyListener = new ListBoxKeyListener();
    listBox->removeKeyListener(listBox);
    listBox->addKeyListener(listBoxKeyListener);
#ifdef ANDROID
    checkBox_extfilter = new gcn::CheckBox("ext. filter");
    checkBox_extfilter->setPosition(10,320);
    checkBox_extfilter->setId("extFilter");
    extfilterActionListener = new ExtfilterActionListener();
    checkBox_extfilter->addActionListener(extfilterActionListener);
#endif
    window_load->add(button_ok);
    window_load->add(button_select);
    window_load->add(button_open);
    window_load->add(button_cancel);
    window_load->add(listBoxScrollArea);
#ifdef ANDROID
    window_load->add(checkBox_extfilter);
#endif
    window_load->setVisible(false);
}


void loadMenu_Exit()
{
    delete listBox;
    delete listBoxScrollArea;

    delete button_ok;
    delete button_select;
    delete button_open;
    delete button_cancel;
#ifdef ANDROID
    delete checkBox_extfilter;
#endif

    delete okButtonActionListener;
    delete selectButtonActionListener;
    delete openButtonActionListener;
    delete cancelButtonActionListener;
    delete listBoxActionListener;
    delete listBoxKeyListener;
#ifdef ANDROID
    delete extfilterActionListener;
#endif

    delete window_load;
}


static void unraise_loadMenu_guichan()
{
    window_load->releaseModalFocus();
    window_load->setVisible(false);
    if(activateAfterClose != NULL)
        activateAfterClose->requestFocus();
    activateAfterClose = NULL;
    show_settings();
}


static void raise_loadMenu_guichan()
{
    if (menu_load_type != MENU_ADD_DIR) {
        button_ok->setVisible(true);
        button_select->setVisible(false);
        button_open->setVisible(false);
        button_ok->setId("cmdOk");
        button_select->setId("cmdSelect");
        button_open->setId("cmdOpen");
        button_cancel->setId("cmdCancel1");
        listBox->setId("dirList1");
    } else {
        button_ok->setVisible(false);
        button_select->setVisible(true);
        button_open->setVisible(true);
        button_ok->setId("cmdOk");
        button_select->setId("cmdSelect");
        button_open->setId("cmdOpen");
        button_cancel->setId("cmdCancel2");
        listBox->setId("dirList2");
    }
#ifdef ANDROID
    if (menuLoad_extfilter==0)
        checkBox_extfilter->setSelected(false);
    else if (menuLoad_extfilter==1)
        checkBox_extfilter->setSelected(true);
#endif
    window_load->setVisible(true);
    window_load->requestModalFocus();
    listBox->requestFocus();
}


static int menuLoadLoop(char *curr_path)
{
    char *ret = NULL;
    char *fname = NULL;
    DIR *dir;

    // is this a dir or a full path?
    if ((dir = opendir(curr_path)))
        closedir(dir);
    else {
        char *p;
        for (p = curr_path + strlen(curr_path) - 1; p > curr_path && *p != '/'; p--);
        *p = 0;
        fname = p+1;
    }
    dirList = curr_path;

    if (menu_load_type == MENU_ADD_DIR)
        window_load->setCaption("  Select HD-dir  ");
    else if (menu_load_type == MENU_SELECT_FILE)
        window_load->setCaption(" Select File to Run ");
    else
        window_load->setCaption("File Manager");

    if(lastSelectedIndex >= 0 && lastSelectedIndex < dirList.getNumberOfElements())
        listBox->setSelected(lastSelectedIndex);
}

void run_menuLoad_guichan(char *curr_path, int aLoadType)
{
    menu_load_type = aLoadType;
    raise_loadMenu_guichan();
    int ret = menuLoadLoop(curr_path);
}

extern "C" {
    int autostart_autodetect(const char *file_name,
                             const char *program_name,
                             unsigned int program_number,
                             unsigned int runmode);
    int file_system_attach_disk(unsigned int unit, const char *filename);
#define AUTOSTART_MODE_RUN  0
#define AUTOSTART_MODE_LOAD 1
}

void checkfilename (char *currentfilename)
{
    char *ret = NULL, *fname = NULL;
    char *ptr;
    char actualpath [PATH_MAX];
    struct dirent **namelist;
    DIR *dir;

    if ((dir = opendir(currentfilename))) {
        dirList=currentfilename;
        ptr = realpath(currentfilename, actualpath);
        strcpy(currentDir, ptr);
        closedir(dir);
    } else {
        if ((menu_load_type == MENU_SELECT_FILE) && confirmselection) {
            selectedFilePath=currentfilename;
            if (autostart_autodetect(selectedFilePath.c_str(), NULL, 0, AUTOSTART_MODE_RUN) < 0) {
                //ui_error("could not start auto-image");
            }
#ifdef ANDROID
            SDL_ANDROID_SetScreenKeyboardShown(1);
#endif
            running = false;
        } else if ((menu_load_type == MENU_SELECT_DRIVE8) && confirmselection) {
            selectedFilePath8=currentfilename;
            int err8 = file_system_attach_disk(8, selectedFilePath8.c_str());
        } else if ((menu_load_type == MENU_SELECT_DRIVE9) && confirmselection) {
            selectedFilePath9=currentfilename;
            int err9 = file_system_attach_disk(9, selectedFilePath9.c_str());
        } else if ((menu_load_type == MENU_SELECT_DRIVE10) && confirmselection) {
            selectedFilePath10=currentfilename;
            int err10 = file_system_attach_disk(10, selectedFilePath9.c_str());
        } else if ((menu_load_type == MENU_SELECT_DRIVE11) && confirmselection) {
            selectedFilePath11=currentfilename;
            int err11 = file_system_attach_disk(11, selectedFilePath10.c_str());
        }
        if (confirmselection)
            unraise_loadMenu_guichan();
    }
}
}
