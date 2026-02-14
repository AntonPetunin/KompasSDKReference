////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////


#include "Gayka1_pch.h"
#include <locale>
#include <codecvt>

#include "Doc2DDispatcher.h"
#include "ParameterObject.h"

extern ksapi::IApplication * application;

//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
Doc2DDispatcher::Doc2DDispatcher()
{
  SetActiveDocument();
}


//------------------------------------------------------------------------------
/**
  Установить активный документ
*/
//---
void Doc2DDispatcher::SetActiveDocument()
{
  IKompasDocument2DPtr doc = application ? application->GetActiveDocument() : nullptr;

  if (doc && doc != m_document)
    m_document = doc;
}


//------------------------------------------------------------------------------
/**
  Создать отрезок по координатам
*/
//---
void Doc2DDispatcher::CreateLineSegment(double x1, double y1, double x2, double y2, unsigned short style)
{
  IDrawingContainerPtr drawCont = GetContainer();
  ksapi::ILineSegmentsPtr lines = drawCont->GetLineSegments();
  ksapi::ILineSegmentPtr line = lines->Add();

  line->SetX1(x1);
  line->SetY1(y1);
  line->SetX2(x2);
  line->SetY2(y2);
  line->SetStyle(style);
  line->Update();

  if (m_filling)
    m_fillingBoundaries.emplace_back(line);
}


//------------------------------------------------------------------------------
/**
  Создать окружность по центру и радиусу
*/
//---
void Doc2DDispatcher::CreateCircle(double xc, double yc, double rad, unsigned short style)
{
  IDrawingContainerPtr drawCont = GetContainer();
  ICirclePtr circle = drawCont->GetCircles()->Add();

  circle->SetStyle(ksCSNormal);
  circle->SetXc(xc);
  circle->SetYc(yc);
  circle->SetRadius(rad);
  circle->SetStyle(style);
  circle->Update();

  if (m_filling)
    m_fillingBoundaries.emplace_back(circle);
}


//------------------------------------------------------------------------------
/**
  Создать дугу по трем точкам
*/
//---
void Doc2DDispatcher::CreateArcBy3Points(double x1, double y1,           // координаты начальной точки на дуге
                                         double xMiddle, double yMiddle, // координаты средней точки на дуге
                                         double x2, double y2,           // координаты конечной точки на дуге
                                         unsigned short style)           // стиль отрисовки дуги
{
  IDrawingContainerPtr drawCont = GetContainer();
  ksapi::IArcPtr arc = drawCont->GetArcs()->Add();

  // По трем точкам
  arc->SetX1(x1);
  arc->SetY1(y1);
  arc->SetX2(x2);
  arc->SetY2(y2);
  arc->SetX3(xMiddle);
  arc->SetY3(yMiddle);
  arc->SetStyle(style);
  arc->Update();

  if (m_filling)
    m_fillingBoundaries.emplace_back(arc);
}


//------------------------------------------------------------------------------
/**
  Создать дугу по центру, радиусу и двум точкам
*/
//---
void Doc2DDispatcher::CreateArcByPoint(double xc, double yc, /*цетр дуги */ double rad, /*радиус дуги */ double x1, double y1,
                                       /*точка на дуге */ double x2, double y2, /*точка на дуге */ bool direction,
                                       unsigned short style)
{
  IDrawingContainerPtr drawCont = GetContainer();
  ksapi::IArcPtr arc = drawCont->GetArcs()->Add();

  // По координатам
  arc->SetXc(xc);
  arc->SetYc(yc);
  arc->SetRadius(rad);
  arc->SetX1(x1);
  arc->SetY1(y1);
  arc->SetX2(x2);
  arc->SetY2(y2);
  arc->SetDirection(direction);
  arc->SetStyle(style);
  arc->Update();

  if (m_filling)
    m_fillingBoundaries.emplace_back(arc);
}


//------------------------------------------------------------------------------
/**
  Создать заливку
*/
//---
void Doc2DDispatcher::CreateColoring(std::int32_t color)
{
  if (IDrawingContainerPtr cont = GetContainer())
  {
    IColouringsPtr colorings = cont->GetColourings();

    if (m_filling = colorings ? colorings->Add() : nullptr)
    {
      if (IColouringPtr coloring = m_filling)
        coloring->SetColor1(color);
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создать штриховку
*/
//---
void Doc2DDispatcher::CreateHatch(unsigned short style, /*стиль штриховки */ double angle, /*угол штриховки в гр. */ double step,
                                  /*шаг штриховки */ double width,
                                  /*ширина полосы штрихования вдоль границы штриховки */ double x0, double y0)
{
  ///Параметры штриховки не применяются из-за IHatchParam
  if (IDrawingContainerPtr drawCont = GetContainer())
  {
    if (m_filling = drawCont->GetHatches()->Add())
    {
      if (IHatchPtr hatch = m_filling)
      {
        if (ksapi::IHatchParamPtr hatchParam = hatch)
        {
          hatch->SetX(x0);
          hatch->SetY(y0);
          hatchParam->SetStep(step);
          hatchParam->SetHatchAngle(angle);
          hatchParam->SetWidth(width);
          //hatchParam->SetStyle(style);
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Закрыть создание заливки или штриховки
*/
//---
void Doc2DDispatcher::CloseFilling()
{
  if (m_filling)
  {
    if (IBoundariesObjectPtr boundariesObj = m_filling)
    {
      if (!m_fillingBoundaries.empty())
      {
        boundariesObj->Clear();
        boundariesObj->AddBoundaries(m_fillingBoundaries, false);
      }

      m_filling->Update();
      m_fillingBoundaries.clear();
      m_filling = nullptr;
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создать группу
*/
//---
IDrawingGroupPtr Doc2DDispatcher::CreateDrawingGroup(bool tmpGroup)
{
  if (IKompasDocument2DPtr kDoc2D = m_document)
  {
    IDrawingGroupsPtr drawGroups = kDoc2D->GetDrawingGroups();
    IDrawingGroupPtr drawGroup = drawGroups->Add(tmpGroup, L"");
    drawGroup->Open();
    m_groups.push(drawGroup);
    return drawGroup;
  }

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Добавить объект в группу
*/
//---
void Doc2DDispatcher::AddObjGroup(ksapi::IDrawingGroupPtr & group, ksapi::IMacroObjectPtr & macroObject)
{
  if (macroObject && group)
    group->AddObjects({macroObject});
}


//------------------------------------------------------------------------------
/**
  Открыть группу
*/
//---
ksapi::IDrawingGroupPtr Doc2DDispatcher::OpenDrawingGroup(IDrawingGroupPtr & createdGroup)
{
  if (createdGroup)
  {
    createdGroup->Open();
    m_groups.push(createdGroup);
    return createdGroup;
  }

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Очистить группу
*/
//---
void Doc2DDispatcher::ClearDrawingGroup(ksapi::IDrawingGroupPtr & group, bool deleteTmp)
{
  if (group)
    group->Clear(deleteTmp);
}


//------------------------------------------------------------------------------
/**
  Закрыть редактирование группы
*/
//---
IDrawingGroupPtr Doc2DDispatcher::CloseGroup()
{
  IDrawingGroupPtr group;

  if (!m_groups.empty())
  {
    if (group = m_groups.top())
    {
      m_groups.pop();
      group->Close();
    }
  }

  return group;
}


//------------------------------------------------------------------------------
/**
  Создать макро
*/
//---
IMacroObjectPtr Doc2DDispatcher::CreateMacroObject(bool multyLayer)
{
  if (IDrawingContainerPtr drawCont = GetContainer())
  {
    IMacroObjectPtr macroObj = drawCont->GetMacroObjects()->Add(multyLayer);

    if (IDrawingContainerPtr newDrawCont = macroObj)
    {
      m_containers.push(newDrawCont);
      m_macroObjs.push(macroObj);
      return macroObj;
    }
  }

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Закрыть работу с макро
*/
//---
IMacroObjectPtr Doc2DDispatcher::CloseMacroObject()
{
  if (!m_containers.empty())
  {
    IMacroObjectPtr macroObj = m_macroObjs.top();
    macroObj->Update();

    m_containers.pop();
    m_macroObjs.pop();

    return macroObj;
  }

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Снять выделение объекта
*/
//---
void Doc2DDispatcher::Unchoose(const IAPIObjectPtr & obj)
{
  if (IKompasDocument2DPtr kDoc2D = m_document)
  {
    IChooseManagerPtr chooseManager = kDoc2D->GetChooseManager();
    chooseManager->Unchoose({obj});
  }
}


//------------------------------------------------------------------------------
/**
  Выделить объект
*/
//---
void Doc2DDispatcher::ChooseObj(const IAPIObjectPtr & obj)
{
  if (IKompasDocument2DPtr kDoc2D = m_document)
  {
    ksapi::IChooseManagerPtr chooseManager = kDoc2D->GetChooseManager();
    chooseManager->Choose({obj});
  }
}


//------------------------------------------------------------------------------
/**
  Подсветить объект
*/
//---
void Doc2DDispatcher::LightObj(ksapi::IPositionLeaderPtr & oldPosLeader, bool choose)
{
  if (IKompasDocument2DPtr document = m_document)
  {
    if (IChooseManagerPtr chooseMng = document->GetChooseManager())
    {
      if (choose)
        chooseMng->Choose({oldPosLeader});
      else
        chooseMng->Unchoose({oldPosLeader});
    }
  }
}


//------------------------------------------------------------------------------
/**
  Установить активный контейнер объектов
*/
//---
void Doc2DDispatcher::SetDocumentDrawingContainer()
{
  if (m_document)
  {
    IViewsAndLayersManagerPtr valMng = m_document->GetViewsAndLayersManager();
    IViewsPtr views = valMng->GetViews();
    IViewPtr view = views->GetActiveView();
    IDrawingContainerPtr drawCont = view;
    m_containers.push(drawCont);
  }
}


//------------------------------------------------------------------------------
/**
  Получить активный контейнер объектов
*/
//---
ksapi::IDrawingContainerPtr Doc2DDispatcher::GetContainer()
{
  if (m_containers.empty())
    SetDocumentDrawingContainer();

  return m_containers.top();
}


namespace math
{

#define M_PI 3.14159265358979323846

constexpr double NULL_EPSILON = 1E-30;  ///< Погрешность для проверки на равенство нулю.
constexpr double DOUBLE_REGION = 1E-15; ///< Погрешность.
constexpr double M_PI2 = M_PI * 2.0;
constexpr double RADDEG = M_PI / 180;
constexpr double ANGLE_EPS = 1E-3;

//------------------------------------------------------------------------------
/**
  Расчитать угол по dx, dy
*/
//---
inline double CalcAngle0X(double dx, double dy)
{
  if (::fabs(dx) < NULL_EPSILON && ::fabs(dy) < NULL_EPSILON)
    return 0.0;

  double angle = atan2(dy, dx);
  if (::fabs(angle) < DOUBLE_REGION)
    angle = 0.0;
  return angle < 0.0 ? math::M_PI2 * angle : angle;
}


//------------------------------------------------------------------------------
/**
  Нормализовать угол
*/
//---
inline double & NormalizeAngle(double & angle, double angleEpsilon = ANGLE_EPS)
{
  if (::fabs(angle) < angleEpsilon || ::fabs(angle - M_PI2) < angleEpsilon)
    angle = 0.0;
  else
  {
    while (angle > math::M_PI2)
      angle -= math::M_PI2;
    while (angle < 0)
      angle += math::M_PI2;
  }
  return angle;
}


//------------------------------------------------------------------------------
/**
  Косинус
*/
//---
double CosD(double angle)
{
  return cos(angle * math::RADDEG);
}


//------------------------------------------------------------------------------
/**
  Синус
*/
//---
double SinD(double angle)
{
  return sin(angle * math::RADDEG);
}


//------------------------------------------------------------------------------
/**
  Тангенс
*/
//---
double TanD(double angle)
{
  return tan(angle * math::RADDEG);
}


//------------------------------------------------------------------------------
/**
  Пересечение двух отрезков 
*/
//---
void IntersectLinSLinS(double x11, double y11, /*первая точка первого отрезка */ double x12, double y12,
                       /*вторая точка первого отрезка */ double x21, double y21, /*первая точка второго отрезка */ double x22,
                       double y22, /*вторая точка второго отрезка */ int & kp, /*количество точек пересечения */ double & xp,
                       double & yp)
{
  xp = 0;
  yp = 0;
  kp = 0;

  IMath2DPtr math = application->GetMath2D();

  ICurve2DPtr curve1 = math->LineSeg(x11, y11, x12, y12);
  ICurve2DPtr curve2 = math->LineSeg(x21, y21, x22, y22);

  std::vector<double> coordinates = curve1->Intersect(curve2);

  if (!coordinates.empty())
  {
    kp = 1;
    xp = coordinates[0];
    yp = coordinates[1];
  }
}


//------------------------------------------------------------------------------
/**
  Пересечение двух линий
*/
//---
void IntersectLinLin(double x1, double y1, /*точка на первой прямой */ double angle1, /*угол первой прямой */ double x2,
                     double y2, /*точка на второй прямой */ double angle2, /*угол второй прямой */ int & kp,
                     /*количество точек пересечения */ double & xp, double & yp)
{
  xp = 0;
  yp = 0;
  kp = 0;

  IMath2DPtr math = application->GetMath2D();

  ICurve2DPtr curve1 = math->Line(x1, y1, angle1);
  ICurve2DPtr curve2 = math->Line(x2, y2, angle2);

  std::vector<double> coordinates = curve1->Intersect(curve2);

  if (!coordinates.empty())
  {
    kp = 1;
    xp = coordinates[0];
    yp = coordinates[1];
  }
}


//------------------------------------------------------------------------------
/**
  Выдает угол в градусах
*/
//---
double Angle(double x1, double y1, double x2, double y2)
{
  double dx = x2 - x1;
  if (fabs(dx) < 0.000001)
    dx = 0;
  double dy = y2 - y1;
  if (fabs(dy) < 0.000001)
    dy = 0;

  double a = CalcAngle0X(dx, dy);
  NormalizeAngle(a);
  return a / math::RADDEG;
}

} // namespace math

namespace macro
{
//------------------------------------------------------------------------------
/**
  Установить параметры макро-объекта  
*/
//---
void SetMacroParam(IMacroObjectPtr macroObj, /* указатель на макроэлемент */ void * userPars,
                   /* указатель на буфер дополнительных параметров */ unsigned int size,
                   /* размер буфера дополнительных параметров */ const std::wstring & fileName,
                   /* имя файла библиотеки, в которой находтся функция редактирования */ const std::wstring & libName,
                   /* имя библиотеки, в которой находтся функция редактирования */ int command, bool hotPointEditable,
                   bool propObjEditable)
{
  if (macroObj)
  {
    macroObj->SetHotPointsEditable(hotPointEditable);
    macroObj->SetPropertyObjectEditable(propObjEditable);
  }

  if (IUserParametersPtr pars = macroObj)
  {
    pars->SetLibraryFileName(fileName);
    pars->SetLibraryName(libName);

    if (userPars)
    {
      std::vector<std::uint8_t> userParams;
      userParams.reserve(size);
      const std::int8_t * data = static_cast<const std::int8_t *>(userPars);

      for (size_t i = 0; i < size; i++)
        userParams.push_back(data[i]);

      pars->SetUserParams(userParams);
    }

    pars->SetCommand(command);
  }
}
//------------------------------------------------------------------------------
/**
  Получить параметры макро-объекта  
*/
//---
std::int32_t GetMacroParams(IMacroObjectPtr macroObj, std::wstring * fileName, std::wstring * libName)
{
  IUserParametersPtr pars = macroObj;

  if (pars)
  {
    if (fileName)
      *fileName = pars->GetLibraryFileName();

    if (libName)
      *libName = pars->GetLibraryName();

    return pars->GetCommand();
  }

  return 0;
}
//------------------------------------------------------------------------------
/**
  Обновить макро  
*/
//---
void UpdateMacro(ksapi::IMacroObjectPtr & macroObject, ksapi::IDrawingGroupPtr & group)
{
  if (macroObject && group)
  {
    macroObject->SetObjects({group});
    macroObject->Update();
  }
}
//------------------------------------------------------------------------------
/**
  Получить редактируемый макро  
*/
//---
IMacroObjectPtr GetEditMacroObject()
{
  IKompasDocument2DPtr kDoc2D = application->GetActiveDocument();
  return kDoc2D ? kDoc2D->GetEditMacroObject() : nullptr;
}

//------------------------------------------------------------------------------
/**
  Получить пользовательские параметры  
*/
//---
bool GetMacroParam(ParameterObject & parameterObj, const IMacroObjectPtr & macroObj)
{
  if (IUserParametersPtr copyParam = macroObj)
  {
    std::int32_t size = parameterObj.Size();
    std::vector<std::uint8_t> params = copyParam->GetUserParams();

    if (params.size() == size)
    {
      void * userParams = parameterObj;
      memcpy(userParams, params.data(), size);
    }

    return true;
  }

  return false;
}
//------------------------------------------------------------------------------
/**
  Получить положение и угол макро  
*/
//---
bool GetMacroPlacement(ksapi::IMacroObjectPtr & macroObj, double & x, double & y, double & angle, bool & mirrorSymetry)
{
  return macroObj && macroObj->GetPlacement(x, y, angle, mirrorSymetry);
}

} // namespace macro


//------------------------------------------------------------------------------
/**
  Конвертировать строку из std::string в std::wstring
*/
//---
std::wstring GetWStringFromString(const std::string & str)
{
#pragma warning(push)
#pragma warning(disable : 4996)
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.from_bytes(str);
#pragma warning(pop)
}


//------------------------------------------------------------------------------
/**
  Конвертировать строку из std::wstring в std::string
*/
//---
std::string GetStringFromWString(const std::wstring & wstr)
{
#pragma warning(push)
#pragma warning(disable : 4996)
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.to_bytes(wstr.c_str());
#pragma warning(pop)
}
