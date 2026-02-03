#include "OneWindow_pch.h"

#include "Resource.h"

static ksapi::IResourcesModulePtr resourceModule;

void * GetModule()
{
  return &resourceModule;
}

#define MENU_ID(idName)   resourceModule->AddMenuId(L"" #idName, idName);
#define STRING_ID(idName) resourceModule->AddStringId(L"" #idName, idName);

void LoadResources(ksapi::IApplication & app)
{
  resourceModule = app.CreateResourcesModule(L"OneWindow");

  MENU_ID(IDR_LIBID)
  MENU_ID(IDC_COMMAND_1)
  MENU_ID(IDC_COMMAND_2)
  MENU_ID(IDC_COMMAND_3)
  MENU_ID(IDC_COMMAND_4)
  MENU_ID(IDC_COMMAND_5)
  MENU_ID(IDM_TREE_MENU)

  STRING_ID(IDR_LIBID)
  STRING_ID(IDS_BUTTON1_TIPS)
  STRING_ID(IDS_BUTTON2_TIPS)
  STRING_ID(IDS_BUTTON3_TIPS)
  STRING_ID(IDS_FORCES)
  STRING_ID(STR_SELECT_FACES)
  STRING_ID(IDP_FORCE)
  STRING_ID(IDS_FILE_NOT_FOUND)
  STRING_ID(IDS_TREE)
  STRING_ID(IDS_TREE_PANEL)
  STRING_ID(IDS_TREE_TAB)
  STRING_ID(IDS_TEXT_EDIT_INFO)

  resourceModule->End();
}
