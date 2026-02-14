////////////////////////////////////////////////////////////////////////////////
//
// Step2_KsAPI_2D.cpp - Размеры 2D
//
//	1:	CreateLineDimension( doc );				- Создать линейный размер
//	2:	LineDimensionNavigation( doc );		- Навигация по коллекции линейных размеров
//	3:	EditLineDimension( doc );					- Редактирование линейного размера
// 	4:	RadialDimensionWork( doc );				- Создание и редактирование радиального размера
//	5:	DiamrtralDimensionWork( doc );		- Создание и редактирование диаметрального размера
//	6:	AngleDimensionWork( doc );				- Создание и редактирование углового размера
// 	7:	ArcDimensionWork( doc );					- Создание и редактирование размера дуги окружности
// 	8:	BreakLineDimensionWork( doc );		- Создание и редактирование линейного размера с обрывом
// 	9:	BreakRadialDimensionWork( doc );	- Создание и редактирование радиального размера с изломом
// 	10:	BreakAngleDimensionWork( doc );		- Создание и редактирование углового размера с обрывом
// 	11:	HeightDimensionWork( doc );				- Создание и редактирование размера высоты
//
////////////////////////////////////////////////////////////////////////////////
#include "Step2_KsAPI_2D.h"
#include "Step2_KsAPI_2D_pch.h"
#include "Resource.h"
#include "CursorProcess.h"

#include <format>

// Интерфейс приложения
ksapi::IApplication * ksAppl = nullptr;

extern void LoadResources(ksapi::IApplication & app);
extern void * GetModule();

namespace lib_func
{
void LightObj(ksapi::IKompasAPIObjectPtr lightObj, bool choose);
void UnChooseAll();
std::int32_t LibMessage(const std::wstring & message, ksMessageTypeEnum dlgType = ksMessageInformation,
                        ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false);
bool YesNoMessage(const std::wstring & message);
ksapi::IProcessPtr CreateProcess(ksProcess2DTypeEnum procType);
} // namespace lib_func

//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов
*/
//---
std::int32_t GetLibraryId()
{
  return IDR_LIBID;
}


//------------------------------------------------------------------------------
/**
  Получить активный документ
*/
//---
ksapi::IKompasDocument2DPtr GetCurrentDocument()
{
  return ksAppl ? ksAppl->GetActiveDocument() : nullptr;
}


//-------------------------------------------------------------------------------
// Описания используемых функций
// ---
/// Создать линейный размер
void CreateLineDimension(ksapi::IKompasDocument2DPtr & doc);
///	Навигация по коллекции линейных размеров
void LineDimensionNavigation(ksapi::IKompasDocument2DPtr & doc);
///	Редактирование линейного размера
void EditLineDimension(ksapi::IKompasDocument2DPtr & doc);
///	Создание и редактирование радиального размера
void RadialDimensionWork(ksapi::IKompasDocument2DPtr & doc);
///	Создание и редактирование диаметрального размера
void DiametralDimensionWork(ksapi::IKompasDocument2DPtr & doc);
///	Создание и редактирование углового размера
void AngleDimensionWork(ksapi::IKompasDocument2DPtr & doc);
///	Создание и редактирование размера дуги окружности
void ArcDimensionWork(ksapi::IKompasDocument2DPtr & doc);
/// Создание и редактирование линейного размера с обрывом
void BreakLineDimensionWork(ksapi::IKompasDocument2DPtr & doc);
///	Создание и редактирование радиального размера с изломом
void BreakRadialDimensionWork(ksapi::IKompasDocument2DPtr & doc);
///	Создание и редактирование углового размера с обрывом
void BreakAngleDimensionWork(ksapi::IKompasDocument2DPtr & doc);
///	Создание и редактирование размера высоты
void HeightDimensionWork(ksapi::IKompasDocument2DPtr & doc);


//------------------------------------------------------------------------------
/**
  Головная функция библиотеки
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  // Получить текущий документ
  ksapi::IKompasDocument2DPtr doc = GetCurrentDocument();

  if (doc)
  {
    switch (commandId)
    {
      case CM_LINE_DIMM:
        CreateLineDimension(doc);
        break; // Создать линейный размер
      case CM_DIM_COLLECTION:
        LineDimensionNavigation(doc);
        break; // Навигация по коллекции линейных размеров
      case CM_CHANGE_LINE_DIMM:
        EditLineDimension(doc);
        break; // Редактирование линейного размера
      case CM_RADIAL_DIMM:
        RadialDimensionWork(doc);
        break; // Создание и редактирование радиального размера
      case CM_DIAM_DIMM:
        DiametralDimensionWork(doc);
        break; // Создание и редактирование диаметрального размера
      case CM_ANGLE_DIMM:
        AngleDimensionWork(doc);
        break; // Создание и редактирование углового размера
      case CM_ARC_DIMM:
        ArcDimensionWork(doc);
        break; // Создание и редактирование размера дуги окружности
      case CM_BREAK_LINE_DIMM:
        BreakLineDimensionWork(doc);
        break; // Создание и редактирование линейного размера с обрывом
      case CM_BREAK_RADIAL_DIMM:
        BreakRadialDimensionWork(doc);
        break; // Создание и редактирование радиального размера с изломом
      case CM_BREAK_ANGLE_DIMM:
        BreakAngleDimensionWork(doc);
        break; // Создание и редактирование углового размера с обрывом
      case CM_HEIGHT_DIMM:
        HeightDimensionWork(doc);
        break; // Создание и редактирование размера высоты
    }
  }
  else
  {
    lib_func::LibMessage(LoadStr(IDS_NODOC), ksMessageError);
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
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libraryActions)
{
  libraryActions.AddGetApplicationVersion(GetApplicationVersion);
  libraryActions.AddRunCommandHandler(RunCommand);
  libraryActions.AddGetMenuIdHandler(GetLibraryId);

  ksAppl = &app;
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

//-------------------------------------------------------------------------------
//  Получить контейнер обозначений 2D
// ---
ksapi::ISymbols2DContainerPtr GetSymbols2DContainer(ksapi::IKompasDocument2DPtr & doc)
{
  if (doc)
  {
    // Получим менеджер для работы с видами и слоями
    if (ksapi::IViewsAndLayersManagerPtr viewsMng = doc->GetViewsAndLayersManager())
    {
      // Получим коллекцию видов
      if (ksapi::IViewsPtr views = viewsMng->GetViews())
        // Получаем контейнер
        return views->GetActiveView();
    }
  }

  return nullptr;
}


//-------------------------------------------------------------------------------
// Создать линейный размер
// ---
void CreateLineDimension(ksapi::IKompasDocument2DPtr & doc)
{
  lib_func::UnChooseAll();

  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию линейных размеров
    if (ksapi::ILineDimensionsPtr dimCol = symbCont->GetLineDimensions())
    {
      //Добавить объект в коллекцию
      if (ksapi::ILineDimensionPtr newDim = dimCol->Add())
      {
        // Координаты первой точки привязки размера
        newDim->SetX1(50.0);
        newDim->SetY1(150.0);
        // Координаты второй точки привязки размера
        newDim->SetX2(100.0);
        newDim->SetY2(150.0);
        // Положение размерной линии
        newDim->SetX3(75.0);
        newDim->SetY3(180.0);
        // Тип ориентации линейного размера
        newDim->SetOrientation(ksLinDHorizontal);
        // Применить параметры
        newDim->Update();
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Выдать сообщение с параметрами линейного размера
// ---
void GetLineDimensionParam(ksapi::ILineDimensionPtr & dim)
{
  if (!dim)
    return;

  // Координаты первой точки привязки размера
  double x1 = dim->GetX1();
  double y1 = dim->GetY1();
  // Координаты второй точки привязки размера
  double x2 = dim->GetX2();
  double y2 = dim->GetY2();
  // Выдать сообщение с параметрами объекта
  lib_func::LibMessage(std::vformat(std::wstring_view(LoadStr(IDS_COORDS)), std::make_wformat_args(x1, y1, x2, y2)));
}


//-------------------------------------------------------------------------------
// Навигация по коллекции линейных размеров
// ---
void LineDimensionNavigation(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию линейных размеров
    if (ksapi::ILineDimensionsPtr dimCol = symbCont->GetLineDimensions())
    {
      for (int32_t index = 0; index < dimCol->GetCount(); index++)
      {
        // Получить объект из коллекции по индексу
        if (ksapi::ILineDimensionPtr lineDim = dimCol->GetLineDimension(index))
        {
          // Подсветить объект
          lib_func::LightObj(lineDim, true /*включить подсветку*/);
          // Выдать сообщение с параметрами линейного размера
          GetLineDimensionParam(lineDim);
          // Погасить объект
          lib_func::LightObj(lineDim, false /*выключить подсветку*/);
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Изменить параметры у линейного размера
// ---
void ChangeLineDimensionParam(ksapi::ILineDimensionPtr & dim)
{
  if (!dim)
    return;

  // Получить интерфейс параметров размера
  if (ksapi::IDimensionParamsPtr dimPar = dim)
  {
    // Направление полки - влево
    dimPar->SetShelfDirection(ksLSLeft);
    // Типы стрелок - Угол 90 град.
    dimPar->SetArrowType1(ksRightAngle);
    dimPar->SetArrowType2(ksRightAngle);
  }

  // Координаты точки начала полки
  dim->SetShelfX(60.0);
  dim->SetShelfY(200.0);
}


//-------------------------------------------------------------------------------
// Изменить текст у линейного размера
// ---
void ChangeLineDimensionText(ksapi::ILineDimensionPtr & dim)
{
  if (!dim)
    return;

  // Получить интерфейс текста размера
  if (ksapi::IDimensionTextPtr dimText = dim)
  {
    // Текст в рамке
    dimText->SetRectangle(true);
    // Подчеркнуть текст
    dimText->SetUnderLine(true);

    // Изменить текст префикса (до значения размера)
    if (ksapi::ITextLinePtr prefix = dimText->GetPrefix())
      prefix->SetStr(LoadStr(IDS_DIM));
  }
}


//-------------------------------------------------------------------------------
// Изменить линейный размер
// ---
void LineDimChange(ksapi::ILineDimensionPtr & dim)
{
  if (dim)
  {
    // Изменить параметры размера
    ChangeLineDimensionParam(dim);
    // Изменить текст размера
    ChangeLineDimensionText(dim);
    // Применить изменения
    dim->Update();
  }
}

//-------------------------------------------------------------------------------
// Запустить процесс указания объекта
// ---
ksapi::IDrawingObjectPtr ExecuteFindObjectProcess(int32_t command, ksCursorEnum cursorType)
{
  // Процесс указания объектов
  CursorProcess cursorProcess;

  if (ksapi::IProcessPtr & process = cursorProcess.Get())
  {
    cursorProcess.Get()->SetCaption(LoadStr(command));
    cursorProcess.Get()->SetCursorId(cursorType);
  }

  // Объект выбранный пользователем
  ksapi::IDrawingObjectPtr baseObj;

  auto findObjCall = [&baseObj](double x, double y)
  {
    // Найти объект по указанным координатам
    if (ksapi::IKompasDocument2DPtr doc2D = GetCurrentDocument())
      baseObj = doc2D->FindObject(x, y, doc2D->GetCursorLimit(), nullptr);
  };

  cursorProcess.Execute(findObjCall);

  return baseObj;
}

//-------------------------------------------------------------------------------
// Запустить процесс указания положения объекта
// ---
std::pair<double, double> ExecutePlacementObjectProcess(int32_t command, ksCursorEnum cursorId)
{
  // Процесс указания объектов
  CursorProcess cursorProcess;

  if (ksapi::IProcessPtr & process = cursorProcess.Get())
  {
    cursorProcess.Get()->SetCaption(LoadStr(command));
    cursorProcess.Get()->SetCursorId(cursorId);
  }

  // Объект выбранный пользователем
  double x = 0.0, y = 0.0;

  auto placeObjCall = [&x, &y](double x_, double y_)
  {
    x = x_;
    y = y_;
  };

  cursorProcess.Execute(placeObjCall);

  return {x, y};
}


//-------------------------------------------------------------------------------
// Редактирование линейного размера
// ---
void EditLineDimension(ksapi::IKompasDocument2DPtr & doc)
{
  // Запустить процесс указания объекта
  if (ksapi::IDrawingObjectPtr baseObj = ExecuteFindObjectProcess(IDS_COMMAND1, ksCursorCatch))
  {
    // Если это линейный размер, редактировать
    if (baseObj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrLDimension)
    {
      // Получить интерфейс линейного размера
      ksapi::ILineDimensionPtr lineDim(baseObj);
      // Изменить линейный размер
      LineDimChange(lineDim);
    }
    else
      lib_func::LibMessage(LoadStr(IDS_NOTDIM));
  }
}


//-------------------------------------------------------------------------------
// Создать радиальный размер
// ---
bool CreateRadDimension(ksapi::IRadialDimensionPtr & dim)
{
  bool result = false;

  if (!dim)
    return result;

  // Запустить процесс указания объекта
  if (ksapi::IDrawingObjectPtr baseObj = ExecuteFindObjectProcess(IDS_COMMAND1, ksCursorCatch))
  {
    // Является ли объект окружностью
    if (baseObj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle)
    {
      // Получить интерфейс окружности
      if (ksapi::ICirclePtr circle = baseObj)
      {
        // Задать базовый объект размера
        dim->SetBaseObject(circle);
        // Задать координаты центра размера, совпадающие с центром указанной окружности
        dim->SetXc(circle->GetXc());
        dim->SetYc(circle->GetYc());
        // Задать радиус размера, совпадающий с радиусом указанной окружности
        dim->SetRadius(circle->GetRadius());
        // Тип размера - от центра
        dim->SetDimensionType(true);
        // Угол наклона размерной линии
        dim->SetAngle(30.0);
        // Применить параметры
        result = dim->Update();
      }
      else
        lib_func::LibMessage(LoadStr(IDS_NOOBJ));
    }
    else
      lib_func::LibMessage(LoadStr(IDS_NOTCREATE1));
  }
  else
  {
    lib_func::LibMessage(LoadStr(IDS_NOTCREATE1));
  }

  return result;
}


//-------------------------------------------------------------------------------
// Изменить параметры и текст у радиального размера
// ---
void ChangeRadialDimensionParamText(ksapi::IRadialDimensionPtr & dim)
{
  if (!dim)
    return;

  // Получить интерфейс параметров размера
  if (ksapi::IDimensionParamsPtr dimPar = dim)
  {
    // Направление полки - вправо
    dimPar->SetShelfDirection(ksLSRight);
    // Угол наклона полки
    dimPar->SetShelfAngle(180.0);
    // Длина полки
    dimPar->SetShelfLength(30.0);
    // Тип стрелки - засечка
    dimPar->SetArrowType1(ksNotch);
  }

  // Получить интерфейс текстов размера
  if (ksapi::IDimensionTextPtr dimText = dim)
  {
    // Значок перед номиналом - радиус
    dimText->SetSign(3);

    // Получить интерфейс текста единицы измерения и задать текстовую строку
    if (ksapi::ITextLinePtr unit = dimText->GetUnit())
      unit->SetStr(LoadStr(IDS_UNIT));
  }
}


//-------------------------------------------------------------------------------
// Редактировать радиальный размер
// ---
void EditRadialDimension(ksapi::IRadialDimensionPtr & dim)
{
  if (dim)
  {
    // Тип размера - не от центра
    dim->SetDimensionType(false);
    // Изменить параметры и текст размера
    ChangeRadialDimensionParamText(dim);
    // Применить изменения
    dim->Update();
  }
}


//-------------------------------------------------------------------------------
// Создание и редактирование радиального размера
// ---
void RadialDimensionWork(ksapi::IKompasDocument2DPtr & doc)
{
  lib_func::UnChooseAll();

  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию радиальных размеров
    if (ksapi::IRadialDimensionsPtr radCol = symbCont->GetRadialDimensions())
    {
      // Добавить радиальный размер в коллекцию
      if (ksapi::IRadialDimensionPtr radDim = radCol->Add())
      {
        // Создать радиальный размер
        if (CreateRadDimension(radDim) && lib_func::YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Редактировать радиальный размер
          EditRadialDimension(radDim);
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Создать диаметральный размер
// ---
bool CreateDiamrtralDimension(ksapi::IDiametralDimensionPtr & dim)
{
  bool result = false;

  if (!dim)
    return result;

  // Запустить процесс указания объекта
  if (ksapi::IDrawingObjectPtr baseObj = ExecuteFindObjectProcess(IDS_COMMAND2, ksCursorCatch))
  {
    // Является ли объект окружностью
    if (baseObj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle)
    {
      // Получить интерфейс графического объекта из референса
      if (ksapi::ICirclePtr circle = baseObj)
      {
        // Задать базовый объект размера
        dim->SetBaseObject(circle);
        // Задать координаты центра размера, совпадающие с центром указанной окружности
        dim->SetXc(circle->GetXc());
        dim->SetYc(circle->GetYc());
        // Задать радиус размера, совпадающий с радиусом указанной окружности
        dim->SetRadius(circle->GetRadius());
        // Тип размера - полная размерная линия
        dim->SetDimensionType(true);
        // Угол наклона размерной линии
        dim->SetAngle(45.0);
        // Применить параметры
        result = dim->Update();
      }
      else
        lib_func::LibMessage(LoadStr(IDS_NOOBJ));
    }
    else
      lib_func::LibMessage(LoadStr(IDS_NOTCREATE2));
  }
  else
  {
    lib_func::LibMessage(LoadStr(IDS_NOTCREATE2));
  }

  return result;
}


//-------------------------------------------------------------------------------
// Изменить параметры и текст у диаметрального размера
// ---
void ChangeDiametralDimensionParamText(ksapi::IDiametralDimensionPtr & dim)
{
  if (!dim)
    return;

  // Получить интерфейс параметров размера
  if (ksapi::IDimensionParamsPtr dimPar = dim)
  {
    // Направление полки - вправо
    dimPar->SetShelfDirection(ksLSRight);
    // Длина полки
    dimPar->SetShelfLength(20.0);
    // Тип стрелок - точка
    dimPar->SetArrowType1(ksPoint);
    dimPar->SetArrowType2(ksPoint);
  }

  // Получить интерфейс текстов размера
  if (ksapi::IDimensionTextPtr dimText = dim)
  {
    // Значок перед номиналом - диаметр
    dimText->SetSign(1);
    // Подчеркнуть
    dimText->SetUnderLine(true);

    // Получить интерфейс текста единицы измерения и задать текстовую строку
    if (ksapi::ITextLinePtr suffix = dimText->GetSuffix())
      suffix->SetStr(LoadStr(IDS_UNIT));
  }
}


//-------------------------------------------------------------------------------
// Редактировать диаметральный размер
// ---
void EditDiametralDimension(ksapi::IDiametralDimensionPtr & dim)
{
  if (dim)
  {
    // Тип размера - размерная линия с обрывом
    dim->SetDimensionType(false);
    // Угол наклона размерной линии
    dim->SetAngle(90.0);
    // Изменить параметры и текст размера
    ChangeDiametralDimensionParamText(dim);
    // Применить изменения
    dim->Update();
  }
}


//-------------------------------------------------------------------------------
// Создание и редактирование диаметрального размера
// ---
void DiametralDimensionWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию диаметральных размеров
    if (ksapi::IDiametralDimensionsPtr diamCol = symbCont->GetDiametralDimensions())
    {
      // Добавить диаметральный размер в коллекцию
      if (ksapi::IDiametralDimensionPtr diamDim = diamCol->Add())
      {
        // Создать диаметральный размер
        if (CreateDiamrtralDimension(diamDim) && lib_func::YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Редактировать диаметральный размер
          EditDiametralDimension(diamDim);
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Создать отрезок. Определить его параметры, изменить его параметры
*/
//---
ksapi::ILineSegmentPtr DrawLineSeg(ksapi::IKompasDocument2DPtr & document, double x1, double y1, double x2, double y2,
                                   int32_t lineStyle)
{
  if (!document)
    return nullptr;

  ksapi::IViewsAndLayersManagerPtr layersMngr = document->GetViewsAndLayersManager();
  if (!layersMngr)
    return nullptr;

  ksapi::IViewsPtr views = layersMngr->GetViews();
  if (!views)
    return nullptr;

  // создание отрезка
  if (ksapi::IDrawingContainerPtr drawingContainer = views->GetActiveView())
  {
    if (ksapi::ILineSegmentsPtr lineSegs = drawingContainer->GetLineSegments())
    {
      if (ksapi::ILineSegmentPtr lineSeg = lineSegs->Add())
      {
        lineSeg->SetX1(x1);           // координаты начальной точки
        lineSeg->SetY1(y1);           // координаты начальной точки
        lineSeg->SetX2(x2);           // координаты конечной точки
        lineSeg->SetY2(y2);           // координаты конечной точки
        lineSeg->SetStyle(lineStyle); // Стиль линии
        lineSeg->Update();

        return lineSeg;
      }
    }
  }

  return nullptr;
}

//-------------------------------------------------------------------------------
// Создать угловой размер
// ---
bool CreateAngleDimension(ksapi::IAngleDimensionPtr & dim)
{
  bool result = false;

  ksapi::IKompasDocument2DPtr doc2D = GetCurrentDocument();

  if (!dim || !doc2D)
    return result;

  // Построение размера на объектах, указанных в чертеже
  if (lib_func::YesNoMessage(LoadStr(IDS_ASK1)))
  {
    // Запустить процесс указания объекта
    if (ksapi::IDrawingObjectPtr baseObj1 = ExecuteFindObjectProcess(IDS_COMMAND3, ksCursorCatch))
    {
      // Получить тип первого объекта
      auto type1 = baseObj1->GetDrawingObjectType();
      if (type1 == DrawingObjectTypeEnum::ksDrLineSeg || type1 == DrawingObjectTypeEnum::ksDrPolyline ||
          type1 == DrawingObjectTypeEnum::ksDrRectangle)
      {
        // Подсветить указанный объект
        lib_func::LightObj(baseObj1, true);

        // Указать второй базовый объект в документе
        if (ksapi::IDrawingObjectPtr baseObj2 = ExecuteFindObjectProcess(IDS_COMMAND4, ksCursorCatch))

        {
          // Получить тип второго объекта
          auto type2 = baseObj2->GetDrawingObjectType();

          if (type2 == DrawingObjectTypeEnum::ksDrLineSeg || type2 == DrawingObjectTypeEnum::ksDrPolyline ||
              type2 == DrawingObjectTypeEnum::ksDrRectangle)
          {
            // Подсветить указанный объект
            lib_func::LightObj(baseObj2, true);

            if (baseObj1 && baseObj2)
            {
              // Задать базовые объекты размера
              dim->SetBaseObject1(baseObj1);
              dim->SetBaseObject2(baseObj2);
              // Радиус дуги
              dim->SetRadius(20.0);
              // Применить параметры
              dim->Update();
              // Убрать подсветку объекта
              lib_func::LightObj(baseObj2, false);
              result = true;
            }
            else
              lib_func::LibMessage(LoadStr(IDS_NOOBJS));

            // Убрать подсветку объекта
            lib_func::LightObj(baseObj1, false);
          }
          else
            lib_func::LibMessage(LoadStr(IDS_NOTCREATE3));
        }
      }
      else
        lib_func::LibMessage(LoadStr(IDS_NOTCREATE3));
    }
    else
    {
      lib_func::LibMessage(LoadStr(IDS_NOTCREATE3));
    }
  }
  else // Построение размера на двух отрезках
  {
    // Построить первый опорный объект
    ksapi::ILineSegmentPtr line1 = DrawLineSeg(doc2D, 80.0, 120.0, 100.0, 120.0, 1);
    // Построить второй опорный объект
    ksapi::ILineSegmentPtr line2 = DrawLineSeg(doc2D, 80.0, 120.0, 120.0, 160.0, 1);

    if (line1 && line2)
    {
      // Задать опорные объекты
      dim->SetBaseObject1(line1);
      dim->SetBaseObject2(line2);
      // Координаты центра
      dim->SetXc(90.0);
      dim->SetYc(150.0);
      // Радиус дуги
      dim->SetRadius(25.0);
      // Начальный угол  размерной дуги
      dim->SetAngle1(45.0);
      // Конечный угол  размерной дуги
      dim->SetAngle2(30.0);
      // Координаты точки выхода первой выносной линии
      dim->SetX1(80.0);
      dim->SetY1(120.0);
      // Координаты точки выхода второй выносной линии
      dim->SetX2(100.0);
      dim->SetY2(160.0);
      // Тип размера - на минимальный (острый) угол
      dim->SetDimensionType(ksADMinAngle);

      // Включить полку
      if (ksapi::IDimensionParamsPtr dimPar = dim)
        // Направление полки - вправо
        dimPar->SetShelfDirection(ksLSRight);

      // Точка начала полки
      dim->SetShelfX(160.0);
      dim->SetShelfY(140.0);
      // Направление размерной дуги - против часовой стрелки
      dim->SetDirection(false);
      // Начало ножки или точка на дуге
      dim->SetX3(140.0);
      dim->SetY3(120.0);
      // Применить параметры
      result = dim->Update();
    }
  }

  return result;
}


//-------------------------------------------------------------------------------
// Редактирование углового размера
// ---
void EditAngleDimension(ksapi::IAngleDimensionPtr & dim)
{
  if (dim)
  {
    // Тип размера - На максимальный (тупой) угол
    dim->SetDimensionType(ksADMaxAngle);
    // Направление размерной дуги - по часовой стрелке
    dim->SetDirection(true);
    // Точка начала полки
    dim->SetShelfX(150);
    dim->SetShelfY(170);
    // Радиус дуги
    dim->SetRadius(40);
    // Применить изменения
    dim->Update();
  }
}


//-------------------------------------------------------------------------------
// Создание и редактирование углового размера
// ---
void AngleDimensionWork(ksapi::IKompasDocument2DPtr & doc)
{
  lib_func::UnChooseAll();

  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию угловых размеров
    if (ksapi::IAngleDimensionsPtr angCol = symbCont->GetAngleDimensions())
    {
      // Добавить угловой размер в коллекцию
      if (ksapi::IAngleDimensionPtr angDim = angCol->Add(ksDrADimension))
      {
        // Создать угловой размер
        if (CreateAngleDimension(angDim) && lib_func::YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Редактировать угловой размер
          EditAngleDimension(angDim);
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Создание размера дуги окружности
// ---
bool CreateArcDimension(ksapi::IArcDimensionPtr & dim)
{
  bool result = false;

  if (!dim)
    return result;

  // Запустить процесс указания объекта
  if (ksapi::IDrawingObjectPtr baseObj = ExecuteFindObjectProcess(IDS_COMMAND5, ksCursorCatch))
  {
    // Подсветить указанный объект
    lib_func::LightObj(baseObj, true);

    // Является ли объект дугой
    if (baseObj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrArc)
    {
      // Получить интерфейс графического объекта из референса
      if (ksapi::IArcPtr arc = baseObj)
      {
        // Базовый объект
        dim->SetBaseObject(arc);
        // Координаты центра
        dim->SetXc(arc->GetXc());
        dim->SetYc(arc->GetYc());
        // Координаты первой точки дуги
        dim->SetX1(arc->GetX1());
        dim->SetY1(arc->GetY1());
        // Координаты второй точки дуги
        dim->SetX2(arc->GetX2());
        dim->SetY2(arc->GetY2());

        // Указать положение размерной линии
        auto coords = ExecutePlacementObjectProcess(IDS_COMMAND6, ksCursorDefault);
        dim->SetX3(coords.first);
        dim->SetY3(coords.second);

        // Направление размерной дуги
        dim->SetDirection(true);
        // Тип размера - параллельные выносные линии
        dim->SetDimensionType(false);
        // Указатель от текста к дуге
        dim->SetTextPointer(true);
        // Применить параметры
        result = dim->Update();
      }
      else
        lib_func::LibMessage(LoadStr(IDS_NOOBJ));
    }
    else
      lib_func::LibMessage(LoadStr(IDS_NOTCREATE4));

    // Убрать подсветку объекта
    lib_func::LightObj(baseObj, false);
  }
  else
  {
    lib_func::LibMessage(LoadStr(IDS_NOTCREATE4));
  }

  return result;
}


//-------------------------------------------------------------------------------
// Редактирование размера дуги окружности
// ---
void EditArcDimension(ksapi::IArcDimensionPtr & dim)
{
  if (!dim)
    return;

  // Тип размера - выносные линии от центра
  dim->SetDimensionType(true);
  // Указатель от текста к (дуге)
  dim->SetTextPointer(false);
  // Направление размерной дуги - поменять на противоположное
  dim->SetDirection(!dim->GetDirection());

  // Получить интерфейс параметров размера
  if (ksapi::IDimensionParamsPtr dimPar = dim)
  {
    // Тип стрелок
    dimPar->SetArrowType1(ksLeaderPoint);
    dimPar->SetArrowType2(ksLeaderPoint);
    // Направление полки - вниз
    dimPar->SetShelfDirection(ksLSDown);
    // Угол наклона полки
    dimPar->SetShelfAngle(180);
  }

  // Применить изменения
  dim->Update();

  // Получить интерфейс текста размера
  if (ksapi::IDimensionTextPtr dimText = dim)
  {
    // Отключить автоматическое определение номанального значения
    dimText->SetAutoNominalValue(false);
    // Установить новое номинальное значение
    dimText->SetNominalValue(50.0);

    // Получить интерфейс текста единицы измеренияи задать текстовую строку
    if (ksapi::ITextLinePtr unit = dimText->GetUnit())
      unit->SetStr(LoadStr(IDS_UNIT2));
  }

  // Применить изменения
  dim->Update();
}


//-------------------------------------------------------------------------------
// Создание и редактирование размера дуги окружности
// ---
void ArcDimensionWork(ksapi::IKompasDocument2DPtr & doc)
{
  lib_func::UnChooseAll();

  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию размеров дуги окружности
    if (ksapi::IArcDimensionsPtr arcCol = symbCont->GetArcDimensions())
    {
      // Добавить размер дуги окружности в коллекцию
      if (ksapi::IArcDimensionPtr arcDim = arcCol->Add())
      {
        // Создать размер дуги окружности
        if (CreateArcDimension(arcDim) && lib_func::YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Редактировать размер дуги окружности
          EditArcDimension(arcDim);
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Создание линейного размера с обрывом
// ---
bool CreateBreakLineDim(ksapi::IBreakLineDimensionPtr & dim)
{
  bool result = false;

  if (!dim)
    return result;

  // Запустить процесс указания объекта
  if (ksapi::IDrawingObjectPtr baseObj = ExecuteFindObjectProcess(IDS_COMMAND7, ksCursorCatch))
  {
    // Является ли объект отрезком
    if (baseObj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrLineSeg)
    {
      // Подсветить указанный объект
      lib_func::LightObj(baseObj, true);
      // Получить интерфейс графического объекта
      if (ksapi::ILineSegmentPtr lineSeg = baseObj)
      {
        // Базовый объект
        dim->SetBaseObject(lineSeg);

        // Указать положение размерной линии
        auto coords = ExecutePlacementObjectProcess(IDS_COMMAND6, ksCursorDefault);


        // Координаты первой точки привязки - координаты первого конца отрезка
        dim->SetX1(lineSeg->GetX1());
        dim->SetY1(lineSeg->GetY1());
        // Координаты второй точки привязки - координаты второго конца отрезка
        dim->SetX2(lineSeg->GetX2());
        dim->SetY2(lineSeg->GetY2());
        // Положение размерной линии - указанные координаты
        dim->SetX3(coords.first);
        dim->SetY3(coords.second);

        // Применить параметры
        result = dim->Update();
      }
      else
        lib_func::LibMessage(LoadStr(IDS_NOOBJ));
    }
    else
      lib_func::LibMessage(LoadStr(IDS_NOTCREATE5));

    // Убрать подсветку объекта
    lib_func::LightObj(baseObj, false);
  }
  else
  {
    lib_func::LibMessage(LoadStr(IDS_NOTCREATE5));
  }

  return result;
}


//-------------------------------------------------------------------------------
// Редактирование линейного размера с обрывом
// ---
void EditBreakLineDim(ksapi::IBreakLineDimensionPtr & dim)
{
  if (!dim)
    return;

  // Получить интерфейс параметров размера
  if (ksapi::IDimensionParamsPtr dimPar = dim)
  {
    // Тип стрелки
    dimPar->SetArrowType1(ksLeftNotch);
  }

  // Получить интерфейс текста размера
  if (ksapi::IDimensionTextPtr dimText = dim)
  {
    // Получить текст перед размером
    if (ksapi::ITextLinePtr prefix = dimText->GetPrefix())
      // Изменить текст
      prefix->SetStr(LoadStr(IDS_DIM));

    // Получить текст номинального значения
    if (ksapi::ITextLinePtr nominal = dimText->GetNominalText())
      // Изменить текст
      nominal->SetStr(LoadStr(IDS_NOMINAL));
  }

  // Применить изменения
  dim->Update();
}


//-------------------------------------------------------------------------------
// Создание и редактирование линейного размера с обрывом
// ---
void BreakLineDimensionWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию линейных размеров с обрывом
    if (ksapi::IBreakLineDimensionsPtr breakCol = symbCont->GetBreakLineDimensions())
    {
      // Добавить линейный размер с обрывом в коллекцию
      if (ksapi::IBreakLineDimensionPtr breakDim = breakCol->Add())
      {
        // Создать линейный размер с обрывом
        if (CreateBreakLineDim(breakDim) && lib_func::YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Редактировать линейный размер с обрывом
          EditBreakLineDim(breakDim);
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Создание радиального размера с изломом
// ---
bool CreateBreakRadialDim(ksapi::IBreakRadialDimensionPtr & dim)
{
  bool result = false;

  if (!dim)
    return result;

  // Указать объект в документе
  if (ksapi::IDrawingObjectPtr baseObj = ExecuteFindObjectProcess(IDS_COMMAND2, ksCursorCatch))
  {
    // Является ли объект окружностью
    if (baseObj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle)
    {
      // Получить интерфейс графического объекта из референса
      if (ksapi::ICirclePtr circle = baseObj)
      {
        // Задать базовый объект размера
        dim->SetBaseObject(circle);
        // Задать координаты центра размера, совпадающие с центром указанной окружности
        dim->SetXc(circle->GetXc());
        dim->SetYc(circle->GetYc());
        // Задать радиус размера, совпадающий с радиусом указанной окружности
        dim->SetRadius(circle->GetRadius());
        // Угол наклона размерной линии
        dim->SetAngle(45.0);
        // Длина излома
        dim->SetBreakLength(3.0);
        // Положение размерной надписи
        dim->SetTextOnLine(ksDimTextParallelOnLine);
        // Применить параметры
        result = dim->Update();
      }
      else
        lib_func::LibMessage(LoadStr(IDS_NOOBJ));
    }
    else
      lib_func::LibMessage(LoadStr(IDS_NOTCREATE1));
  }
  else
  {
    lib_func::LibMessage(LoadStr(IDS_NOTCREATE1));
  }

  return result;
}


//-------------------------------------------------------------------------------
// Редактировать радиальный размер с изломом
// ---
void EditBreakRadialDim(ksapi::IBreakRadialDimensionPtr & dim)
{
  if (!dim)
    return;

  // Угол наклона размерной линии
  dim->SetAngle(90.0);
  // Длина излома
  dim->SetBreakLength(1);
  // Положение размерной надписи
  dim->SetTextOnLine(ksDimTextParallelInCut);

  // Получить интерфейс текста размера
  if (ksapi::IDimensionTextPtr dimText = dim)
  {
    // Убрать значок радиуса
    dimText->SetSign(0);
  }
  // Применить изменения
  dim->Update();
}


//-------------------------------------------------------------------------------
// Создание и редактирование радиального размера с изломом
// ---
void BreakRadialDimensionWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию радиальных размеров с изломом
    if (ksapi::IBreakRadialDimensionsPtr breakCol = symbCont->GetBreakRadialDimensions())
    {
      // Добавить радиальный размер с изломом в коллекцию
      if (ksapi::IBreakRadialDimensionPtr breakDim = breakCol->Add())
      {
        // Создать радиальный размер с изломом
        if (CreateBreakRadialDim(breakDim) && lib_func::YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Редактировать радиальный размер с изломом
          EditBreakRadialDim(breakDim);
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Создание углового размера с обрывом
// ---
bool CreateBreakAngleDim(ksapi::IBreakAngleDimensionPtr & dim)
{
  bool result = false;

  ksapi::IKompasDocument2DPtr doc2D = GetCurrentDocument();

  if (!dim || !doc2D)
    return result;

  // Построить первый опорный объект
  ksapi::ILineSegmentPtr line1 = DrawLineSeg(doc2D, 80.0, 120.0, 120.0, 160.0, 1);
  // Построить второй опорный объект
  ksapi::ILineSegmentPtr line2 = DrawLineSeg(doc2D, 80.0, 120.0, 100.0, 120.0, 1);

  if (line1 && line2)
  {
    // Задать опорные объекты
    dim->SetBaseObject1(line1);
    dim->SetBaseObject2(line2);
    // Координаты центра
    dim->SetXc(80.0);
    dim->SetYc(120.0);
    // Радиус дуги
    dim->SetRadius(20.0);
    // Начальный угол  размерной дуги
    dim->SetAngle1(45.0);
    // Конечный угол  размерной дуги
    dim->SetAngle2(30.0);
    // Координаты точки выхода первой выносной линии
    dim->SetX1(120.0);
    dim->SetY1(160.0);
    // Координаты точки выхода второй выносной линии
    dim->SetX2(100.0);
    dim->SetY2(160.0);
    // Тип размера - на минимальный (острый) угол
    dim->SetDimensionType(ksADMinAngle);
    // Направление размерной дуги - против часовой стрелки
    dim->SetDirection(true);
    // Начало ножки или точка на дуге
    dim->SetX3(165.0);
    dim->SetY3(140.0);
    // Применить параметры
    result = dim->Update();
  }

  return result;
}


//-------------------------------------------------------------------------------
// Редактирование углового размера с обрывом
// ---
void EditBreakAngleDim(ksapi::IBreakAngleDimensionPtr & dim)
{
  if (!dim)
    return;

  // Включить полку
  if (ksapi::IDimensionParamsPtr dimPar = dim)
    // Направление полки - вправо
    dimPar->SetShelfDirection(ksLSRight);

  // Точка начала полки
  dim->SetShelfX(167.0);
  dim->SetShelfY(177.0);

  // Получить интерфейс текстов размера
  if (ksapi::IDimensionTextPtr dimText = dim)
  {
    // Выравнивание размерной надписи
    dimText->SetTextAlign(ksDimALowerBoundary);
    // Текст в круглых скобках
    dimText->SetBrackets(ksDimBrackets);
  }
  // Применить изменения
  dim->Update();
}


//-------------------------------------------------------------------------------
// Создание и редактирование углового размера с обрывом
// ---
void BreakAngleDimensionWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию угловых размеров с обрывом
    if (ksapi::IAngleDimensionsPtr breakCol = symbCont->GetAngleDimensions())
    {
      // Добавить угловой размер с обрывом в коллекцию
      if (ksapi::IBreakAngleDimensionPtr breakDim = breakCol->Add(ksDrABreakDimension))
      {
        // Создать угловой размер с обрывом
        if (CreateBreakAngleDim(breakDim) && lib_func::YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Редактировать угловой размер с обрывом
          EditBreakAngleDim(breakDim);
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Создать размер высоты
// ---
bool CreateHeightDimension(ksapi::IHeightDimensionPtr & dim)
{
  if (!dim)
    return false;

  // Указать точку нулевого уровня
  auto coords = ExecutePlacementObjectProcess(IDS_COMMAND8, ksCursorDefault);
  // Указать точку измеряемого уровня
  auto coords1 = ExecutePlacementObjectProcess(IDS_COMMAND9, ksCursorDefault);
  // Указать положение размерной надписи
  auto coords2 = ExecutePlacementObjectProcess(IDS_COMMAND10, ksCursorDefault);

  // Точка нулевого уровня
  dim->SetX(coords.first);
  dim->SetY(coords.second);
  // Точка измеряемого уровня
  dim->SetX1(coords1.first);
  dim->SetY1(coords1.second);
  // Положение размерной надписи
  dim->SetX2(coords2.first);
  dim->SetY2(coords2.second);

  // Применить параметры
  return dim->Update();
}


//-------------------------------------------------------------------------------
// Редактировать размер высоты
// ---
void EditHeightDimension(ksapi::IHeightDimensionPtr & dim)
{
  if (!dim)
    return;

  // Тип размера
  dim->SetDimensionType(ksHDTopViewLeader);

  // Получить интерфейс текста размера
  if (ksapi::IDimensionTextPtr dimText = dim)
  {
    // Условный значок - квадрат
    dimText->SetSign(2);

    // Получить текст номинального значения
    if (ksapi::ITextLinePtr nominal = dimText->GetNominalText())
      // Изменить текст
      nominal->SetStr(LoadStr(IDS_NOMINAL));
  }

  // Применить изменения
  dim->Update();
}


//-------------------------------------------------------------------------------
// Создание и редактирование размера высоты
// ---
void HeightDimensionWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию размеров высоты
    if (ksapi::IHeightDimensionsPtr heightCol = symbCont->GetHeightDimensions())
    {
      // Добавить размер высоты в коллекцию
      if (ksapi::IHeightDimensionPtr heightDim = heightCol->Add())
      {
        // Создать размер высоты
        if (CreateHeightDimension(heightDim) && lib_func::YesNoMessage(LoadStr(IDS_EDIT)))
        {
          // Редактировать размер высоты
          EditHeightDimension(heightDim);
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса
*/
//---
std::wstring LoadStr(std::int32_t strID)
{
  std::wstring resStr;

  if (ksAppl)
  {
    if (ksapi::ILibraryManagerPtr libMng = ksAppl->GetLibraryManager())
    {
      if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
      {
        if (ksapi::IProceduresLibraryPtr procLib = procLibs->GetItemByName(L"Step2_KsAPI_2D"))
        {
          if (ksapi::IKompasDictionaryPtr dictionary = procLib->GetDictionary())
            return dictionary->LoadAndConvertLangString(GetModule(), strID);
        }
      }
    }
  }

  return resStr;
}

namespace lib_func
{
//------------------------------------------------------------------------------
/**
  Подсветить объект
*/
//---
void LightObj(ksapi::IKompasAPIObjectPtr lightObj, bool choose)
{
  if (ksapi::IKompasDocument2DPtr document = GetCurrentDocument())
  {
    if (ksapi::IChooseManagerPtr chooseMng = document->GetChooseManager())
    {
      if (choose)
        chooseMng->Choose({lightObj});
      else
        chooseMng->Unchoose({lightObj});
    }
  }
}

//------------------------------------------------------------------------------
/**
  Сбросить подсветку
*/
//---
void UnChooseAll()
{
  if (ksapi::IKompasDocument2DPtr document = GetCurrentDocument())
  {
    if (ksapi::IChooseManagerPtr chooseMng = document->GetChooseManager())
    {
      chooseMng->UnchooseAll();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Сообщение от библиотеки
*/
//---
std::int32_t LibMessage(const std::wstring & message, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons,
                        bool showModal)
{
  std::int32_t res = 0;

  if (!message.empty() && ksAppl)
    res = ksAppl->ShowMessageBox(message, LoadStr(IDR_LIBID), dlgType, dlgButtons, showModal);

  return res;
}


/// Результат диалога
enum DialogResult
{
  Positive = 6,
  Negative = 7,
  Cancel = 2
};


//------------------------------------------------------------------------------
/**
  Диалог да/нет/отмена
*/
//---
bool YesNoMessage(const std::wstring & message)
{
  std::int32_t res = LibMessage(message, ksMessageWarning, ksButtonSetYesNoCancel, false);
  return res == DialogResult::Positive;
}

ksapi::IProcessPtr CreateProcess(ksProcess2DTypeEnum procType)
{
  ksapi::IProcessPtr process;

  if (ksAppl)
  {
    ksapi::IKompasDocument2DPtr doc2D = ksAppl->GetActiveDocument();
    process = doc2D->GetLibProcess(procType);
    process->Run(false, false);
  }

  return process;
}

} // namespace lib_func
