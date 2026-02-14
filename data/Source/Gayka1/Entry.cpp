////////////////////////////////////////////////////////////////////////////////
//
// Класс для работы с хот точками
// ILibHPObject - Интерфейс хот точек
//
////////////////////////////////////////////////////////////////////////////////
#include "Gayka1_pch.h"

#include <memory>
#include <KompasLibraryActions.h>
#include "Doc2DDispatcher.h"
#include "ParameterObject.h"
#include "ParameterObjectProcess.h"
#include "HotPoint.h"
#include "ProcessDispatcher.h"

ksapi::IApplication * application = nullptr;
ProcessDispatcher processDispatcher;


/// Новая гайка ГОСТ 5915.
ParameterObject * NewGayka5915(ksapi::IMacroObjectPtr & obj);
/// Новый болт ГОСТ 7787-81.
ParameterObject * NewBolt7787(ksapi::IMacroObjectPtr & obj);

//------------------------------------------------------------------------------
/**
  Создание класса элемента
*/
//---
ParameterObject * NewCParObject(int comm, ksapi::IMacroObjectPtr & macroObj)
{
  ParameterObject * obj;

  switch (comm)
  {
    case CM_GAYKA5915:
      obj = NewGayka5915(macroObj);
      break;
    case CM_BOLT7787:
      obj = NewBolt7787(macroObj);
      break;
      // Здесь можно вставлять другие элементы
  }

  return obj;
}


//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов.
*/
//---
int32_t GetMenuId()
{
  return ID_LIBID;
}


//------------------------------------------------------------------------------
/**
  Получить идентификаторы инструментальных и компактных панелей
*/
//---
ksapi::IMacroObjectPtr GetFirstSelectedObj()
{
  ksapi::IMacroObjectPtr obj;

  if (application)
  {
    if (ksapi::IKompasDocument2DPtr doc = application->GetActiveDocument())
    {
      std::vector<ksapi::IKompasAPIObjectPtr> selectedObjs;

      if (ksapi::ISelectionManagerPtr manager = doc->GetSelectionManager())
        selectedObjs = manager->GetSelectedObjects();

      if (!selectedObjs.empty())
        obj = selectedObjs[0];
    }
  }

  return obj;
}


//------------------------------------------------------------------------------
/**
  Получить идентификаторы инструментальных и компактных панелей
*/
//---
std::vector<ksapi::IKompasAPIObjectPtr> GetSelectedObjects()
{
  if (ksapi::IKompasDocument2DPtr kDoc2D = application ? application->GetActiveDocument() : nullptr)
  {
    if (ksapi::ISelectionManagerPtr selectionMng = kDoc2D->GetSelectionManager())
      return selectionMng->GetSelectedObjects();
  }

  return {};
}


//------------------------------------------------------------------------------
/**
  Получить номер команды макро
*/
//---
std::int32_t GetMacroCommand(ksapi::IMacroObjectPtr obj)
{
  std::int32_t command = 0;
  ksapi::IUserParametersPtr userParams = obj;

  if (userParams && !userParams->GetLibraryName().compare(LoadStr(ID_LIBID)))
    command = userParams->GetCommand();

  return command;
}


//------------------------------------------------------------------------------
/**
  Головная функция библиотеки
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  if (application && application->GetActiveDocument()) // Получить активный 2D документ
  {
    switch (commandId)
    {
      case ID_VIEW:
      case ID_TOPVID:
      case ID_VIDSEC:
      case ID_LEFT:
      case ID_SIMPLE:
      case ID_AXIS:
      {
        std::vector<ksapi::IKompasAPIObjectPtr> selected = GetSelectedObjects();

        for (size_t i = 0, count = selected.size(); i < count; i++)
        {
          ksapi::IMacroObjectPtr macro = selected[i];

          if (std::int32_t cm = GetMacroCommand(macro))
          {
            // Создаем объект
            if (ParameterObject * obj = NewCParObject(cm, macro))
            {
              obj->ButtonClick(commandId);
              obj->RedrawMacro();
            }
          }
        }
      }
      break;

      case CM_GAYKA5915:
      case CM_BOLT7787:
      {
        processDispatcher.ResetAll(application->GetActiveDocument().Get());

        // Создаем объект
        ksapi::IMacroObjectPtr macroObject = macro::GetEditMacroObject();

        if (ParameterObject * obj = NewCParObject(commandId, macroObject))
        {
          obj->SetEdit(macro::GetEditMacroObject());
          processDispatcher.AddParameterObjectCreation(*obj, commandId);
          processDispatcher.RunActive(ParameterObjectCreation, false, false);
        }
      }
    }
  }
  else
    LibMessage(LoadStr(ID_ERROR_2DDOC), ksMessageError); // Выдать сообщение о ошибке
}


//------------------------------------------------------------------------------
/**
  Получить идентификаторы инструментальных и компактных панелей
*/
//---
bool FillContextPanel(ksapi::IContextPanel & contextPanel) // Индекс панели
{
  bool res = false;

  if (ksapi::IKompasDocument2DPtr doc = application->GetActiveDocument())
  {
    if (ksapi::IMacroObjectPtr macro = GetFirstSelectedObj())
    {
      int command = GetMacroCommand(macro);
      if (command)
      {
        ParameterObject * obj = NewCParObject(command, macro);

        switch (command)
        {
          case CM_GAYKA5915:
          {
            contextPanel.Fill(L"GAYKA5915_BAR");
            int view = ID_VIEW;
            if (obj->IsButtonChecked(ID_TOPVID))
              view = ID_TOPVID;
            else if (obj->IsButtonChecked(ID_SIDEVID))
              view = ID_SIDEVID;
            else if (obj->IsButtonChecked(ID_VIDSEC))
              view = ID_VIDSEC;

            contextPanel.FillComboBoxImage(L"GAYKA5915_VIEW", L"GAYKA5915_VIEW_GROUP", {}, view);

            std::vector<std::int32_t> styles;
            styles.push_back(ksHatchMetal);
            styles.push_back(ksHatchTimber);
            styles.push_back(ksHatchGlass);
            contextPanel.FillComboBoxStyle(L"Gayka1_Hatch", ksControlHatchStyle, styles, ksHatchMetal);

            res = true;
            break;
          }
        }
      }
    }
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Изменение значения комбо-бокса стилей
*/
//---
extern "C" __declspec(dllexport) int ContextPanelStyleComboChanged(wchar_t * styleComboID, long styleType, long newValue)
{
  LibMessage(L"Здесь нужно изменить стиль штриховки у селектированных объектов.");
  return 1;
}


//------------------------------------------------------------------------------
/**
  Состояние команды
*/
//---
int LibCommandState(int32_t commmandId, bool & enable, int32_t & checked, std::wstring & reasonStr)
{
  if (enable)
  {
    ksapi::IKompasDocumentPtr doc = application ? application->GetActiveDocument() : nullptr;
    DocumentTypeEnum type = doc ? doc->GetDocumentType() : ksDocumentUnknown;
    enable = type == ksDocumentDrawing || type == ksDocumentFragment;

    if (enable && checked)
    {
      switch (commmandId)
      {
        case ID_VIEW:
        case ID_TOPVID:
        case ID_SIDEVID:
        case ID_VIDSEC:
        case ID_SIMPLE:
        case ID_AXIS:
        {
          enable = false;

          if (ksapi::IMacroObjectPtr macro = GetFirstSelectedObj())
          {
            if (int cm = GetMacroCommand(macro))
            {
              // Создаем объект
              if (ParameterObject * obj = NewCParObject(cm, macro))
              {
                if (enable = obj->IsButtonEnable(commmandId))
                  checked = obj->IsButtonChecked(commmandId);
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

extern void LoadResources(ksapi::IApplication & app);

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
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libraryActions)
{
  libraryActions.AddGetApplicationVersion(GetApplicationVersion);
  libraryActions.AddRunCommandHandler(RunCommand);
  libraryActions.AddFillContextPanelHandler(FillContextPanel);
  libraryActions.AddGetCommandStateHandler(LibCommandState);
  libraryActions.AddGetMenuIdHandler(GetMenuId);
  libraryActions.AddGetLibraryHotPointsObjectHandler(LibObjInterfaceEntry);
  libraryActions.AddGetLibraryPropertiesObjectHandler(LibObjInterfaceEntry);

  application = &app;
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
}
