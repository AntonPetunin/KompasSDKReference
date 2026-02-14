////////////////////////////////////////////////////////////////////////////////
//
// Orifice.cpp - класс Отверстие
//
////////////////////////////////////////////////////////////////////////////////
#include "Orifice.h"

#include <vector>
#include <MacroObj_pch.h>
#include <Events/Process3DEvent.h>

extern ksapi::IApplication * ksAppl;
extern ksapi::IProceduresLibraryPtr myLibrary;
extern void * GetModule();

//-------------------------------------------------------------------------------
// Структура параметров макрообъекта
// ---
struct MacroPar
{
  double diam;
};


//-------------------------------------------------------------------------------
// Подсветить грани
// ---
void SelectFaces(ksapi::IModelObjectPtr face1, ksapi::IModelObjectPtr face2)
{
  if (ksapi::IKompasDocument3DPtr doc3D{ksAppl ? ksAppl->GetActiveDocument() : nullptr})
  {
    if (ksapi::IChooseManagerPtr chMng{doc3D->GetChooseManager()})
    {
      chMng->UnchooseAll();
      chMng->Choose({face1, face2});
    }
  }
}


//-------------------------------------------------------------------------------
// Подсветить грани
// ---
void UnSelectAll()
{
  if (ksapi::IKompasDocument3DPtr doc3D = ksAppl ? ksAppl->GetActiveDocument() : nullptr)
  {
    // Получаем указатель на менеджер выбора
    if (ksapi::IChooseManagerPtr choiseManager = doc3D->GetChooseManager())
      choiseManager->UnchooseAll();
  }
}


//-------------------------------------------------------------------------------
// Конструктор
// ---
Orifice::Orifice(bool forProp)
  : m_refCount(0)
  , m_diameter(5)
  , m_face1(nullptr)
  , m_face2(nullptr)

{
  if (ksapi::IKompasDocument3DPtr doc{ksAppl ? ksAppl->GetActiveDocument() : nullptr})
  {
    m_macroObject = doc->GetLibraryEditObject();

    if (m_macroObject && m_macroObject->IsValid())
    {
      GetUserData();

      if (!forProp)
      {
        if (m_face1 || m_face2)
        {
          if (m_face1 && !m_face1->IsValid())
            m_face1 = nullptr;
          if (m_face2 && !m_face2->IsValid())
            m_face2 = nullptr;

          SelectFaces(m_face1, m_face2);
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Установить первую грань
// ---
void Orifice::SetFace1(ksapi::IModelObjectPtr face)
{
  m_face1 = face;
}


//-------------------------------------------------------------------------------
// Установить вторую грань
// ---
void Orifice::SetFace2(ksapi::IModelObjectPtr face)
{
  m_face2 = face;
}

//-------------------------------------------------------------------------------
// Получить первую грань
// ---
ksapi::IModelObjectPtr Orifice::GetFace1()
{
  return m_face1;
}


//-------------------------------------------------------------------------------
// Получить вторую грань
// ---
ksapi::IModelObjectPtr Orifice::GetFace2()
{
  return m_face2;
}


//-------------------------------------------------------------------------------
//
// ---
std::wstring GetFeatureName(const ksapi::IModelObjectPtr & entity)
{
  std::wstring name;

  if (entity && entity->GetModelObjectType() == o3d_face)
  {
    name = L"Грань.";
    ksapi::IFeaturePtr owner(entity->GetOwner());

    if (owner)
      name += owner->GetName();
  }

  return name;
}


//----------------------------------------------------------------------------------------------
//ksPlacementChanged     Изменение положения.
// ---
bool Orifice::PlacementChanged(const ksapi::IKompasAPIObjectPtr & object)
{
  bool res = false;

  ksapi::IModelObjectPtr modelObject(object);
  ksapi::IKompasDocument3DPtr doc3D(ksAppl ? ksAppl->GetActiveDocument() : nullptr);

  if (doc3D && modelObject && modelObject->GetModelObjectType() == o3d_face)
  {
    // Получаем указатель на менеджер выбора
    // Записываем имя выбранной грани в Edit
    std::int32_t index = m_objectIndex;

    if (ksapi::IPropertyControlPtr edit = GetPropertyControl(index ? ID_FACE2 : ID_FACE1))
    {
      ksapi::IPropertyEditPtr propEdit(edit);
      propEdit->SetStringValue(GetFeatureName(modelObject));
      propEdit->SetWithCheck(true);
      propEdit->SetCheckState(ksCheckCurrent);

      // Грань 2
      if (index == 1)
      {
        // Текущее поле вода активно
        propEdit->SetCheckState(ksCheckCurrent);

        ksapi::IPropertyControlPtr control = GetPropertyControl(ID_FACE1);
        ksapi::IPropertyEditPtr propertyEditAnother(control);
        // Предыдущее поле ввода неактивно
        propertyEditAnother->SetCheckState(ksCheckUndefined);

        SetFace2(modelObject);

        if (modelObject == GetFace1())
          SetFace1(nullptr);
      }
      // Грань 1
      else if (index == 0)
      {
        // Текущее поле вода неактивно
        propEdit->SetCheckState(ksCheckUndefined);

        ksapi::IPropertyEditPtr propertyEditAnother(GetPropertyControl(ID_FACE2));
        // Следующее поле ввода активно
        propertyEditAnother->SetCheckState(ksCheckCurrent);

        SetFace1(modelObject);
        m_objectIndex = 1;

        if (modelObject == GetFace2())
          SetFace2(nullptr);
      }

      res = true;
    }

    SelectFaces(GetFace1(), GetFace2());
  }
  return res;
}


//---------------------------------------------------------------
// ksFilterProc    -Фильтрация объектов
// ---
bool Orifice::FilterProc(const ksapi::IKompasAPIObjectPtr & object)
{
  ksapi::IModelObjectPtr modelObject(object);
  return modelObject && modelObject->GetModelObjectType() == o3d_face;
}


//----------------------------------------------------------------------------------------------
// Наполнение контролов элемента
// ---
void Orifice::ShowControls()
{
  // Создаем Edit для ввода диаметра
  if (ksapi::IPropertySpinEditPtr diameter = CreateControl(ksControlSpinReal))
  {
    InitPropertyControl(diameter, ID_DIAMETER);
    diameter->SetDoubleValue(m_diameter);
  }

  CreateControl(ksControlSeparator);

  // Грань 1
  ksapi::IPropertyEditPtr edit(CreateControl(ksControlEditCheckBox));
  ksapi::IPropertyEditCheckBoxPtr editCheckBox(edit);
  ksapi::IPropertyControlPtr control(edit);

  if (edit)
  {
    InitPropertyControl(edit, ID_FACE1);
    edit->SetReadOnly(false);
    edit->SetWithCheck(true);

    // Установка значения по умолчанию
    editCheckBox->SetDefaultValue(LoadStr(IDP_DEFAULTNAME));
    editCheckBox->SetEnableDeleteValue(true);

    // Поле ввода активно
    edit->SetCheckState(ksCheckCurrent);
    ksapi::IPropertyControlPtr(edit)->SetHyperLinkNameStyle(true);
    ksapi::IModelObjectPtr face1(GetFace1());

    if (face1)
      edit->SetStringValue(GetFeatureName(face1));
  }
  CreateControl(ksControlSeparator);

  // Грань 2
  edit = CreateControl(ksControlEditCheckBox);
  editCheckBox = edit;
  control = edit;

  if (edit)
  {
    InitPropertyControl(edit, ID_FACE2);
    edit->SetReadOnly(false);
    edit->SetWithCheck(true);

    // Установка значения по умолчанию
    editCheckBox->SetDefaultValue(LoadStr(IDP_DEFAULTNAME));
    editCheckBox->SetEnableDeleteValue(true);

    // Поле ввода неактивно
    edit->SetCheckState(ksCheckUndefined);
    control->SetHyperLinkNameStyle(true);
    ksapi::IModelObjectPtr face2 = GetFace2();

    if (face2)
      edit->SetStringValue(GetFeatureName(face2));
  }
}


//----------------------------------------------------------------------------------------------
// Получение типа контекстного меню
// ---
ksProcessContextMenuType Orifice::GetContextMenuType(std::int32_t screenX, std::int32_t screenY)
{
  return ksProcessContextPanel;
}


//----------------------------------------------------------------------------------------------
// Накачка контекстной панели параметрами
// ---
bool Orifice::FillContextPanel(const ksapi::IProcessContextPanelPtr & contextPanel)
{
  if (ksapi::IProcessContextPanelPtr panel = contextPanel)
  {
    if (ksapi::IPropertySpinEditPtr diameter = GetPropertyControl(ID_DIAMETER))
      panel->AddControl(diameter);
  }
  return true;
}


//-------------------------------------------------------------------------------
// Выбор грани
// ---
void Orifice::ChoiceFaces()
{
  // Получаем указатель на активный документ
  ksapi::IKompasDocument3DPtr doc3D(ksAppl ? ksAppl->GetActiveDocument() : nullptr);

  if (doc3D)
  {
    ksapi::IProcessPtr process(doc3D->GetLibProcess(ksProcess3DPlacementAndEntity));
    m_process3D = process;

    if (m_process3D)
    {
      process->SetProcessParam(m_procParam);
      new Process3DEvent(m_process3D, *this);
      process->Run(false, false);
    }
  }
}


//-----------------------------------------------------------------------------
// prControlCommand Нажатие кнопки контрола
// ---
bool Orifice::ControlCommand(const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID)
{
  ksapi::IPropertyEditPtr propertyEdit(ctrl);

  switch (buttonID)
  {
    case 0:
    {
      ksapi::IPropertyControlPtr control(propertyEdit);

      if (control->GetId() == ID_FACE1)
      {
        propertyEdit->SetCheckState(ksCheckCurrent);
        m_objectIndex = 0;

        if (ksapi::IPropertyEditPtr propertyEditAnother = GetPropertyControl(ID_FACE2))
          propertyEditAnother->SetCheckState(ksCheckUndefined);
      }
      else
      {
        propertyEdit->SetCheckState(ksCheckCurrent);
        m_objectIndex = 1;

        if (ksapi::IPropertyEditPtr propertyEditAnother = GetPropertyControl(ID_FACE1))
          propertyEditAnother->SetCheckState(ksCheckUndefined);
      }
      break;
    }
    // Нажатие на "крестик"
    case ksListItemDelete:
    {
      ksapi::IPropertyEditPtr propertyEdit(ctrl);
      ksapi::IPropertyEditCheckBoxPtr checkbox(propertyEdit);
      ksapi::IPropertyControlPtr control(propertyEdit);

      if (std::int32_t index = m_objectIndex == 1)
      {
        propertyEdit->SetCheckState(ksCheckCurrent);

        if (ksapi::IPropertyEditPtr propertyEditAnother = GetPropertyControl(ID_FACE1))
          propertyEditAnother->SetCheckState(ksCheckUndefined);

        SetFace2(nullptr);
      }
      else
      {
        propertyEdit->SetCheckState(ksCheckCurrent);

        if (ksapi::IPropertyEditPtr propertyEditAnother = GetPropertyControl(ID_FACE2))
          propertyEditAnother->SetCheckState(ksCheckUndefined);

        SetFace1(nullptr);
      }

      SelectFaces(GetFace1(), GetFace2());
      break;
    }
  }

  return true;
}


//-------------------------------------------------------------------------------
//prChangeControlValue - Изменение значения контрола
// ---
bool Orifice::OnChangeControlValue(std::int32_t ctrlID, double newVal)
{
  bool res = false;

  switch (ctrlID)
  {
    case ID_DIAMETER:
      m_diameter = newVal;
      res = true;
      break;
  }

  return res;
}


//----------------------------------------------------------------------------------------------
// Завершение процесса
// ---
void Orifice::EndProcess()
{
  // Скрываем панель
  UnSelectAll();
  PropertyManagerObject::EndProcess();
}


bool Orifice::ProcessRun()
{
  if (m_process3D)
  {
    if (ksapi::IPropertyControlPtr diameter = GetPropertyControl(ID_DIAMETER))
    {
      if (ksapi::IProcessWithVariablesPtr varProc = m_process3D)
      {
        if (m_varName.size() > 0)
          varProc->SetControlExpression(diameter, m_varName, GetVariableExpression());
        else
          varProc->SetControlExpression(diameter, L"newTempLen", std::to_wstring(m_diameter));
      }
    }
  }

  return true;
}

std::wstring Orifice::GetVariableExpression()
{
  std::wstring res;
  ksapi::IFeaturePtr macroFeature(m_macroObject);

  if (macroFeature)
  {
    std::vector<ksapi::IFeaturePtr> subFeatures = macroFeature->GetSubFeatures(ksOperTree, true, true);

    if (subFeatures.size() >= 2)
    {
      // Получаем объект дерева посторения
      ksapi::ISketchPtr sketch(subFeatures[0]);

      if (ksapi::IFeaturePtr sketchFeat = sketch)
      {
        if (ksapi::IVariablePtr var = sketchFeat->GetVariableByName(false, false, m_varName))
          res = var->GetExpression();
      }
    }
  }
  return res;
}


//-----------------------------------------------------------------------------
// prChangeControlValue - Событие изменения значения контрола
// ---
bool Orifice::ButtonClick(int32_t buttonID)
{
  bool res = true;

  // Создаем панель
  if (buttonID == pbEnter)
  {
    res = false;
    ksapi::IModelObjectPtr face1 = GetFace1();
    ksapi::IModelObjectPtr face2 = GetFace2();

    if (face1 && face2 && face1 != face2)
    {
      Create(false);
      res = true;
    }
  }

  return res;
}


//-----------------------------------------------------------------------------
// Установка состояния кнопки спецпанели.
// ---
bool Orifice::ButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable)
{
  if (buttonID == pbEnter && enable)
  {
    ksapi::IModelObjectPtr face1 = GetFace1();
    ksapi::IModelObjectPtr face2 = GetFace2();
    enable = face1 && face2 && face1 != face2;
  }

  return true;
}


//-------------------------------------------------------------------------------
// Запись параметров макрообъекта в хранилище
// ---
void Orifice::SetUserData()
{
  if (!m_params)
  {
    if (ksapi::IKompasDocument3DPtr doc = ksAppl->GetActiveDocument())
    {
      if (ksapi::IUserDataStoragesMngPtr storageMng = doc->GetUserDataStoragesMng())
      {
        if (m_macroObject)
        {
          if (ksapi::IPropertyKeeperPtr propertyKeeper{m_macroObject->GetParent()})
          {
            if (ksapi::IUserDataStoragesMngPtr storageMng1 = propertyKeeper->GetUserDataStoragesManager(true))
              storageMng = storageMng1;

            if (storageMng)
            {
              if (ksapi::IUserDataStoragesPtr storages = storageMng->Add(m_macroObject))
              {
                if (ksapi::IUserDataStoragePtr macroPars = storages->Add(LoadStr(IDS_CAPTION)))
                  m_params = macroPars;
              }
            }
          }
        }
      }
    }
  }

  if (m_params)
  {
    std::wstring password = L"";

    m_params->Clear(password);
    m_params->AddValue(password, m_diameter, LoadStr(ID_DIAMETER));

    if (m_face1)
      m_params->AddObjects(password, std::vector<ksapi::IKompasAPIObjectPtr>{m_face1}, LoadStr(ID_FACE1));

    if (m_face2)
      m_params->AddObjects(password, std::vector<ksapi::IKompasAPIObjectPtr>{m_face2}, LoadStr(ID_FACE2));

    m_params->AddValue(password, m_varName, LoadStr(ID_ORIFICE_VAR));
  }
}


//-------------------------------------------------------------------------------
// Получить параметры макрообъекта из хранилища
// ---
void Orifice::GetUserData()
{
  if (ksapi::IKompasDocument3DPtr doc = ksAppl ? ksAppl->GetActiveDocument() : nullptr)
  {
    if (ksapi::IUserDataStoragesMngPtr storageMng = doc->GetUserDataStoragesMng())
    {
      if (m_macroObject)
      {
        if (ksapi::IPropertyKeeperPtr propertyKeeper{m_macroObject->GetParent()})
        {
          if (ksapi::IUserDataStoragesMngPtr storageMng1 = propertyKeeper->GetUserDataStoragesManager(true))
            storageMng = storageMng1;

          if (storageMng)
          {
            if (ksapi::IUserDataStoragesPtr storages = storageMng->GetItemByObject(m_macroObject))
            {
              if (ksapi::IUserDataStoragePtr macroPars = storages->GetItemByName(LoadStr(IDS_CAPTION)))
              {
                {
                  ksapi::StorageValue value;
                  macroPars->GetValueByComment(LoadStr(ID_DIAMETER), value);

                  m_diameter = value.GetDouble();

                  std::vector<ksapi::IKompasAPIObjectPtr> objs;
                  macroPars->GetObjectsByComment(LoadStr(ID_FACE1), objs);

                  if (!objs.empty())
                    m_face1 = objs.front();
                }
                {
                  std::vector<ksapi::IKompasAPIObjectPtr> objs;
                  macroPars->GetObjectsByComment(LoadStr(ID_FACE2), objs);

                  if (!objs.empty())
                    m_face2 = objs.front();
                }
                {
                  ksapi::StorageValue value;
                  macroPars->GetValueByComment(LoadStr(ID_ORIFICE_VAR), value);

                  m_varName = value.GetString();
                }

                m_params = macroPars;
              }
            }
          }
        }
      }
    }
  }
}

#define KS_MAXDOUBLE (1.0E+300) // 1.7976931348623158E+308


//-------------------------------------------------------------------------------
// Удалить все объекты из текущего эскиза
// ---
void ClearCurrentSketch(SketchDispatcher & dispatcher)
{
  if (ksapi::IDrawingGroupPtr gr = dispatcher.CreateDrawingGroup(true))
  {
    dispatcher.CloseGroup();
    gr->AddRegion(ksRTCutFrame, -KS_MAXDOUBLE, -KS_MAXDOUBLE, KS_MAXDOUBLE, KS_MAXDOUBLE);
    dispatcher.DeleteDrawingGroup(gr);
  }
}


//-------------------------------------------------------------------------------
// Создать макрообъект - отверстие
// ---
void Orifice::Create(bool forProp)
{
  m_objectIndex = -1;

  //Получаем диаметр отверстия
  double rad = m_diameter * 0.5;

  //Грани должны быть указаны
  if (m_face1 && m_face2)
  {
    //Получаем интерфейс активного 3D документа
    if (ksapi::IKompasDocument3DPtr doc3D{ksAppl ? ksAppl->GetActiveDocument() : nullptr})
    {
      //Объединять команды в Undo
      doc3D->SetUndoContainer(true);

      // Если нет макрообъекта, создаем
      if (!m_macroObject)
      {
        //Получаем интерфейс детали
        if (ksapi::IPartPtr part = doc3D->GetTopPart())
        {
          //Создаем эскиз
          ksapi::IModelContainerPtr modelCont(part);
          ksapi::ISketchsPtr sketchs(modelCont ? modelCont->GetSketchs() : nullptr);

          //Получаем интерфейс нового эскиза
          if (ksapi::ISketchPtr sketch{sketchs->Add()})
          {
            //Базовая плоскость - одна из указанных граней
            sketch->SetPlane(m_face1);
            //Угол поворота эскиза
            sketch->SetAngle(0);
            //Эскиз создан
            sketch->Update();

            //Редактируем эскиз
            if (ksapi::IKompasDocument2DPtr document = sketch->BeginEdit(false))
            {
              m_sketchDisp.SetSketch(document);

              //Создаем в эскизе окружность
              ksapi::IDrawingObjectPtr circle = m_sketchDisp.CreateCircle(0, 0, rad, 1 /*тип линии - основная*/);
              m_varName = CreateVariable(circle);
              sketch->EndEdit();
            }

            ksapi::IFeaturePtr sketchFeat(sketch);
            ksapi::IProcessWithVariablesPtr varProc(m_process3D);

            if (sketchFeat && varProc)
            {
              if (varProc)
              {
                ksapi::IPropertyControlPtr diameter(GetPropertyControl(ID_DIAMETER));
                ksapi::IVariablePtr variable(sketchFeat->GetVariableByName(false, false, m_varName));

                if (diameter && variable)
                {
                  std::wstring expression = varProc->GetControlExpression(diameter);

                  if (expression.size())
                    variable->SetExpression(expression);
                }
              }
            }

            //Создаем операцию вырезания выдавливанием
            ksapi::IExtrusionsPtr extrusions(modelCont->GetExtrusions());
            ksapi::IExtrusionPtr cutExtrusion(extrusions ? extrusions->Add(o3d_cutExtrusion) : nullptr);

            if (cutExtrusion)
            {
              std::int32_t resCreate = 0;
              //Получаем интерфейс параметров вырезанного элемента выдавливания
              //Направление выдавливания - прямое
              cutExtrusion->SetDirection(dtNormal);
              //Устанавливаем эскиз
              cutExtrusion->SetProfiles({sketch});
              cutExtrusion->SetSideParameters(true, etUpToSurfaceTo, 0, 0, false, m_face2);

              //Операция вырезания выдавливанием создана
              resCreate = cutExtrusion->Update();

              //Если операция создана
              if (resCreate)
              {
                //Создаем макрообъект
                ksapi::IMacroObjects3DPtr macroObjects3D(modelCont->GetMacroObjects3D());
                m_macroObject = macroObjects3D->Add();

                if (m_macroObject)
                {
                  //Макрообъект создан
                  m_macroObject->Update();
                  //Получаем интерфейс параметров макрообъекта

                  if (m_macroObject)
                  {
                    //Устанавливаем видимость объекта
                    m_macroObject->SetStaffVisible(false);

                    //Добавляем в макро вырезанный элемент выдавливания
                    m_macroObject->SetObjects({sketch, cutExtrusion});

                    MacroPar pars;
                    pars.diam = m_diameter;
                    std::wstring modulePath = myLibrary ? myLibrary->GetFullPath() : L"";

                    ksapi::IUserParametersPtr userParam(m_macroObject);
                    userParam->SetCommand(1);
                    userParam->SetLibraryName(LoadStr(IDR_LIBID));
                    userParam->SetLibraryFileName(modulePath);

                    void * params = static_cast<void *>(&pars);
                    const uint8_t * data = static_cast<const uint8_t *>(params);
                    userParam->SetUserParams(std::vector<uint8_t>{data, data + sizeof(pars)});
                    // macroObject->DoubleClickEditable = TRUE; // Запретить редактирование библиотекой
                    m_macroObject->SetPropertyObjectEditable(true);
                  }

                  m_macroObject->Update();
                  // Записываем параметры макрообъекта в хранилище
                  SetUserData();
                }
              }
            }
          }
        }
      }
      // Если редактируем макрообъект
      else
      {
        if (ksapi::IPartPtr part{m_macroObject->GetParent()})
        {
          // Получаем интерфейс параметров макрообъекта
          ksapi::IFeaturePtr macroFeature(m_macroObject);
          std::vector<ksapi::IFeaturePtr> objects = macroFeature->GetSubFeatures(ksOperTree, true, true);

          if (objects.size() >= 2)
          {
            // Получаем объект дерева посторения
            ksapi::ISketchPtr sketch(objects[0]);
            ksapi::IExtrusionPtr cutExtrusion(objects[1]);

            if (sketch)
            {
              sketch->SetPlane(m_face1);
              sketch->Update();
              // Создаем в эскизе окружность
              if (!UpdateVariable(sketch))
              {
                if (ksapi::IKompasDocument2DPtr document = sketch->BeginEdit(false))
                {
                  m_sketchDisp.SetSketch(document);

                  ClearCurrentSketch(m_sketchDisp);
                  ksapi::IDrawingObjectPtr circle = m_sketchDisp.CreateCircle(0, 0, rad, 1 /*тип линии - основная*/);
                  m_varName = CreateVariable(circle);
                  sketch->EndEdit();
                }
              }

              ksapi::IFeaturePtr sketchFeat(sketch);
              ksapi::IProcessWithVariablesPtr varProc(m_process3D);

              if (sketchFeat && varProc)
              {
                if (varProc)
                {
                  ksapi::IPropertyControlPtr diameter(GetPropertyControl(ID_DIAMETER));
                  ksapi::IVariablePtr variable(sketchFeat->GetVariableByName(false, false, m_varName));

                  if (diameter && variable)
                  {
                    std::wstring expression = varProc->GetControlExpression(diameter);

                    if (expression.size())
                      variable->SetExpression(expression);
                  }
                }
              }

              // Устанавливаем глубину выдавливания
              cutExtrusion->SetDirection(dtNormal);
              cutExtrusion->SetSideParameters(true, etUpToSurfaceTo, 0, 0, false, m_face2);
              cutExtrusion->Update();
            }
          }

          // Обновляем макрообъект
          m_macroObject->Update();
          SetUserData();
          doc3D->RebuildDocument();
        }
      }

      // Не объединять команды в Undo
      doc3D->SetUndoContainer(false);
    }
  }
}


//-----------------------------------------------------------------------------
//
// ---
std::wstring Orifice::CreateVariable(const ksapi::ICirclePtr & circle)
{
  std::wstring res;

  if (circle)
  {
    // Тип стрелки у выносной линии ( 0 - стрелки нет, 1 - внутри, 2 - снаружи, 3 - засечка, 4 - точка )
    std::int32_t arrowType = 1;

    // Параметры изображения размера
    // Номер условного значка перед номиналом ( 0 - нет значка, 1 - диаметр, 2 - квадрат,
    // 3 - радиус, > 3 - номер значка из шрифта Symbol type A )
    std::int32_t sign = 1;

    //parRDim.tPar.bitFlag = _AUTONOMINAL;

    // Привязка диаметрального размера
    double xc = circle->GetXc(); // Координаты центра
    double yc = circle->GetYc();
    double rad = circle->GetRadius(); // Радиус

    if (ksapi::IKompasDocument2DPtr activeDoc = ksAppl ? ksAppl->GetActiveDocument() : nullptr)
    {
      // Создание диаметрального размера
      ksapi::IDrawingObjectPtr rDiamDimension = m_sketchDisp.CreateDiamDimension(xc, yc, rad, arrowType, sign);

      if (rDiamDimension)
      {
        rDiamDimension->Associate();

        if (ksapi::IParametricConstraintPtr constraint = rDiamDimension->NewConstraint())
        {
          constraint->SetConstraintType(ksCFixedDim);
          constraint->SetComment(L"Фиксированный размер");
          constraint->Create();
        }

        if (ksapi::IParametricConstraintPtr constraint = rDiamDimension->NewConstraint())
        {
          constraint->SetConstraintType(ksCDimWithVariable);
          constraint->SetComment(L"Диаметр");

          if (constraint->Create())
            res = constraint->GetVariable();
          else
            constraint->Delete();
        }
      }
    }
  }
  return res;
}


//-----------------------------------------------------------------------------
//
// ---
bool Orifice::UpdateVariable(const ksapi::IModelObjectPtr & sketch)
{
  bool res = false;

  if (ksapi::IFeaturePtr sketchFeat{sketch})
  {
    if (ksapi::IVariablePtr variable{sketchFeat->GetVariableByName(false, false, m_varName)})
    {
      variable->SetValue(m_diameter);
      res = true;
    }
  }

  return res;
}


// ILibPropertyObject
//-----------------------------------------------------------------------------
// Имя группы объектов по умолчанию макроэлементы
// ---
std::wstring Orifice::GetGroupName()
{
  return L"Отверстия";
}


bool Orifice::GetProperty(int32_t index, const ksapi::IPropertyParamPtr & propertyParam)
{
  propertyParam->SetId(GetPropertyID(index));
  propertyParam->SetResModule(GetModule());

  switch (propertyParam->GetId())
  {
    case IDS_CAPTION:
    {
      propertyParam->SetType(ksOPControlGroup); // Группа
      propertyParam->SetEnable(true);
      break;
    }
    case ID_DIAMETER:
    {
      propertyParam->SetType(ksOPControlEditDouble);
      propertyParam->SetEnable(true);
      break;
    }
  }

  return propertyParam->GetId();
}

bool Orifice::UpdateProperty(const ksapi::IPropertyParamPtr & propertyParam)
{
  bool res = false;

  switch (propertyParam->GetId())
  {
    case ID_DIAMETER:
    {
      propertyParam->SetValue(m_diameter);
      res = true;
      break;
    }
  }

  return res;
}


std::int32_t Orifice::ApplyProperty(const ksapi::IPropertyParamPtr & propertyParam)
{
  bool res = false;

  switch (propertyParam->GetId())
  {
    case ID_DIAMETER:
    {
      ksapi::PropertyValue value = propertyParam->GetValue();

      m_diameter = value.GetDouble();
      Create(true);

      break;
    }
  }

  return res;
}

bool Orifice::OnChoiceProperty(const ksapi::IPropertyParamPtr & propertyParam)
{
  return false;
}

bool Orifice::GetMouseEnterLeavePoint(int32_t propertyId, int32_t pointIndex, const IMouseEnterLeaveParametersPtr & parameters)
{
  return false;
}

//-----------------------------------------------------------------------------
// Выдать идентификатор свойства по индексу
// ---
std::int32_t Orifice::GetPropertyID(std::int32_t index)
{
  std::int32_t res = 0;
  switch (index)
  {
    case 0:
      res = IDS_CAPTION;
      break;
    case 1:
      res = ID_DIAMETER;
      break;
  }
  return res;
}


//------------------------------------------------------------------------------
// AddRef
// ---
void Orifice::AddRef()
{
  m_refCount++;
}

//------------------------------------------------------------------------------
// Release
// ---
void Orifice::Release()
{
  if (m_refCount)
    m_refCount--;

  if (!m_refCount)
    delete this;
}


ksapi::IAPIObject * Orifice::QueryInterface(ksapi::IAPIObjectIID iid)
{
  ksapi::IAPIObject * res = nullptr;

  if (iid == ksapi::IID_ILibraryObject)
  {
    res = static_cast<ILibraryObject *>(this);
    AddRef();
  }
  if (iid == ksapi::IID_ILibraryPropertiesObject)
  {
    res = static_cast<ILibraryPropertiesObject *>(this);
    AddRef();
  }

  return res;
}


//-------------------------------------------------------------------------------
// Создает соответствующий объект для работы с Hot точками
// ---
ksapi::ILibraryPropertiesObject * LibPropObjectEntry(ksapi::IKompasAPIObjectPtr currentObject)
{
  ksapi::ILibraryPropertiesObject * res = nullptr;

  if (currentObject)
    res = new Orifice(true);

  return res;
}
