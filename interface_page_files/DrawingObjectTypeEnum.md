<!-- # **Документация перечисления КОМПАС API** -->

# **Перечисление DrawingObjectTypeEnum**

## Общее описание

Перечисление `DrawingObjectTypeEnum` определяет типы графических объектов, которые могут быть размещены на чертеже КОМПАС. Используется для идентификации типа графического объекта при работе с коллекциями, для фильтрации объектов по типу, при создании новых объектов и для определения типа существующего объекта.

## Таблица значений

| Идентификатор | Значение | Название объекта | Интерфейс |
|---------------|----------|-----------------|------------|
| ksUnknown | -1 | Неизвестный объект | - |
| ksAllObj | 0 | Все объекты | - |
| ksDrLineSeg | 1 | Отрезок | [`ILineSegment`](interface_page_files/ILineSegment.md) |
| ksDrCircle | 2 | Окружность | [`ICircle`](interface_page_files/ICircle.md) |
| ksDrArc | 3 | Дуга | [`IArc`](interface_page_files/IArc.md) |
| ksDrDrawText | 4 | Текст на чертеже | [`IDrawingText`](interface_page_files/IDrawingText.md) |
| ksDrPoint | 5 | Точка | [`IPoint`](interface_page_files/IPoint.md) |
| ksDrHatch | 7 | Штриховка | [`IHatch`](interface_page_files/IHatch.md) |
| ksDrBezier | 8 | Bezier сплайн | [`IBezier`](interface_page_files/IBezier.md) |
| ksDrLDimension | 9 | Линейный размер | [`ILineDimension`](interface_page_files/ILineDimension.md) |
| ksDrADimension | 10 | Угловой размер | [`IAngleDimension`](interface_page_files/IAngleDimension.md) |
| ksDrDDimension | 13 | Диаметральный размер | [`IDiametralDimension`](interface_page_files/IDiametralDimension.md) |
| ksDrRDimension | 14 | Радиальный размер | [`IRadialDimension`](interface_page_files/IRadialDimension.md) |
| ksDrRBreakDimension | 15 | Радиальный размер с изломом | [`IBreakRadialDimension`](interface_page_files/IBreakRadialDimension.md) |
| ksDrRough | 16 | Шероховатость | [`IRough`](interface_page_files/IRough.md) |
| ksDrBase | 17 | База | [`IBase`](interface_page_files/IBase.md) |
| ksDrWPointer | 18 | Стрелка вида | [`IViewPointer`](interface_page_files/IViewPointer.md) |
| ksDrCut | 19 | Линия разреза | [`ICutLine`](interface_page_files/ICutLine.md) |
| ksDrLeader | 20 | Простая линия выноски | [`ILeader`](interface_page_files/ILeader.md) |
| ksDrPosLeader | 21 | Линия выноски для обозначения позиции | [`IPositionLeader`](interface_page_files/IPositionLeader.md) |
| ksDrBrandLeader | 22 | Линия выноски для обозначения клеймения | [`IBrandLeader`](interface_page_files/IBrandLeader.md) |
| ksDrMarkerLeader | 23 | Линия выноски для обозначения маркирования | [`IMarkLeader`](interface_page_files/IMarkLeader.md) |
| ksDrTolerance | 24 | Допуск формы | [`ITolerance`](interface_page_files/ITolerance.md) |
| ksDrTable | 25 | Таблица | [`IDrawingTable`](interface_page_files/IDrawingTable.md) |
| ksDrContour | 26 | Контур | [`IDrawingContour`](interface_page_files/IDrawingContour.md) |
| ksDrMacro | 27 | Нетипизированный макроэлемент | [`IMacroObject`](interface_page_files/IMacroObject.md) |
| ksDrLine | 28 | Линия | [`ILine`](interface_page_files/ILine.md) |
| ksLayer | 29 | Слой | [`ILayer`](interface_page_files/ILayer.md) |
| ksDrFragment | 30 | Вставной фрагмент | [`IFragment`](interface_page_files/IFragment.md) |
| ksDrPolyline | 31 | Полилиния | [`IPolyLine2D`](interface_page_files/IPolyLine2D.md) |
| ksDrEllipse | 32 | Эллипс | [`IEllipse`](interface_page_files/IEllipse.md) |
| ksDrNurbs | 33 | NURBS-кривая по полюсам | [`INurbs`](interface_page_files/INurbs.md) |
| ksDrEllipseArc | 34 | Дуга эллипса | [`IEllipseArc`](interface_page_files/IEllipseArc.md) |
| ksDrRectangle | 35 | Прямоугольник | [`IRectangle`](interface_page_files/IRectangle.md) |
| ksDrRegularPolygon | 36 | Многоугольник | [`IRegularPolygon`](interface_page_files/IRegularPolygon.md) |
| ksDrEquid | 37 | Эквидистанта | [`IEquidistant`](interface_page_files/IEquidistant.md) |
| ksDrLBreakDimension | 38 | Линейный размер с обрывом | [`IBreakLineDimension`](interface_page_files/IBreakLineDimension.md) |
| ksDrABreakDimension | 39 | Угловой размер с обрывом | [`IBreakLineDimension`](interface_page_files/IBreakLineDimension.md) |
| ksDrOrdinateDimension | 40 | Размер высоты | [`IHeightDimension`](interface_page_files/IHeightDimension.md) |
| ksDrColorFill | 41 | Фоновая заливка цветом | [`IColouring`](interface_page_files/IColouring.md) |
| ksDrCentreMarker | 42 | Обозначение центра | [`ICentreMarker`](interface_page_files/ICentreMarker.md) |
| ksDrArcDimension | 43 | Размер длины дуги | [`IArcDimension`](interface_page_files/IArcDimension.md) |
| ksDrRaster | 45 | Растровый объект | [`IRaster`](interface_page_files/IRaster.md) |
| ksDrChangeLeader | 46 | Обозначение изменения | [`IChangeLeader`](interface_page_files/IChangeLeader.md) |
| ksDrRemoteElement | 47 | Выносной элемент | [`IRemoteElement`](interface_page_files/IRemoteElement.md) |
| ksDrAxisLine | 48 | Осевая линия | [`IAxisLine`](interface_page_files/IAxisLine.md) |
| ksDrOLEObject | 49 | Вставка OLE объекта | [`IOleDrawingObject`](interface_page_files/IOleDrawingObject.md) |
| ksDrUnitNumber | 50 | Номер узла | [`IUnitNumber`](interface_page_files/IUnitNumber.md) |
| ksDrBrace | 51 | Фигурная скобка | [`IBrace`](interface_page_files/IBrace.md) |
| ksDrMarkOnLeader | 52 | Марка/позиционное обозначение с линией-выноской | [`IMark`](interface_page_files/IMark.md) |
| ksDrMarkOnLine | 53 | Марка/позиционное обозначение на линии | [`IMark`](interface_page_files/IMark.md) |
| ksDrMarkInsideForm | 54 | Марка/позиционное обозначение без линии-выноски | [`IMark`](interface_page_files/IMark.md) |
| ksDrWaveLine | 55 | Волнистая линия | [`IWaveLine`](interface_page_files/IWaveLine.md) |
| ksDrStraightAxis | 56 | Прямая ось | [`IAxisLine`](interface_page_files/IAxisLine.md) |
| ksDrBrokenLine | 57 | Линия обрыва с изломами | [`IBrokenLine`](interface_page_files/IBrokenLine.md) |
| ksDrCircleAxis | 58 | Круговая ось | [`ICircularsCentres`](interface_page_files/ICircularsCentres.md) |
| ksDrArcAxis | 59 | Дуговая ось | [`ILinearsCentres`](interface_page_files/ILinearsCentres.md) |
| ksDrCutUnitMarking | 60 | Обозначение узла в сечении | [`ICutUnitMarkings`](interface_page_files/ICutUnitMarkings.md) |
| ksDrUnitMarking | 61 | Обозначение узла | [`IUnitMarkings`](interface_page_files/IUnitMarkings.md) |
| ksDrMultiTextLeader | 62 | Выносная надпись к многослойным конструкциям | [`IMultiTextLeaders`](interface_page_files/IMultiTextLeaders.md) |
| ksDrExternalView | 63 | Вставка внешнего вида | [`IInsertionView`](interface_page_files/IInsertionView.md) |
| ksDrAnnLineSeg | 64 | Аннотационный отрезок | [`IAnnotativeObject`](interface_page_files/IAnnotativeObject.md) |
| ksDrAnnCircle | 65 | Аннотационная окружность | [`IAnnotativeObject`](interface_page_files/IAnnotativeObject.md) |
| ksDrAnnEllipse | 66 | Аннотационный эллипс | [`IAnnotativeObject`](interface_page_files/IAnnotativeObject.md) |
| ksDrAnnArc | 67 | Аннотационная дуга | [`IAnnotativeObject`](interface_page_files/IAnnotativeObject.md) |
| ksDrAnnEllipseArc | 68 | Аннотационная дуга эллипса | [`IAnnotativeObject`](interface_page_files/IAnnotativeObject.md) |
| ksDrAnnPolyline | 69 | Аннотационная полилиния | [`IAnnotativeObject`](interface_page_files/IAnnotativeObject.md) |
| ksDrAnnPoint | 70 | Аннотационная точка | [`IAnnotativeObject`](interface_page_files/IAnnotativeObject.md) |
| ksDrAnnText | 71 | Текст с аннотационной точкой привязки | [`IAnnotativeObject`](interface_page_files/IAnnotativeObject.md) |
| ksDrMultiLine | 72 | Мультилиния | [`IMultiline`](interface_page_files/IMultiline.md) |
| ksDrBuildingCutLine | 73 | Линия разреза/сечения для СПДС | [`ICutLine`](interface_page_files/ICutLine.md) |
| ksDrAttachedLeader | 74 | Присоединённая линия выноски | [`ILeader`](interface_page_files/ILeader.md) |
| ksDrConditionCrossing | 75 | Условное пересечение | - |
| ksReportTable | 76 | Ассоциативная таблица отчёта | [`IAssociationTable`](interface_page_files/IAssociationTable.md) |
| ksEmbodimentsTable | 77 | Таблица исполнений | [`IAssociationTable`](interface_page_files/IAssociationTable.md) |
| ksDrSpecialCurve | 78 | Кривая общего вида | - |
| ksArrayParamTable | 79 | Таблица параметров массива | [`IAssociationTable`](interface_page_files/IAssociationTable.md) |
| ksDrNurbsByPoints | 80 | NURBS-кривая по точкам | - |
| ksDrConicCurve | 81 | Коническая кривая | - |
| ksDrCircularCentres | 84 | Круговая сетка центров | [`ICircularsCentres`](interface_page_files/ICircularsCentres.md) |
| ksDrLinearCentres | 85 | Линейная сетка центров | [`ILinearsCentres`](interface_page_files/ILinearsCentres.md) |
| ksDrEllipseArcAxis | 86 | Дуговая осевая линия | - |
| ksDrStraightSlot | 87 | Паз | [`IStraightSlots`](interface_page_files/IStraightSlots.md) |
| ksDrArcSlot | 88 | Паз дуговой | [`IArcSlots`](interface_page_files/IArcSlots.md) |
| ksDrCsPoint | 89 | Точка начала СК | - |
| ksView | 123 | Вид | [`IView`](interface_page_files/IView.md) |

---

## Примеры использования

### Пример 1: Определение типа объекта

```cpp
// Определение типа графического объекта
ksapi::IDrawingObjectPtr obj = /* получение объекта */;
if (!obj)
    return;

switch (obj->GetDrawingObjectType())
{
    case DrawingObjectTypeEnum::ksDrCircle:
        // Это окружность
        break;
    case DrawingObjectTypeEnum::ksDrLineSeg:
        // Это отрезок
        break;
    case DrawingObjectTypeEnum::ksDrArc:
        // Это дуга
        break;
    case DrawingObjectTypeEnum::ksDrText:
        // Это текст
        break;
    default:
        // Другой тип объекта
        break;
}
```

### Пример 2: Создание объекта с указанием типа

```cpp
// Создание линии выноски
ksapi::ILeaderPtr leader = leaders->Add(DrawingObjectTypeEnum::ksDrLeader);
if (!leader)
    return;

// Создание позиционной линии выноски
ksapi::IPositionLeaderPtr posLeader = leaders->Add(DrawingObjectTypeEnum::ksDrPosLeader);

// Создание углового размера
ksapi::IAngleDimensionPtr angleDimension = angleDimensions->Add(DrawingObjectTypeEnum::ksDrADimension);
```

### Пример 3: Фильтрация объектов по типу

```cpp
// Получение только макрообъектов вида
std::vector<ksapi::IDrawingObjectPtr> objects = drawingContainer->GetObjects({DrawingObjectTypeEnum::ksDrMacro});

// Получение всех графических объектов
std::vector<ksapi::IDrawingObjectPtr> allObjects = drawingContainer->GetObjects({DrawingObjectTypeEnum::ksAllObj});

// Получение отрезков и полилиний
std::vector<ksapi::IDrawingObjectPtr> lineObjects = drawingContainer->GetObjects({
    DrawingObjectTypeEnum::ksDrLineSeg, 
    DrawingObjectTypeEnum::ksDrPolyline,
    DrawingObjectTypeEnum::ksDrRectangle
});
```

### Пример 4: Проверка типа объекта

```cpp
// Проверка, является ли объект окружностью
bool IsCircle(ksapi::IDrawingObjectPtr obj)
{
    if (!obj)
        return false;
    return obj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle;
}

// Проверка, является ли объект размером
bool IsDimension(ksapi::IDrawingObjectPtr obj)
{
    if (!obj)
        return false;
    auto type = obj->GetDrawingObjectType();
    return type == DrawingObjectTypeEnum::ksDrLDimension ||
           type == DrawingObjectTypeEnum::ksDrADimension ||
           type == DrawingObjectTypeEnum::ksDrDDimension ||
           type == DrawingObjectTypeEnum::ksDrRDimension;
}
```

---

## Частые ошибки

### 1. Неправильное сравнение типов

```cpp
// НЕПРАВИЛЬНО
if (obj->GetDrawingObjectType() == "ksDrCircle")  // Сравнение со строкой!

// ПРАВИЛЬНО
if (obj->GetDrawingObjectType() == DrawingObjectTypeEnum::ksDrCircle)
```

### 2. Использование неполного списка типов

```cpp
// НЕПРАВИЛЬНО - забыли про прямоугольник
if (type == DrawingObjectTypeEnum::ksDrLineSeg || 
    type == DrawingObjectTypeEnum::ksDrPolyline)

// ПРАВИЛЬНО
if (type == DrawingObjectTypeEnum::ksDrLineSeg || 
    type == DrawingObjectTypeEnum::ksDrPolyline ||
    type == DrawingObjectTypeEnum::ksDrRectangle)
```