Чтобы скомпилировать нужны библиотеки SDL2_main и SDL2_ttf

Команда для компиляции:
g++ main.cpp -o editor.exe -I/ucrt64/include/SDL2 -L/ucrt64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -std=c++17

Не обращать внимание на ругания VS Code на отсуствие библиотеки, компилятор их автоматически подтянет и все выполнится без ошибок