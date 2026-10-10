# Редактор трасс Windows

Сборка Release / Win32 оригинального MFC-редактора проверена в Parallels,
Windows 11 ARM64. Рендеринг — Direct3D 9, физика — PhysX 2.8.4.

## Установка и запуск

Скачайте `MotorRock-Windows-MapEditor.zip` из
[релиза parity-2026-10-10](https://github.com/dmitrcher/rrr3d/releases/tag/parity-2026-10-10),
распакуйте весь каталог и откройте `Launch-Editor.cmd`.
Архив включает игровые ресурсы и обычные VC90 CRT/MFC runtime. Visual Studio
для запуска Release не требуется. Нужны legacy DirectX 9 / PhysX prerequisites
оригинальной игры; проверенная VM уже содержит их. На чистой Windows установка
этих prerequisites отдельно не проверялась.

На текущей VM редактор установлен в `C:\MotorRock\editor`, на рабочем столе
есть ярлык **MotorRock Track Editor**. Исходные ресурсы игры сохранены отдельно.

Через **File → Open** выберите `Data\Map\World1\map1.r3dMap` или другую карту.
Для экспериментов сначала сделайте копию файла или используйте **File → Save As**.
**Ctrl+S** сохраняет карту. Красные контуры объектов — включённый режим показа
bounding boxes, который переключает кнопка с кубом в панели инструментов.

Логи запуска через CMD сохраняются в `%LOCALAPPDATA%\MotorRock\editor\logs`.
Каждый запуск создаёт отдельный каталог с `launch.json` и `trace.jsonl`:
временные метки, имена загружаемых ресурсов и стадии загрузки карты.
Из PowerShell можно выбрать файл карты и каталог логов:

```powershell
.\Launch-Editor.ps1 -EditorRoot . -Map .\Data\Map\World1\map1.r3dMap -LogRoot .\logs
```

## Пересборка

Нужен оригинальный SDK `q4a/rrr3d` на коммите
`eff933868c1fbdfd266738a403fac80084f2b51e` со структурой `prog/`,
его DirectX / PhysX SDK, Boost 1.53 и Visual C++ 2008 SP1 с MFC Feature Pack.
VC++ 2008 Express не включает MFC. Современный Mac checkout с Metal/Jolt
не заменяет Windows SDK.

Сначала примените `tools/parity/windows-reference.patch` по инструкции
в [CROSS_PLATFORM_PARITY.md](CROSS_PLATFORM_PARITY.md), затем:

```bat
tools\editor\build-windows.cmd C:\MotorRock\source\prog Release
```

Скрипт собирает пять проектов: LexStd, Rock3dEngine, NetLib, Rock3dGame,
MapEditor. Он выполняет rebuild, чтобы не использовать старые precompiled headers.
Результат — `prog\Release\MapEditor.exe`, `Rock3dGame.dll`, `NetLib.dll`.
Скопируйте их в отдельный каталог ресурсов игры и добавьте обычные
`VC\redist\x86\Microsoft.VC90.CRT` и `Microsoft.VC90.MFC`.
Лаунчеры из `tools/editor` установите как `Launch-Editor.ps1` и `Launch-Editor.cmd`.

Для этой VM установлен официальный **Visual Studio 2008 Professional
90-day Trial + SP1**: [Microsoft: ссылка на trial](https://learn.microsoft.com/en-au/answers/questions/938619/purchase-visual-studio-2008),
[Microsoft: SP1](https://www.microsoft.com/en-us/download/details.aspx?id=13276).
Среда разработки имеет ограниченный срок; для дальнейшей пересборки после его
истечения потребуется лицензированная совместимая среда. Это ограничение среды
разработки, а не срока работы готового Release-редактора.

## Проверка

10 октября 2026: пять проектов успешно собраны, у MapEditor — 0 ошибок и
3 предупреждения. Проверены запуск, отображение карты World1/map1 в 3D,
список waypoint/path и сохранение её тестовой копии через команду Save.
Сохранённый XML повторно загружен редактором; исходная карта игры не изменялась.
Сохранение без правок сохранило размер и SHA-256 карты.

Материалы проверки: `editor-build-manifest.json`, `editor-verification.json`,
`windows-map-editor.png` в релизе. Полный сценарий создания новой трассы,
редактирования всех объектов и запуска изменённой трассы в игре не проверялся.
Нативный MFC-интерфейс под Mac ещё не портирован.
