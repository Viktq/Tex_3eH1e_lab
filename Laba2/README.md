# Вторая лаба

Вторую лабу я уже делал недавно, так что тут все хорошо помню.

Все пункты выполнены, результат можно посмотреть прогнав программу. Общая важность - переключение между картинки происходит на ESC, так что для включения следующей жмите ее.

Первые три пункта представлены на этом рисунке.
![image](https://user-images.githubusercontent.com/90345098/211564170-e53e12bc-52f1-4c76-a04b-f36e1d640283.png)
Тут, собственно, бокс фильтр через итераторы, встроеный бокс и разница между ними (никакая, ха-ха). Время измеряется и пишется в консоль - 0.283000 для самодельного, 
0.017000 для строенного (что не удивительно). Сходство также записывается в консоль - 99.357200 %.

На следующем изображении 4 пункт.
![image](https://user-images.githubusercontent.com/90345098/211564599-adf22ed3-a0eb-4d37-b6fa-68413bb3f026.png)
Первая рамка - Гауссов фильтр, вторая - разница между боксом и гауссом, НО без логарифмиеского преобразования, потому что, во-первых, логарифмическое преобразования в 
OpenCV надо отдельно докачивать и ставить (а гайда на винду естесно нет); во-вторых, разницы между этими изображениями все равно нет...

Дальше пятый пункт - Unsharp маски с коэффициентом резкости 2.
![image](https://user-images.githubusercontent.com/90345098/211565361-a1c16ea8-eee3-408c-a3ed-32b997a6b6cb.png)
Кроме того, рази интереса я естесно потестил еще и другие коэффициенты резкости. Например, по-моему, что видно становится на 7-10 коэффициенте, но тогда становится плохо
Unsharp у Лапласа из последнего пункта, а мы же этого не хотим, правда?) Третья картинка - это их различия.

И наконец, 6 и 7 пункты.
![image](https://user-images.githubusercontent.com/90345098/211565698-8f94e5b0-5255-4baf-9f25-d4f099c3f8ab.png)
Фильтр Лапласа и его Unsharp.
Все работает, все на месте, можно проверять :).
