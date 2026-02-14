
////////////////////////////////////////////////////////////////////////////////
//
// Studs3D.cpp - Крепежный элемент на Visual C++ - шпилька
//
////////////////////////////////////////////////////////////////////////////////
#include <filesystem>

#include <KompasLibraryActions.h>

#include "Studs3D.h"

#include "DataBase.h"
#include "Resource.h"

//------------------------------------------------------------------------------
//
// ---
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
      if (IProceduresLibraryPtr lib = proceduresLibraries->GetItemByName(libname))
      {
        if (ksapi::IKompasDictionaryPtr dictionary = lib->GetDictionary())
          return dictionary->LoadAndConvertLangString(GetModule(), strId);
      }
    }
  }

  return std::wstring();
}


//------------------------------------------------------------------------------
// Вспомогательная функция, вывод сообщения
// ---
int32_t LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType = ksMessageWarning,
                   ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false)
{
  if (kompasApp && !str.empty())
    return kompasApp->ShowMessageBox(str, L"", dlgType, dlgButtons, showModal);

  return 0;
}


//------------------------------------------------------------------------------
// Вспомогательная функция, вывод сообщения
// ---
int32_t LibMessage(int32_t strId, const std::wstring & appendMessage, ksMessageTypeEnum dlgType = ksMessageWarning,
                   ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false)
{
  return LibMessage(LoadStr(strId).append(appendMessage), dlgType, dlgButtons, showModal);
}


////////////////////////////////////////////////////////////////////////////////
//
// Внекласные функции
//
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Определяет класс точности по ГОСТу
// ---
static short GetClassStrForGost(short gost)
{
  short n = 0;
  switch (gost)
  {
    case 22032:
    case 22034:
    case 22036:
    case 22038:
    case 22040:
    case 22042:
      n = IDS_STUD_B;
      break; // B
    case 22033:
    case 22035:
    case 22037:
    case 22039:
    case 22041:
    case 22043:
      n = IDS_STUD_A;
      break; // A
  }
  return n;
}


//------------------------------------------------------------------------------
// Определяет класс точности по ГОСТу
// ---
short GetGostForTypeAndKlass(short type, short klass)
{
  short n = 0;
  switch (type)
  {
    case 0:
      n = klass ? 22032 : 22033;
      break; // 1d
    case 1:
      n = klass ? 22034 : 22035;
      break; // 1,25 d
    case 2:
      n = klass ? 22036 : 22037;
      break; // 1,6 d
    case 3:
      n = klass ? 22038 : 22039;
      break; // 2d
    case 4:
      n = klass ? 22040 : 22041;
      break; // 2,5 d
    case 5:
      n = klass ? 22042 : 22043;
      break; // b0
  }
  return n;
}


//------------------------------------------------------------------------------
// Определяет длину ввинчиваемого конца по ГОСТу
// ---
static short GetTypeStrForGost(short gost)
{
  short n = 0;
  switch (gost)
  {
    case 22032:
    case 22033:
      n = IDS_STUD_1D;
      break; // 1d
    case 22034:
    case 22035:
      n = IDS_STUD_125D;
      break; // 1,25 d
    case 22036:
    case 22037:
      n = IDS_STUD_16D;
      break; // 1,6 d
    case 22038:
    case 22039:
      n = IDS_STUD_2D;
      break; // 2d
    case 22040:
    case 22041:
      n = IDS_STUD_25D;
      break; // 2,5 d
    case 22042:
    case 22043:
      n = IDS_STUD_B0;
      break; // b0
  }
  return n;
}


//------------------------------------------------------------------------------
// Строка по ГОСТу (Таблица в базе данных)
// ---
static void GetGostLoa(short gost, short & gNumb, short & stNumb)
{
  switch (gost)
  {
    case 22032:
      gNumb = STR_200;
      stNumb = STR_220;
      break;
    case 22033:
      gNumb = STR_201;
      stNumb = STR_221;
      break;
    case 22034:
      gNumb = STR_202;
      stNumb = STR_222;
      break;
    case 22035:
      gNumb = STR_203;
      stNumb = STR_223;
      break;
    case 22036:
      gNumb = STR_204;
      stNumb = STR_224;
      break;
    case 22037:
      gNumb = STR_205;
      stNumb = STR_225;
      break;
    case 22038:
      gNumb = STR_206;
      stNumb = STR_226;
      break;
    case 22039:
      gNumb = STR_207;
      stNumb = STR_227;
      break;
    case 22040:
      gNumb = STR_208;
      stNumb = STR_228;
      break;
    case 22041:
      gNumb = STR_209;
      stNumb = STR_229;
      break;
    case 22042:
      gNumb = STR_210;
      stNumb = STR_230;
      break;
    case 22043:
      gNumb = STR_211;
      stNumb = STR_231;
      break;
    default:
      gNumb = 0;
      stNumb = 0;
  }
}


//------------------------------------------------------------------------------
// Строка по ГОСТу (Название)
// ---
static short GetGostStr(short gost)
{
  short n = 0;
  switch (gost)
  {
    case 22032:
      n = STR_101;
      break;
    case 22033:
      n = STR_102;
      break;
    case 22034:
      n = STR_103;
      break;
    case 22035:
      n = STR_104;
      break;
    case 22036:
      n = STR_105;
      break;
    case 22037:
      n = STR_106;
      break;
    case 22038:
      n = STR_107;
      break;
    case 22039:
      n = STR_108;
      break;
    case 22040:
      n = STR_109;
      break;
    case 22041:
      n = STR_110;
      break;
    case 22042:
      n = STR_111;
      break;
    case 22043:
      n = STR_112;
      break;
  }
  return n;
}


//------------------------------------------------------------------------------
// Присваивает переменной с именем varName значение val
// ---
void SetVarValue(std::vector<IVariablePtr> & vars, const std::wstring & varName, double val)
{
  for (IVariablePtr var : vars)
  {
    if (var->GetName() == varName)
      var->SetValue(val);
  }
}


//------------------------------------------------------------------------------
// Определяет является ли объект поверхностью ( planar = true - плоскостью,
// planar = false - конической поверхностью )
// ---
bool IsSurface(IModelObjectPtr entity, bool planar = true, double * = nullptr)
{
  bool res = false;
  // КЕ K8+ после второго двойного клика мы считаем, что создаем новую шпильку и все время пытаемся мэйтить ее с собой
  if (entity && entity->IsValid())
  {
    if (IFacePtr faceDef = entity)
    {
      if (planar && faceDef->IsPlanar())
        res = true;
      if (!planar && faceDef->IsCylinder())
        res = true;
    }
  }
  return res;
}


//------------------------------------------------------------------------------
// Определяет является ли объект осью
// ---
bool IsAxis(IModelObjectPtr entity)
{
  if (entity)
  {
    short type = entity->GetModelObjectType();
    return type == o3d_axis2Planes ||              // Ось по двум плоскостям
           type == o3d_axisOperation ||            // Ось операций
           type == o3d_axis2Points ||              // Ось по двум точкам
           type == o3d_axisConeFace ||             // Ось конической поверхности
           type == o3d_axisEdge ||                 // Ось по ребру
           type == o3d_AxisFromPointByDirection || // Ось через точку по направлению
           type == o3d_axisOX ||                   // Ось X
           type == o3d_axisOY ||                   // Ось Y
           type == o3d_axisOZ;                     // Ось Z
  }
  return false;
}


//------------------------------------------------------------------------------
//
// ---
int GetShpeelDataCallback(void * result, int count, char ** rowData, char **)
{
  SHPEEL * shpeel = reinterpret_cast<SHPEEL *>(result);
  if (shpeel)
  {
    shpeel->d = std::stof(rowData[2]);
    shpeel->p1 = std::stof(rowData[3]);
    shpeel->p2 = std::stof(rowData[4]);
    shpeel->b1 = std::stof(rowData[5]);
    shpeel->c = std::stof(rowData[6]);
  }

  return 0;
}


//------------------------------------------------------------------------------
// Чтение БД
// ---
bool ReadShpeelBase(float d, SHPEEL & shpeel)
{
  bool res = false;

  if (DataBase * db = DataBase::GetDBConnection())
  {
    short gNumb, stNumb;
    GetGostLoa(shpeel.gost, gNumb, stNumb);
    std::string query = "SELECT * FROM '" + GetStringFromWString(LoadStr(gNumb)) + "' WHERE t='1' AND d=" + std::to_string(d);
    res = db->ExecuteQuery(query, GetShpeelDataCallback, &shpeel);
  }

  return res;
}


//------------------------------------------------------------------------------
//
// ---
int GetShpeelStDataCallback(void * result, int count, char ** rowData, char **)
{
  if (SHPEEL * shpeel = reinterpret_cast<SHPEEL *>(result))
  {
    shpeel->l = std::stof(rowData[2]);
    shpeel->b = std::stof(rowData[4]);
    shpeel->m1 = std::stof(rowData[5]);
    shpeel->m2 = std::stof(rowData[6]);

    // ALLST - резьба до головки
    if (shpeel->gost > 22041 && !(fabs(shpeel->b) > EPSILON))
    {
      shpeel->f |= ALLST;
      shpeel->f &= ~ISPOLN; // Исполнение 1 становится текущим, тк. исполнение 2 недоступно
    }
    else
      shpeel->f &= ~ALLST;
  }

  return 0;
}


//------------------------------------------------------------------------------
// Читать параметры стержня
// ---
bool ReadShpeelStBase(SHPEEL & shpeel)
{
  bool res = false;

  if (DataBase * db = DataBase::GetDBConnection())
  {
    short gNumb, stNumb;
    GetGostLoa(shpeel.gost, gNumb, stNumb);
    std::string query = "SELECT * FROM '" + GetStringFromWString(LoadStr(stNumb)) +
                        "' WHERE t='1' AND l=" + std::to_string(shpeel.l) + " AND d=" + std::to_string(shpeel.d);
    res = db->ExecuteQuery(query, GetShpeelStDataCallback, &shpeel);
  }

  return res;
}


//------------------------------------------------------------------------------
// Получить параметры
// ---
bool Shpeel::GetParam()
{
  if (IUserParametersPtr userParams = m_part)
  {
    std::int32_t size = Size();
    std::vector<uint8_t> params = userParams->GetUserParams();
    if (params.size() == size)
    {
      void * userParams = &m_par;
      memcpy(userParams, params.data(), size);
      return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Конструктор
// ---
Shpeel::Shpeel()
  : m_part(nullptr)
  , m_changed(true)
  , m_flagMode(false)
{
  m_par.flagAttr = 1;
  m_par.typeSwitch = 0;
  m_par.ang = 0;
  m_par.drawType = 0;

  m_doc3D = kompasApp->GetActiveDocument();
  if (IAssemblyDocumentPtr docAsm = m_doc3D)
  {
    m_flagMode = m_doc3D->IsLibraryEditMode();

    // Взяли деталь
    if (m_flagMode)
      m_part = m_doc3D->GetLibraryEditObject();

    if (!GetParam())
      Init();
    else if (!m_tmp.ver)
      m_tmp.ver = 1;
  }
}


//------------------------------------------------------------------------------
// Конструктор копий
// ---
Shpeel::~Shpeel()
{
}


//------------------------------------------------------------------------------
// Инициализация
// ---
void Shpeel::Init()
{
  m_tmp.gost = 22032;
  m_par.drawType = ID_VID;
  m_par.ang = 0;
  m_tmp.f = 0;
  m_tmp.f |= TAKEISPOLN;
  m_tmp.d = 20;
  m_tmp.p1 = 2.5;
  m_tmp.p2 = 1.5;
  m_tmp.c = 2.5;
  m_tmp.indexMassa = 0; // 0 - металл 1 - алюмин сплав 3 - бронза 2 - латунь
  m_tmp.ver = 1;
  m_tmp.b1 = 20;
  m_tmp.klass = 2; // klass=B
  m_tmp.l = 90;
  m_tmp.b = 46;
  m_tmp.m1 = 245.9f;
  m_tmp.m2 = 228.9f;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
std::wstring GetModuleFilePath()
{
  if (ksapi::ILibraryManagerPtr libraryManager = kompasApp->GetLibraryManager())
  {
    if (ksapi::IProceduresLibrariesPtr proceduresLibraries = libraryManager->GetProceduresLibraries())
    {
      if (ksapi::IProceduresLibraryPtr library = proceduresLibraries->GetItemByName(L"Studs3D1"))
        return library->GetFullPath();
    }
  }

  return {};
}


//------------------------------------------------------------------------------
// При вставке из библиотеки моделей указывается полный путь к файлу библиотеки и путь внутри библиотеки моделей
// ---
std::wstring Shpeel::GetFileName()
{
  std::wstring fullName;
  bool res = false;

  std::wstring moduleFilePath = GetModuleFilePath();

  if (moduleFilePath.length() > 1)
  {
    std::filesystem::path pathModule(moduleFilePath);
    std::filesystem::path pathParent = pathModule.parent_path();
    pathParent += std::filesystem::path::preferred_separator;
    pathParent += STUDS_FILE;

    if (std::filesystem::exists(std::filesystem::path(pathParent)))
    {
      fullName = pathParent.wstring();
      res = true;
    }
    else
    {
      std::filesystem::path pathModule(moduleFilePath);
      std::filesystem::path pathParent = pathModule.parent_path();
      pathParent += std::filesystem::path::preferred_separator;
      pathParent += L"model\\";
      pathParent += STUDS_FILE;

      if (std::filesystem::exists(std::filesystem::path(pathParent)))
      {
        fullName = pathParent.wstring();
        res = true;
      }
    }
  }

  if (res)
    fullName += STUDS_INPUT_PATH;

  return fullName;
}


//------------------------------------------------------------------------------
// Редактируем переменные
// ---
void Shpeel::SetParam(IPartPtr & pPart)
{
  // Редактируем внешние переменные
  if (!pPart)
    pPart = m_part;

  if (pPart)
  {
    if (IFeaturePtr feature = pPart->GetOwner())
    {
      std::vector<IVariablePtr> vars = feature->GetVariables(true, false);
      if (!vars.empty())
      {
        float l, b1 = 0, b;     // Условие резьбы на всю длину - b1=0
        l = m_tmp.b1 + m_tmp.l; // Длина всей шпильки
        b = m_tmp.b;
        float step = m_tmp.f & PITCH ? m_tmp.p1 : m_tmp.p2;
        if (fabs(m_tmp.b1) < EPSILON) // ГОСТ 22042, ГОСТ 22043
        {
          if (fabs(m_tmp.b) > EPSILON)
            b1 = b;
        }
        else
        {
          b1 = m_tmp.b1;
          if (fabs(m_tmp.b) < EPSILON)
            b = m_tmp.l - 0.5f * m_tmp.d - 2 * step;
        }

        // Основные параметры
        SetVarValue(vars, L"d", m_tmp.d); // Диаметр резьбы
        float len = fabs(b1 - b) < EPSILON ? m_tmp.l - b1 : m_tmp.l;
        SetVarValue(vars, L"l", len);      // Длина шпильки
        SetVarValue(vars, L"b1", b1);      // Длина ввинчиваемого конца
        SetVarValue(vars, L"c1", m_tmp.c); // Размер фаски на конце стержня
        if (fabs(l - b1 - b) > EPSILON)
          SetVarValue(vars, L"l1", l - b1 - b);                   // Длина гладкой части стержня
        SetVarValue(vars, L"t", (step * 0.866 * 3 / 8));          // Толщина удаляемого материала
        int perf2 = (!(m_tmp.f & ISPOLN)) || (m_tmp.f & ALLST) || // ALLST - резьба до головки
                        (fabs(l - b1 - b) < EPSILON)
                    ? 1
                    : 0;
        SetVarValue(vars, L"cut1", perf2);                         // Вкл/выкл исполнение 2
        SetVarValue(vars, L"f1", !(m_tmp.f & SIMPLE) ? 0.0 : 1.0); // Вкл/выкл подголовок

        // Задать название детали
        std::wstring name =
          std::vformat(LoadStr(IDS_STUD_NAME), std::make_wformat_args(m_tmp.d, m_tmp.l, m_tmp.gost)); // создать имя детали
        pPart->SetName(name);                                                                         // Присвоить детали новое имя

        // Перестроим модель с измененными внешними переменными
        pPart->RebuildModel(true);
        pPart->Update(); // Обновить деталь
      }

      if (IUserParametersPtr userParams = pPart)
      {
        std::vector<uint8_t> data;
        data.resize(Size());
        void * pData = data.data();
        memcpy(pData, &m_par, Size());
        userParams->SetUserParams(data);
        userParams->SetLibraryFileName(L"");
        userParams->SetLibraryName(L"");
        userParams->SetCommand(1);
      }
      pPart->SetPropertyObjectEditable(true);
    }
  }
}


//------------------------------------------------------------------------------
// Добавить сопряжения
// ---
bool Shpeel::AddMate(MateConstraintType constraintType, ksMateConstraintAlignmentEnum direction, ksMateFixedTypeEnum fixed,
                     double val, IModelObjectPtr ent, const std::wstring & surfaceName, IPartPtr p, IMateConstraints3DPtr mCol)
{
  bool res = false;

  if (p && mCol) // Временное сопряжение
  {
    if (IMateConstraint3DPtr mate = mCol->Add(constraintType))
    {
      mate->SetAlignment(direction);
      mate->SetFixed(fixed);
      mate->SetBaseObject1(ent);

      mate->SetBaseObject2(GetEntityByName(surfaceName, p));
      res = mate->Update();
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// Редактирование объекта спецификации
// ---
ISpecificationBaseObjectPtr Shpeel::EditSpcObj(ISpecificationBaseObjectPtr spcObj)
{
  ISpecificationBaseObjectPtr base;
  IKompasDocumentPtr kompasDoc = kompasApp->GetActiveDocument();

  ISpecificationObjectCreateParamPtr objectCreateParam(kompasDoc->GetInterface(ksSpecificationObjectCreateParam));

  IPropertyKeeperPtr propertyKeeperIn1(m_part);

  objectCreateParam->SetSection(STANDART_SECTION);
  objectCreateParam->SetAttributeNumber(81764182256ull);

  if (!m_spcBaseObjects)
  {
    ISpecificationDescriptionsPtr descriptions = kompasDoc->GetSpecificationDescriptions();
    ISpecificationDescriptionPtr description = descriptions->GetActive();

    if (!description)
      description = descriptions->Add(L"", 0, L"");

    if (description)
      m_spcBaseObjects = description->GetBaseObjects();
  }

  ISpecificationBaseObjectPtr obj1;
  ISpecificationBaseObjectsPtr Objects;

  if (m_flagMode && m_spcBaseObjects)
  {
    std::vector<ISpecificationBaseObjectPtr> arr =
      m_spcBaseObjects->GetSpecificationObjectsForGeom({propertyKeeperIn1}, false, STANDART_SECTION, 81764182256ull);
    if (arr.size() > 0)
      obj1 = arr[0];
  }

  if (m_spcBaseObjects)
  {
    if (!obj1)
    {
      // Базовый объект добавляется.
      obj1 = m_spcBaseObjects->AddWithParam(objectCreateParam, propertyKeeperIn1, false, {propertyKeeperIn1});
    }
  }

  if (m_flagMode || obj1)
  {
    ISpecificationColumnsPtr columns = obj1->GetColumns();
    ISpecificationColumnPtr column = columns->GetColumn(ksSColumnName, 1, 0);
    ISpecificationColumnItemsPtr columnItems = column->GetColumnItems();

    ISpecificationColumnItemPtr item;
    if (columnItems)
    {
      item = columnItems->GetItem(1);
      uint32_t uBuf = !!(m_tmp.f & ISPOLN);

      if (item)
        item->SetVisible(uBuf);

      if (uBuf)
      {
        uBuf = 2;
        if (item)
          item->SetValue(static_cast<int32_t>(uBuf));
      }

      item = columnItems->GetItem(3);
      if (item)
        item->SetValue(m_tmp.d);


      item = columnItems->GetItem(4);
      if (item)
        item->SetVisible(false);

      item = columnItems->GetItem(5);
      if (item)
        item->SetVisible(false);

      item = columnItems->GetItem(6);
      if (item)
        item->SetVisible(false);

      // Изменим длину
      uBuf = static_cast<uint32_t>(m_tmp.l);

      item = columnItems->GetItem(8);
      if (item)
        item->SetValue(static_cast<int32_t>(uBuf));

      item = columnItems->GetItem(6);
      if (item)
        item->SetVisible(false);

      item = columnItems->GetItem(9);
      if (item)
        item->SetVisible(false);

      item = columnItems->GetItem(10);
      if (item)
        item->SetVisible(false);

      item = columnItems->GetItem(11);
      if (item)
        item->SetVisible(false);

      // Изменим ГОСТ
      uBuf = m_tmp.gost;
      item = columnItems->GetItem(13);
      if (item)
        item->SetValue(static_cast<int32_t>(uBuf));

      float massa = m_tmp.f & ISPOLN ? m_tmp.m2 : m_tmp.m1;
      // 0 - металл 1 - алюмин сплав 3 - бронза 2 - латунь
      massa = massa *
              (!m_tmp.indexMassa       ? 1
               : m_tmp.indexMassa == 1 ? 0.356f
               : m_tmp.indexMassa == 3 ? 0.97f
                                       : 1.08f) /
              COUNT_MASSA;
      columns = obj1->GetAdditionalColumns();
      column = columns->GetColumn(ksSColumnMass, 1, 0);
      if (column)
      {
        columnItems = column->GetColumnItems();
        if (columnItems)
          columnItems->GetItem(0)->SetValue(massa);
      }

      obj1->Update();
      return obj1;
    }
  }

  return nullptr;
}


//-------------------------------------------------------------------------------
// Отрисовка объекта спецификации
// ---
bool Shpeel::DrawSpcObj(ISpecificationBaseObjectPtr & spcObj)
{
  spcObj = nullptr;
  if (IsSpcObjCreate())
  {
    IKompasErrorPtr kError = kompasApp->GetKompasError();
    if (kError->GetCode() == etError10) // Вырожденный объект
      kError->Clear();

    if (m_part->IsCreateSpcObjects() == !m_par.flagAttr)
    {
      m_part->SetCreateSpcObjects(m_par.flagAttr);
      m_part->Update();
    }

    if (m_par.flagAttr)
      spcObj = EditSpcObj(spcObj);
  }
  return !spcObj.IsEmpty();
}


//----------------------------------------------------------------------------------------------------
// Вывод параметров
// ---
void Shpeel::ShowParam()
{
  // Перезаполняем грид
  if (m_paramGrid)
  {
    m_rowIndex = 1;

    // "b(мм) Гаечный конец"
    AddDoubleToGrig(ID_PROP_WRENCH_ENDING, m_tmp.b);
    // "b0(мм) Ввинчиваемый конец"
    AddDoubleToGrig(ID_PROP_SCREW_ENDING, m_tmp.b1);
    // "p(мм) Шаг резьбы"
    float step = !(m_tmp.f & PITCHOFF || !(m_tmp.f & PITCH)) ? m_tmp.p1 : m_tmp.p2;
    AddDoubleToGrig(ID_PROP_PITCH, step);
    // "c(мм) Фаска"
    AddDoubleToGrig(ID_PROP_FACET, m_tmp.c);
    // "m(кг) Масса 1000 шт"
    float massa = m_tmp.f & ISPOLN ? m_tmp.m2 : m_tmp.m1;
    // 0 - металл 1 - алюмин сплав 3 - бронза 2 - латунь
    massa = massa * (!m_tmp.indexMassa ? 1 : m_tmp.indexMassa == 1 ? 0.356f : m_tmp.indexMassa == 3 ? 0.97f : 1.08f);
    AddDoubleToGrig(ID_PROP_MASSA_1000, massa);

    m_paramGrid->SetRowCount(m_rowIndex);
    m_paramGrid->UpdateParam();
  }
}


//-------------------------------------------------------------------------------------
// Идентификатор слайда
// ---
int32_t Shpeel::GetSlideID()
{
  if (m_tmp.gost > 22041)
  {
    if (m_tmp.f & ALLST) // ALLST - резьба до головки
      return SH22042_3;
    else
    {
      if (m_tmp.f & ISPOLN)
        return SH22042_2;
      else
        return SH22042_1;
    }
  }
  else
  {
    if (m_tmp.f & ISPOLN)
      return SH22032_2;
    else
      return SH22032_1;
  }
}


//----------------------------------------------------------------------------------------------------
// Обработка смены ГОСТа
// ---
void Shpeel::OnChangeGost(const std::wstring & newGost)
{
  short gost = std::stoi(newGost.substr(0, 5));
  if (gost && gost != m_tmp.gost)
  {
    m_tmp.gost = gost;
    if (ReadShpeelBase(m_tmp.d, m_tmp) && ReadShpeelStBase(m_tmp)) // Чтение БД
    {
      IPropertyListPtr diamList(GetPropertyControl(ID_PROP_DIAM));
      if (diamList)
        FillDiametr(diamList); // Заполняет список диаметров

      IPropertyListPtr lenghtList(GetPropertyControl(ID_PROP_LENGHT));
      if (lenghtList)
        FillLenght(lenghtList); // Заполняет список длин

      IPropertyListPtr screwEndingList(GetPropertyControl(ID_PROP_SCREW_ENDING_LENGHT));
      if (screwEndingList)
        // Выделяем в списке
        screwEndingList->SetStringValue(LoadStr(GetTypeStrForGost(m_tmp.gost)));

      IPropertyListPtr classList(GetPropertyControl(ID_PROP_CLASS));
      if (classList)
        // Выделяем в списке
        classList->SetStringValue(LoadStr(GetClassStrForGost(m_tmp.gost)));

      IPropertyMultiButtonPtr ispButtons(GetPropertyControl(ID_PROP_ISP));
      if (ispButtons)
      {
        // Проверка возможности исполнения 2
        bool enable = m_tmp.f & ALLST ? false : true; // ALLST - резьба до головки
        ispButtons->SetButtonEnable(IDP_BMP_G_I2, enable);
        if (!enable)
          ispButtons->SetButtonChecked(IDP_BMP_G_I1,
                                       !(m_tmp.f & ISPOLN)); // Исполнение 1 становится текущим, тк. исполнение 2 недоступно
      }

      // Вывод параметров в гриде ( ГОСТ шпильки - новые параметры )
      ShowParam();
      // Перерисовать фантом и слайд
      SetChanged();
    }
  }
}


//-------------------------------------------------------------------------------------
// Изменение значения контрола
// ---
void Shpeel::OnChangeControlValue(const IPropertyControlPtr & control)
{
  int32_t controlId = control ? control->GetId() : -1;
  switch (controlId)
  {
    case IDP_SPC_OBJ: // Объект спецификации
      m_par.flagAttr = m_par.flagAttr ? 0 : 1;
      break;

    case ID_PROP_GOST: // ГОСТ
    {
      if (IPropertyListPtr propList = control)
        OnChangeGost(propList->GetStringValue());
      break;
    }

    case ID_PROP_DIAM: // Диаметр
    {
      if (IPropertyListPtr propList = control)
      {
        float diam = static_cast<float>(propList->GetDoubleValue());
        if (fabs(diam - m_tmp.d) > EPSILON)
        {
          if (ReadShpeelBase(diam, m_tmp) && ReadShpeelStBase(m_tmp)) // Чтение БД
          {
            IPropertyListPtr lenghtList(GetPropertyControl(ID_PROP_LENGHT));
            if (lenghtList)
              FillLenght(lenghtList); // Заполняет список длин

            IPropertyMultiButtonPtr ispButtons(GetPropertyControl(ID_PROP_ISP));
            if (ispButtons)
            {
              // Проверка возможности исполнения 2
              bool enable = m_tmp.f & ALLST ? false : true; // ALLST - резьба до головки
              ispButtons->SetButtonEnable(IDP_BMP_G_I2, enable);
              if (!enable)
                ispButtons->SetButtonChecked(IDP_BMP_G_I1,
                                             !(m_tmp.f & ISPOLN)); // Исполнение 1 становится текущим, тк. исполнение 2 недоступно
            }

            // Вывод параметров в гриде ( Диаметр шпильки - новые параметры )
            ShowParam();
            // Перерисовать фантом и слайд
            SetChanged();
          }
        }
      }
      break;
    }

    case ID_POINT_3D_X:
    case ID_POINT_3D_Y:
    case ID_POINT_3D_Z:
    {
      IPropertyPoint3DPtr point3D(GetPropertyControl(ID_POINT_3D));
      if (point3D && m_part)
      {
        double x = point3D->GetCoordinateValue(0);
        double y = point3D->GetCoordinateValue(1);
        double z = point3D->GetCoordinateValue(2);

        IPlacement3DPtr PartPlace(m_part->GetPlacement());
        if (PartPlace)
        {
          PartPlace->SetOrigin(x, y, z);
          m_part->UpdatePlacement(true);
        }

        if (m_baseMan)
        {
          IPlacement3DPtr IPlace3D(m_baseMan->GetPlacement());
          if (IPlace3D)
          {
            IPlace3D->SetOrigin(x, y, z);
            m_baseMan->UpdatePlacement(true);
          }
        }
        InitPoint3D(true, true);
      }
      break;
    }

    case ID_PROP_LENGHT: // Длина шпильки
    {
      if (IPropertyListPtr propList = control)
      {
        float lenght = static_cast<float>(propList->GetDoubleValue());
        if (fabs(lenght - m_tmp.l) > EPSILON)
        {
          m_tmp.l = lenght;

          // Перерисовать фантом и слайд
          SetChanged();
        }
      }
      break;
    }

    case ID_PROP_SCREW_ENDING_LENGHT: // Длина ввинчиваемого конца шпильки
    {
      if (IPropertyListPtr propList = control)
      {
        std::wstring strScrewEnding = propList->GetStringValue();
        for (int i = 0; i < 6; i++)
        {
          if (strScrewEnding == LoadStr(IDS_STUD_1D + i))
          {
            short gost = GetGostForTypeAndKlass(i, m_tmp.klass);
            if (gost != m_tmp.gost)
            {
              IPropertyListPtr gostList(GetPropertyControl(ID_PROP_GOST));
              if (gostList)
              {
                // Выделяем в списке
                std::wstring str = LoadStr(GetGostStr(gost));
                gostList->SetStringValue(str);
                OnChangeGost(str);
                break;
              }
            }
          }
        }
      }
      break;
    }

    case ID_PROP_CLASS: // Класс точности шпильки
    {
      if (IPropertyListPtr propList = control)
      {
        std::wstring strClass = propList->GetStringValue();
        for (int j = 0; j < 2; j++)
        {
          if (strClass == LoadStr(IDS_STUD_A + j))
          {
            IPropertyListPtr screwEndingList(GetPropertyControl(ID_PROP_SCREW_ENDING_LENGHT));
            if (screwEndingList)
            {
              std::wstring strScrewEnding = screwEndingList->GetStringValue();
              for (int i = 0; i < 6; i++)
              {
                if (strScrewEnding == LoadStr(IDS_STUD_1D + i))
                {
                  short gost = GetGostForTypeAndKlass(i, j);
                  if (gost != m_tmp.gost)
                  {
                    IPropertyListPtr gostList(GetPropertyControl(ID_PROP_GOST));
                    if (gostList)
                    {
                      // Выделяем в списке
                      std::wstring str = LoadStr(GetGostStr(gost));
                      gostList->SetStringValue(str);
                      OnChangeGost(str);
                      return;
                    }
                  }
                }
              }
            }
          }
        }
      }
      break;
    }

    case ID_PROP_MATERIAL: // Материал шпильки
    {
      if (IPropertyListPtr propList = control)
      {
        std::wstring strMaterial = propList->GetStringValue();
        for (int i = 0; i < 4; i++)
        {
          if (strMaterial == LoadStr(IDS_MATERIAL_STEEL + i))
          {
            if (i != m_tmp.indexMassa)
            {
              m_tmp.indexMassa = i;

              // Вывод параметров в гриде ( Масса )
              ShowParam();
              break;
            }
          }
        }
      }
      break;
    }
  }
}


//----------------------------------------------------------------------------------------------------
// Обработка нажатия кнопок
// ---
void Shpeel::OnButtonClick(int32_t buttonID)
{
  switch (buttonID)
  {
    case IDP_BMP_G_I1: // Исполнение 1
      m_tmp.f &= ~ISPOLN;

      // Вывод параметров в гриде ( Исполнение влияет на массу )
      ShowParam();
      // Перерисовать фантом и слайд
      SetChanged();

      break;

    case IDP_BMP_G_I2: // Исполнение 2
      m_tmp.f |= ISPOLN;

      // Вывод параметров в гриде ( Исполнение влияет на массу )
      ShowParam();
      // Перерисовать фантом и слайд
      SetChanged();

      break;

    case ID_PROP_G_STEP: // Шаг
    {
      m_tmp.f & PITCH ? m_tmp.f &= ~PITCH : m_tmp.f |= PITCH;

      // Вывод параметров в гриде ( Шаг резьбы )
      ShowParam();
      // Перерисовать фантом и слайд
      SetChanged();

      break;
    }

    case ID_PROP_SIMPLE: // Упрощенно
    {
      m_tmp.f & SIMPLE ? m_tmp.f &= ~SIMPLE : m_tmp.f |= SIMPLE;

      // Перерисовать фантом и слайд
      SetChanged();

      break;
    }

    case pbEnter: // Вставка детали в сборку и окончание процесса
    {
      m_fixingPart = true;

      m_process->Stop();

      break;
    }

    case IDS_TAKE_POINT:
    {
      if (m_doc3D)
      {
        bool lost = true;
        if (!m_takePoint)
        {
          IPartPtr topPart(m_doc3D->GetTopPart());
          if (topPart)
          {
            IModelContainerPtr mc = topPart;
            m_takePoint = mc->AddObject(o3d_point3D);

            lost = !!m_flagMode;
          }
          IPoint3DPtr point3D(m_takePoint);

          if (point3D)
          {
            IPlacement3DPtr place(m_part->GetPlacement());

            if (place)
            {
              double x = 0.0, y = 0.0, z = 0.0;
              place->GetOrigin(x, y, z);
              point3D->SetX(x);
              point3D->SetY(y);
              point3D->SetZ(z);
            }
          }

          IModelObjectPtr obj(m_takePoint);
          m_process3D->RunTakeCreateObjectProcess(prPoint3D, obj, false, lost); // Запуска подпроцесса создания 3D- точки
        }
        break;
      }
    }
  }
}


//------------------------------------------------------------------------------
//
// ---
int FillDoubleListCallback(void * result, int count, char ** rowData, char **)
{
  std::vector<double> * diamList = reinterpret_cast<std::vector<double> *>(result);
  if (diamList)
    diamList->push_back(std::stod(rowData[0]));

  return 0;
}


//------------------------------------------------------------------------------
// Заполняет список диаметров
// ---
void Shpeel::FillDiametr(std::vector<double> & diamList, float & curDiam)
{
  float dMin = MAX_LENGTH;   // Минимальное значение в списке
  float dMax = MIN_LENGTH;   // Максимальное значение в списке
  bool enterInRange = false; // Текущее значение не входит в новые значения

  if (DataBase * db = DataBase::GetDBConnection())
  {
    short gNumb, stNumb;
    GetGostLoa(m_tmp.gost, gNumb, stNumb);
    std::string query = "SELECT DISTINCT(d) FROM '" + GetStringFromWString(LoadStr(gNumb)) + "' WHERE t='1' ORDER BY d";
    if (db->ExecuteQuery(query, FillDoubleListCallback, &diamList))
    {
      for (double & diam : diamList)
      {
        if (diam == curDiam)
          enterInRange = true; // Текущее значение диаметра входит в новые значения
        if (diam < dMin)
          dMin = static_cast<float>(diam); // Находим минимальное значение
        if (diam > dMax)
          dMax = static_cast<float>(diam); // Находим максимальное значение
      }

      if (curDiam < dMin)      // Если диаметр выходит за минимальную границу новых значений
        curDiam = dMin;        // Присваиваем ему минимальное значение
      else if (curDiam > dMax) // Если диаметр выходит за максимальную границу новых значений
        curDiam = dMax;        // Присваиваем ему максимальное значение
      else if (!enterInRange)  // Если текущего значения диаметра нет в списке
        curDiam = dMin;        // Присваиваем диаметру минимальное значение
    }
  }
}


//------------------------------------------------------------------------------
// Заполняет список диаметров
// ---
void Shpeel::FillDiametr(IPropertyListPtr & diamList)
{
  std::vector<double> list;
  FillDiametr(list, m_tmp.d);
  diamList->AddDoubleValues(list);
  diamList->SetDoubleValue(m_tmp.d);
}


//------------------------------------------------------------------------------
// Заполняет список длин
// ---
void Shpeel::FillLenght(std::vector<double> & lenList, float & curLen)
{
  float lMin = MAX_LENGTH;   // Минимальное значение в списке
  float lMax = MIN_LENGTH;   // Максимальное значение в списке
  bool enterInRange = false; // Текущее значение не входит в новые значения

  if (DataBase * db = DataBase::GetDBConnection())
  {
    short gNumb, stNumb;
    GetGostLoa(m_tmp.gost, gNumb, stNumb);
    std::string query = "SELECT DISTINCT(l) FROM '" + GetStringFromWString(LoadStr(stNumb)) + "' WHERE t='1' AND d='" +
                        std::to_string(m_tmp.d) + "' ORDER BY l";
    if (db->ExecuteQuery(query, FillDoubleListCallback, &lenList))
    {
      for (double & len : lenList)
      {
        if (len == curLen)
          enterInRange = true; // Текущее значение диаметра входит в новые значения
        if (len < lMin)
          lMin = static_cast<float>(len); // Находим минимальное значение
        if (len > lMax)
          lMax = static_cast<float>(len); // Находим максимальное значение
      }

      if (curLen < lMin)      // Если диаметр выходит за минимальную границу новых значений
        curLen = lMin;        // Присваиваем ему минимальное значение
      else if (curLen > lMax) // Если диаметр выходит за максимальную границу новых значений
        curLen = lMax;        // Присваиваем ему максимальное значение
      else if (!enterInRange) // Если текущее значение диаметра не входит в в новые значения
        curLen = lMin;        // Присваиваем диаметру минимальное значение
    }
  }
}


//------------------------------------------------------------------------------
// Заполняет список длин
// ---
void Shpeel::FillLenght(IPropertyListPtr & lenghtList)
{
  std::vector<double> list;
  FillLenght(list, m_tmp.l);
  lenghtList->AddDoubleValues(list);
  lenghtList->SetDoubleValue(m_tmp.l);
}


//-------------------------------------------------------------------------------
// Заполняет список длин ввинчиваемого конца
// ---
void Shpeel::FillScrewEnding(std::vector<std::wstring> & screwEndingList)
{
  for (int i = 0; i < 6; i++)
    screwEndingList.push_back(LoadStr(IDS_STUD_1D + i));
}


//-------------------------------------------------------------------------------
// Заполняет список длин ввинчиваемого конца
// ---
void Shpeel::FillScrewEnding(IPropertyListPtr & screwEndingList)
{
  std::vector<std::wstring> list;
  FillScrewEnding(list);
  screwEndingList->AddStringValues(list);
  screwEndingList->SetStringValue(LoadStr(GetTypeStrForGost(m_tmp.gost)));
}


//-------------------------------------------------------------------------------
// Заполняет список длин ввинчиваемого конца
// ---
void Shpeel::FillClass(std::vector<std::wstring> & classList)
{
  for (int i = 0; i < 2; i++)
    classList.push_back(LoadStr(IDS_STUD_A + i));
}


//-------------------------------------------------------------------------------
// Заполняет список класса точности
// ---
void Shpeel::FillClass(IPropertyListPtr & classList)
{
  std::vector<std::wstring> list;
  FillClass(list);
  classList->AddStringValues(list);
  classList->SetStringValue(LoadStr(GetClassStrForGost(m_tmp.gost)));
}


//-------------------------------------------------------------------------------
// Заполняет список ГОСТов
// ---
void Shpeel::FillGost(std::vector<std::wstring> & gostList)
{
  for (int i = 0; i < 12; i++)
    gostList.push_back(LoadStr(STR_101 + i));
}


//-------------------------------------------------------------------------------
// Заполняет список ГОСТов
// ---
void Shpeel::FillGost(IPropertyListPtr & gostList)
{
  std::vector<std::wstring> list;
  FillGost(list);
  gostList->AddStringValues(list);
  // Выделяем в списке
  gostList->SetStringValue(LoadStr(GetGostStr(m_tmp.gost)));
}


//-------------------------------------------------------------------------------
// Определяет материал по indexMassa
// 0 - металл 1 - алюмин сплав 2 - латунь 3 - бронза
// ---
static short GetMaterialStr(short indexMassa)
{
  short n = 0;
  if (indexMassa >= 0 && indexMassa <= 3)
    n = IDS_MATERIAL_STEEL + indexMassa;
  return n;
}


//-------------------------------------------------------------------------------
// Заполняет список ГОСТов
// ---
void Shpeel::FillMaterial(std::vector<std::wstring> & materialList)
{
  for (int i = 0; i < 4; i++)
    materialList.push_back(LoadStr(IDS_MATERIAL_STEEL + i));
}


//-------------------------------------------------------------------------------
// Заполняет список материалов
// ---
void Shpeel::FillMaterial(IPropertyListPtr & materialList)
{
  std::vector<std::wstring> list;
  FillMaterial(list);
  materialList->AddStringValues(list);
  // Выделяем в списке
  materialList->SetStringValue(LoadStr(GetMaterialStr(m_tmp.indexMassa)));
}


//-------------------------------------------------------------------------------
// Наполнить панель контролами
// ---
void Shpeel::ShowControls()
{
  if (m_curentCollection)
  {
    IPropertyPoint3DPtr point3D(m_curentCollection->Add(ksControlPoint3D));
    if (point3D)
    {
      InitPropertyControl(point3D.Get(), ID_POINT_3D, ID_POINT_3D, ID_POINT_3D);
      IPropertyEditPtr xEdit = point3D->GetCoordinate(0);
      if (xEdit)
        xEdit->SetId(ID_POINT_3D_X);
      IPropertyEditPtr yEdit = point3D->GetCoordinate(1);
      if (yEdit)
        yEdit->SetId(ID_POINT_3D_Y);
      IPropertyEditPtr zEdit = point3D->GetCoordinate(2);
      if (zEdit)
        zEdit->SetId(ID_POINT_3D_Z);
      IPropertyControlPtr ctrl1(point3D);
      if (ctrl1)
        ctrl1->AddAdditionButton(IDS_TAKE_POINT, 0xE1A0, LoadStr(IDS_TAKE_POINT), L"");
      if (m_flagMode && m_part)
      {
        IPlacement3DPtr place(m_part->GetPlacement());
        double x = 0.0, y = 0.0, z = 0.0;
        if (place && place->GetOrigin(x, y, z))
        {
          point3D->SetCoordinateValue(0, x);
          point3D->SetCoordinateValue(1, y);
          point3D->SetCoordinateValue(2, z);
          point3D->SetCoordinateState(0, ksCheckFixed);
          point3D->SetCoordinateState(1, ksCheckFixed);
          point3D->SetCoordinateState(2, ksCheckFixed);
        }
        m_procParam->SetDefaultControlFix(ksAllFix);
      }
    }
  }
  // Комбобокс ГОСТа
  IPropertyListPtr gostList(CreateStringList());
  if (gostList)
  {
    InitPropertyControl(gostList.Get(), ID_PROP_GOST, IDS_GOST_TIPS, IDS_GOST_TIPS);
    FillGost(gostList); // Заполняет список ГОСТов
  }

  // Комбобокс диаметров резьбы
  IPropertyListPtr diamList(CreateRealList());
  if (diamList)
  {
    InitPropertyControl(diamList.Get(), ID_PROP_DIAM, IDS_DIAM_TIPS, IDS_DIAM_TIPS);
    FillDiametr(diamList); // Заполняет список диаметров
  }

  // Комбобокс длин шпильки
  IPropertyListPtr lenghtList(CreateRealList());
  if (lenghtList)
  {
    InitPropertyControl(lenghtList.Get(), ID_PROP_LENGHT, IDS_LENGHT_TIPS, IDS_LENGHT_TIPS);
    FillLenght(lenghtList); // Заполняет список длин
  }

  // Комбобокс длин ввинчиваемого конца
  IPropertyListPtr screwEndingList(CreateStringList());
  if (screwEndingList)
  {
    InitPropertyControl(screwEndingList.Get(), ID_PROP_SCREW_ENDING_LENGHT, IDS_SCREW_ENDING_TIPS, IDS_SCREW_ENDING_TIPS);
    FillScrewEnding(screwEndingList); // Заполняет список длин ввинчиваемого конца
  }

  // Комбобокс класса точности
  IPropertyListPtr classList(CreateStringList());
  if (classList)
  {
    InitPropertyControl(classList.Get(), ID_PROP_CLASS, IDS_CLASS_TIPS, IDS_CLASS_TIPS);
    FillClass(classList); // Заполняет список класса точности
  }

  // Комбобокс материала
  IPropertyListPtr materialList(CreateStringList());
  if (materialList)
  {
    InitPropertyControl(materialList.Get(), ID_PROP_MATERIAL, IDS_MATERIAL_TIPS, IDS_MATERIAL_TIPS);
    FillMaterial(materialList); // Заполняет список материалов
  }

  CreateSeparator(ksSeparatorDownName);

  // Исполнение
  IPropertyMultiButtonPtr ispButtons(CreateMultiButton(ksRadioButton));
  if (ispButtons)
  {
    InitPropertyControl(ispButtons.Get(), ID_PROP_ISP, IDS_ISP_HINT, IDS_ISP_TIPS, true, ksNameVerticalVisible);

    // Кнопка исполнение 1
    AddButton2(ispButtons, IDP_BMP_G_I1, IDF_BMP_G_I1, !(m_tmp.f & ISPOLN));
    // Проверка возможности исполнения 2
    bool enable = m_tmp.f & ALLST ? false : true; // ALLST - резьба до головки
    // Кнопка исполнение 2
    AddButton2(ispButtons, IDP_BMP_G_I2, IDF_BMP_G_I2, !!(m_tmp.f & ISPOLN), enable);
  }

  CreateSeparator(ksSeparatorDownName);

  // Дополнительные параметры
  IPropertyMultiButtonPtr paramButtons(CreateMultiButton(ksCheckButton));
  if (paramButtons)
  {
    InitPropertyControl(paramButtons.Get(), IDP_FLAGS, IDS_FLAGS_TIPS, IDS_FLAGS_TIPS, true, ksNameVerticalVisible);
    // Шаг мелкий
    AddButton2(paramButtons, ID_PROP_G_STEP, IDF_PROP_G_STEP, !!(m_tmp.f & PITCH), !(m_tmp.f & PITCHOFF));
    // Упрощенно
    AddButton2(paramButtons, ID_PROP_SIMPLE, IDF_PROP_SIMPLE, !!(m_tmp.f & SIMPLE), true);
  }

  // Создавать объект спецификации
  if (IsSpcObjCreate())
  {
    IPropertyCheckBoxPtr checkBox(CreateCheckBox(!!m_par.flagAttr));
    if (checkBox)
      InitPropertyControl(checkBox.Get(), IDP_SPC_OBJ, IDS_SPC_OBJ_TIPS, IDS_SPC_OBJ_TIPS);
  }

  CreateTab(ID_PROP_ELEMENT_PARAM);
}


//-------------------------------------------------------------------------------------
// Перерисовать фантом, слайд, вывести новые параметры
// ---
void Shpeel::RedrawPhantom()
{
  if (Changed())
  {
    // Изменения фантома
    if (m_part)
    {
      SetParam(m_part);
      m_part->Update();
    }

    // Перерисовываем слайд в окне
    if (m_slideBox)
    {
      m_slideBox->SetSlide(GetSlideID(), GetModule());
      m_slideBox->UpdateParam();
    }
    SetChanged(false);
  }
}


//-------------------------------------------------------------------------------------
// Рисование 3D объектов
// ---
void Shpeel::Draw3D()
{
  if (m_doc3D && (!m_flagMode || m_part))
  {
    long toolBarID = IDP_STUDS;
    long firstTabID = IDP_STUDS;
    std::wstring fileName;

    if (m_flagMode)
      GetParam();
    else
      fileName = GetFileName();

    SpecPropertyToolBarEnum toolBarType = pnEnterEscHelp;

    if (InitProcessParam(toolBarID, toolBarType, firstTabID))
    {
      m_procParam->SetAutoReduce(false);

      IKompasDocument3DPtr m_doc3D(kompasApp->GetActiveDocument());

      if (m_doc3D)
      {
        m_process = m_doc3D->GetLibProcess(ksProcess3DPlacementAndEntity);
        m_process3D = m_process;
        if (m_process3D)
        {
          if (!m_flagMode)
            m_part = m_process3D->CreatePhantomFromFile(fileName);
          IManipulatorsPtr manColl(m_process3D->GetManipulators()); // Получаем коллекцию манипуляторов процесса
          if (manColl)
          {
            m_baseMan = manColl->Add(ksPlacement3DManipulator); // Добавляем новый манипулятор в коллекцию
            m_manipulator = m_baseMan;
            if (m_manipulator)
            {
              m_baseMan->SetId(IDR_BASEMANID);
              m_baseMan->SetActive(true);
              m_baseMan->SetVisible(false);

              // Определение видимости определенных примитивов манипулятора
              m_manipulator->SetPrimitiveVisible(ksMPPlaceXOY, false);
              m_manipulator->SetPrimitiveVisible(ksMPPlaceXOZ, false);
              m_manipulator->SetPrimitiveVisible(ksMPPlaceYOZ, false);
              m_manipulator->SetPrimitiveVisible(ksMPTextX, false);
              m_manipulator->SetPrimitiveVisible(ksMPTextY, false);
              m_manipulator->SetPrimitiveVisible(ksMPTextZ, false);
              m_manipulator->SetPrimitiveVisible(ksMPOriginal, true);

              m_baseMan->UpdatePlacement(true);
              new Process3DManipulatorsEvent(manColl, *this);
            }
          }
          if (m_flagMode)
            m_process3D->SetPhantomObject(m_part);
          m_process->SetProcessParam(m_procParam);
          new Process3DEvent(m_process3D, *this);
          SetChanged();
          RedrawPhantom();
          m_process->Run(false, false);
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// ksFilterObjects - Фильтрация объектов
// ---
bool Shpeel::FilterObjects(const IModelObjectPtr & object)
{
  bool res = false; // Признак неподходящего объекта

  if (object && !object->IsExternalObject()) // Используем только внутренние объекты
  {
    if (IsSurface(object) || IsSurface(object, false) || IsAxis(object))
      res = true;
  }

  return res;
}


//-------------------------------------------------------------------------------------
// ksEndProcess - Окончание процесса.
// ---
bool Shpeel::EndProcess()
{
  if (m_fixingPart) // Закрепляем деталь
  {
    SetParam(m_part);
    if (!m_flagMode && m_process3D)
      m_process3D->StorePhantom();

    m_part->UpdatePlacement(true); // Изменение положения детали

    if (IMateConstraints3DPtr mateConstraints = m_doc3D->GetMateConstraints())
    {
      mateConstraints->SetMatesMultiCreatedMode(true);

      for (unsigned int Idx = 0; Idx < m_objects.size(); Idx++)
      {
        IModelObjectPtr next = m_objects[Idx];

        // Создаем постоянное сопряжение в зависимости от поверхности
        IMateConstraints3DPtr constraints = m_doc3D->GetMateConstraints();
        if (IsSurface(next))
        {
          IMateConstraint3DPtr constraint = constraints->Add(mc_Coincidence);
          constraint->SetBaseObject1(next);
          constraint->SetBaseObject2(GetEntityByName(L"Plane", m_part));
          constraint->SetAlignment(ksMCAlignmentCooriented);
          constraint->SetFixed(ksMFixedPart1);
          constraint->SetParamValue(0.0);
        }
        else if (IsSurface(next, false) || IsAxis(next))
        {
          IMateConstraint3DPtr constraint = constraints->Add(mc_Concentric);
          constraint->SetBaseObject1(next);
          constraint->SetBaseObject2(GetEntityByName(L"Axis", m_part));
          constraint->SetAlignment(ksMCAlignmentClosest);
          constraint->SetFixed(ksMFixedPart1);
          constraint->SetParamValue(0.0);
        }
      }
      mateConstraints->SetMatesMultiCreatedMode(false);
      // Устанавливаем признак, стандартное изделие
      m_part->SetStandard(true);
    }
    ISpecificationBaseObjectPtr spcObj;
    if (DrawSpcObj(spcObj)) // Вывод объекта СП
    {
      // Редактируем параметры
      spcObj->Edit();
    }
  }

  return PropertyManagerObject::EndProcess();
}


//-------------------------------------------------------------------------------------
//
// ---
void Shpeel::UnadviseEvents()
{
  if (m_process3D)
    BaseEvent::TerminateEvents(m_process3D->GetManipulators());
  PropertyManagerObject::UnadviseEvents();
}


//-------------------------------------------------------------------------------------
// ksEndProcess - Событие создания объекта в подчиненном режиме.
// ---
bool Shpeel::CreateTakeObject(const IModelObjectPtr & object)
{
  m_takePoint = object;

  if (IPoint3DPtr point3D = object)
  {
    if (IPlacement3DPtr place = m_part->GetPlacement())
    {
      double x = point3D->GetX(), y = point3D->GetY(), z = point3D->GetZ();
      place->SetOrigin(x, y, z);
      if (m_baseMan)
      {
        IPlacement3DPtr IPlace3D(m_baseMan->GetPlacement());
        if (IPlace3D)
        {
          IPlace3D->SetOrigin(x, y, z);
          m_baseMan->UpdatePlacement(true);
        }
      }
      InitPoint3D(true, true);
    }
  }

  return true;
}

//-------------------------------------------------------------------------------------
// ksPlacementChanged - Изменено положения объекта
// ---
bool Shpeel::PlacementChanged(const IModelObjectPtr & object)
{
  IKompasDocument3DPtr Doc3D = kompasApp->GetActiveDocument();
  if (!Doc3D)
    return true;

  IChooseManagerPtr ChooseMan = Doc3D->GetChooseManager();
  if (!ChooseMan)
    return true;

  if (FilterObjects(object)) // Если поверхность под курсором подходит
  {
    if (ChooseMan->IsChoosen(object)) // Проверяем, выбрана ли поверхность
      ChooseMan->Unchoose({object});
    else
      ChooseMan->Choose({object});

    if (m_mCol)
      m_mCol->DeleteTemporaryMateConstraints();

    std::vector<IKompasAPIObjectPtr> Objects = ChooseMan->GetChoosenObjects();
    bool visible = Objects.size() > 0;
    InitPoint3D(visible /*visible*/, false /*fix*/);

    for (unsigned int Idx = 0; Idx < Objects.size(); Idx++)
      MateProcess(Objects[Idx]);

    m_baseMan->SetVisible(false);
  }
  else
  {
    ChooseMan->UnchooseAll();

    IPlacement3DPtr newPlace = m_process3D->GetPlacement(); // Местоположение курсора при клике
    if (newPlace)
    {
      double x = 0.0, y = 0.0, z = 0.0;

      newPlace->GetOrigin(x, y, z); // Получим координаты

      if (IPlacement3DPtr partPlace = m_part->GetPlacement())
      {
        partPlace->SetOrigin(x, y, z);
        m_part->UpdatePlacement(true);
      }

      if (m_baseMan)
      {
        if (IPlacement3DPtr place3D = m_baseMan->GetPlacement())
        {
          place3D->SetOrigin(x, y, z);
          m_baseMan->UpdatePlacement(true); // Обновим местоположение манипулятора
        }
        m_baseMan->SetVisible(true); // Манипулятор виден
      }

      InitPoint3D(true /*visible*/, true /*fix*/);

      m_procParam->SetDefaultControlFix(ksAllFix); // Зафиксируем объекты
    }
  }
  return true;
}


//-------------------------------------------------------------------------------------
//Создать временное сопряжение в соответствии с указанной поверхностью
// ---
bool Shpeel::MateProcess(const IModelObjectPtr & entity)
{
  m_mCol = m_process3D->GetMateConstraints();

  // Создание временного сопряжение в зависимости от поверхности
  if (IsSurface(entity))
    AddMate(mc_Coincidence, ksMCAlignmentCooriented, ksMFixedPart1, 0, entity, L"Plane", m_part, m_mCol);
  else if (IsSurface(entity, false))
    AddMate(mc_Concentric, ksMCAlignmentClosest, ksMFixedPart1, 0, entity, L"Axis", m_part, m_mCol);
  else if (IsAxis(entity))
    AddMate(mc_Concentric, ksMCAlignmentClosest, ksMFixedPart1, 0, entity, L"Axis", m_part, m_mCol);

  return true;
}


//-------------------------------------------------------------------------------------
// Инициализация 3D-точки
// ---
bool Shpeel::InitPoint3D(bool visible, bool fix)
{
  if (m_part)
  {
    IPropertyPoint3DPtr point3D(GetPropertyControl(ID_POINT_3D));
    if (IPlacement3DPtr place = m_part->GetPlacement())
    {
      double x = 0.0, y = 0.0, z = 0.0;
      place->GetOrigin(x, y, z);

      if (point3D)
      {
        if (visible != point3D->IsVisible())
          point3D->SetVisible(visible);
        point3D->SetCoordinateValue(0, x);
        point3D->SetCoordinateValue(1, y);
        point3D->SetCoordinateValue(2, z);
        point3D->SetCoordinateState(0, fix ? ksCheckFixed : ksCheckVariable);
        point3D->SetCoordinateState(1, fix ? ksCheckFixed : ksCheckVariable);
        point3D->SetCoordinateState(2, fix ? ksCheckFixed : ksCheckVariable);
        m_procParam->SetDefaultControlFix(fix ? ksAllFix : ksAllFixOff);
      }
    }
  }

  return true;
}


//------------------------------------------------------------------------------
// Находит у детали поверхность с заданным именем
// ---
IModelObjectPtr Shpeel::GetEntityByName(const std::wstring & name, IPartPtr & part)
{
  if (!part)
    part = m_part;

  if (part)
    return part->GetObjectByName(name, o3d_unknown, true, true);

  return nullptr;
}


//----------------------------------------------------------------------------------------------------------------------------------------
// ksRotateManipulator - Поворот манипулятора
// ---
bool Shpeel::RotateManipulator(double x0, double y0, double z0, double axisX, double axisY, double axisZ, double angle,
                               bool fromEdit)
{
  IPlacement3DPtr Place(m_part->GetPlacement());

  Place->Rotate(x0, y0, z0, axisX, axisY, axisZ, angle);

  m_part->UpdatePlacement(true);

  IPlacement3DPtr PlaceBase(m_baseMan->GetPlacement());
  PlaceBase->InitByMatrix3D(Place->GetMatrix3D()); // Установка новой системы координат по матрице
  m_baseMan->UpdatePlacement(true);

  InitPoint3D(true /*visible*/, true /*fix*/);

  return true;
}


//----------------------------------------------------------------------------------------------------------------------------------------
// ksMoveManipulator - Передвижение манипулятора
// ---
bool Shpeel::MoveManipulator(int32_t manipulatorId, double vX, double vY, double vZ, double delta, bool fromEdit)
{
  IPlacement3DPtr Place(m_part->GetPlacement());

  IPlacement3DPtr PlaceBase(m_baseMan->GetPlacement());
  double x = 0.0, y = 0.0, z = 0.0;
  Place->GetOrigin(x, y, z);
  PlaceBase->SetOrigin(vX + x, vY + y, vZ + z); // Установка новых координат манипулятора с добавлением смещения

  Place->SetOrigin(vX + x, vY + y, vZ + z);
  m_baseMan->UpdatePlacement(true);
  m_part->UpdatePlacement(true);
  InitPoint3D(true /*visible*/, true /*fix*/);
  return true;
}


//-------------------------------------------------------------------------------------------
// ksBeginDragManipulator - Начало перетаскивания манипулятора
// ---
bool Shpeel::BeginDragManipulator(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType)
{
  return true;
}


//-------------------------------------------------------------------------------------------
// ksEndDragManipulator - окончание перетаскивания манипулятора
// ---
bool Shpeel::EndDragManipulator(int32_t manipulatorId, ksManipulatorPrimitiveEnum primitiveType)
{
  InitPoint3D(true /*visible*/, true /*fix*/);
  return true;
}


//-------------------------------------------------------------------------------------------
// Обработчик команд библиотеки.
// ---
void RunCommand(unsigned int commandId, ksRunCommandModeEnum mode)
{
  IKompasDocument3DPtr doc3D = kompasApp->GetActiveDocument(); // Получить указатель на активный документ трехмерной модели
  if (doc3D && !doc3D->GetTopPart()->IsDetail())
  {
    if (Shpeel * shpeel = new Shpeel())
      shpeel->Draw3D();
  }
  else
  {
    LibMessage(LoadStr(IDS_3DDOCERROR)); // Выдать сообщение о ошибке
  }

  IKompasErrorPtr error = kompasApp->GetKompasError();
  if (error->GetCode() == etError10) // "Ошибка! Вырожденный объект"
    error->Clear();                  // Обнулить ошибки
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


//-------------------------------------------------------------------------------------------
// Инициализация библиотеки.
// ---
APP_EXP_FUNC(bool) LoadKompasLibrary(IApplication & app, IKompasLibraryActions & libaryActions)
{
  libaryActions.AddRunCommandHandler(RunCommand);
  libaryActions.AddGetApplicationVersion(GetApplicationVersion);

  kompasApp = &app;
  LoadResources(app);

  return true;
}


//-------------------------------------------------------------------------------------------
// Освобождение ресурсов перед выгрузкой библиотеки.
// ---
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
}
