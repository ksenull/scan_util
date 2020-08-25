# Утилита для сканирования файлов

Выполняет сканирование файлов в директории, с целью нахождение в ней “подозрительных” (suspicious) файлов.

Реализовано 3 типа “подозрительного” содержимого в файле.

- **JS suspicious**: файл с расширением .js, содержащий строку: `<script>evil_script()</script>`

- **Unix suspicious**: любой файл, содержащий строку: `rm -rf ~/Documents`

- **macOS suspicious**: любой файл, содержащий строку: `system("launchctl load /Library/LaunchAgents/com.malware.agent")`

 

После завершения выполнения утилиты пользователю выводится отчет о сканировании, в котором присутствует следующая информация:
- общее количество обработанных файлов;
- количество детектов на каждый тип “подозрительного” содержимого;
- количество ошибок анализа файлов (например, не хватает прав на чтение файла);
- время выполнения утилиты.

 

Пример исполнения утилиты из командной строки:

``` 
$ ./scan_util /Users/user/Downloads

 
====== Scan result ======
Processed files: 150
JS detects: 5
Unix detects: 1
macOS detects: 2
Errors: 1
Exection time: 00:00:31 345
=========================
```

# Запуск
Для проверки работоспособности удобно использовать тест на слом:
```
$ ./TestSearch /Users/user/test_dir
```
Тест генирирует заданное дефолтное количество файлов с определенным количеством "подозрительных" строк в указанной директории, 
затем ищет и выводит отчет, а также сверяет количество найденных вхождений с заданными.

# Реализация
Поиск выполняется проходом по regular-файлам в директории. Каждый файл обрабатывается асинхронно (потенциально тред пулом, может зависеть от компилятора).
Для файла фильтруются возможные подозрительные строки по расширению.
Оставшиеся строки ищутся алгоритмом Ахо-Корасика (примерно за линию от длины файла).

Так же реализована возможность искать перебором по строкам файла (для больших многострочных файлов) (см. `SearchMode::Simple`)
