#include "LeaderBuildProcess.h"

#include "Gayka1_pch.h"
#include "Resource.h"
#include "ParameterObject.h"
#include "Gayka5915Process.h"
#include "BoltMainProcess.h"
#include "Gayka.h"
#include "Bolt.h"
#include "ProcessDispatcher.h"

extern ksapi::IApplication * application;
extern ProcessDispatcher processDispatcher;

//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
LeaderBuildProcess::LeaderBuildProcess(ProcessState & processState, ParameterObject * leaderBaseObj)
  : BaseProcess()
  , m_processState(processState)
  , m_leaderBaseObject(leaderBaseObj)
{
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
LeaderBuildProcess::~LeaderBuildProcess()
{
}


//------------------------------------------------------------------------------
/**
  Получить базовый объект процесса
*/
//---
ParameterObject * LeaderBuildProcess::GetProcessObject()
{
  return m_leaderBaseObject;
}


void LeaderBuildProcess::SetRunProcessCallBack(RunBaseProcessCallBack callback)
{
  m_baseProcessRunFunc = callback;
}

//------------------------------------------------------------------------------
/**
  Инициализация параметров процесса установки линии выноски
*/
//---
bool LeaderBuildProcess::InitPosLeaderParam()
{
  // Если для объекта есть возможность создания объекта спецификации
  if (application && !m_positionLeaderParam)
  {
    try
    {
      // Получаем параметры процессы
      if (m_positionLeaderParam = application->CreateProcessParam())
      {
        ksapi::IPropertiesManagerEventsPtr events = m_positionLeaderParam->Events();

        if (!events)
          return false;

        std::wstring handlerOwner = L"Gayka1";

        events->AddButtonClickHandler(handlerOwner, [this](std::int32_t buttonId) { return OnButtonClick(buttonId); });
        events->AddChangeControlValueHandler(handlerOwner,
                                             [this](ksapi::IPropertyControlPtr ctrl) { OnChangeControlValue(ctrl); });
        events->AddControlCommandHandler(handlerOwner, [this](ksapi::IPropertyControlPtr control, std::int32_t buttonId)
                                         { OnControlCommand(control, buttonId); });

        m_positionLeaderParam->SetSpecToolbar(pnEnterEscHelp); // Тип кнопок на панели свойств

        // Получаем коллекцию закладок панели свойств
        ShowControls();
      }
    }
    catch (wchar_t * mes)
    {
      LibMessage(mes, ksMessageInformation); // Вывод сообщений о ошибке
    }
  }

  return !m_positionLeaderParam.IsEmpty();
}


//------------------------------------------------------------------------------
/**
  Запуск процесса установки линии выноски
*/
//---
void LeaderBuildProcess::InitLeaderProcess()
{
  if (m_leaderBaseObject && m_leaderBaseObject->IsSpcObjCreate() && InitPosLeaderParam())
  {
    if (application && !m_leaderProcess2D)
    {
      if (ksapi::IKompasDocument2DPtr cursorDoc2D = application->GetActiveDocument())
      {
        m_leaderProcess2D = cursorDoc2D->GetLibProcess(ksProcess2DCursor);

        if (ksapi::IProcessPtr leaderProcess = m_leaderProcess2D)
        {
          leaderProcess->SetProcessParam(m_positionLeaderParam);
          ksapi::IProcess2DEventsPtr events = m_leaderProcess2D->Events();

          if (!events)
            return;

          std::wstring handlerOwner = L"Gayka1";
          events->AddPlacementChangedHandler(handlerOwner, [this](double x, double y, double angle, bool dynamic)
                                             { return OnPlacementChanged(x, y, angle, dynamic); });
          events->AddEndProcessHandler(handlerOwner, [this]() { return OnEndProcess(); });
          events->AddAbortProcessHandler(handlerOwner,
                                         [this]()
                                         {
                                           m_baseProcessRunFunc = nullptr;
                                           return true;
                                         });
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Запустить процесс создания объекта спецификации
*/
//---
bool LeaderBuildProcess::EditSpcObject()
{
  bool res = false;

  if (m_leaderBaseObject && m_leaderBaseObject->EditSpcObject())
    res = processDispatcher.RunActive(LeaderForSpcCreation, false, true);

  return res;
}


//------------------------------------------------------------------------------
/**
  Отрисовать позиционную линию выноски
*/
//---
void LeaderBuildProcess::DrawPositionLeader()
{
  if (m_leaderBaseObject->DrawPosLeader())
    processDispatcher.PauseActive(LeaderForSpcCreation, GetParentDocument());
}


//------------------------------------------------------------------------------
/**
  Создать линии выноски
*/
//---
bool LeaderBuildProcess::CreateLeader(ProcessState processState)
{
  bool res = m_leaderBaseObject && m_leaderBaseObject->CreateLeader(processState);
  m_processState = ProcessState::processPlacement;

  if (res)
    DrawPositionLeader();

  return res;
}


//------------------------------------------------------------------------------
/**
  Нажатие на кнопку
*/
//---
bool LeaderBuildProcess::ButtonClick(std::int32_t buttonID)
{
  switch (buttonID)
  {
    // Создать новую линию выноски
    case ID_LEADER_CREATE:
    {
      if (application)
      {
        // Остановка текущего процесса.
        processDispatcher.PauseActive(ParameterObjectCreation, GetParentDocument());
        m_processState = ProcessState::processCreateLeader;
      }
      break;
    }
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Изменено положения объекта
*/
//---
bool LeaderBuildProcess::PlacementChanged(double x, double y, double angle, bool dynamic)
{
  if (m_leaderBaseObject)
  {
    m_leaderBaseObject->PosLeader(x, y, dynamic);

    if (!dynamic)
      DrawPositionLeader();
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Завершение процесса
*/
//---
bool LeaderBuildProcess::EndProcess()
{
  if (!m_leaderBaseObject)
    return true;

  bool res = true;

  if (CreateLeader(m_processState))
    Draw1();

  if (res = m_baseProcessRunFunc ? m_baseProcessRunFunc(false, true) : true)
    processDispatcher.ResetActive(LeaderForSpcCreation, GetParentDocument());

  return res;
}


//------------------------------------------------------------------------------
/**
  Запустить процесс
*/
//---
bool LeaderBuildProcess::Run(bool modal, bool postMessage)
{
  m_processState = ProcessState::processSelectLeader;

  if (ksapi::IProcessPtr leaderProcess = m_leaderProcess2D)
    return leaderProcess->Run(modal, postMessage);

  return false;
}


//------------------------------------------------------------------------------
/**
  Отписаться от событий
*/
//---
void LeaderBuildProcess::UnadviseEvents()
{
  if (m_leaderProcess2D)
  {
    if (ksapi::IProcess2DEventsPtr leaderEvents = m_leaderProcess2D->Events())
      leaderEvents->RemoveAllHandlers(L"Gayka1");
  }

  if (m_positionLeaderParam)
  {
    if (ksapi::IPropertiesManagerEventsPtr events = m_positionLeaderParam->Events())
      events->RemoveAllHandlers(L"Gayka1");
  }
}


//------------------------------------------------------------------------------
/**
  Вывод контролов
*/
//---
void LeaderBuildProcess::ShowControls()
{
  if (ksapi::IPropertyTabsPtr liaderPropTabs = m_positionLeaderParam ? m_positionLeaderParam->GetPropertyTabs() : nullptr)
  {
    if (ksapi::IPropertyTabPtr propTab = liaderPropTabs->Add(::LoadStr(IDP_POS_LEADER)))
    {
      if (ksapi::IPropertyControlsPtr ctrlCollection = propTab->GetPropertyControls())
      {
        if (ksapi::IPropertyMultiButtonPtr buttons = ctrlCollection->Add(ksControlMultiButton))
        {
          InitPropertyControl(buttons, ID_LEADER_CREATE);
          buttons->SetButtonsType(ksPushButton);
          buttons->AddButton(ID_LEADER_CREATE, 0xE328);
          buttons->SetButtonTips(ID_LEADER_CREATE, L"Создать линию выноски");
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создать подпроцесс линии выноски
*/
//---
LeaderBuildProcess * CreateLeaderProcess(ProcessState & processState, ParameterObject * processObject)
{
  return new LeaderBuildProcess(processState, processObject);
}
