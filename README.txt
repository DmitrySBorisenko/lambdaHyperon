Инструкция по применению:
- изменить значение dirPath в main.C сообразно с расположением входных root файлов
- запустить последовательно main.C со значением переменной mode=0,1,2 (конечно нескольно топорно, но поскольку код интерпретируется, то пока и так пойдет)
 результат в графическом режиме выполнения отобразится как гистограмма распределения инвариантной массы, но также сохранится в файл output.root 



Подробности:

- настройку всех катов можно осуществлять меняя значения переменных в секции TRIGGER BLOCK. 
После их изменения достаточно перезапустить с mode=2

- Режимы выполнения:
	1) mode=0 - собираем в chain входные файлы, подключаем Proof-lite, запускаем на доступных ядрах chain->Process("PhenixSelector.C"). 
		В качестве результата дерево charged в файле medival.root
	2) mode=1 - добавляем в chain только medival.root, подключаем Proof-lite, также параллельно обрабатываем chain->Process("ChargedSelector.C").
		Выхлоп - деревья protons и pions в файле output.root
	3) mode=2 - перебор частиц из protons|pions в одном потоке с применением всех катов, настраиваемых в секции TRIGGER BLOCK 
		и постоение распределения инвариантной массы