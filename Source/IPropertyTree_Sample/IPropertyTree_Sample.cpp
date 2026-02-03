////////////////////////////////////////////////////////////////////////////////
//
// IPropertyTree_Sample.cpp - Библиотека на Visual C++
//
////////////////////////////////////////////////////////////////////////////////
#include <KompasLibraryActions.h>

#include "IPropertyTree_Sample.h"
#include "PropMan.h"
#include "PropUserEvent.h"

#include <KsAPI.h>


IApplication * kompasApp = nullptr;

extern void * GetModule();
extern void LoadResources(IApplication & app);

//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса.
*/
//---
std::wstring LoadStr(int32_t strId)
{
  if (ILibraryManagerPtr libraryManager = kompasApp->GetLibraryManager())
  {
    if (IProceduresLibrariesPtr proceduresLibraries = libraryManager->GetProceduresLibraries())
    {
      if (IProceduresLibraryPtr lib = proceduresLibraries->GetItemByName(L"IPropertyTree_Sample"))
        if (IKompasDictionaryPtr dictionary = lib->GetDictionary())
          return dictionary->LoadAndConvertLangString(GetModule(), strId);
    }
  }

  return std::wstring();
}


//------------------------------------------------------------------------------
// Вспомогательная функция, вывод сообщения
// ---
int32_t LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons, bool showModal)
{
  if (kompasApp && !str.empty())
    return kompasApp->ShowMessageBox(str, L"", dlgType, dlgButtons, showModal);

  return 0;
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
class LibObj : public PropertyManagerObject,
               public TreeEventCallBack
{
protected:
  IPropertyTreePtr m_tree;       // Дерево
  IPropertyManagerPtr m_propMng; // Менеджер панели свойств
  BaseEvent * m_treeEvent;
  BaseEvent * m_propManEvent;

  bool m_currentNodeRequiredDropAfter;

public:
  LibObj();
  ~LibObj();

protected:
  // Проинициализировать дерево
  void InitTree(IPropertyTreePtr & ctrl);

public:
  void CreateUserPanel();

  virtual bool OnButtonClick(int32_t buttonID) override;

  virtual bool OnExpandOrCollapse(const IPropertyTreeNodePtr & node, bool collapse) override;
  virtual bool OnSelectNodes(const std::vector<IPropertyTreeNodePtr> & nodes, bool selected) override;

  virtual bool OnNodeDblClick(const IPropertyTreeNodePtr & node) override;
  virtual bool OnBeginEditText(const IPropertyTreeNodePtr & node) override;
  virtual bool OnNodeDragStarted(const IPropertyTreeNodePtr & node) override;
  virtual bool OnNodeDragOver(const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode, bool ctrlPressed,
                              bool requiredDropAfter) override;
  virtual bool OnNodeDragDroped(const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode,
                                bool ctrlPressed) override;
  virtual bool OnContextMenuRequest(const IPropertyTreeNodePtr & node) override;
  // Нажатие кнопки контрола.
  virtual void OnControlCommand(int32_t ctrlID, int32_t buttonID) override;

private:
  void CloseTree();
};


std::vector<LibObj *> m_libObjects;

//-----------------------------------------------------------------------------
//
// ---
LibObj::LibObj()
  : m_tree()
  , m_treeEvent(nullptr)
  , m_currentNodeRequiredDropAfter(false)
{
}

//-----------------------------------------------------------------------------
//
// ---
LibObj::~LibObj()
{
}


//-----------------------------------------------------------------------------
//
// ---
void LibObj::CreateUserPanel()
{
  m_propMng = kompasApp->CreatePropertyManager(true);
  if (m_propMng)
  {
    m_propMng->SetLayout(pmAlignRight);
    m_propMng->SetCaption(LoadStr(IDS_PANEL_CAPTION));
    m_propMng->SetSpecToolbar(pnEnterEscHelp);
    m_propManEvent = new PropertyManagerEvent(m_propMng, *this); // Подписываемся на события процесса
    IPropertyTabPtr tab = m_propMng->GetPropertyTabs()->Add(LoadStr(IDS_TAB_CAPTION));

    // Получаем коллекцию контролов
    IPropertyControlsPtr collection = tab->GetPropertyControls();
    m_tree = collection->Add(ksControlTree);
    if (m_tree)
    {
      m_tree->SetName(LoadStr(IDS_TREE));
      m_tree->SetMinHeight(100);
      m_tree->SetResizable(true);
      m_tree->SetId(1000);
      InitTree(m_tree);

      m_treeEvent = new TreeEvent(m_tree, *this);
    }
    m_propMng->ShowTabs();
  }
}

//-----------------------------------------------------------------------------
//
// ---
bool LibObj::OnButtonClick(int32_t buttonID)
{
  if (m_tree && buttonID == pbEnter)
  {
    std::vector<IPropertyTreeNodePtr> nodes = m_tree->GetSelectedNodes();
    LibMessage(LoadStr(IDS_SELECTED_NODES_NUMBER) + std::to_wstring(nodes.size()), ksMessageWarning, ksButtonSetOk, true);

    if (nodes.size() == 1)
    {
      IPropertyTreeNodePtr node = nodes[0];
      if (node)
      {
        LibMessage(LoadStr(IDS_NODE) + (node->IsExpanded() ? LoadStr(IDS_EXPANDED) : LoadStr(IDS_COLLAPSED)) +
                     (node->IsRootNode() ? LoadStr(IDS_ROOT) : LoadStr(IDS_NESTED)),
                   ksMessageWarning, ksButtonSetOk, true);
        LibMessage(LoadStr(IDS_NESTED_NODES_NUMBER) + std::to_wstring(node->GetChildsCount()), ksMessageWarning, ksButtonSetOk,
                   true);
      }
    }
  }
  else
  {
    switch (buttonID)
    {
      case IDM_MENU_COMMAND1:
        LibMessage(LoadStr(IDS_PRESSED_BUTTON1), ksMessageWarning, ksButtonSetOk, true);
        break;
      case IDM_MENU_COMMAND2:
        LibMessage(LoadStr(IDS_PRESSED_BUTTON2), ksMessageWarning, ksButtonSetOk, true);
        break;
      case IDM_MENU_COMMAND3:
        LibMessage(LoadStr(IDS_PRESSED_BUTTON3), ksMessageWarning, ksButtonSetOk, true);
        break;

      case pbEsc:
      {
        CloseTree();
        break;
      }
    }
  }

  return true;
}

//-----------------------------------------------------------------------------
//
// ---
bool LibObj::OnExpandOrCollapse(const IPropertyTreeNodePtr & node, bool collapse)
{
  if (node)
  {
    int64_t nodeData = node->GetNodeData();
    if (nodeData == 3 || nodeData == 2 || nodeData == 10)
      node->SetNodeIcon(collapse ? IDB_ROOT : IDB_ROOTSELECT, L"");

    LibMessage((collapse ? LoadStr(IDS_COLLAPSED_NODE) : LoadStr(IDS_EXPANDED_NODE)) + node->GetText(), ksMessageWarning,
               ksButtonSetOk, true);
  }
  return true;
}

//-----------------------------------------------------------------------------
//
// ---
bool LibObj::OnSelectNodes(const std::vector<IPropertyTreeNodePtr> & nodes, bool selected)
{
  if (nodes.size())
  {
    std::wstring message = selected ? LoadStr(IDS_SELECTED_NODES) : LoadStr(IDS_UNSELECTED_NODES);

    for (size_t i = 0, count = nodes.size(); i < count; ++i)
    {
      if (IPropertyTreeNodePtr node = nodes[i])
      {
        message += node->GetText();
        if (i != count - 1)
          message += L", ";
      }
    }

    LibMessage(message, ksMessageWarning, ksButtonSetOk, true);
  }
  return true;
}

//-----------------------------------------------------------------------------
//
// ---
bool LibObj::OnNodeDblClick(const IPropertyTreeNodePtr & node)
{
  if (node)
    LibMessage(LoadStr(IDS_DOUBLE_CLICKED) + node->GetText(), ksMessageWarning, ksButtonSetOk, true);
  return true;
}

//-----------------------------------------------------------------------------
//
// ---
bool LibObj::OnBeginEditText(const IPropertyTreeNodePtr & node)
{
  if (node && LibMessage(LoadStr(IDS_EDIT_TREE_NODE), ksMessageQuestion, ksButtonSetYesNo, false) == ksButtonYes)
    node->EditText();
  return true;
}

//-----------------------------------------------------------------------------
//
// ---
bool LibObj::OnNodeDragStarted(const IPropertyTreeNodePtr & node)
{
  if (node)
  {
    if (!node->IsRootNode())
      node->SetDragPermitted(true);
  }
  return true;
}

//-----------------------------------------------------------------------------
//
// ---
bool LibObj::OnNodeDragOver(const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode, bool ctrlPressed,
                            bool requiredDropAfter)
{
  m_currentNodeRequiredDropAfter = requiredDropAfter;
  if (node && dragNode && node != dragNode)
  {
    int64_t dragNodeData = dragNode->GetNodeData();
    if (dragNodeData == 3 || dragNodeData == 2)
      node->SetDropPermission(ksDropAfter);
    else
    {
      int64_t nodeData = node->GetNodeData();
      if (nodeData == 3 || nodeData == 2 || nodeData == 10)
        node->SetDropPermission(ksDropInto);
      else
        node->SetDropPermission(ksDropAfter);
    }
  }
  return true;
}

//-----------------------------------------------------------------------------
//
// ---
bool LibObj::OnNodeDragDroped(const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode, bool ctrlPressed)
{
  if (node && dragNode && node != dragNode)
  {
    if (m_currentNodeRequiredDropAfter)
    {
      node->MoveNodes(m_tree->GetSelectedNodes(), dragNode);
    }
    else
    {
      int64_t nodeData = node->GetNodeData();
      if (nodeData == 3 || nodeData == 2 || nodeData == 10)
        node->MoveNodes(m_tree->GetSelectedNodes(), nullptr);
    }
  }
  return true;
}

//-----------------------------------------------------------------------------
//
// ---
bool LibObj::OnContextMenuRequest(const IPropertyTreeNodePtr & node)
{
  if (m_tree)
  {
    ksapi::IMenuPtr menu;
    if (node)
    {
      menu = kompasApp->CreateMenu();
      menu->Load(IDM_TREE_MENU, GetModule());
    }
    m_tree->SetMenu(menu);
  }
  return true;
}

//-----------------------------------------------------------------------------
//
// ---
void LibObj::OnControlCommand(int32_t ctrlID, int32_t buttonID)
{
  if (m_tree && buttonID == IDM_USERMENU_COMMAND1)
  {
    std::vector<IPropertyTreeNodePtr> nodes = m_tree->GetSelectedNodes();
    if (nodes.size())
    {
      auto node = nodes[nodes.size() - 1];
      if (node)
        OnBeginEditText(node);
    }
  }
}


void LibObj::CloseTree()
{
  delete m_treeEvent;
  m_treeEvent = nullptr;
  delete m_propManEvent;
  m_propManEvent = nullptr;

  if (m_tree)
  {
    m_propMng->HideTabs();
    m_propMng = nullptr;
    m_tree = nullptr;
  }
}


//-----------------------------------------------------------------------------
/// Проинициализировать дерево.
// ---
void LibObj::InitTree(IPropertyTreePtr & tree)
{
  IPropertyToolBarPtr toolbar(tree);
  if (toolbar)
  {
    toolbar->AddButton(IDM_MENU_COMMAND1, 0xECA6, -1);
    toolbar->SetButtonEnable(IDM_MENU_COMMAND1, true);
    toolbar->SetButtonTips(IDM_MENU_COMMAND1, LoadStr(IDS_TIPS_BUTTON1));

    toolbar->AddButton(IDM_MENU_COMMAND2, 0xECA7, -1);
    toolbar->SetButtonEnable(IDM_MENU_COMMAND2, false);
    toolbar->SetButtonTips(IDM_MENU_COMMAND2, LoadStr(IDS_TIPS_BUTTON2));

    toolbar->AddButton(IDM_MENU_COMMAND3, 0xECA8, -1);
    toolbar->SetButtonEnable(IDM_MENU_COMMAND3, true);
    toolbar->SetButtonTips(IDM_MENU_COMMAND3, LoadStr(IDS_TIPS_BUTTON3));

    toolbar->AddButton(IDM_MENU_COMMAND4, 0xECA9, -1);
    toolbar->SetButtonVisible(IDM_MENU_COMMAND4, false);
  }

  IPropertyTreeNodePtr rootNode = tree->CreateNode(LoadStr(IDS_ROOT_NODE));
  rootNode->SetNodeData(10);
  if (rootNode)
  {
    rootNode->SetNodeIcon(IDB_ROOT, L"");
    rootNode->SetDropPermission(ksDropInto);

    tree->AddNode(rootNode);
    tree->SetEnableDrag(true);

    auto node1 = tree->CreateNode(LoadStr(IDS_NODE1));
    node1->SetNodeData(1);

    rootNode->AddNode(node1, nullptr);

    auto nodes1 = tree->CreateNode(LoadStr(IDS_FOLDER1));
    nodes1->SetNodeIcon(IDB_ROOT, L"");
    nodes1->SetNodeData(2);

    rootNode->AddNode(nodes1, nullptr);

    auto nodes2 = tree->CreateNode(LoadStr(IDS_FOLDER2));
    nodes2->SetNodeIcon(IDB_ROOT, L"");
    nodes2->SetNodeData(3);

    rootNode->AddNode(nodes2, nodes1);

    auto node2 = tree->CreateNode(LoadStr(IDS_NODE2));
    node2->SetNodeData(4);

    nodes2->AddNode(node2, nullptr);

    nodes2 = tree->FindNodeByName(LoadStr(IDS_FOLDER1));
    if (nodes2)
      nodes2->AddNode(tree->CreateNode(LoadStr(IDS_NODE3)), nullptr);
  }
}

//------------------------------------------------------------------------------
/**
  Выполнение команды
*/
//---
void RunCommand(unsigned int commandId, ksRunCommandModeEnum mode)
{
  if (kompasApp)
  {
    switch (commandId)
    {
      case 1:
        LibObj * obj = new LibObj();
        m_libObjects.push_back(obj);
        obj->CreateUserPanel();
        break;
    }
  }
}

//------------------------------------------------------------------------------
/**
  Получить номер версии приложения.
*/
//---
void GetApplicationVersion(int32_t & major, int32_t & minor, int32_t & release, int32_t & build)
{
  major = BuildVersionMajor;
  minor = BuildVersionMinor;
  release = BuildVersionPatch;
  build = BuildVersionBuild;
}


//------------------------------------------------------------------------------
/**
  Инициализация библиотеки.
*/
//---
APP_EXP_FUNC(bool) LoadKompasLibrary(IApplication & app, IKompasLibraryActions & libaryActions)
{
  libaryActions.AddGetApplicationVersion(GetApplicationVersion);
  libaryActions.AddRunCommandHandler(RunCommand);

  kompasApp = &app;
  LoadResources(app);

  return true;
}


//------------------------------------------------------------------------------
/**
  Освобождения ресурсов перед выгрузкой библиотеки.
*/
//---
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
  for (auto libObj : m_libObjects)
    delete libObj;
  m_libObjects.clear();
  BaseEvent::TerminateEvents();
}
