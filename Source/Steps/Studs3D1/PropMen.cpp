////////////////////////////////////////////////////////////////////////////////
//
// Класс для работы с панелью свойств
//
////////////////////////////////////////////////////////////////////////////////
#include "PropMen.h"

#include "Resource.h"
#include "format"

extern IApplication * kompasApp;
extern void * GetModule();

//------------------------------------------------------------------------------
//
// ---
std::wstring LoadStr(int32_t id); // Вспомогательная функция, загрузить строку из ресурса


//----------------------------------------------------------------------------------------------
// Конструктор
// ---
PropertyManagerObject::PropertyManagerObject()
  : m_procParam(nullptr)        // Параметры процесса
  , m_propTabs(nullptr)         // Коллекция закладок
  , m_curentCollection(nullptr) // Коллекция контролов
  , m_paramGrid(nullptr)        // Грид для вывода дополнительных параметров элемента
  , m_slideBox(nullptr)         // Окно просмотра
  , m_rowIndex(0)               // Индекс строки
  , m_flagMode(0)               // Признак редактирования по дабл-клику
{
}


//----------------------------------------------------------------------------------------------
// Деструктор
// ---
PropertyManagerObject::~PropertyManagerObject()
{
  m_paramGrid = nullptr;        // Грид для вывода дополнительных параметров элемента
  m_slideBox = nullptr;         // Окно просмотра
  m_curentCollection = nullptr; // Коллекция контролов
  m_propTabs = nullptr;         // Коллекция закладок
  m_procParam = nullptr;        // Параметры процесса
  m_rowIndex = 0;               // Индекс строки
  m_flagMode = 0;               // Признак редактирования по дабл-клику
}


//----------------------------------------------------------------------------------------------
// Инициализация параметров процесса
// ---
bool PropertyManagerObject::InitProcessParam(int32_t toolBarID, SpecPropertyToolBarEnum toolBarType, int32_t firstTabID)
{
  bool res = false;

  if (kompasApp)
  {
    m_procParam = kompasApp->CreateProcessParam(); // Получаем параметры процессы
    if (m_procParam)
    {
      new PropertyManagerEvent(m_procParam, *this); // Подписываемся на события процесса

      // Получаем интерфейс панели свойств
      m_procParam->SetSpecToolbar(toolBarType); // Тип кнопок на панели свойств
      std::wstring tmpBstr(LoadStr(toolBarID));
      m_procParam->SetCaption(tmpBstr);        // Устанавливаем заголовок панели свойств
      m_procParam->SetAutoReduce(!m_flagMode); // Автосоздание объекта

      if (m_flagMode)
      {
        m_procParam->SetDefaultControlFix(ksAllFix);
      }

      // Получаем коллекцию закладок панели свойств
      m_propTabs = m_procParam->GetPropertyTabs();

      // Создаем закладку параметров элемента
      if (firstTabID)
      {
        CreateTab(firstTabID, true, true);
        // Наполнение закладки контролами для вывода параметров элемента
        ShowControls();

        // Создать окно просмотра
        m_slideBox = m_curentCollection->Add(ksControlSlideBox);
        m_slideBox->SetSlide(GetSlideID(), GetModule());
        m_slideBox->SetId(10000);
        m_slideBox->SetName(L"Окно просмотра");
      }

      int paramCount = ParamCount(); // Нужно ли выводить грид ?
      if (paramCount)
      {
        // Создание грида
        m_paramGrid = m_curentCollection->Add(ksControlGrid);
        if (m_paramGrid)
        {
          m_paramGrid->SetColumnCount(2);           // Количество колонок грида
          m_paramGrid->SetFixedColumnCount(1);      // Первая колонка редактироваться не будет
          m_paramGrid->SetFixedRowCount(1);         // Заголовок таблицы
          m_paramGrid->SetRowCount(paramCount + 1); // Количество отображаемых строк в гриде
          m_paramGrid->SetId(10001);
          m_paramGrid->SetName(L"Параметры элемента");

          // Выводим надписи
          m_rowIndex = 0;
          AddStringToGrig(IDS_PARAM_CAPTION, LoadStr(IDS_VALUE_CAPTION));

          // Вывод параметров в гриде
          ShowParam();

          // Высота грида
          m_paramGrid->SetHeight(m_paramGrid->GetRowCount() * 20);
        }
      }
      res = true;
    }
  }
  return res;
}


//----------------------------------------------------------------------------------------------
// Добавить параметр в грид
// ---
void PropertyManagerObject::AddStringToGrig(int32_t paramID, const std::wstring & value)
{
  if (m_paramGrid)
  {
    m_paramGrid->SetCellText(m_rowIndex, 1, value);
    m_paramGrid->SetCellText(m_rowIndex++, 0, LoadStr(paramID)); // Не переставлять так задумано
  }
}


//----------------------------------------------------------------------------------------------
// Добавить параметр в грид
// ---
void PropertyManagerObject::AddDoubleToGrig(int32_t paramID, double value)
{
  if (m_paramGrid)
  {
    m_paramGrid->SetCellText(m_rowIndex, 1, std::format(L"{:g}", value)); // Выводим значение
    m_paramGrid->SetCellText(m_rowIndex++, 0, LoadStr(paramID));          // Имя параметра
  }
}


//----------------------------------------------------------------------------------------------
// Создать закладку
// ---
bool PropertyManagerObject::CreateTab(int32_t tabID, bool visible, bool active)
{
  bool res = false;
  if (tabID)
  {
    if (m_propTabs)
    {
      std::wstring tmpBstr(LoadStr(tabID));
      IPropertyTabPtr propTab = m_propTabs->Add(tmpBstr);
      if (propTab)
      {
        m_curentCollection = propTab->GetPropertyControls();
        if (!visible)
          propTab->SetVisible(visible);

        res = true;
      }
    }
  }
  return res;
}


//----------------------------------------------------------------------------------------------
// Создать комбобокс
// ---
void PropertyManagerObject::InitPropertyControl(IPropertyControl * control, int32_t ctrlID, uint32_t hint, uint32_t tips,
                                                bool enable, PropertyControlNameVisibility nameVisibility, bool visible)
{
  if (control)
  {
    std::wstring tmpBstr(LoadStr(ctrlID));
    control->SetName(tmpBstr);
    control->SetId(ctrlID);
    control->SetEnable(enable);
    control->SetVisible(visible);
  }
}


//----------------------------------------------------------------------------------------------
// Создать комбобокс
// ---
IPropertyListPtr PropertyManagerObject::CreateRealList(double minVal, double maxVal)
{
  IPropertyListPtr realList;

  if (m_curentCollection)
  {
    realList = m_curentCollection->Add(ksControlListReal);
    if (realList)
    {
      if (minVal != maxVal)
        realList->SetValueRange(minVal, maxVal);
      realList->SetReadOnly(true);
    }
  }

  return realList;
}


//----------------------------------------------------------------------------------------------
// Добавление комбобокса строковых значений
// ---
IPropertyListPtr PropertyManagerObject::CreateStringList()
{
  IPropertyListPtr stringList;

  if (m_curentCollection)
  {
    stringList = m_curentCollection->Add(ksControlListStr);
    if (stringList)
      stringList->SetReadOnly(true);
  }

  return stringList;
}

//----------------------------------------------------------------------------------------------
// Добавление комбобокса целых значений
// ---
IPropertyListPtr PropertyManagerObject::CreateIntList()
{
  IPropertyListPtr intList;

  if (m_curentCollection)
  {
    intList = m_curentCollection->Add(ksControlListInt);
    if (intList)
      intList->SetReadOnly(true);
  }

  return intList;
}


//----------------------------------------------------------------------------------------------
// Создать сепаратор
// ---
IPropertySeparatorPtr PropertyManagerObject::CreateSeparator(SeparatorTypeEnum type)
{
  IPropertySeparatorPtr rSepar;

  if (m_curentCollection)
    rSepar = m_curentCollection->Add(ksControlSeparator);

  return rSepar;
}


//----------------------------------------------------------------------------------------------
// Добавление CheckBox-а
// ---
IPropertyCheckBoxPtr PropertyManagerObject::CreateCheckBox(bool checked)
{
  IPropertyCheckBoxPtr check(nullptr);

  if (m_curentCollection)
  {
    check = m_curentCollection->Add(ksControlCheckBox);

    if (check)
      check->SetBoolValue(checked);
  }
  return check;
}


//----------------------------------------------------------------------------------------------
// Создать набор кнопок
// ---
IPropertyMultiButtonPtr PropertyManagerObject::CreateMultiButton(ButtonTypeEnum type)
{
  IPropertyMultiButtonPtr buttons;

  if (m_curentCollection)
  {
    buttons = m_curentCollection->Add(ksControlMultiButton);

    if (buttons)
      buttons->SetButtonsType(type);
  }
  return buttons;
}


//----------------------------------------------------------------------------------------------------------------------
// Добавить кнопку в набор
// ---
void PropertyManagerObject::AddButton(IPropertyMultiButtonPtr & buttons, int32_t btnID, bool cheched, bool enable)
{
  AddButton2(buttons, btnID, btnID, cheched, enable);
}


//-----------------------------------------------------------------------------------------------------------------------------------
// Добавить кнопку в набор
// ---
void PropertyManagerObject::AddButton2(IPropertyMultiButtonPtr & buttons, int32_t btnID, int32_t bmpID, bool cheched, bool enable)
{
  if (buttons)
  {
    buttons->AddButton(btnID, bmpID, -1);
    buttons->SetButtonEnable(btnID, enable);
    buttons->SetButtonChecked(btnID, cheched);
    buttons->SetButtonTips(btnID, LoadStr(btnID));
    buttons->SetButtonIconFont(btnID, GetLibraryIconFont());
  }
}


//----------------------------------------------------------------------------------------------
// Добавить кнопку в набор
// ---
bool PropertyManagerObject::EndProcess()
{
  UnadviseEvents();
  return true;
}

void PropertyManagerObject::UnadviseEvents()
{
  BaseEvent::TerminateEvents(m_procParam);
}


//----------------------------------------------------------------------------------------------
// Найти контрол
// ---
IPropertyControlPtr PropertyManagerObject::GetPropertyControl(int32_t ctrlID)
{
  IPropertyControlPtr control;

  if (m_propTabs)
  {
    for (int32_t i = 0, c = m_propTabs->GetCount(); i < c && !control; i++)
    {
      IPropertyTabPtr tab(m_propTabs->GetItem(i));
      IPropertyControlsPtr ctrls(tab ? tab->GetPropertyControls() : nullptr);

      if (ctrls)
      {
        std::wstring name(LoadStr(ctrlID));
        control = ctrls->GetItemByName(name);
      }
    }
  }
  return control;
}


//----------------------------------------------------------------------------------------------
// Установить доступность контрола
// ---
void PropertyManagerObject::SetControlEnable(int32_t ctrlID, bool enabled)
{
  IPropertyControlPtr control(GetPropertyControl(ctrlID));
  if (control)
    control->SetEnable(enabled);
}


////////////////////////////////////////////////////////////////////////////////
//
// PropertyManagerEvent - обработчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
PropertyManagerEvent::PropertyManagerEvent(const IProcessParamPtr & manager, PropertyManagerObject & obj)
  : BaseEvent(manager)
  , m_obj(obj)
{
  Advise();
}


//-------------------------------------------------------------------------------
//
// ---
PropertyManagerEvent::~PropertyManagerEvent()
{
  Unadvise();
}


//-------------------------------------------------------------------------------
// Подписаться на события
// ---
void PropertyManagerEvent::Advise()
{
  if (IProcessParamPtr procParam = m_container)
  {
    std::wstring handlerOwner = L"Studs3D1";
    IPropertiesManagerEventsPtr events = procParam->Events();

    if (!events)
      return;

    events->AddButtonClickHandler(handlerOwner, [this](int32_t buttonID) { return ButtonClick(buttonID); });
    events->AddChangeControlValueHandler(handlerOwner,
                                         [this](const IPropertyControlPtr & control) { ChangeControlValue(control); });
    events->AddControlCommandHandler(handlerOwner, [this](const IPropertyControlPtr & control, int32_t buttonId)
                                     { ControlCommand(control, buttonId); });
    events->AddGetContextMenuTypeHandler(handlerOwner, [this](int32_t screenX, int32_t screenY)
                                         { return GetContextMenuType(screenX, screenY); });
    events->AddFillContextPanelHandler(handlerOwner,
                                       [this](const IProcessContextPanelPtr & contextPanel) { FillContextPanel(contextPanel); });
  }
}


//-------------------------------------------------------------------------------
// Отписаться от событий
// ---
void PropertyManagerEvent::Unadvise()
{
  if (IProcessParamPtr procParam = m_container)
    procParam->Events()->RemoveAllHandlers(L"Studs3D1");
}


//-----------------------------------------------------------------------------
// prChangeControlValue - Событие изменения значения контрола
// ---
void PropertyManagerEvent::ChangeControlValue(const IPropertyControlPtr & control)
{
  if (control)
  {
    m_obj.OnChangeControlValue(control);
    m_obj.RedrawPhantom();
  }
}


//-----------------------------------------------------------------------------
// prChangeControlValue - Событие изменения значения контрола
// ---
bool PropertyManagerEvent::ButtonClick(int32_t buttonID)
{
  if (buttonID == pbEnter)
    m_obj.OnButtonClick(buttonID);

  if (buttonID == pbHelp && kompasApp)
  {
    if (ILibraryManagerPtr libMng = kompasApp->GetLibraryManager())
    {
      if (IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
      {
        if (IProceduresLibraryPtr procLib = procLibs->GetItemByName(L"Studs3D1"))
          procLib->OpenHelp(1);
      }
    }
  }

  return true;
}


//-----------------------------------------------------------------------------
// prControlCommand - Нажатие кнопки контрола
// ---
void PropertyManagerEvent::ControlCommand(const IPropertyControlPtr & control, int32_t buttonId)
{
  m_obj.OnButtonClick(buttonId);
  m_obj.RedrawPhantom();
}


//-----------------------------------------------------------------------------
// Callback для получения типа контекстного меню
// ---
ksProcessContextMenuType PropertyManagerEvent::GetContextMenuType(int32_t screenX,
                                                                  int32_t screenY) // LX LY это экранные координаты
{
  return ksProcessContextPanel;
}


//-----------------------------------------------------------------------------
// Callback для накачки контекстной панели
// ---
void PropertyManagerEvent::FillContextPanel(const IProcessContextPanelPtr & contextPanel)
{
}


//------------------------------------------------------------------------------
// Получить библиотечный шрифт
// ---
std::wstring GetLibraryIconFont()
{
  std::wstring font;
  if (kompasApp)
  {
    if (ILibraryManagerPtr libMng = kompasApp->GetLibraryManager())
    {
      if (IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
      {
        if (IProceduresLibraryPtr procLib = procLibs->GetItemByName(L"Studs3D1"))
          font = procLib->GetIconsFont();
      }
    }
  }
  return font;
}


////////////////////////////////////////////////////////////////////////////////
//
// Process3DEvent - обработчик событий от процесса 3D
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
Process3DEvent::Process3DEvent(const IProcess3DPtr & process, PropertyManagerObject & obj)
  : BaseEvent(process)
  , m_obj(obj)
{
  Advise();
}


//-------------------------------------------------------------------------------
//
// ---
Process3DEvent::~Process3DEvent()
{
  delete &m_obj;
  Unadvise();
}


//-------------------------------------------------------------------------------
// Подписаться на события
// ---
void Process3DEvent::Advise()
{
  if (IProcess3DPtr proc3D = m_container)
  {
    std::wstring handlerOwner = L"Studs3D1";
    IProcess3DEventsPtr events = proc3D->Events();

    if (!events)
      return;

    events->AddFilterObjectsHandler(handlerOwner, [this](const IKompasAPIObjectPtr & object) { return FilterObjects(object); });
    events->AddPlacementChangedHandler(handlerOwner, [this](const IModelObjectPtr & object) { return PlacementChanged(object); });
    events->AddCreateTakeObjectHandler(handlerOwner, [this](const IModelObjectPtr & object) { return CreateTakeObject(object); });
    events->AddEndProcessHandler(handlerOwner, [this]() { return EndProcess(); });
    events->AddRunHandler(handlerOwner, [this]() { return Run(); });
    events->AddStopHandler(handlerOwner, [this]() { return Stop(); });
  }
}


//-------------------------------------------------------------------------------
// Отписаться от событий
// ---
void Process3DEvent::Unadvise()
{
  if (IProcess3DPtr proc3D = m_container)
    proc3D->Events()->RemoveAllHandlers(L"Studs3D1");
}


//--------------------------------------
// ksStop - Остановка процесса
// ---
bool Process3DEvent::Stop()
{
  return true;
}


//--------------------------------------
// ksRun - Запуск процесса
// ---
bool Process3DEvent::Run()
{
  return true;
}


//---------------------------------------------------------------
// ksFilterObjects - Фильтрация объектов
// ---
bool Process3DEvent::FilterObjects(const IKompasAPIObjectPtr & object)
{
  return m_obj.FilterObjects(object);
}


//---------------------------------------------------------
// ksPlacementChanged - Изменено положения объекта
// ---
bool Process3DEvent::PlacementChanged(const IModelObjectPtr & object)
{
  return m_obj.PlacementChanged(object);
}


//--------------------------------------
// ksEndProcess - Окончание процесса.
// ---
bool Process3DEvent::EndProcess()
{
  // Завершение процесса
  if (m_obj.EndProcess())
  {
    // Отписка от получения событий
    Disconnect();
    delete this;
  }
  return true;
}


//-----------------------------------------------------------------------------------
// ksProcess3DCreateTakeObject - Событие создания объекта в подпроцессе.
// ---
bool Process3DEvent::CreateTakeObject(const IModelObjectPtr & object)
{
  return m_obj.CreateTakeObject(object);
}


////////////////////////////////////////////////////////////////////////////////
//
// Process3DManipulatorsEvent - обработчик событий от манипуляторов 3D
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
Process3DManipulatorsEvent::Process3DManipulatorsEvent(const IManipulatorsPtr & manipulators, Process3DManipulatorsObject & obj)
  : BaseEvent(manipulators)
  , m_obj(obj)
{
  Advise();
}


//-------------------------------------------------------------------------------
//
// ---
Process3DManipulatorsEvent::~Process3DManipulatorsEvent()
{
  Unadvise();
}


//-------------------------------------------------------------------------------
// Подписаться на события
// ---
void Process3DManipulatorsEvent::Advise()
{
  if (IManipulatorsPtr manipulators = m_container)
  {
    std::wstring handlerOwner = L"Studs3D1";
    IProcess3DManipulatorsEventsPtr events = manipulators->Events();

    if (!events)
      return;

    events->AddRotateManipulatorHandler(handlerOwner,
                                        [this](int32_t manipulatorId, double x0, double y0, double z0, double axisZX,
                                               double axisZXY, double axisZZ, double angle, bool fromEdit)
                                        {
                                          return RotateManipulator(manipulatorId, x0, y0, z0, axisZX, axisZXY, axisZZ, angle,
                                                                   fromEdit);
                                        });
    events->AddMoveManipulatorHandler(handlerOwner,
                                      [this](int32_t manipulatorId, double vX, double vY, double vZ, double delta, bool fromEdit)
                                      { return MoveManipulator(manipulatorId, vX, vY, vZ, delta, fromEdit); });
    events->AddClickManipulatorPrimitiveHandler(handlerOwner, [this](int32_t manipulatorId,
                                                                     ksManipulatorPrimitiveEnum primitiveType, bool doubleClick)
                                                { return ClickManipulatorPrimitive(manipulatorId, primitiveType, doubleClick); });
    events->AddBeginDragManipulatorHandler(handlerOwner, [this](int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType)
                                           { return BeginDragManipulator(manipulatorId, primitiveType); });
    events->AddEndDragManipulatorHandler(handlerOwner, [this](int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType)
                                         { return EndDragManipulator(manipulatorId, primitiveType); });
  }
}


//-------------------------------------------------------------------------------
// Отписаться от событий
// ---
void Process3DManipulatorsEvent::Unadvise()
{
  if (IManipulatorsPtr manipulators = m_container)
    manipulators->Events()->RemoveAllHandlers(L"Studs3D1");
}


//--------------------------------------------------------------------------------------------------------------------------------
// ksRotateManipulator - Поворот манипулятора
// ---
bool Process3DManipulatorsEvent::RotateManipulator(int32_t manipulatorId, double x0, double y0, double z0, double axisZX,
                                                   double axisZXY, double axisZZ, double angle, bool fromEdit)
{
  return m_obj.RotateManipulator(x0, y0, z0, axisZX, axisZXY, axisZZ, angle, fromEdit);
}

//--------------------------------------------------------------------------------------------------------------------------------
// ksMoveManipulator - Передвижение манипулятора
// ---
bool Process3DManipulatorsEvent::MoveManipulator(int32_t manipulatorId, double vX, double vY, double vZ, double delta,
                                                 bool fromEdit)
{
  return m_obj.MoveManipulator(manipulatorId, vX, vY, vZ, delta, fromEdit);
}

//------------------------------------------------------------------------------------------------------------------
// ksClickManipulatorPrimitive - клик по примитиву манипулятора
// ---
bool Process3DManipulatorsEvent::ClickManipulatorPrimitive(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType,
                                                           bool doubleClick)
{
  return m_obj.ClickManipulatorPrimitive(manipulatorId, primitiveType, doubleClick);
}

//--------------------------------------------------------------------------------------------
// ksBeginDragManipulator - начало перетаскивания манипулятора
// ---
bool Process3DManipulatorsEvent::BeginDragManipulator(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType)
{
  return m_obj.BeginDragManipulator(manipulatorId, primitiveType);
}

//---------------------------------------------------------------------------------------------
// ksEndDragManipulator - окончание перетаскивания манипулятора
// ---
bool Process3DManipulatorsEvent::EndDragManipulator(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType)
{
  return m_obj.EndDragManipulator(manipulatorId, primitiveType);
}
