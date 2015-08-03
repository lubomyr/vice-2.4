#include <algorithm>
#include <unistd.h>
#include <dirent.h>
#ifdef ANDROID
#include <android/log.h>
#endif
#include <guichan.hpp>
#include <iostream>
#include <sstream>
#include <SDL/SDL_ttf.h>
#include <guichan/sdl.hpp>
#include "sdltruetypefont.hpp"

extern "C"
{
#include "lib.h"
#include "machine.h"
}

static int save_disks = 1;
static int save_roms = 0;

extern bool running;

#define extterms files[q].size()>=4 && files[q].substr(files[q].size()-4)
#define ssterms extterms!=".vsf" && extterms!=".VSF" && extterms!=".Vsf"
#define ext2terms ssstring.size()>=4 && ssstring.substr(ssstring.size()-4)
#define ss2terms ext2terms!=".vsf" && ext2terms!=".VSF" && ext2terms!=".Vsf"

static char savestate_filename_default[255]= {
    '/', 't', 'm', 'p', '/', 'n', 'u', 'l', 'l', '.', 'v', 's', 'f', '\0'
};

char *savestate_filename=(char *)&savestate_filename_default[0];

extern char launchDir[300];

static void BuildBaseDir(char *filename)
{
    strcpy(filename, "");
    strcat(filename, launchDir);
    strcat(filename, "/savestates");
//        strcpy(filename, prefs_get_attr ("savestate_path"));
}

namespace widgets
{
void show_settings_SaveState(void);
void showWarning(const char *msg, const char *msg2 = "");
void showInfo(const char *msg, const char *msg2 = "");

extern gcn::Color baseCol;
extern gcn::Color baseColLabel;
extern gcn::Container* top;
extern gcn::contrib::SDLTrueTypeFont* font2;

// Tab Main
gcn::Window *window_savestate;

gcn::Button* button_ss_load;
gcn::Button* button_ss_save;
gcn::Button* button_ss_delete;
gcn::Button* button_ss_cancel;
gcn::TextField* textField_ss;
gcn::ListBox* savestatelistBox;
gcn::ScrollArea* savestateScrollArea;

class SavestateListModel : public gcn::ListModel
{
    std::vector<std::string> files;

public:
    SavestateListModel(const char * path) {
        changeDir(path);
    }

    int getNumberOfElements() {
        return files.size();
    }

    std::string getElementAt(int i) {
        if(i >= files.size() || i < 0)
            return "---";
        return files[i];
    }

    void changeDir(const char * path) {
        files.clear();
        DIR *dir;
        struct dirent *dent;
        dir = opendir(path);
        if(dir != NULL) {
            while((dent=readdir(dir))!=NULL) {
                if(!(dent->d_type == DT_DIR))
                    files.push_back(dent->d_name);
            }
            closedir(dir);
        }
        std::sort(files.begin(), files.end());
        for (int q=0; q<files.size(); q++) {
            if (ssterms) {
                files.erase(files.begin()+q);
                q--;
            }
        }
    }
};
SavestateListModel savestateList("savestates");
//SavestateListModel savestateList(prefs_get_attr ("savestate_path"));

class SsLoadButtonActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        int selected_item;
        selected_item = savestatelistBox->getSelected();
        BuildBaseDir(savestate_filename);
        strcat(savestate_filename, "/");
        strcat(savestate_filename, savestateList.getElementAt(selected_item).c_str());

	if (machine_read_snapshot(savestate_filename, 0) < 0)
                showInfo("Cannot read snapshot image.");
        else {
            window_savestate->setVisible(false);
            running=false;
        }
    }
};
SsLoadButtonActionListener* ssloadButtonActionListener;


class SsSaveButtonActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        char filename[256]="";
        std::string ssstring;
        BuildBaseDir(savestate_filename);
        strcat(savestate_filename, "/");
        ssstring = textField_ss->getText().c_str();
        strcat(savestate_filename, textField_ss->getText().c_str());
        // check extension of editable name
        if (ss2terms || ssstring.size()<5)
            strcat(savestate_filename, ".vsf");
        window_savestate->setVisible(false);
        if (machine_write_snapshot(savestate_filename, save_roms, save_disks, 0) < 0)
            showInfo("Cannot save snapshot image.");
	else
	    showInfo("Savestate file saved.");

        BuildBaseDir(savestate_filename);
        savestateList = savestate_filename;

    }
};
SsSaveButtonActionListener* sssaveButtonActionListener;

class SsDeleteButtonActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        int selected_item;
        selected_item = savestatelistBox->getSelected();
        BuildBaseDir(savestate_filename);
        strcat(savestate_filename, "/");
        strcat(savestate_filename, savestateList.getElementAt(selected_item).c_str());
        if(unlink(savestate_filename)) {
            showWarning("Failed to delete savestate.");
        } else {
            BuildBaseDir(savestate_filename);
            savestateList = savestate_filename;
        }
    }
};
SsDeleteButtonActionListener* ssdeleteButtonActionListener;

class SsCancelButtonActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        window_savestate->setVisible(false);
    }
};
SsCancelButtonActionListener* sscancelButtonActionListener;

class SavestatelistBoxActionListener : public gcn::ActionListener
{
public:
    void action(const gcn::ActionEvent& actionEvent) {
        int selected_item;
        selected_item = savestatelistBox->getSelected();
        textField_ss->setText(savestateList.getElementAt(selected_item).c_str());
    }
};
SavestatelistBoxActionListener* savestatelistBoxActionListener;

void menuSaveState_Init()
{
    button_ss_load = new gcn::Button("Load");
    button_ss_load->setId("ssLoad");
    button_ss_load->setPosition(316,10);
    button_ss_load->setSize(70, 26);
    button_ss_load->setBaseColor(baseCol);
    ssloadButtonActionListener = new SsLoadButtonActionListener();
    button_ss_load->addActionListener(ssloadButtonActionListener);
    button_ss_save = new gcn::Button("Save");
    button_ss_save->setId("ssSave");
    button_ss_save->setPosition(316,65);
    button_ss_save->setSize(70, 26);
    button_ss_save->setBaseColor(baseCol);
    sssaveButtonActionListener = new SsSaveButtonActionListener();
    button_ss_save->addActionListener(sssaveButtonActionListener);
    button_ss_delete = new gcn::Button("Delete");
    button_ss_delete->setId("ssDelete");
    button_ss_delete->setPosition(316,120);
    button_ss_delete->setSize(70, 26);
    button_ss_delete->setBaseColor(baseCol);
    ssdeleteButtonActionListener = new SsDeleteButtonActionListener();
    button_ss_delete->addActionListener(ssdeleteButtonActionListener);
    button_ss_cancel = new gcn::Button("Cancel");
    button_ss_cancel->setId("ssCancel");
    button_ss_cancel->setPosition(316,175);
    button_ss_cancel->setSize(70, 26);
    button_ss_cancel->setBaseColor(baseCol);
    sscancelButtonActionListener = new SsCancelButtonActionListener();
    button_ss_cancel->addActionListener(sscancelButtonActionListener);


    textField_ss = new gcn::TextField("");
    textField_ss->setId("ssText");
    textField_ss->setPosition(10,10);
    textField_ss->setSize(295,22);
    textField_ss->setBaseColor(baseCol);

    savestatelistBox = new gcn::ListBox(&savestateList);
    savestatelistBox->setId("savestateList");
    savestatelistBox->setSize(650,150);
    savestatelistBox->setBaseColor(baseCol);
    savestatelistBox->setWrappingEnabled(true);
    savestatelistBoxActionListener = new SavestatelistBoxActionListener();
    savestatelistBox->addActionListener(savestatelistBoxActionListener);
    savestateScrollArea = new gcn::ScrollArea(savestatelistBox);
    savestateScrollArea->setFrameSize(1);
    savestateScrollArea->setPosition(10,40);
    savestateScrollArea->setSize(295,228);
    savestateScrollArea->setScrollbarWidth(20);
    savestateScrollArea->setBaseColor(baseCol);

    window_savestate = new gcn::Window("SaveState");
    window_savestate->setPosition(120,90);
    window_savestate->setMovable(false);
    window_savestate->setSize(400,300);
    window_savestate->setBaseColor(baseCol);
    window_savestate->setVisible(false);

    window_savestate->add(button_ss_load);
    window_savestate->add(button_ss_save);
    window_savestate->add(button_ss_delete);
    window_savestate->add(button_ss_cancel);
    window_savestate->add(textField_ss);
    window_savestate->add(savestateScrollArea);
}

void menuSaveState_Exit()
{

    delete button_ss_load;
    delete button_ss_save;
    delete button_ss_delete;
    delete button_ss_cancel;
    delete textField_ss;
    delete savestatelistBox;
    delete savestateScrollArea;

    delete window_savestate;
}

void show_settings_SaveState()
{

}

}
