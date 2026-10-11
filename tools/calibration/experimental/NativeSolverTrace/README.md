# Native solver trace v31

Экспериментальный отладчик для **одной проверенной x86 версии** PhysX 2.8.4.
Не часть игры и не исправленный Mac solver. [Результаты](../../../../docs/PHYSICS_CONTACT_V31_2026-10-11.md).

`trace-contact.cpp` ставит временные INT3 и hardware watchpoint в дочернем
процессе Windows SpringProbe. Физические входы и численные результаты не
подменяются. Software breakpoint снимается на одну инструкцию, затем
восстанавливается до лимита. Watchpoint наблюдает окончательный solver buffer.
Trace содержит raw DWORD, включая адреса и неопределённые поля: интерпретировать
все слова как float нельзя.

## Зафиксированные версии

```text
PhysXCore.dll      c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb
SpringProbe.exe    8c331b7ea35ed6181c0f8dd52cddca451f60c817d381d360cb53852e8c49ed9d
TraceContact.exe   e17d7e186c5c6e41d67ce6f48a25f3dace7a9cb84411100f151c83ce68b4d15f
```

Сам C++ helper определяет DLL по имени, **не проверяет SHA**. Поэтому запуски
производятся только через `capture.py`, который до инструментирования проверяет
все три файла через guest `certutil`. Запуск helper с другой DLL недопустим:
private RVA/layout не универсальны. CLI предназначен для путей без пробелов
и cmd metacharacters; зафиксированные пути ниже этому соответствуют.

## Сборка и запуск

В Windows необходим VC2008 x86 и `Microsoft.VC90.CRT` из рабочего стенда.
Скопировать `trace-contact.cpp` и `build.cmd` в
`C:\MotorRock\calibration-probes\contact-parent-v30\solver-kernel-v31`, выполнить
`build.cmd`. Probe, PhysX DLL и CRT находятся в родительском каталоге;
копия CRT также нужна рядом с debugger exe. Опубликованный binary выгружен
из VM; его SHA и идентичность скомпилированного исходника проверены.

На Mac с Parallels VM `Windows 11`:

```sh
python3 tools/calibration/experimental/NativeSolverTrace/capture.py \
  --measurements /absolute/path/to/new-measurements \
  --tracer-sha256 e17d7e186c5c6e41d67ce6f48a25f3dace7a9cb84411100f151c83ce68b4d15f
```

Capture снимает 12 сценариев дважды, по два JSONL на запуск. Существующие
файлы не перезаписываются. `--vm` и `--guest-base` допускают альтернативные
имена при тех же зафиксированных binaries. Используется `prlctl exec ... cmd.exe`.
Не требуется окно VM; данные получаются от реального SDK в Windows.

## Проверка сохранённых данных

Архив v31 содержит 48 capture files, два v30 baseline JSONL, отчёты, binaries,
исходники и manifest. Сохранить расположение `tools/calibration/compare.py`
относительно этой папки: predictor/verifier импортируют его.

```sh
python3 tools/calibration/experimental/NativeSolverTrace/model.py \
  --measurements measurements --out model.json
python3 tools/calibration/experimental/NativeSolverTrace/verify.py \
  --measurements measurements --out audit.json
```

Verifier требует `reference-row-iterations4-run0-v30-windows.jsonl` и
`calibration-row-arithmetic-both-mask0-run0-v30-mac.jsonl` в measurements.
Он проверяет сохранение всех 120 Native физических состояний, повторяемость
событий и определённых trace полей, аппаратную конфигурацию, последовательную
float32 реконструкцию и сравнение K с диагностическими Mac строками.

Model использует первые 8 записанных contact rows, первые 8 constraint errors,
начальное состояние тела и фактический quaternion. K и scaled bias вычисляются
самостоятельно. Затем predictor проходит 4×8 biased и 1×8 unbiased строк
без получения промежуточных ожидаемых состояний. Ожидаемые поля используются
только для проверки после расчёта. Геометрия и constraint error остаются
**записанными Native входами**, поэтому `goal_complete` всегда false.

## Поля фактического solver

Final row stride 128 bytes: body pointers +4/+8, R +10 hex, quaternion +40 hex,
effective mass +50 hex, scaled bias +60 hex, accumulated normal lambda +6c hex,
max lambda +74 hex, min lambda +78 hex. +70 hex неопределён в этих captures.
Body fields float words 0..2 — linear v, 4..6 — angular v, 11 — invMass,
15..23 — inverse inertia matrix. Неопределённый padding исключён из verifier.
Initial/prepared records и final rows имеют разную семантику одинаковых offsets.

Все формулы и layout проверены только на первых плоских контактах identity
orientation и static ground. Это не доказательство общего решения контактов,
точной генерации геометрии, наклонной инерции или тангенциального трения.
