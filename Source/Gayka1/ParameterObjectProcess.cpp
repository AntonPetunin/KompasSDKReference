#include "ParameterObjectProcess.h"

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
ParameterObjectProcess::ParameterObjectProcess(ParameterObject & parameterObj)
  : BaseProcess()
  , m_processObject(parameterObj)
  , m_processState(ProcessState::processPlacement)
{
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
ParameterObjectProcess::~ParameterObjectProcess()
{
  delete &m_processObject;
}


//------------------------------------------------------------------------------
/**
  Получить базовый объект процесса
*/
//---
ParameterObject * ParameterObjectProcess::GetProcessObject()
{
  return &m_processObject;
}

//------------------------------------------------------------------------------
/**
  Флаг установлен в true
*/
//---
void ParameterObjectProcess::SetChanged()
{
  m_processObject.SetChanged();
}


//------------------------------------------------------------------------------
/**
  Флаг установлен в false
*/
//---
void ParameterObjectProcess::SetUnchanged()
{
  m_processObject.SetUnchanged();
}


//------------------------------------------------------------------------------
/**
  Получить флаг изменения параметров
*/
//---
bool ParameterObjectProcess::Changed()
{
  return m_processObject.Changed();
}


//------------------------------------------------------------------------------
/**
  Отрисовка фантома
*/
//---
void ParameterObjectProcess::DrawPhantom(double x, double y, double angle, bool dynamic)
{
  if (!dynamic)
  {
    m_processObject.DrawPhantom(x, y, angle, dynamic);

    if (m_processObject.GetBaseParam().flags.flagAttr && m_processObject.GetSpcObject())
      processDispatcher.PauseActive(ParameterObjectCreation, GetParentDocument());
    else
      Draw1();
  }
}


//------------------------------------------------------------------------------
/**
  Инициализация параметров процесса
*/
//---
bool ParameterObjectProcess::InitProcessParam(long toolBarID, SpecPropertyToolBarEnum toolBarType, long firstTabID /*= 0*/)
{
  bool res = false;

  if (application)
  {
    try
    {
      m_procParam = application->CreateProcessParam(); // Получаем параметры процесса

      if (m_procParam)
      {
        ksapi::IPropertiesManagerEventsPtr events = m_procParam->Events();

        if (!events)
          return false;

        std::wstring handlerOwner = L"Gayka1";

        events->AddButtonClickHandler(handlerOwner, [this](std::int32_t buttonId) { return OnButtonClick(buttonId); });
        events->AddChangeControlValueHandler(handlerOwner,
                                             [this](ksapi::IPropertyControlPtr ctrl) { OnChangeControlValue(ctrl); });
        events->AddControlCommandHandler(handlerOwner, [this](ksapi::IPropertyControlPtr control, std::int32_t buttonId)
                                         { OnControlCommand(control, buttonId); });

        if (auto doc = application->GetActiveDocument())
        {
          if (IKompasDocumentEventsPtr docEvents = doc->Events())
            docEvents->AddBeginCloseDocumentHandler(handlerOwner, [this]() { return OnCloseDocument(); });
        }

        // Получаем интерфейс панели свойств
        m_procParam->SetSpecToolbar(toolBarType);      // Тип кнопок на панели свойств
        m_procParam->SetCaption(::LoadStr(toolBarID)); // Устанавливаем заголовок панели свойств

        bool flagMode = m_processObject.GetFlagMode();
        m_procParam->SetAutoReduce(!flagMode); // Автосоздание объекта

        if (flagMode)
          m_procParam->SetDefaultControlFix(ksAllFix);

        // Получаем коллекцию закладок панели свойств
        m_propTabs = m_procParam->GetPropertyTabs();

        // Создаем закладку параметров элемента
        if (firstTabID)
        {
          CreateTab(firstTabID, true, true);

          // Наполнение закладки контролами для вывода параметров элемента
          ShowControls();
          // Создать окно просмотра
          // Отрисовка группы в окне просмотра
          if (IDrawingGroupPtr previewGroup = m_processObject.GetPreviewGroup())
          {
            if (m_curentCollection)
              m_slideBox = m_curentCollection->Add(ksControlSlideBox);

            if (m_slideBox)
            {
              m_slideBox->SetGroup(previewGroup);
              m_slideBox->SetId(10000);
              m_slideBox->SetName(::LoadStr(IDS_SLIDEBOX));
              m_slideBox->UpdateParam();
            }
          }
        }

        int paramCount = m_processObject.ParamCount(); // Нужно ли выводить грид?

        if (paramCount || m_processObject.IsHatchObject())
        {
          CreateTab(IDP_ELEMENT_PARAM); // Грид будем создавать на новой закладке

          if (m_processObject.IsHatchObject())
            ShowHatchControl();

          if (paramCount)
          {
            // Создание грида
            if (m_curentCollection)
              m_propGrid = m_curentCollection->Add(ksControlGrid);

            if (m_propGrid)
            {
              m_propGrid->SetColumnCount(2);           // Количество колонок грида
              m_propGrid->SetFixedColumnCount(1);      // Первая колонка редактироваться не будет
              m_propGrid->SetFixedRowCount(1);         // Заголовок таблицы
              m_propGrid->SetRowCount(paramCount + 1); // Количество отображаемых строк в гриде
              m_propGrid->SetId(10001);
              m_propGrid->SetName(::LoadStr(IDP_ELEMENT_PARAM));
              // Выводим надписи
              SetGridRowIndex(0); // Индекс строки
              AddStringToGrig(IDS_PARAM_CAPTION, ::LoadStr(IDS_VALUE_CAPTION));
              ShowParam(); // Вывод параметров в гриде
              m_propGrid->SetHeight(GetGridRowIndex() * 20);
              m_propGrid->SetRowCount(GetGridRowIndex());
            }
          }
        }

        res = true;
      }
    }
    catch (wchar_t * mes)
    {
      LibMessage(mes, ksMessageInformation); // Вывод сообщений о ошибке
    }
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Запуск процесса установки объекта
*/
//---
void ParameterObjectProcess::InitPlacementProcess()
{
  ksapi::IKompasDocument2DPtr doc2D(application ? application->GetActiveDocument() : nullptr);

  if (doc2D)
  {
    m_process = doc2D->GetLibProcess(ksProcess2DPlacement);
    m_process2D = m_process;

    ksapi::IPhantom2DPtr phantom(m_process2D->GetPhantom2D());

    if (m_process)
    {
      m_process2D->SetX(m_processObject.GetX());
      m_process2D->SetY(m_processObject.GetY());
      m_process2D->SetAngle(m_processObject.GetAngle());

      m_process->SetProcessParam(m_procParam);

      Draw1();

      ksapi::IProcess2DEventsPtr events = m_process2D->Events();

      if (!events)
        return;

      std::wstring handlerOwner = L"Gayka1";

      events->AddPlacementChangedHandler(handlerOwner, [this](double x, double y, double angle, bool dynamic)
                                         { return OnPlacementChanged(x, y, angle, dynamic); });
      events->AddEndProcessHandler(handlerOwner, [this]() { return OnEndProcess(); });
      events->AddAbortProcessHandler(handlerOwner,
                                     [this]()
                                     {
                                       m_processState = ProcessState::processAborted;
                                       return true;
                                     });
    }
  }
}


//------------------------------------------------------------------------------
/**
  Получить фантом
*/
//---
ksapi::IPhantom2DPtr ParameterObjectProcess::GetPhantom2D()
{
  return m_process2D ? m_process2D->GetPhantom2D() : nullptr;
}


//------------------------------------------------------------------------------
/**
  Изменено положения объекта
*/
//---
bool ParameterObjectProcess::PlacementChanged(double x, double y, double angle, bool dynamic)
{
  if (application)
  {
    if (ksapi::IKompasDocument2DPtr document = application->GetActiveDocument())
      document->SetUndoContainer(true);
  }

  if (m_processObject.IsEditMode()) // Редактирование
  {
    m_processObject.PlacementChanged(x, y, angle);
  }
  else if (m_processState == ProcessState::processPlacement)
  {
    DrawPhantom(x, y, angle, dynamic);
  }
  else
  {
    if (BaseProcess * leaderProcess = processDispatcher.GetActive(LeaderForSpcCreation))
      leaderProcess->PlacementChanged(x, y, angle, dynamic);
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Завершение процесса
*/
//---
bool ParameterObjectProcess::EndProcess()
{
  bool res = true;

  if (m_processState == ProcessState::processPlacement)
  {
    if (m_processObject.GetBaseParam().flags.flagAttr && m_processObject.GetSpcObject())
    {
      LeaderBuildProcess * leaderProcess =
        static_cast<LeaderBuildProcess *>(processDispatcher.AddLeaderForSpcCreation(m_processState, &m_processObject));

      if (leaderProcess && m_processState == ProcessState::processPlacement)
      {
        leaderProcess->SetRunProcessCallBack(
          [this](bool modal, bool postMessage)
          {
            bool res = processDispatcher.RunActive(ParameterObjectCreation, modal, postMessage);

            if (res)
            {
              m_specificationCheckBox->SetBoolValue(m_processObject.GetBaseParam().flags.flagAttr);
              SetChanged();
              Draw1();
              RedrawPhantom();
            }

            return res;
          });

        application->PumpWaitingMessages();
        res = !leaderProcess->EditSpcObject();
      }
    }
  }

  if (res)
    processDispatcher.ResetAll(GetParentDocument());

  return res;
}


//------------------------------------------------------------------------------
/**
  Обработчик закрытия документа
*/
//---
bool ParameterObjectProcess::OnCloseDocument()
{
  processDispatcher.PauseActive(ParameterObjectCreation, GetParentDocument());
  return true;
}


//------------------------------------------------------------------------------
/**
  Вывод контролов
*/
//---
void ParameterObjectProcess::ShowControls()
{
  // Создавать объект спецификации
  if (m_processObject.IsSpcObjCreate())
  {
    BaseMacroParam & baseParam = m_processObject.GetBaseParam();

    if (m_specificationCheckBox = CreateCheckBox(baseParam.flags.flagAttr))
      InitPropertyControl(m_specificationCheckBox, IDP_SPC_OBJ);
  }
}


//------------------------------------------------------------------------------
/**
  Вывод контрола штриховки
*/
//---
void ParameterObjectProcess::ShowHatchControl()
{
  if (m_processObject.IsHatchObject())
  {
    bool hEnable = m_processObject.IsHatchEnable();

    if (m_curentCollection)
    {
      if (ksapi::IPropertyControlPtr groupBegin = m_curentCollection->Add(ksControlGroupBegin))
      {
        groupBegin->SetName(LoadStr(IDP_HATCH_PARAM));

        ksapi::IPropertyEditPtr hatchAngle = CreateRealEdit(m_processObject.GetHatchAngle(), -360, 360, ksControlEditAngle);
        ksapi::IPropertyEditPtr hatchStep = CreateRealEdit(m_processObject.GetHatchStep(), 0.01, 100, ksControlEditReal);

        if (hatchAngle)
          InitPropertyControl(hatchAngle, IDP_HATCHANG_ANGLE, true, hEnable);
        if (hatchStep)
          InitPropertyControl(hatchStep, IDP_HATCHANG_STEP, true, hEnable);

        m_curentCollection->Add(ksControlGroupEnd);
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Запустить процесс
*/
//---
bool ParameterObjectProcess::Run(bool modal, bool postMessage)
{
  return m_process && m_process->Run(modal, postMessage);
}


//------------------------------------------------------------------------------
/**
  Отписаться от событий
*/
//---
void ParameterObjectProcess::UnadviseEvents()
{
  if (m_process2D)
  {
    if (ksapi::IProcess2DEventsPtr events2d = m_process2D->Events())
      events2d->RemoveAllHandlers(L"Gayka1");
  }

  if (m_procParam)
  {
    if (ksapi::IPropertiesManagerEventsPtr events = m_procParam->Events())
      events->RemoveAllHandlers(L"Gayka1");
  }

  if (auto parent = GetParentDocument())
  {
    if (ksapi::IKompasDocumentEventsPtr events = parent->Events())
      events->RemoveAllHandlers(L"Gayka1");
  }
}

//------------------------------------------------------------------------------
/**
  Перерисовать фантом
*/
//---
void ParameterObjectProcess::RedrawPhantom()
{
  if (Changed())
  {
    if (m_process)
      m_process->Update();

    // Перезаполняем грид
    if (m_propGrid)
    {
      m_rowIndex = 1;
      ShowParam();
      m_propGrid->SetRowCount(m_rowIndex);
      m_propGrid->UpdateParam();
    }

    // Перерисовываем группу в окне
    if (m_slideBox)
    {
      // Рисуем новую
      ksapi::IDrawingGroupPtr group = m_processObject.GetPreviewGroup();
      m_slideBox->SetGroup(group);
      m_slideBox->UpdateParam();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Процесс создания элемента
*/
//---
void ParameterObjectProcess::Draw1()
{
  if (ksapi::IPhantom2DPtr phantom = GetPhantom2D())
    m_processObject.Draw1(*phantom);
}


//------------------------------------------------------------------------------
/**
  Изменить значение контрола
*/
//---
bool ParameterObjectProcess::ChangeControlValue(ksapi::IPropertyControl & ctrl)
{
  bool res = false;

  switch (ctrl.GetId())
  {
    case IDP_HATCHANG_ANGLE:
    {
      if (ksapi::IPropertyEditPtr realEdit = &ctrl)
      {
        if (m_processObject)
        {
          m_processObject.SetHatchAngle(realEdit->GetDoubleValue());
          res = true;
          SetChanged();
        }
      }

      break;
    }

    case IDP_HATCHANG_STEP:
    {
      if (ksapi::IPropertyEditPtr realEdit = &ctrl)
      {
        if (m_processObject)
        {
          m_processObject.SetHatchStep(realEdit->GetDoubleValue());
          res = true;
          SetChanged();
        }
      }

      break;
    }

    case IDP_SPC_OBJ:
    {
      auto & baseParam = m_processObject.GetBaseParam();
      baseParam.flags.flagAttr = !baseParam.flags.flagAttr;
      res = true;
      break;
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Создать процесс установки элемента
*/
//---
ParameterObjectProcess * CreateParameterObjectProcess(ParameterObject & parameterObj)
{
  ParameterObjectProcess * paramObjProcess = nullptr;

  if (ksapi::IKompasDocument * document = application ? application->GetActiveDocument().Get() : nullptr)
  {
    switch (parameterObj.GetCommand())
    {
      case CM_GAYKA5915:
        if (Gayka5915 * gayka = static_cast<Gayka5915 *>(&parameterObj))
          paramObjProcess = new Gayka5915Process(*gayka);
        break;

      case CM_BOLT7787:
        if (BoltMain * bolt = static_cast<BoltMain *>(&parameterObj))
          paramObjProcess = new BoltMainProcess(*bolt);
        break;
    }
  }

  return paramObjProcess;
}
