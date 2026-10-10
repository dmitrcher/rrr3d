# Изолированные Windows SDK-пробы материалов

`anisotropic-pairs-windows.cpp` измеряет 26 пар материалов без изменения контактов.
Настройки: PhysX 2.8.4 SW, Z-up, variable dt=1/60, adaptive force=0, skin .025,
solver iterations=4, массы 1200, четыре исходных wheel shapes, фиксированное вращение.
Колёса подняты до локального Z=100, чтобы исключить лучевые контакты. Проверены также
общий handle материала и разные наклоны. Это изоляция коэффициентов, не полная гонка.

В командной строке с VC2008 SP1:

```
build-anisotropic-windows.cmd C:\MotorRock\source\prog C:\MotorRock\calibration-probes\materials
```

Для запуска нужны PhysX DLL и Microsoft.VC90.CRT из рабочего Windows-стенда рядом
с исполняемым файлом. В cmd.exe перенаправление сохраняет ASCII/UTF-8 JSONL:

```
anisotropic-pairs.exe > reference-material-pairs.jsonl
```

PowerShell 5 перенаправляет stdout в UTF-16; перед сравнением требуется декодирование.
Опубликованный reference-material-pairs-v10.jsonl уже преобразован в UTF-8.
Getter-события сохраняют значения материала, callback-события — фактические контактные
коэффициенты и оси. Они могут различаться. Эти данные подтверждают модель v10.

Static SDK coefficients are also present in the raw reference JSONL. The v10 coefficient cap and numeric material oracles cover dynamic friction; static limits and their solver transition remain under investigation in the v11 audit.
