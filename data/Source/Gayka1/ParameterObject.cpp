////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "Gayka1_pch.h"

#include <map>
#include "Doc2DDispatcher.h"
#include "ParameterObject.h"
#include "BaseProcess.h"
#include "PropertyManagerObject.h"
#include <filesystem>
#include "ParameterObjectProcess.h"

extern ksapi::IApplication * application;


std::wstring GetPropertyName(int id)
{
  switch (id)
  {
    case ID_PROP_VIEW: // Вид
      return L"ID_PROP_VIEW";
    case ID_PROP_ISPOLN: // Исполнение
      return L"ID_PROP_ISPOLN";
    case ID_PROP_BOLT_VIEW:
      return L"ID_PROP_BOLT_VIEW";
  }

  return L"";
}


//------------------------------------------------------------------------------
/**
  Базовые параметры элемента
*/
//---
BaseMacroParam::BaseMacroParam()
{
  memset(&version, 0, sizeof(BaseMacroParam));
  drawType = ID_VIEW;
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
ParameterObject::ParameterObject(unsigned int _command, ksapi::IMacroObjectPtr macroObj)
  : PropertyManagerObject()
  , command(_command)
  , m_macroObject(macroObj)
  , x(0)
  , y(0)
  , angle(0)
{
  if (macroObj)
  {
    m_flagMode = true;
    macroObj->GetPlacement(x, y, angle, mirrorSymetry);
  }
  else
    m_baseParam.version = GetVersion();
}


ParameterObject::ParameterObject(ParameterObject & other)
{
  Assign(other);
}


//------------------------------------------------------------------------------
/**
  Оператор присваивания
*/
//---
void ParameterObject::Assign(const ParameterObject & other)
{
  command = other.command;
  m_macroObject = other.m_macroObject;
  m_flagMode = other.m_flagMode;
  angle = other.angle;
  m_baseParam = other.m_baseParam;
}


//------------------------------------------------------------------------------
/**
  Создание объекта спецификации
*/
//---
bool ParameterObject::DrawSpcObj()
{
  m_spcObject = nullptr;

  if (m_baseParam.flags.flagAttr && IsSpcObjCreate())
  {
    _MessageBoxResult();
    m_spcObject = EditSpcObj();
  }

  return !m_spcObject.IsEmpty();
}


//------------------------------------------------------------------------------
/**
  Отрисовка фантома
*/
//---
void ParameterObject::DrawPhantom(double x, double y, double angle, bool dynamic)
{
  if (!dynamic)
  {
    // Создание временной группы
    ksapi::IDrawingGroupPtr group = dispatcher.CreateDrawingGroup(true);
    dispatcher.CloseGroup();

    // Меняем местами две группы
    dispatcher.AddObjGroup(group, m_macroObject);
    SetParam();
    mirrorSymetry = false;
    m_macroObject->SetPlacement(x, y, angle, mirrorSymetry, false);
    group->Store();
    DrawSpcObj();
  }
}


//------------------------------------------------------------------------------
/**
  Редактирование объекта спецификации
*/
//---
bool ParameterObject::EditSpcObject()
{
  return m_baseParam.flags.flagAttr && m_spcObject && m_spcObject->Edit();
}


//------------------------------------------------------------------------------
/**
  Изменено положения объекта
*/
//---
bool ParameterObject::PlacementChanged(double _x, double _y, double _angle)
{
  m_macroObject = m_editMode;
  m_macroObject->SetPlacement(_x, _y, _angle, mirrorSymetry, false);
  RedrawMacro();
  m_phantomGroup->Clear(false);

  return true;
}


//------------------------------------------------------------------------------
/**
  Создание элемента
*/
//---
void ParameterObject::Draw1(ksapi::IPhantom2D & phantom)
{
  bool newPh = !m_phantomGroup;

  if (newPh)
  {
    phantom.SetPhantomType(ksMoveGroupPhantom);
    m_phantomGroup = dispatcher.CreateDrawingGroup(true);
    phantom.SetPhantomGroup(m_phantomGroup);
  }
  else
  {
    dispatcher.ClearDrawingGroup(m_phantomGroup);
    dispatcher.OpenDrawingGroup(m_phantomGroup);
  }

  dispatcher.CreateMacroObject(false);
  GetGroup();
  m_macroObject = dispatcher.CloseMacroObject();
  dispatcher.CloseGroup();

  if (!newPh)
    phantom.Update();
}


//------------------------------------------------------------------------------
/**
  Группа для отрисовки в окне
*/
//---
void ParameterObject::RedrawMacro()
{
  if (m_macroObject)
  {
    if (ksapi::IDrawingGroupPtr group = dispatcher.CreateDrawingGroup(true))
    {
      GetGroup(); // геометрия эл-та
      dispatcher.CloseGroup();
      macro::UpdateMacro(m_macroObject, group);
      dispatcher.ClearDrawingGroup(group, false);
      group->Delete();
      SetParam();
    }
  }

  SetUnchanged();
}


//------------------------------------------------------------------------------
/**
  Группа для отрисовки в окне
*/
//---
ksapi::IDrawingGroupPtr ParameterObject::GetPreviewGroup()
{
  dispatcher.CreateDrawingGroup(true);
  RedrawPreview();
  return dispatcher.CloseGroup();
}


//------------------------------------------------------------------------------
/**
  Создать линию выноски
*/
//---
bool ParameterObject::CreateLeader(ProcessState processState)
{
  bool res = false;

  if (processState == ProcessState::processCreateLeader)
  {
    if (application)
    {
      if (ksapi::IKompasDocument2DPtr document = application->GetActiveDocument())
      {
        if (posLeader = document->VisualCreateObject(ksDrPosLeader))
          res = true;
      }
    }

    m_flagMode = false;
  }

  m_spcObject = nullptr;

  return res;
}


//------------------------------------------------------------------------------
/**
  Изменение положения и угла поворота
*/
//---
bool ParameterObject::SetHotPoint(double newX, double newY, int index)
{
  switch (index)
  {
    case 0: // Хотточка перемещения
    {
      SetPlacement(newX, newY, 0.0);
      break;
    }
    case 1: // Хотточка перемещения
    {
      double angle = math::Angle(0, 0, newX, newY);
      SetPlacement(0, 0, angle);
      break;
    }
  }
  return index < 2;
}

//------------------------------------------------------------------------------
/**
  Параметры макро
*/
//---
void ParameterObject::SetParam()
{
  if (m_macroObject)
  {
    m_baseParam.version = GetVersion();
    macro::SetMacroParam(m_macroObject, *this, Size(), L"", L"", command, true, true);
  }
}


//------------------------------------------------------------------------------
/**
  Изменение положения объекта
*/
//---
void ParameterObject::SetPlacement(double newX, double newY, double newAng)
{
  if (m_macroObject)
  {
    m_macroObject->SetPlacement(newX, newY, newAng, mirrorSymetry, true);
    m_macroObject->GetPlacement(x, y, angle, mirrorSymetry);
  }
}


//------------------------------------------------------------------------------
/**
  Создание спецификации
*/
//---
ksapi::ISpecificationBaseObjectPtr ParameterObject::EditSpcObj()
{
  m_spcObject = nullptr;
  ksapi::ISpecificationBaseObjectPtr resSpcObj;

  ksapi::IKompasDocumentPtr kompasDoc(application->GetActiveDocument());
  ksapi::ISpecificationObjectCreateParamPtr objectCreateParam(
    kompasDoc ? kompasDoc->GetInterface(ksSpecificationObjectCreateParam) : nullptr);

  if (!m_spcBaseObjects)
  {
    ksapi::ISpecificationDescriptionsPtr descriptions = kompasDoc->GetSpecificationDescriptions();
    ksapi::ISpecificationDescriptionPtr description = descriptions->GetActive();

    if (!description)
      description = descriptions->Add(L"", 0, L"");

    if (description)
      m_spcBaseObjects = description->GetBaseObjects();
  }

  if (m_macroObject && objectCreateParam)
  {
    m_macroObject->SetName(GetGroupName());

    ksapi::IPropertyKeeperPtr propertyKeeperIn1(m_macroObject);

    objectCreateParam->SetSection(STANDART_SECTION);
    objectCreateParam->SetAttributeNumber(AttrNumber());

    if (m_flagMode && m_spcBaseObjects)
    {
      std::vector<ISpecificationBaseObjectPtr> spcObjs =
        m_spcBaseObjects->GetSpecificationObjectsForGeom({propertyKeeperIn1}, false, STANDART_SECTION, AttrNumber());

      if (spcObjs.size() > 0)
        resSpcObj = spcObjs[0];
    }

    if (m_spcBaseObjects && !m_flagMode)
    {
      if (!resSpcObj)
      {
        // Базовый объект добавляется.
        resSpcObj = m_spcBaseObjects->AddWithParam(objectCreateParam, propertyKeeperIn1, false, {propertyKeeperIn1});
      }
    }

    if (resSpcObj)
    {
      FillSpcObj(resSpcObj);
      resSpcObj->Update();
    }
  }

  return resSpcObj;
}


//------------------------------------------------------------------------------
/**
  Нажата ли кнопка
*/
//---
bool ParameterObject::IsButtonChecked(long buttonID)
{
  bool res = false;
  switch (buttonID)
  {
    case ID_VIEW: // Вид
    case ID_LEFT: // Cбоку
    case ID_TOP:  // Cверху
    case ID_SEC:  // Вид/разрез
      res = m_baseParam.drawType == static_cast<short>(buttonID);
      break;
    case ID_SIMPLE: // Упрощенно
      res = m_baseParam.flags.flagSimp;
      break;

    case ID_AXIS: // Рисовать ось
      res = m_baseParam.flags.flagAxis;
      break;
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Поиск линии-выноски
*/
//---
bool ParameterObject::PosLeader(double x, double y, bool dynamic)
{
  if (!application)
    return false;

  posLeader = nullptr;

  // Ищем объект в текущих координатах курсора
  if (ksapi::IKompasDocumentPtr document = application ? application->GetActiveDocument() : nullptr)
  {
    if (ksapi::IKompasDocument2DPtr document2d = document)
    {
      if (ksapi::IFindObjectParametersPtr findObjParams = document->GetInterface(ksObjectFindObjectParameters))
      {
        findObjParams->SetDrawingObjectType(ksDrPosLeader);
        ksapi::IPositionLeaderPtr newPosLeader = document2d->FindObject(x, y, /*TODO */ 2.5, findObjParams);

        if (!dynamic) // Фиксация
        {
          if (!newPosLeader)
            LibMessage(LoadStr(ID_ERROR_LEADER), ksMessageInformation);
          else
          {                           // Если линия выноски найдена
            posLeader = newPosLeader; // Запоминаем уазатель на линию выноски

            if (oldPosLeader)
              dispatcher.LightObj(oldPosLeader, false); // Убираем подсветку со старой линии выноски

            return false; // Остановить процесс
          }
        }
        else
        {
          // Если нашли новую линию выноски
          if (newPosLeader != oldPosLeader)
          {
            if (oldPosLeader)
              dispatcher.LightObj(oldPosLeader, false); // Убираем подсветку со старой линии выноски

            oldPosLeader = newPosLeader;

            if (oldPosLeader)
              dispatcher.LightObj(oldPosLeader, true); // Подсвечиваем новую линию связи
          }
        }
      }
    }
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Отрисовать позиционную линию выноски
*/
//---
bool ParameterObject::DrawPosLeader()
{
  // Линия выноски есть, подключим ее к объекту спецификации
  if (posLeader)
  {
    // Ввойдем в режим редактирования объекта спецификации
    if (m_spcObject)
    {
      // Подключим линию выноски
      m_spcObject->IncludeGeometry({posLeader}, false);
      // Закроем объект спецификации
      m_spcObject->Update();

      return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
/**
  По индексу свойства получить идентификатор
*/
//---
int ParameterObject::GetPropertyID(int index)
{
  int res = 0;
  return res;
}

extern void * GetModule();

//------------------------------------------------------------------------------
/**
  Добавить свойство в список свойств
*/
//---
bool ParameterObject::GetProperty(int32_t index, IPropertyParam & propertyParam)
{
  propertyParam.SetId(GetPropertyID(index));

  if (propertyParam.GetId())
  {
    propertyParam.SetResModule(GetModule());

    switch (propertyParam.GetId())
    {
      case ID_PROP_ELEMENT_PARAM: // Параметры элемента
      case ID_PROP_HATCH_PARAM:   // Параметры штриховки
      case ID_PROP_VIEW_PARAM:    // Вид
      case ID_PROP_OTHER_PARAM:   // Справочные данные
      {
        propertyParam.SetType(ksOPControlGroup); // Группа
        propertyParam.SetEnable(true);
        break;
      }
      case ID_PROP_DIAM: // Диаметр резьбы
      {
        propertyParam.SetType(ksOPControlListDouble); // Вещественный комбобокс
        propertyParam.SetValue(0.0);
        propertyParam.SetEnable(true);
        propertyParam.SetReadOnly(true);
        break;
      }
      case ID_PROP_HATCH_STEP: // Шаг штриховки
      {
        propertyParam.SetType(ksOPControlEditDouble); // Вещественное значение
        propertyParam.SetValue(0.0);
        propertyParam.SetValue(1.e-002);
        propertyParam.SetValue(100.0);
        break;
      }
      case ID_PROP_HATCH_ANGLE: // Угол штриховки
      {
        propertyParam.SetType(ksOPControlEditDouble); // Вещественное значение
        propertyParam.SetValue(0.0);
        propertyParam.SetValue(-360.0);
        propertyParam.SetValue(360.0);
        break;
      }
      case ID_PROP_VIEW:   // Вид
      case ID_PROP_ISPOLN: // Исполнение
      {
        propertyParam.SetType(ksOPControlFontImageList); // Список с иконками
        propertyParam.SetAdditionText(GetPropertyName(propertyParam.GetId()));
        propertyParam.SetValue(0);
        propertyParam.SetEnable(true);
        break;
      }
      case ID_PROP_SIMPLE: // Упрощенно
      case ID_PROP_AXIS:   // Ось
      {
        propertyParam.SetType(ksOPControlListCheck); // Логическое значение Да \ Нет
        propertyParam.SetValue(false);
        propertyParam.SetEnable(true);
        break;
      }
    }
  }
  return !!propertyParam.GetId();
}


//------------------------------------------------------------------------------
/**
  Обновить параметры свойства
*/
//---
bool ParameterObject::UpdateProperty(IPropertyParam & propertyParam)
{
  // Возможно элемент уже изменили через всплывающую панель или както еще нужно перечитать параметры
  if (m_macroObject && propertyParam.GetId() == GetPropertyID(0))
    macro::GetMacroParam(*this, m_macroObject);

  propertyParam.SetResModule(GetModule());

  switch (propertyParam.GetId())
  {
    case ID_PROP_HATCH_STEP: // Шаг штриховки
    {
      propertyParam.SetEnable(IsHatchEnable());
      propertyParam.SetValue(GetHatchStep());
      break;
    }
    case ID_PROP_HATCH_ANGLE: // Угол штриховки
    {
      propertyParam.SetEnable(IsHatchEnable());
      propertyParam.SetValue(GetHatchAngle());
      break;
    }
    case ID_PROP_VIEW: // Вид
    {
      std::int32_t view = IDB_ID_VIEW;
      switch (m_baseParam.drawType)
      {
        case ID_VIEW:
          view = IDB_ID_VIEW;
          break; // Вид
        case ID_TOPVID:
          view = IDB_ID_TOPVID;
          break; // Вид сверху
        case ID_LEFT:
          view = IDB_ID_SIDEVID;
          break; // Вид сбоку
        case ID_VIDSEC:
          view = IDB_ID_VIDSEC;
          break; // Вид разрез
      }
      propertyParam.SetValue(view);
      break;
    }
    case ID_PROP_SIMPLE: // Упрощенно
    {
      propertyParam.SetValue(!!m_baseParam.flags.flagSimp);
      break;
    }
    case ID_PROP_AXIS: // Ось
    {
      propertyParam.SetValue(!!m_baseParam.flags.flagAxis);
      break;
    }
    default:
      return false;
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Изменить свойство
*/
//---
bool ParameterObject::ApplyProperty(IPropertyParam & propertyParam)
{
  bool res = false;
  auto value = propertyParam.GetValue();

  switch (propertyParam.GetId())
  {
    case ID_PROP_HATCH_STEP: // Шаг штриховки
    {
      auto value = propertyParam.GetValue();

      SetHatchStep(value.GetDouble());
      res = true;

      break;
    }
    case ID_PROP_HATCH_ANGLE: // Угол штриховки
    {
      SetHatchAngle(value.GetDouble());
      res = true;

      break;
    }
    case ID_PROP_VIEW: // Вид
    {
      short view = ID_VIEW;

      switch (value.GetInt())
      {
        case IDB_ID_VIEW:
          view = ID_VIEW;
          break; // Вид
        case IDB_ID_TOPVID:
          view = ID_TOPVID;
          break; // Вид сверху
        case IDB_ID_SIDEVID:
          view = ID_LEFT;
          break; // Вид сбоку
        case IDB_ID_VIDSEC:
          view = ID_VIDSEC;
          break; // Вид разрез
      }

      m_baseParam.drawType = view;
      res = true;

      break;
    }
    case ID_PROP_SIMPLE: // Упрощенно
    {
      m_baseParam.flags.flagSimp = value.GetBool();
      res = true;

      break;
    }
    case ID_PROP_AXIS: // Ось
    {
      m_baseParam.flags.flagAxis = value.GetBool();
      res = true;

      break;
    }
  }

  if (res)
    SetChanged();

  return res;
}


extern int GetDoubleColumnDataCallback(void * result, int count, char ** rowData, char **)
{
  std::vector<double> * diameters = reinterpret_cast<std::vector<double> *>(result);

  if (count == 1)
    diameters->push_back(std::stod(rowData[0]));

  return 0;
}


//------------------------------------------------------------------------------
/**
  Возвращает полное имя файла
*/
//---
bool GetFullName(const std::wstring & inName, std::wstring & outName)
{
  bool res = false;

  outName = L"";

  if (ILibraryManagerPtr libMng = application->GetLibraryManager())
  {
    if (IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
    {
      if (IProceduresLibraryPtr lib = procLibs->GetItemByName(L"Gayka1"))
        outName = lib->GetFullPath();
    }
  }

  if (!outName.empty())
  {
    auto fullName = std::filesystem::path(outName).parent_path();
    res = std::filesystem::exists(fullName.append(inName));

    if (!res && application)
    {
      std::wstring outPath;

      if (ksapi::ISystemSettingsPtr settings = application->GetSystemSettings())
      {
        fullName = settings->GetSystemPath(ksSystemFiles);
        fullName.append(inName);
      }
    }

    if (res = std::filesystem::exists(fullName))
      outName = fullName.wstring();
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Сообшение о ошибке
*/
//---
void _MessageBoxResult()
{
  if (ksapi::IKompasErrorPtr error = application ? application->GetKompasError() : nullptr)
  {
    if (std::int32_t res = error->GetCode())
    {
      if (res != etError10) //  10 - Ошибка! Вырожденный объект
        error->Report();
      error->Clear();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Получить библиотечный шрифт
*/
//---
std::wstring GetLibraryIconFont()
{
  static std::wstring font;

  if (!font.size() && application)
  {
    if (ksapi::ILibraryManagerPtr libMng = application->GetLibraryManager())
    {
      if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
      {
        if (ksapi::IProceduresLibraryPtr procLib = procLibs->GetItemByName(LoadStr(ID_LIBID)))
          font = procLib->GetIconsFont();
      }
    }
  }

  return font;
}
