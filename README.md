# Tex_3eH1e_lab

Итак, здесь у нас представлены все три лабы для этого семестра. В общем-то интересное начнется внутри каждой директории, но есть кое-что, что стоит упомянтуть заранее.

Во-первых, лабы я делал на Винде в VS2022. Поэтому, для функционировании OpenCV я шаманил с настройкой каждого проекта. Сделано это было так.

![image](https://user-images.githubusercontent.com/90345098/211558378-3e79bd95-e22d-417c-9d8c-ed10da2c6ddd.png)
При создании проекта нужно по порядку тыкнуть указаные кнопки. Откроется следующее меню.
![image](https://user-images.githubusercontent.com/90345098/211558619-4fb478e3-ce10-4977-968d-5906bf81b888.png)
Здесь добавляется путь до OpenCV. Далее в следующем меню
![image](https://user-images.githubusercontent.com/90345098/211559020-7b040650-371f-4a7f-b9f9-f10646c09735.png)
Тоже путь, но до папки lib. И наконец...
![image](https://user-images.githubusercontent.com/90345098/211559157-604e5d25-2286-4e0f-b399-b76562d740de.png)
Указать нужные lib файлы (дебаг версия, или обычная в зависимости от желаний)

И все - OpenCV на VS2022 работает и можно ей пользоваться.

Во-вторых, мне ПРИШЛОСЬ складировать ВСЕ картинки рядом с исполняемыми файлами. Мне это тоже не очень нравится, но если файлы лежат чуть дальше (тип, в каталоге ./img/img.jpg), происходит какая-то шляпа с доступом в память и любой код ломается. Такое могло бы быть, если бы в пути к каталогу были, скажем, русские буквы или еще какие символы. Но их нет - так что я оставил как есть. Работает и слава богу. Главное файлы нормально называть :)

На этом все, пошли в каждую директорию.
