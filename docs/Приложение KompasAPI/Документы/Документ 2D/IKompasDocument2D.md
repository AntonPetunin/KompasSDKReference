<!-- # **Документация интерфейса КОМПАС API** -->

# **Интерфейс IKompasDocument2D**

## Иерархия наследования

<style>
.top-parent{
    color: #ffe3d8;
    background: green;
    border: 3px solid #046709;
    border-radius: 4px;
    padding: 5px;
    text-align: center;
    margin-bottom: 5px;
    font-weight: bold;
    width: 40%;
    max-width: 220px;
}

.other-parents{
    color: #ffe3d8;
    background: #4a6fa5;
    border: 3px solid #3d5b87;
    border-radius: 4px;
    padding: 5px;
    text-align: center;
    margin: 5px 10%;
    font-weight: bold;
    width: 40%;
    max-width: 220px;
}

.interface{
    color: #ffe3d8;
    background: #103874;
    border: 3px solid #002152;
    border-radius: 4px;
    padding: 5px;
    text-align: center;
    margin: 5px 10%;
    font-weight: bold;
    width: 40%;
    max-width: 220px;
}
</style>

<div style="padding: 10px; background: #f5f5f5; border-radius: 5px; max-width: auto; margin-bottom: 20px">
<div class="top-parent">IAPIObject</div>

<div style="text-align: left; color:black; margin: 5px 15%;">▼</div>
<div class="other-parents">IKompasAPIObject</div>

<div style="text-align: left; color:black; margin: 5px 25%;">▼</div>
<div class="other-parents" style="margin: 5px 20%;">IKompasDocument</div>

<div style="text-align: left; color:black; margin: 5px 35%;">▼</div>
<div class="interface" style="margin: 5px 30%;">IKompasDocument2D</div>
</div>

## Общее описание

Интерфейс **`IKompasDocument2D`** представляет собой основной интерфейс для работы с 2D документами (чертежами и фрагментами) в системе КОМПАС. Он наследует функциональность [`IKompasDocument`](files/IKompasDocument.md) и расширяет её возможностями для работы с графическими объектами, видами, слоями, параметрическими переменными, группами объектов и различными операциями трансформации.

Этот интерфейс является основным инструментом для:

- Создания и редактирования графических объектов (отрезки, дуги, окружности, тексты и др.)
- Управления видами и слоями документа
- Работы с параметрическими переменными
- Выполнения операций копирования, перемещения, поворота и симметрии объектов
- Управления гиперссылками
- Работы с выделением объектов

## Важные примечания

1. **Проверка указателей** - Все методы возвращают указатели, которые могут быть nullptr. Всегда проверяйте полученные указатели перед использованием.
2. **Обновление документа** - После изменения параметрических переменных необходимо вызывать [`UpdateVariables()`](#updatevariables) для применения изменений.
3. **Undo/Redo** - Интерфейс поддерживает механизм отмены/повтора действий. Управление осуществляется через методы `SetEnableUndo()`, `IsEnableUndo()`, `SetUndoContainer()`.
4. **Получение видов и слоев** - Для доступа к графическим объектам сначала получите менеджер видов и слоев через [`GetViewsAndLayersManager()`](#getviewsandlayersmanager).

## Получение интерфейса

### Основные способы получения:

1. **Из активного документа приложения:**
   - [`IApplication::GetActiveDocument()`](files/IApplication.md) - получение активного документа

2. **Из коллекции документов:**
   - [`IDocuments::Add()`](files/IDocuments.md) - создание нового документа
   - [`IDocuments::GetObjects()`](files/IDocuments.md) - получение всех документов

3. **Приведение типов:**
   - От [`IKompasDocument`](files/IKompasDocument.md) к `IKompasDocument2D` через SmartPtr
   - Через `QueryInterface` с IID `ccec40f6-ef13-42a7-80c8-a91fd7b07228`

### Примеры получения:

```cpp
// Получение активного 2D документа
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
if (doc2D)
{
    // Работа с документом
}

// Создание нового документа
ksapi::IKompasDocument2DPtr newDoc = documents.Add(ksDocumentDrawing, true);
if (newDoc)
{
    // Работа с новым документом
}
```

### Обязательные функции библиотеки

Для любой библиотеки, работающей с KsAPI, необходимо объявить функции точки входа и выхода:

```cpp
// Точка входа - вызывается при загрузке библиотеки в КОМПАС
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libaryActions)
{
    // Здесь сохраняется указатель на приложение КОМПАС
    // Через app доступны все интерфейсы API
    return true;
}

// Точка выхода - вызывается при выгрузке библиотеки
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
    // Освобождение ресурсов
}
```

**Важно:** Функция `LoadKompasLibrary` обязательна и является основным способом получения интерфейса [`IApplication`](files/IApplication.md). Через параметр `app` передаётся экземпляр приложения КОМПАС, к которому подключена библиотека.

## Дополнительные интерфейсы

Интерфейс `IKompasDocument2D` имеет дополнительные связанные интерфейсы:

- **[IKompasDocument2DEvents](interface_page_files/IKompasDocument2DEvents.md)** - интерфейс событий 2D документа
- **[IDrawingDocument](files/IDrawingDocument.md)** - интерфейс чертежа (наследует IKompasDocument2D)
- **[IFragmentDocument](files/IFragmentDocument.md)** - интерфейс фрагмента (наследует IKompasDocument2D)
- **[IViewsAndLayersManager](files/IViewsAndLayersManager.md)** - менеджер видов и слоев
- **[ISelectionManager](files/ISelectionManager.md)** - менеджер выделения объектов
- **[IChooseManager](files/IChooseManager.md)** - менеджер выбора объектов

## Методы интерфейса

### Группа 1: Основные операции с видами и слоями

- [`GetViewsAndLayersManager()`](#getviewsandlayersmanager) - получить менеджер видов и слоев
- [`GetSelectionManager()`](#getselectionmanager) - получить менеджер селектированных объектов
- [`GetChooseManager()`](#getchoosemanager) - получить менеджер выбранных объектов

### Группа 2: Параметрические переменные

- [`GetVariables()`](#getvariables) - получить массив параметрических переменных
- [`GetVariable()`](#getvariable) - получить переменную по индексу
- [`GetVariableByName()`](#getvariablebyname) - получить переменную по имени
- [`AddVariable()`](#addvariable) - создать переменную
- [`GetVariablesCount()`](#getvariablescount) - получить количество переменных
- [`UpdateVariables()`](#updatevariables) - обновить значения внешних переменных

### Группа 3: Группы объектов

- [`GetDrawingGroups()`](#getdrawinggroups) - получить коллекцию групп
- [`GetNamedGroups()`](#getnamedgroups) - получить коллекцию именованных групп
- [`GetCurrentGroup()`](#getcurrentgroup) - получить текущую группу

### Группа 4: Операции с объектами

- [`FindObject()`](#findobject) - найти ближайший объект
- [`FindObjects()`](#findobjects) - найти ближайшие объекты
- [`SelectObjects()`](#selectobjects) - выбрать объекты прямоугольником
- [`GetObjectById()`](#getobjectbyid) - получить объект по идентификатору

### Группа 5: Трансформация объектов

- [`MoveObjects()`](#moveobjects) - переместить объекты
- [`RotateObjects()`](#rotateobjects) - повернуть объекты
- [`SymmetryObjects()`](#symmetryobjects) - отразить объекты
- [`TransformObjects()`](#transformobjects) - трансформировать объекты в локальную СК

### Группа 6: Управление документом

- [`RebuildDocument()`](#rebuilddocument) - перестроить документ
- [`CompleteRebuildDocument()`](#completerebuilddocument) - полностью перестроить документ
- [`ClearUndo()`](#clearundo) - очистить контейнер отмены

---

### GetViewsAndLayersManager

[Группа 1: Основные операции с видами и слоями](#группа-1-основные-операции-с-видами-и-слоями) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить менеджер видов и слоев документа.

**Полное описание:**
Возвращает интерфейс [`IViewsAndLayersManager`](files/IViewsAndLayersManager.md), который позволяет управлять видами и слоями 2D документа. Через этот интерфейс можно получить доступ к коллекциям видов, слоев и графических объектов.

**Синтаксис:**

```cpp
virtual IViewsAndLayersManagerPtr GetViewsAndLayersManager() = 0;
```

**Возвращаемое значение:** Указатель на интерфейс [`IViewsAndLayersManager`](files/IViewsAndLayersManager.md). Возвращает nullptr в случае ошибки.

#### **Пример использования**

**Минимальный пример:**

```cpp
// Получение менеджера видов и слоев
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
ksapi::IViewsAndLayersManagerPtr viewsLayersMgr = doc2D->GetViewsAndLayersManager();
```

**Расширенный пример:**

```cpp
// Получение активного вида и создание объекта
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
if (doc2D)
{
    ksapi::IViewsAndLayersManagerPtr viewsLayersMgr = doc2D->GetViewsAndLayersManager();
    if (viewsLayersMgr)
    {
        ksapi::IViewsPtr views = viewsLayersMgr->GetViews();
        ksapi::IViewPtr activeView = views->GetActiveView();
        if (activeView)
        {
            ksapi::IDrawingContainerPtr container = activeView;
            // Работа с контейнером объектов
        }
    }
}
```

**Примечания:**

- Это первый метод, который нужно вызвать для получения доступа к графическим объектам документа
- Всегда проверяйте возвращаемый указатель на nullptr

---

### GetSelectionManager

[Группа 1: Основные операции с видами и слоями](#группа-1-основные-операции-с-видами-и-слоями) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить менеджер селектированных объектов.

**Полное описание:**
Возвращает интерфейс [`ISelectionManager`](files/ISelectionManager.md), который позволяет управлять выделением объектов в документе. Через этот интерфейс можно добавлять, удалять и получать выделенные объекты.

**Синтаксис:**

```cpp
virtual ISelectionManagerPtr GetSelectionManager() = 0;
```

**Возвращаемое значение:** Указатель на интерфейс [`ISelectionManager`](files/ISelectionManager.md).

---

### GetChooseManager

[Группа 1: Основные операции с видами и слоями](#группа-1-основные-операции-с-видами-и-слоями) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить менеджер выбранных объектов.

**Полное описание:**
Возвращает интерфейс [`IChooseManager`](files/IChooseManager.md), который используется для интерактивного выбора объектов пользователем.

**Синтаксис:**

```cpp
virtual IChooseManagerPtr GetChooseManager() = 0;
```

**Возвращаемое значение:** Указатель на интерфейс [`IChooseManager`](files/IChooseManager.md).

---

### GetVariables

[Группа 2: Параметрические переменные](#группа-2-параметрические-переменные) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить массив параметрических переменных документа.

**Полное описание:**
Возвращает вектор указателей на интерфейсы [`IVariable`](files/IVariable.md), представляющие параметрические переменные документа. Параметр `externalOnly` определяет, возвращаются только внешние переменные или все переменные.

**Синтаксис:**

```cpp
virtual std::vector<IVariablePtr> GetVariables(bool externalOnly) = 0;
```

**Параметры:**

- `externalOnly` (in) - если true, возвращаются только внешние переменные; если false - все переменные

**Возвращаемое значение:** Вектор указателей на [`IVariable`](files/IVariable.md).

#### **Пример использования**

```cpp
// Получение всех параметрических переменных
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
if (doc2D)
{
    std::vector<ksapi::IVariablePtr> variables = doc2D->GetVariables(false);
    for (const auto& var : variables)
    {
        // Обработка переменной
    }
}
```

---

### GetVariable

[Группа 2: Параметрические переменные](#группа-2-параметрические-переменные) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить параметрическую переменную по индексу.

**Полное описание:**
Возвращает указатель на параметрическую переменную по её индексу в коллекции. Параметр `external` определяет, из какой коллекции получается переменная.

**Синтаксис:**

```cpp
virtual IVariablePtr GetVariable(bool external, int32_t index) = 0;
```

**Параметры:**

- `external` (in) - если true - из коллекции внешних переменных, если false - из всех
- `index` (in) - индекс переменной

**Возвращаемое значение:** Указатель на [`IVariable`](files/IVariable.md) или nullptr, если переменная не найдена.

---

### GetVariableByName

[Группа 2: Параметрические переменные](#группа-2-параметрические-переменные) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить параметрическую переменную по имени.

**Полное описание:**
Возвращает указатель на параметрическую переменную по её имени.

**Синтаксис:**

```cpp
virtual IVariablePtr GetVariableByName(bool external, const std::wstring & name) = 0;
```

**Параметры:**

- `external` (in) - если true - из коллекции внешних переменных, если false - из всех
- `name` (in) - имя переменной

**Возвращаемое значение:** Указатель на [`IVariable`](files/IVariable.md) или nullptr, если переменная не найдена.

---

### AddVariable

[Группа 2: Параметрические переменные](#группа-2-параметрические-переменные) | [К оглавлению](#методы-интерфейса)

**Кратко:** Создать переменную и добавить её в документ.

**Полное описание:**
Создает новую параметрическую переменную с заданным именем, значением и комментарием, и добавляет её в документ.

**Синтаксис:**

```cpp
virtual IVariablePtr AddVariable(const std::wstring & name, double value, const std::wstring & note) = 0;
```

**Параметры:**

- `name` (in) - имя переменной
- `value` (in) - значение переменной
- `note` (in) - комментарий к переменной

**Возвращаемое значение:** Указатель на созданную [`IVariable`](files/IVariable.md) или nullptr в случае ошибки.

#### **Пример использования**

```cpp
// Создание новой переменной
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
if (doc2D)
{
    ksapi::IVariablePtr newVar = doc2D->AddVariable(L"d1", 10.0, L"Диаметр");
    if (newVar)
    {
        // Переменная создана
    }
}
```

---

### GetVariablesCount

[Группа 2: Параметрические переменные](#группа-2-параметрические-переменные) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить количество параметрических переменных.

**Полное описание:**
Возвращает количество параметрических переменных в документе. Параметр определяет, считаются только внешние переменные или все.

**Синтаксис:**

```cpp
virtual int32_t GetVariablesCount(bool external) = 0;
```

**Параметры:**

- `external` (in) - если true - количество внешних переменных, если false - всех

**Возвращаемое значение:** Количество переменных.

---

### UpdateVariables

[Группа 2: Параметрические переменные](#группа-2-параметрические-переменные) | [К оглавлению](#методы-интерфейса)

**Кратко:** Обновить значения внешних переменных.

**Полное описание:**
Применяет изменения параметрических переменных к документу. Этот метод необходимо вызывать после изменения значений переменных для отображения результатов в документе.

**Синтаксис:**

```cpp
virtual bool UpdateVariables() = 0;
```

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

#### **Пример использования**

```cpp
// Изменение переменной и обновление документа
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
if (doc2D)
{
    ksapi::IVariablePtr var = doc2D->GetVariableByName(true, L"d1");
    if (var)
    {
        var->SetValue(25.0);
        doc2D->UpdateVariables();  // Применить изменения
    }
}
```

---

### GetDrawingGroups

[Группа 3: Группы объектов](#группа-3-группы-объектов) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить коллекцию групп документа.

**Полное описание:**
Возвращает коллекцию всех групп объектов в документе через интерфейс [`IDrawingGroups`](files/IDrawingGroups.md).

**Синтаксис:**

```cpp
virtual IDrawingGroupsPtr GetDrawingGroups() = 0;
```

**Возвращаемое значение:** Указатель на [`IDrawingGroups`](files/IDrawingGroups.md).

---

### GetNamedGroups

[Группа 3: Группы объектов](#группа-3-группы-объектов) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить коллекцию именованных групп.

**Полное описание:**
Возвращает коллекцию именованных групп объектов в документе.

**Синтаксис:**

```cpp
virtual IDrawingGroupsPtr GetNamedGroups() = 0;
```

**Возвращаемое значение:** Указатель на [`IDrawingGroups`](files/IDrawingGroups.md).

---

### GetCurrentGroup

[Группа 3: Группы объектов](#группа-3-группы-объектов) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить текущую группу.

**Полное описание:**
Возвращает указатель на текущую активную группу объектов документа.

**Синтаксис:**

```cpp
virtual IDrawingGroupPtr GetCurrentGroup() = 0;
```

**Возвращаемое значение:** Указатель на текущую [`IDrawingGroup`](files/IDrawingGroup.md) или nullptr.

---

### FindObject

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Найти ближайший объект к заданной точке.

**Полное описание:**
Выполняет поиск ближайшего графического объекта к указанной точке в пределах заданного расстояния. Параметры поиска позволяют фильтровать объекты по различным критериям.

**Синтаксис:**

```cpp
virtual IDrawingObjectPtr FindObject(double x, double y, double limit, const IFindObjectParametersPtr & param) = 0;
```

**Параметры:**

- `x`, `y` (in) - координаты точки для поиска объекта
- `limit` (in) - максимальное расстояние от указанной точки до объекта
- `param` (in) - параметры поиска объектов (можно получить через GetInterface с типом KompasAPIObjectTypeEnum::ksObjectFindObjectParameters)

**Возвращаемое значение:** Указатель на найденный [`IDrawingObject`](files/IDrawingObject.md) или nullptr, если объект не найден.

#### **Пример использования**

```cpp
// Найти объект в точке
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
if (doc2D)
{
    double x = 100.0, y = 50.0;
    double limit = doc2D->GetCursorLimit();
    ksapi::IDrawingObjectPtr obj = doc2D->FindObject(x, y, limit, nullptr);
    if (obj)
    {
        // Объект найден
    }
}
```

---

### FindObjects

[Группа 4: Операции с объектами](#группа-4-операция-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Найти ближайшие объекты к заданной точке.

**Полное описание:**
Выполняет поиск всех графических объектов в пределах заданного расстояния от указанной точки.

**Синтаксис:**

```cpp
virtual std::vector<IDrawingObjectPtr> FindObjects(double x, double y, double limit,
                                                   const IFindObjectParametersPtr & param) = 0;
```

**Параметры:**

- `x`, `y` (in) - координаты точки для поиска объектов
- `limit` (in) - максимальное расстояние от указанной точки до объектов
- `param` (in) - параметры поиска объектов

**Возвращаемое значение:** Вектор указателей на найденные [`IDrawingObject`](files/IDrawingObject.md).

---

### SelectObjects

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить объекты с помощью выделения прямоугольником.

**Полное описание:**
Возвращает массив объектов, попадающих в заданный прямоугольник. Способ выбора определяется параметром `regionType` (внутри, снаружи, пересекающие границу).

**Синтаксис:**

```cpp
virtual std::vector<IDrawingObjectPtr> SelectObjects(ksRegionTypeEnum regionType, double minX, double minY, double maxX,
                                                     double maxY) = 0;
```

**Параметры:**

- `regionType` (in) - способ выбора относительно прямоугольника (ksRegionTypeEnum)
- `minX`, `minY` (in) - координаты левой нижней вершины прямоугольника
- `maxX`, `maxY` (in) - координаты правой верхней вершины прямоугольника

**Возвращаемое значение:** Вектор указателей на найденные [`IDrawingObject`](files/IDrawingObject.md).

---

### GetObjectById

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить объект по уникальному идентификатору.

**Полное описание:**
Возвращает графический объект по его уникальному идентификатору в документе.

**Синтаксис:**

```cpp
virtual IDrawingObjectPtr GetObjectById(int64_t id) = 0;
```

**Параметры:**

- `id` (in) - уникальный идентификатор объекта

**Возвращаемое значение:** Указатель на [`IDrawingObject`](files/IDrawingObject.md) или nullptr.

---

### MoveObjects

[Группа 5: Трансформация объектов](#группа-5-трансформация-объектов) | [К оглавлению](#методы-интерфейса)

**Кратко:** Сместить заданные объекты на заданное расстояние.

**Полное описание:**
Перемещает указанные объекты на заданное расстояние по осям X и Y.

**Синтаксис:**

```cpp
virtual bool MoveObjects(const std::vector<IKompasAPIObjectPtr> & objects, double dx, double dy) = 0;
```

**Параметры:**

- `objects` (in) - массив перемещаемых объектов
- `dx` (in) - смещение по оси X
- `dy` (in) - смещение по оси Y

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

---

### RotateObjects

[Группа 5: Трансформация объектов](#группа-5-трансформация-объектов) | [К оглавлению](#методы-интерфейса)

**Кратко:** Повернуть заданные объекты.

**Полное описание:**
Поворачивает указанные объекты вокруг заданной точки на указанный угол.

**Синтаксис:**

```cpp
virtual bool RotateObjects(const std::vector<IKompasAPIObjectPtr> & objects, double x, double y, double angle) = 0;
```

**Параметры:**

- `objects` (in) - массив поворачиваемых объектов
- `x`, `y` (in) - координаты точки поворота
- `angle` (in) - угол поворота в градусах

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

---

### SymmetryObjects

[Группа 5: Трансформация объектов](#группа-5-трансформация-объектов) | [К оглавлению](#методы-интерфейса)

**Кратко:** Отразить объекты относительно заданной оси симметрии.

**Полное описание:**
Создает отраженную копию объектов или отражает существующие объекты относительно оси, заданной двумя точками.

**Синтаксис:**

```cpp
virtual IKompasAPIObjectPtr SymmetryObjects(const std::vector<IKompasAPIObjectPtr> & objects, double x1, double y1, double x2,
                                             double y2, bool copy, bool textSymmetry, bool copyAttributes) = 0;
```

**Параметры:**

- `objects` (in) - массив отражаемых объектов
- `x1`, `y1` (in) - координаты первой точки оси симметрии
- `x2`, `y2` (in) - координаты второй точки оси симметрии
- `copy` (in) - true - симметрия с копированием, false - симметрия без копирования
- `textSymmetry` (in) - true - отражать текст, false - не отражать
- `copyAttributes` (in) - true - копировать атрибуты при симметрии копированием

**Возвращаемое значение:** Группа отражённых объектов или nullptr в случае ошибки.

---

### TransformObjects

[Группа 5: Трансформация объектов](#группа-5-трансформация-объектов) | [К оглавлению](#методы-интерфейса)

**Кратко:** Трансформировать объекты в локальную систему координат документа.

**Полное описание:**
Применяет матрицу трансформации (локальную СК) документа к указанным объектам.

**Синтаксис:**

```cpp
virtual bool TransformObjects(const std::vector<IKompasAPIObjectPtr> & objects) = 0;
```

**Параметры:**

- `objects` (in/out) - трансформируемые объекты

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

---

### RebuildDocument

[Группа 6: Управление документом](#группа-6-управление-документом) | [К оглавлению](#методы-интерфейса)

**Кратко:** Перестроить документ.

**Полное описание:**
Выполняет перестроение документа - перерисовку всех графических объектов с учетом всех зависимостей.

**Синтаксис:**

```cpp
virtual bool RebuildDocument() = 0;
```

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

---

### CompleteRebuildDocument

[Группа 6: Управление документом](#группа-6-управление-документом) | [К оглавлению](#методы-интерфейса)

**Кратко:** Перестроить документ полностью (вместе со связями).

**Полное описание:**
Выполняет полное перестроение документа, включая все внешние связи и зависимости.

**Синтаксис:**

```cpp
virtual bool CompleteRebuildDocument() = 0;
```

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

---

### ClearUndo

[Группа 6: Управление документом](#группа-6-управление-документом) | [К оглавлению](#методы-интерфейса)

**Кратко:** Очистить контейнер отмены (Undo).

**Полное описание:**
Очищает историю отмены действий для текущего документа.

**Синтаксис:**

```cpp
virtual bool ClearUndo() = 0;
```

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

---

### GetEditMacroObject

[Группа 1: Основные операции с видами и слоями](#группа-1-основные-операции-с-видами-и-слоями) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить редактируемый макроэлемент.

**Полное описание:**
Возвращает указатель на интерфейс [`IMacroObject`](files/IMacroObject.md), если документ находится в режиме редактирования макроэлемента.

**Синтаксис:**

```cpp
virtual IMacroObjectPtr GetEditMacroObject() = 0;
```

**Возвращаемое значение:** Указатель на [`IMacroObject`](files/IMacroObject.md) или nullptr.

---

### Events2D

[Группа 1: Основные операции с видами и слоями](#группа-1-основные-операции-с-видами-и-слоями) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить обработчик событий документа.

**Полное описание:**
Возвращает интерфейс событий 2D документа для подписки на события.

**Синтаксис:**

```cpp
virtual IKompasDocument2DEventsPtr Events2D() = 0;
```

**Возвращаемое значение:** Указатель на [`IKompasDocument2DEvents`](files/IKompasDocument2DEvents.md).

---

### CopyObjects

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Скопировать объекты по заданным параметрам.

**Полное описание:**
Создает копии указанных объектов с использованием различных параметров копирования.

**Синтаксис:**

```cpp
virtual std::vector<IDrawingObjectPtr> CopyObjects(const std::vector<IKompasAPIObjectPtr> & objects,
                                                     const ICopyObjectParamPtr & params) = 0;
```

**Параметры:**

- `objects` (in) - массив объектов для копирования
- `params` (in) - параметры копирования (ICopyObjectParam, ICurveCopyObjectParam, ICircleCopyObjectParam, ICircularCopyObjectParam, IMeshCopyObjectParam)

**Возвращаемое значение:** Массив скопированных объектов.

---

### DestroyObjects

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Разрушить объекты.

**Полное описание:**
Удаляет указанные объекты из документа.

**Синтаксис:**

```cpp
virtual bool DestroyObjects(const std::vector<IKompasAPIObjectPtr> & objects) = 0;
```

**Параметры:**

- `objects` (in) - массив удаляемых объектов

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

---

### CreateHyperLink

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Создать гиперссылку.

**Полное описание:**
Добавляет атрибут гиперссылки к указанным объектам.

**Синтаксис:**

```cpp
virtual bool CreateHyperLink(const std::vector<IDrawingObjectPtr> & objects, ksHyperLinkTypeEnum linkType,
                             const std::wstring & text, const IDrawingObjectPtr & linkObject, int32_t level) = 0;
```

**Параметры:**

- `objects` (in) - объекты для добавления гиперссылки
- `linkType` (in) - тип ссылки (ksHyperLinkTypeEnum)
- `text` (in) - текст гиперссылки
- `linkObject` (in) - объект, на который указывает гиперссылка
- `level` (in) - уровень ссылки

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

---

### GetHyperLinkObjects

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить объекты с гиперссылками.

**Полное описание:**
Возвращает массив объектов, имеющих гиперссылки, с возможностью фильтрации по различным параметрам.

**Синтаксис:**

```cpp
virtual std::vector<IDrawingObjectPtr> GetHyperLinkObjects(ksHyperLinkTypeEnum linkType, const IDrawingObjectPtr & linkObject,
                                                            int32_t level, const std::wstring & text) = 0;
```

**Параметры:**

- `linkType` (in) - тип ссылки (ksHLUnknown - не учитывать)
- `linkObject` (in) - объект ссылки
- `level` (in) - уровень (-1 - не учитывать)
- `text` (in) - текст (пустая строка - не учитывать)

**Возвращаемое значение:** Массив объектов с гиперссылками.

---

### DeleteHyperLinks

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Удалить гиперссылки у объектов.

**Полное описание:**
Удаляет атрибуты гиперссылок у указанных объектов.

**Синтаксис:**

```cpp
virtual bool DeleteHyperLinks(const std::vector<IDrawingObjectPtr> & objects) = 0;
```

**Параметры:**

- `objects` (in) - объекты, у которых удаляются гиперссылки

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

---

### DecomposeObject

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Разбить объект на составляющие части.

**Полное описание:**
Разбивает сложный объект (размер, шероховатость и др.) на простые графические примитивы - отрезки, дуги, тексты.

**Синтаксис:**

```cpp
virtual IDrawingGroupPtr DecomposeObject(const IKompasAPIObjectPtr & object, ksDecomposeObjectLevelEnum level, double arrow,
                                         ksDrawingObjectParamTypeEnum decomposeType) = 0;
```

**Параметры:**

- `object` (in) - разбиваемый объект
- `level` (in) - уровень разбиения (ksDecomposeObjectLevelEnum)
- `arrow` (in) - стрелка прогиба
- `decomposeType` (in) - параметры СК (ksAllParam, ksViewAllParam или ksSheetAllParam)

**Возвращаемое значение:** Группа объектов после разбиения.

---

### VisualCreateObject

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Запустить визуальный процесс создания объекта.

**Полное описание:**
Запускает интерактивный процесс создания графического объекта указанного типа. Параметры объекта задаются пользователем в интерактивном режиме.

**Синтаксис:**

```cpp
virtual IDrawingObjectPtr VisualCreateObject(DrawingObjectTypeEnum objectType) = 0;
```

**Параметры:**

- `objectType` (in) - тип создаваемого объекта (DrawingObjectTypeEnum)

**Возвращаемое значение:** Созданный объект или nullptr.

---

### VisualEditObject

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Запустить визуальный процесс редактирования объекта.

**Полное описание:**
Запускает интерактивный процесс редактирования существующего графического объекта.

**Синтаксис:**

```cpp
virtual bool VisualEditObject(const IDrawingObjectPtr & object) = 0;
```

**Параметры:**

- `object` (in) - редактируемый объект

**Возвращаемое значение:** true в случае успеха, false в случае ошибки.

---

### RunCreateObjectProcess

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Запустить визуальный процесс создания объекта по типу процесса.

**Полное описание:**
Запускает интерактивный процесс создания объекта с использованием встроенного процесса КОМПАС.

**Синтаксис:**

```cpp
virtual IDrawingObjectPtr RunCreateObjectProcess(ProcessTypeEnum processType) = 0;
```

**Параметры:**

- `processType` (in) - тип процесса (ProcessTypeEnum)

**Возвращаемое значение:** Созданный объект или nullptr.

---

### GetLibProcess

[Группа 4: Операции с объектами](#группа-4-операции-с-объектами) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить объект процесса.

**Полное описание:**
Возвращает указатель на интерфейс процесса для работы с различными операциями (размещение, курсор и др.).

**Синтаксис:**

```cpp
virtual IProcess2DPtr GetLibProcess(ksProcess2DTypeEnum processType) = 0;
```

**Параметры:**

- `processType` (in) - тип процесса (ksProcess2DTypeEnum)

**Возвращаемое значение:** Указатель на [`IProcess2D`](files/IProcess2D.md).

#### **Пример использования**

```cpp
// Получение процесса размещения
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
if (doc2D)
{
    ksapi::IProcess2DPtr process = doc2D->GetLibProcess(ksProcess2DPlacement);
    if (process)
    {
        // Работа с процессом
    }
}
```

---

### IsEditMacroVisibleRegime

[Группа 1: Основные операции с видами и слоями](#группа-1-основные-операции-с-видами-и-слоями) | [К оглавлению](#методы-интерфейса)

**Кратко:** Проверить режим редактирования макроэлемента.

**Полное описание:**
Возвращает признак нахождения документа в режиме редактирования макроэлемента.

**Синтаксис:**

```cpp
virtual bool IsEditMacroVisibleRegime() = 0;
```

**Возвращаемое значение:** true - документ в режиме редактирования макроэлемента, false - нет.

---

### IsLibraryEditMode

[Группа 6: Управление документом](#группа-6-управление-документом) | [К оглавлению](#методы-интерфейса)

**Кратко:** Проверить режим редактирования библиотечного элемента.

**Полное описание:**
Возвращает признак нахождения в режиме редактирования библиотечного элемента.

**Синтаксис:**

```cpp
virtual bool IsLibraryEditMode() = 0;
```

**Возвращаемое значение:** true - в режиме редактирования библиотеки, false - нет.

---

### SetEnableUndo / IsEnableUndo

[Группа 6: Управление документом](#группа-6-управление-документом) | [К оглавлению](#методы-интерфейса)

**Кратко:** Управление механизмом отмены действий.

**Полное описание:**
Методы для включения/выключения механизма отмены действий (Undo) и проверки его состояния.

**Синтаксис:**

```cpp
virtual void SetEnableUndo(bool enable) = 0;
virtual bool IsEnableUndo() = 0;
```

**Параметры:**

- `enable` (in) - true включить, false выключить

**Возвращаемое значение:** `SetEnableUndo` - void; `IsEnableUndo` - true если включен, false если выключен.

---

### SetUndoContainer / IsUndoContainer

[Группа 6: Управление документом](#группа-6-управление-документом) | [К оглавлению](#методы-интерфейса)

**Кратко:** Управление объединением команд в контейнере отмены.

**Полное описание:**
Позволяет объединять несколько команд в один блок отмены. Используется счетчик вызовов.

**Синтаксис:**

```cpp
virtual void SetUndoContainer(bool beginContainer) = 0;
virtual bool IsUndoContainer() = 0;
```

**Параметры:**

- `beginContainer` (in) - признак начала/конца контейнера

**Возвращаемое значение:** `SetUndoContainer` - void; `IsUndoContainer` - true если команды объединяются.

---

### GetCursorLimit

[Группа 1: Основные операции с видами и слоями](#группа-1-основные-операции-с-видами-и-слоями) | [К оглавлению](#методы-интерфейса)

**Кратко:** Получить радиус окружности курсора.

**Полное описание:**
Возвращает радиус окружности, используемой для захвата объектов при поиске.

**Синтаксис:**

```cpp
virtual double GetCursorLimit() = 0;
```

**Возвращаемое значение:** Радиус в единицах документа.

---

### SetParametricMode / IsParametricMode

[Группа 2: Параметрические переменные](#группа-2-параметрические-переменные) | [К оглавлению](#методы-интерфейса)

**Кратко:** Управление параметрическим режимом.

**Полное описание:**
Включение/выключение параметрического режима черчения и проверка его состояния.

**Синтаксис:**

```cpp
virtual void SetParametricMode(bool enable) = 0;
virtual bool IsParametricMode() = 0;
```

**Параметры:**

- `enable` (in) - true включить, false выключить

**Возвращаемое значение:** `SetParametricMode` - void; `IsParametricMode` - true если включен.

---

### SetOrthogonalMode / IsOrthogonalMode

[Группа 6: Управление документом](#группа-6-управление-документом) | [К оглавлению](#методы-интерфейса)

**Кратко:** Управление ортогональным черчением.

**Полное описание:**
Включение/выключение режима ортогонального черчения (только горизонтальные и вертикальные линии).

**Синтаксис:**

```cpp
virtual void SetOrthogonalMode(bool enable) = 0;
virtual bool IsOrthogonalMode() = 0;
```

**Параметры:**

- `enable` (in) - true включить, false выключить

**Возвращаемое значение:** `SetOrthogonalMode` - void; `IsOrthogonalMode` - true если включен.

---

## Частые ошибки

### 1. Не проверять возвращаемый указатель на nullptr

```cpp
// НЕПРАВИЛЬНО
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
ksapi::IViewsAndLayersManagerPtr mgr = doc2D->GetViewsAndLayersManager();  // doc2D может быть nullptr!

// ПРАВИЛЬНО
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
if (doc2D)
{
    ksapi::IViewsAndLayersManagerPtr mgr = doc2D->GetViewsAndLayersManager();
    if (mgr)
    {
        // Работа с менеджером
    }
}
```

### 2. Забывать вызывать UpdateVariables после изменения переменных

```cpp
// НЕПРАВИЛЬНО
var->SetValue(25.0);  // Изменение без обновления
// Переменная не применяется к документу

// ПРАВИЛЬНО
var->SetValue(25.0);
doc2D->UpdateVariables();  // Применение изменений
```

### 3. Использовать неправильные координаты видов

```cpp
// НЕПРАВИЛЬНО
// Работа с объектами без получения правильного вида
ksapi::IDrawingContainerPtr container = doc2D;  // Неправильно!

// ПРАВИЛЬНО
// Получение активного вида документа
ksapi::IViewsAndLayersManagerPtr viewsLayersMgr = doc2D->GetViewsAndLayersManager();
if (viewsLayersMgr)
{
    ksapi::IViewsPtr views = viewsLayersMgr->GetViews();
    ksapi::IViewPtr activeView = views->GetActiveView();
    ksapi::IDrawingContainerPtr container = activeView;
}
```

---

## Практические примеры из исходников

### Пример 1: Получение активного документа и вида

```cpp
// Из Source/Steps/Step1_KsAPI_2D/Step1_KsAPI_2D.cpp
// Получение 2D документа
if (ksapi::IKompasDocument2DPtr pKompasDocument2D = kompasApp->GetActiveDocument())
{
    // Получение менеджера видов и слоёв
    if (ksapi::IViewsAndLayersManagerPtr pViewsAndLayersManager = pKompasDocument2D->GetViewsAndLayersManager())
    {
        // Работа с видами
    }
}
```

### Пример 2: Создание нового документа

```cpp
// Из Source/Steps/Step7/Step7.cpp
ksapi::IKompasDocument2DPtr CreateDocument(ksapi::IDocuments & documents, const std::wstring & name)
{
    ksapi::IKompasDocument2DPtr document = documents.Add(ksDocumentDrawing, /*visible*/ true);
    if (!document)
    {
        // Обработка ошибки
    }
    return document;
}
```

### Пример 3: Поиск объекта по координатам

```cpp
// Из Source/Steps/Step2_KsAPI_2D/Step2_KsAPI_2D.cpp
// Найти объект по указанным координатам
if (ksapi::IKompasDocument2DPtr doc2D = GetCurrentDocument())
{
    baseObj = doc2D->FindObject(x, y, doc2D->GetCursorLimit(), nullptr);
}
```

### Пример 4: Работа с процессом

```cpp
// Из Source/Steps/Step2_KsAPI_2D/Step2_KsAPI_2D.cpp
{
    ksapi::IKompasDocument2DPtr doc2D = ksAppl->GetActiveDocument();
    process = doc2D->GetLibProcess(procType);
}
```

---

## Шаблоны использования

### Шаблон 1: Основной паттерн работы с документом

```cpp
// Получение документа и менеджера видов/слоев
ksapi::IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
if (doc2D)
{
    ksapi::IViewsAndLayersManagerPtr viewsLayersMgr = doc2D->GetViewsAndLayersManager();
    if (viewsLayersMgr)
    {
        ksapi::IViewsPtr views = viewsLayersMgr->GetViews();
        ksapi::IViewPtr activeView = views->GetActiveView();
        if (activeView)
        {
            ksapi::IDrawingContainerPtr container = activeView;
            // Работа с объектами
        }
    }
}
```

### Шаблон 2: Работа с параметрическими переменными

```cpp
// Создание и использование переменных
ksapi::IVariablePtr newVar = doc2D->AddVariable(L"d1", 10.0, L"Диаметр");
if (newVar)
{
    newVar->SetValue(25.0);
    doc2D->UpdateVariables();
}
```

### Шаблон 3: Трансформация объектов

```cpp
// Перемещение объектов
std::vector<ksapi::IKompasAPIObjectPtr> objects = /* получение объектов */;
if (doc2D->MoveObjects(objects, 50.0, 0.0))
{
    // Успешное перемещение
}
```

---

## Связанные интерфейсы

### Работа в паре с:

- **[IApplication](files/IApplication.md)** - основной интерфейс приложения КОМПАС, через который получается документ
- **[IDocuments](files/IDocuments.md)** - коллекция документов
- **[IKompasDocument](files/IKompasDocument.md)** - базовый интерфейс документа

### Часто используется вместе с:

- **[IViewsAndLayersManager](files/IViewsAndLayersManager.md)** - менеджер видов и слоев
- **[IView](files/IView.md)** - интерфейс вида
- **[IDrawingContainer](files/IDrawingContainer.md)** - контейнер графических объектов
- **[IDrawingObject](files/IDrawingObject.md)** - базовый интерфейс графического объекта
- **[ISelectionManager](files/ISelectionManager.md)** - менеджер выделения
- **[IVariable](files/IVariable.md)** - параметрическая переменная

---

## Инструкция по заполнению шаблона:

### 1. **Заголовок и иерархия**

- Замени `[ИмяИнтерфейса]` на реальное имя интерфейса - **IKompasDocument2D**
- Настрой иерархию наследования в CSS-блоках
- Укажи правильные родительские интерфейсы - **IKompasDocument → IKompasAPIObject → IAPIObject**

### 2. **Общее описание**

- Кратко: что делает интерфейс? - Работа с 2D документами
- Контекст: где используется? - Приложения КОМПАС
- Назначение: какие задачи решает? - Создание и редактирование чертежей

### 3. **Важные примечания**

- Критические моменты (Update(), проверка указателей и т.д.)
- Ограничения и особенности
- Требования к использованию

### 4. **Методы интерфейса**

#### Для каждого метода:

- **Кратко** - 1-2 предложения
- **Полное описание** - подробно, с контекстом
- **Синтаксис** - точная сигнатура из .h файла
- **Параметры** - с указанием направления (in/out). Не пиши "нет параметров"
- **Возвращаемое значение** - описание. Не пиши "нет", если возвращать нечего
- **Примеры** - минимальный и расширенный с комментариями
- **Примечания** - важные детали

### 5. **Примеры кода**

- Используй реальные примеры из проектов
- Добавляй комментарии к сложным моментам
- Показывай типичные сценарии использования
- Для указателей интерфейсов используй формат:
  ```cpp
  ksapi::I[Интерфейс]Ptr obj = // получение интерфейса I[Интерфейс]
  ```
  Вместо:
  ```cpp
  ksapi::I[Интерфейс]Ptr obj = nullptr;
  ```

### 6. **Частые ошибки**

- Конкретные ошибки с исправлениями
- Антипаттерны и как их избежать
- Распространенные заблуждения

### 7. **Важные правила**

- **Не используй номера строк** при ссылках на исходные файлы (они могут измениться)
- **Только русский и английский языки** - не используй китайский, польский или любые другие символы/слова
- **Добавляй ссылки на интерфейсы** - даже если страница не существует, ссылка должна быть (файл появится позже)
- **Enum до 7 констант** - включай прямо в описание метода
- **Большие enum** - ссылка на файл в папке `files/`
