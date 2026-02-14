////////////////////////////////////////////////////////////////////////////////
//
// Step2_KsAPI_3D.cpp - Размеры 3D.
//
//	1: CreateLineDimension3D( doc );				- Создание линейного размера 3D.
//	2: LineDimension3DNavigation( doc );		- Навигация по коллекции линейных размеров 3D.
//	3: EditLineDimension3D( doc );					- Редактирование линейного размера 3D.
//	4: RadialDimension3DWork( doc );				- Создание и редактирование радиального размера 3D.
//	5: DiametralDimension3DWork( doc );			- Создание и редактирование диаметрального размера 3D.
//	6: AngleDimension3DWork( doc );					- Создание и редактирование углового размера 3D.
//
////////////////////////////////////////////////////////////////////////////////

#include "Step2_KsAPI_3D.h"
#include "CursorProcess.h"
#include "MessageUtils.h"
#include <format>

ksapi::IApplication * kompasApp = nullptr;

// Описания используемых функций.
void CreateLineDimension3D(ksapi::IKompasDocument3DPtr & doc);
void LineDimension3DNavigation(ksapi::IKompasDocument3DPtr & doc);
void EditLineDimension3D(ksapi::IKompasDocument3DPtr & doc);
void RadialDimension3DWork(ksapi::IKompasDocument3DPtr & doc);
void DiametralDimension3DWork(ksapi::IKompasDocument3DPtr & doc);
void AngleDimension3DWork(ksapi::IKompasDocument3DPtr & doc);

// Тип объектов для функции фильтрации.
int oType = o3d_edge;

//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов.
*/
//---
std::int32_t GetLibraryId()
{
  return IDR_LIBID;
}

//------------------------------------------------------------------------------
/**
  Головная функция библиотеки.
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  ksapi::IKompasDocument3DPtr doc = kompasApp->GetActiveDocument();

  if (doc)
  {
    switch (commandId)
    {
      case 1:
        CreateLineDimension3D(doc);
        break; // Создание линейного размера 3D.
      case 2:
        LineDimension3DNavigation(doc);
        break; // Навигация по коллекции линейных размеров 3D.
      case 3:
        EditLineDimension3D(doc);
        break; // Редактирование линейного размера 3D.
      case 4:
        RadialDimension3DWork(doc);
        break; // Создание и редактирование радиального размера 3D.
      case 5:
        DiametralDimension3DWork(doc);
        break; // Создание и редактирование диаметрального размера 3D.
      case 6:
        AngleDimension3DWork(doc);
        break; // Создание и редактирование углового размера 3D.
    }
  }
  else
  {
    LibMessage(LoadStr(IDS_NODOC), ksMessageWarning, ksButtonSetOk, true);
  }
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
  libraryActions.AddGetMenuIdHandler(GetLibraryId);

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
}

//------------------------------------------------------------------------------
/**
  Получить контейнер обозначений 3D.
*/
//---
ksapi::ISymbols3DContainerPtr GetSymbols3DContainer(ksapi::IKompasDocument3DPtr & doc)
{
  if (doc)
    return doc->GetTopPart();

  return nullptr;
}

//------------------------------------------------------------------------------
/**
  Функция фильтрации.
*/
//---
bool __stdcall UserFilterProc(ksapi::IModelObjectPtr e)
{
  if (e && (!oType || e->GetModelObjectType() == oType))
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------
/**
  Установить объект привязки и базовую плоскость.
*/
//---
bool SetLineDimObjectPlane(ksapi::ILineDimension3DPtr & dim, ksapi::IKompasDocument3DPtr & doc)
{
  bool res = false;

  // Фильтрация объектов - ребра.
  oType = o3d_edge;

  // Указать в документе объект - ребро.
  ksapi::IModelObjectPtr obj1;

  auto && getObj1 = [&](const ksapi::IKompasAPIObjectPtr & object)
  {
    obj1 = object;
    return UserFilterProc(object);
  };

  CursorProcess cursorProcess;

  if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
    process.Get()->SetCaption(LoadStr(IDS_OBJ1));

  cursorProcess.Execute(getObj1);

  if (obj1)
  {
    // Тип фильтрации - грани.
    oType = o3d_face;
    // Указать в документе объект - грань.
    ksapi::IModelObjectPtr plane;

    auto && getPlane = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      plane = object;
      return UserFilterProc(object);
    };

    CursorProcess cursorProcess;

    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_PLANE));

    cursorProcess.Execute(getPlane);

    if (plane && dim)
    {
      // Установить первый объект размера (Второй объект устанавливается
      // только в том случае, если измеряется расстояние между точками).
      dim->SetObject1(obj1);
      // Установить базовую плоскость размера.
      dim->SetPlane(plane);
      res = true;
    }
  }
  return res;
}

//------------------------------------------------------------------------------
/**
  Создание линейного размера 3D.
*/
//---
void CreateLineDimension3D(ksapi::IKompasDocument3DPtr & doc)
{
  // Получить контейнер обозначений 3D.
  ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc);

  if (symbCont)
  {
    // Получить коллекцию линейных размеров 3D.
    ksapi::ILineDimensions3DPtr dimsCol = symbCont->GetLineDimensions3D();

    if (dimsCol)
    {
      // Добавить новый линейный размер 3D.
      ksapi::ILineDimension3DPtr newDim = dimsCol->Add(o3d_lineDimension3D);

      if (newDim)
      {
        bool create = false;

        // Если удалось установить объект привязки и базовую плоскость.
        if (SetLineDimObjectPlane(newDim, doc))
        {
          // Установить длину размера.
          newDim->SetLength(30);
          // Применить параметры.
          create = newDim->Update();
        }
        else
        {
          // Выдать сообщение "Объект не создан".
          LibMessage(LoadStr(IDS_NOCREATE), ksMessageWarning, ksButtonSetOk, true);
        }
        // Если объект не создался, удалить.
        if (!create)
        {
          ksapi::IFeaturePtr obj = newDim;

          if (obj)
            obj->Delete();
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с параметрами линейного размера 3D.
*/
//---
void GetLineDimensionPar(ksapi::ILineDimension3DPtr & dim)
{
  if (dim)
  {
    // Получить длину размера.
    double lenght = dim->GetLength();
    double val = 0;

    // Получить значение размера.
    ksapi::IDimensionTextPtr dimText = dim;

    if (dimText)
      val = dimText->GetNominalValue();

    // Сформировать сообщение.
    std::wstring outMessage;
    outMessage.append(LoadStr(IDS_LINEDIM3D))
      .append(std::vformat(std::wstring_view(LoadStr(IDS_LENGTH)), std::make_wformat_args(lenght)))
      .append(std::vformat(std::wstring_view(LoadStr(IDS_DIMVAL)), std::make_wformat_args(val)));
    LibMessage(outMessage, ksMessageWarning, ksButtonSetOk, true);
  }
}


//------------------------------------------------------------------------------
/**
  Навигация по коллекции линейных размеров 3D.
*/
//---
void LineDimension3DNavigation(ksapi::IKompasDocument3DPtr & doc)
{
  if (doc)
  {
    // Получить контейнер обозначений 3D.
    ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc);

    if (symbCont)
    {
      // Получить коллекцию линейных размеров 3D.
      ksapi::ILineDimensions3DPtr dimsCol = symbCont->GetLineDimensions3D();

      // Получить менеджер селектирования.
      ksapi::ISelectionManagerPtr selectMng = doc->GetSelectionManager();

      if (dimsCol && selectMng)
      {
        // Цикл по коллекции.
        for (int32_t i = 0; i < dimsCol->GetCount(); i++)
        {
          // Получить размер из коллекции по индексу.
          ksapi::ILineDimension3DPtr lineDim = dimsCol->GetLineDimension3D(i);

          if (lineDim)
          {
            // Подсветить объект.
            selectMng->Select({lineDim});
            // Выдать сообщение с параметрами размера.
            GetLineDimensionPar(lineDim);
            // Убрать подсветку.
            selectMng->Unselect({lineDim});
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Установить новую базовую плоскость.
*/
//---
bool SetNewPlane(ksapi::ILineDimension3DPtr & dim, ksapi::IKompasDocument3DPtr & doc)
{
  bool res = false;

  if (dim && doc)
  {
    int oType = o3d_face;
    // Указать в документе объект.
    ksapi::IModelObjectPtr plane;

    auto && getPlane = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      plane = object;
      return UserFilterProc(object);
    };

    CursorProcess cursorProcess;

    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_NEWPLANE));

    cursorProcess.Execute(getPlane);

    if (plane)
    {
      // Установить базовую плоскость.
      dim->SetPlane(plane);
      res = true;
    }
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Изменить параметры линейного размера 3D.
*/
//---
void ChangeLineDimensionPar(ksapi::ILineDimension3DPtr & dim)
{
  if (dim)
  {
    // Получить интерфейс параметров размера.
    ksapi::IDimensionParamsPtr dimPars = dim;

    if (dimPars)
    {
      // Тип стрелок - засечка.
      dimPars->SetArrowType1(ksNotch);
      dimPars->SetArrowType2(ksNotch);
      // Расположение стрелок - снаружи.
      dimPars->SetArrowPos(ksDimArrowOutside);
      // Направление полки - вправо.
      dimPars->SetShelfDirection(ksLSRight);
      // Длина полки.
      dimPars->SetShelfLength(10);
      // Угол наклона полки.
      dimPars->SetShelfAngle(45);
    }
  }
}


//------------------------------------------------------------------------------
/**
  Редактирование линейного размера 3D.
*/
//---
void EditLineDimension3D(ksapi::IKompasDocument3DPtr & doc)
{
  if (doc)
  {
    int oType = o3d_lineDimension3D;
    // Указать в документе объект.
    ksapi::IModelObjectPtr obj1;

    auto && getObj1 = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      obj1 = object;
      return true;
    };

    CursorProcess cursorProcess;

    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_DIM));

    cursorProcess.Execute(getObj1);

    if (obj1)
    {
      if (obj1->GetModelObjectType() == o3d_lineDimension3D)
      {
        // Получить интерфейс линейного размера 3D.
        ksapi::ILineDimension3DPtr lineDim = obj1;

        if (lineDim)
        {
          // Установить новую базовую плоскость.
          if (!SetNewPlane(lineDim, doc))
            // Выдать сообщение "Не удалось установить новую базовую плоскость".
            LibMessage(LoadStr(IDS_NOTSETPLANE), ksMessageWarning, ksButtonSetOk, true);

          // Изменить параметры размера.
          ChangeLineDimensionPar(lineDim);
          // Уменьшить длину размера.
          lineDim->SetLength(lineDim->GetLength() - 10);
          // Применить изменения.
          lineDim->Update();
        }
      }
      else
        // Выдать сообщение "Объект не является линейным размером".
        LibMessage(LoadStr(IDS_NOTDIM), ksMessageWarning, ksButtonSetOk, true);
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создать радиальный размер 3D.
*/
//---
bool CreateRadDimension3D(ksapi::IRadialDimension3DPtr & dim, ksapi::IKompasDocument3DPtr & doc)
{
  bool res = false;

  if (dim)
  {
    if (doc)
    {
      oType = o3d_edge;
      // Указать в документе объект - ребро.
      ksapi::IModelObjectPtr edge;

      auto && getEdge = [&](const ksapi::IKompasAPIObjectPtr & object)
      {
        edge = object;
        return UserFilterProc(object);
      };

      CursorProcess cursorProcess;

      if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
        process.Get()->SetCaption(LoadStr(IDS_OBJ1));

      cursorProcess.Execute(getEdge);

      if (edge)
      {
        // Получить интерфейс свойств ребра.
        ksapi::IEdgePtr edgeObj = edge;

        // Проверить, является ли ребро круговым.
        if (edgeObj && edgeObj->IsCircle())
        {
          // Установить объект.
          dim->SetObject1(edge);

          // Получить интерфейс параметров объекта.
          ksapi::IDimensionParamsPtr dimPars = dim;

          if (dimPars)
          {
            // Направление полки - влево.
            dimPars->SetShelfDirection(ksLSLeft);
            // Длина полки.
            dimPars->SetShelfLength(15);
            // Угол наклона полки.
            dimPars->SetShelfAngle(30);
          }

          // Тип размера - не от центра.
          dim->SetDimensionType(false);
          // Применить параметры.
          dim->Update();
          res = true;
        }
        else
          // Выдать сообщение "Ребро не является круговым".
          LibMessage(LoadStr(IDS_NOTCIRCLE), ksMessageWarning, ksButtonSetOk, true);
      }
    }
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Редактировать радиальный размер 3D.
*/
//---
void EditRadDimension3D(ksapi::IRadialDimension3DPtr & dim)
{
  if (dim)
  {
    // Тип размера - от центра.
    dim->SetDimensionType(true);

    // Получить интерфейс параметров размера.
    ksapi::IDimensionParamsPtr dimPars = dim;

    if (dimPars)
      // Отключить полку.
      dimPars->SetShelfDirection(ksLSNone);

    // Получить интерфейс текстов размера.
    ksapi::IDimensionTextPtr dimText = dim;

    if (dimText)
    {
      // Задать квалитет.
      dimText->SetTolerance(L"h6");

      // Включить квалитет.
      dimText->SetToleranceOn(true);

      // Получить интерфейс текста нижнего отклонения.
      ksapi::ITextLinePtr lowDev = dimText->GetLowDeviation();

      // Задать текстовую строку.
      if (lowDev)
        lowDev->SetStr(L"+0.021");
    }
    // Применить изменения.
    dim->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование радиального размера 3D.
*/
//---
void RadialDimension3DWork(ksapi::IKompasDocument3DPtr & doc)
{
  // Получить контейнер обозначений 3D.
  ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc);

  if (symbCont)
  {
    // Получить коллекцию радиальных размеров 3D.
    ksapi::IRadialDimensions3DPtr dimsCol = symbCont->GetRadialDimensions3D();

    if (dimsCol)
    {
      // Добавить новый радиальный размер 3D.
      ksapi::IRadialDimension3DPtr newDim = dimsCol->Add();

      if (newDim)
      {
        // Создать радиальный размер 3D.
        if (CreateRadDimension3D(newDim, doc))
        {
          std::wstring name = L"";
          // Получить интерфейс объекта дерева построения.
          ksapi::IFeaturePtr feature = newDim;

          // Получить имя объекта.
          if (feature)
            name = feature->GetName();

          if (YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            // Получить объект из коллекции по имени.
            ksapi::IRadialDimension3DPtr radDim = dimsCol->GetRadialDimension3DByName(name);
            // Редактировать размер.
            EditRadDimension3D(radDim);
          }
        }
        else
          // Выдать сообщение "Объект не создан".
          LibMessage(LoadStr(IDS_NOCREATE), ksMessageWarning, ksButtonSetOk, true);
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создать димаметральный размер.
*/
//---
bool CreateDiamDimension3D(ksapi::IDiametralDimension3DPtr & dim, ksapi::IKompasDocument3DPtr & doc)
{
  bool res = false;

  if (dim)
  {
    if (doc)
    {
      // Указать в документе объект - ребро.
      oType = o3d_edge;
      ksapi::IModelObjectPtr edgeObj;

      auto && getEdgeObj = [&](const ksapi::IKompasAPIObjectPtr & object)
      {
        edgeObj = object;
        return UserFilterProc(object);
      };

      CursorProcess cursorProcess;

      if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
        process.Get()->SetCaption(LoadStr(IDS_OBJ1));

      cursorProcess.Execute(getEdgeObj);

      if (edgeObj)
      {
        // Получить интерфейс свойств ребра.
        ksapi::IEdgePtr edge = edgeObj;

        // Проверить, является ли ребро круговым.
        if (edge && edge->IsCircle())
        {
          // Установить объект.
          dim->SetObject1(edgeObj);

          // Применить параметры.
          dim->Update();
          res = true;
        }
        else
          // Выдать сообщение "Ребро не является круговым".
          LibMessage(LoadStr(IDS_NOTCIRCLE), ksMessageWarning, ksButtonSetOk, true);
      }
    }
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Редактировать диаметральный размер.
*/
//---
void EditDiamDimension3D(ksapi::IDiametralDimension3DPtr & dim)
{
  if (dim)
  {
    // Тип размера - с обрывом.
    dim->SetDimensionType(true);

    // Получить интерфейс текстов размера.
    ksapi::IDimensionTextPtr dimText = dim;

    if (dimText)
    {
      // Получить интерфейс текста под размерной надписью.
      ksapi::ITextPtr txtUnder = dimText->GetTextUnder();

      // Задать текстовую строку.
      if (txtUnder)
        txtUnder->SetStr(LoadStr(IDS_DIMTEXT));

      // Подчеркнутый текст.
      dimText->SetUnderLine(true);
    }
    // Применить изменения.
    dim->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование диаметрального размера 3D.
*/
//---
void DiametralDimension3DWork(ksapi::IKompasDocument3DPtr & doc)
{
  // Получить контейнер обозначений 3D.
  ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc);

  if (symbCont)
  {
    // Получить коллекцию диаметральных размеров 3D.
    ksapi::IDiametralDimensions3DPtr dimsCol = symbCont->GetDiametralDimensions3D();

    if (dimsCol)
    {
      // Добавить новый диаметральный размер 3D.
      ksapi::IDiametralDimension3DPtr newDim = dimsCol->Add();

      if (newDim)
      {
        // Создать диаметральный размер 3D.
        if (CreateDiamDimension3D(newDim, doc))
        {
          std::wstring name = L"";
          // Получить интерфейс объекта дерева построения.
          ksapi::IFeaturePtr feature = newDim;

          // Получить имя объекта.
          if (feature)
            name = feature->GetName();

          if (YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            // Получить объект из коллекции по имени.
            ksapi::IDiametralDimension3DPtr diamDim = dimsCol->GetDiametralDimension3DByName(name);
            // Редактировать размер.
            EditDiamDimension3D(diamDim);
          }
        }
        else
          // Выдать сообщение "Объект не создан".
          LibMessage(LoadStr(IDS_NOCREATE), ksMessageWarning, ksButtonSetOk, true);
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создать угловой размер 3D.
*/
//---
bool CreateAngleDimension3D(ksapi::IAngleDimension3DPtr & dim, ksapi::IKompasDocument3DPtr & doc)
{
  bool res = false;

  if (dim && doc)
  {
    // Указать в документе 1-й объект.
    ksapi::IModelObjectPtr obj1;

    auto && getObj1 = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      obj1 = object;
      return true;
    };

    CursorProcess cursorProcess;

    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_OBJECT1));

    cursorProcess.Execute(getObj1);
    // Указать в документе 2-й объект.
    ksapi::IModelObjectPtr obj2;

    auto && getObj2 = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      obj2 = object;
      return true;
    };

    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_OBJECT2));

    cursorProcess.Execute(getObj2);

    if (obj1 && obj2)
    {
      // 1-й объект.
      dim->SetObject1(obj1);
      // 2-й объект.
      dim->SetObject2(obj2);
      // Длина размерной линии.
      dim->SetLength(20);
      // Применить параметры.
      dim->Update();
      res = true;
    }
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Редактировать угловой размер 3D.
*/
//---
void EditAngleDimension3D(ksapi::IAngleDimension3DPtr & dim)
{
  if (dim)
  {
    // Тип размера - на максимальный (тупой) угол.
    dim->SetDimensionType(ksADMaxAngle);
    // Увеличить длину размерной линии.
    dim->SetLength(dim->GetLength() + 10);

    // Получить интерфейс параметров размера.
    ksapi::IDimensionParamsPtr dimPars = dim;

    if (dimPars)
      // Размещение размерной надписи - параллельно, в разрезе линии.
      dimPars->SetTextOnLine(ksDimTextParallelInCut);

    // Получить интерфейс текстов размера.
    ksapi::IDimensionTextPtr dimText = dim;

    if (dimText)
      // Изменить формат отображения текста на десятичный.
      dimText->SetTextFormat(ksDimTextFormatGDD);

    // Применить изменения.
    dim->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование углового размера 3D.
*/
//---
void AngleDimension3DWork(ksapi::IKompasDocument3DPtr & doc)
{
  // Получить контейнер обозначений 3D.
  ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc);

  if (symbCont)
  {
    // Получить коллекцию угловых размеров 3D.
    ksapi::IAngleDimensions3DPtr dimsCol = symbCont->GetAngleDimensions3D();

    if (dimsCol)
    {
      // Добавить новый угловой размер 3D.
      ksapi::IAngleDimension3DPtr newDim = dimsCol->Add();

      if (newDim)
      {
        // Создать угловой размер 3D.
        if (CreateAngleDimension3D(newDim, doc))
        {
          std::wstring name = L"";
          // Получить интерфейс объекта дерева построения.
          ksapi::IFeaturePtr feature = newDim;

          // Получить имя объекта.
          if (feature)
            name = feature->GetName();

          if (YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            // Получить объект из коллекции по имени.
            ksapi::IAngleDimension3DPtr angDim = dimsCol->GetAngleDimension3DByName(name);
            // Редактировать размер.
            EditAngleDimension3D(angDim);
          }
        }
        else
          // Выдать сообщение "Объект не создан".
          LibMessage(LoadStr(IDS_NOCREATE), ksMessageWarning, ksButtonSetOk, true);
      }
    }
  }
}
