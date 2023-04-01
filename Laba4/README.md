# Важно
Окна картинок имеют говорящие названия. Если что, смотрите на них

# 1.	Написать свою функцию преобразования Фурье (прямое и обратное) используя «лобовой подход».
Дело сделано, функция работает. Называется `forward_fourier_tf_comparison` и `backward_fourier_tf_comparison` для прямого и обратного преобразования соответственно. В них происходит преобразование и потом еще сравнение с встроенным.
На картинке слева направо исходное изображение, мое преобразование и встроенное преобразование. Картиник две - для прямого и обратного
![image](https://user-images.githubusercontent.com/90345098/229307221-408e1ec1-803e-417f-b81f-155c734aaf16.png)
![image](https://user-images.githubusercontent.com/90345098/229307302-a79194fd-8483-46e3-bd2d-2aa7385b7682.png)

# 2.	Написать преобразование Фурье используя алгоритм Radix-2 (по основанию 2, или «Бабочка») 
Готово, работает. Функция одна `fft`, у нее есть булевый аргумент, который отвечает за прямое или обратное преобразоование (вызывают прямую/обратную рекурсии).
На картинке слева направо исходное изображение, прямое FFT и обратное FFT.
![image](https://user-images.githubusercontent.com/90345098/229307457-d23ac4a6-dc2e-4424-b20d-14189e6476f0.png)

# 3.	Сравнить быстродействие из функций из первой и второй части, а также с преобразованием из OpenCV
Тож сделано. Вышло быстрее, но не сильно... Ладно
![image](https://user-images.githubusercontent.com/90345098/229307393-1db02724-08d0-4ffc-b6fd-dccab554be4a.png)

# 7.	Произвести по отдельности свёртку какого-либо изображения  с ядром фильтров: Собеля (по горизонтали и вертикали), усредняющего (BoxFilter), Лапласа  . Необходимо «красиво» вывести магнитуду образа Фурье исходного изображения и ядра свёртки.
Тоже готово. Функция работы - `convolution_task`. Сделано как и было прошено - сначала исходная картинка и ядро свертки переводились в общи размер (увеличивались), потом переходили в спектр, эти спектры выводились. Затем картинка умножалась на спектр, итоговая картинка обрезалась до исходника, проходила обратное преобразование обратное.
На четырех картинках: выглядывающий исходный Фурье; спектр Фурье; спектр ядра; обратно преобразованный Фурье.
![image](https://user-images.githubusercontent.com/90345098/229307617-9850a5c0-207d-4aef-8132-a4a14f4b9eac.png)
![image](https://user-images.githubusercontent.com/90345098/229307633-fbbe0645-843c-477e-85a2-6e932d3d1758.png)
![image](https://user-images.githubusercontent.com/90345098/229307642-94f54169-ca88-4ce7-b526-77e728620098.png)
![image](https://user-images.githubusercontent.com/90345098/229307653-df8cdd4f-95c3-4555-98a7-bd655f554537.png)


# 8.	Взять какое-нибудь изображение и в его спектре обрезать в одном случае элементы спектра с высокими частотами, в другом – низкими. А потом выполнить обратное преобразование на основе полученных спектров:
Тоже все готово. Функция `filter_task`. Сначала картинка переводится в спектр, потом из нее вырезается круг. В зависимости от флага `isUpper` отрезаются или верхние или нижние частоты.
На двух картинках последовательно входное изображение, изображение с порезанным спектром и итоговое изображение. Да, на второй картинке ЕСТЬ обрезанный спектр, но он очень маленький)
![image](https://user-images.githubusercontent.com/90345098/229308109-aa222a64-93e7-4b04-8839-d5b60e92b76c.png)
![image](https://user-images.githubusercontent.com/90345098/229308114-f747b329-e392-46f4-ad84-150160c2cb53.png)


# 9.	Провести корреляцию (сравнение) изображений автомобильных номеров по очереди с 3-мя символами. Полученный образ Фурье обратно преобразовать в обычное изображение. Найти на нём наибольшее значение, которое принимают элементы. Отнять от этого значения небольшое число (около 0.01). Использовать полученное число в качестве порога для пороговой фильтрации от полученного изображения.
Все сделано в точности, как и сказано. Функция `correlation_task`. Кроме того, перед работой изображения инвертированы, потому что вы так предлагали на лекции 30.03)). Картинок там выводится много, но важных, я думаю, 3.
Вот они - начальное изображение номера, начальное изображение буквы и итоговая корреляция - найдены все буквы, и еще верхние части восьмерок - видимо их отверстия похожи на отверстия букв А. В свою защиту скажу, что в вашем примере тоже были найдены куски 0 и 7 (потому что у нулей верх плоский, а у семерок ножки косые - похожи на части буквы А).
![image](https://user-images.githubusercontent.com/90345098/229308295-d5e4a198-37ad-4871-ad95-09599c156e3a.png)