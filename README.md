# Windmill

## Правила игры
Доска представляет собой сетку с двадцатью четырьмя точками. У каждого игрока есть девять фигур (так называемых «человечков»). Цель игры – формировать «мельницы». Мельница образуется, когда три фигуры выстраиваются в одну линию (горизонтально или вертикально). Образование мельницы позволяет убрать одну фигуру соперника с доски. Игрок побеждает, если сократит количество фигур противника до двух или оставит его без возможных ходов.

Игра проходит в три фазы:

1. **Размещение**: игроки поочередно размещают свои фигуры на пустых точках.
2. **Перемещение**: игроки перемещают фигуры на соседние точки.

### Размещение
Игра начинается с пустой доски. Игроки по очереди размещают свои фигуры на пустых точках. Если игрок выстраивает три свои фигуры в одну линию (вертикально или горизонтально), он образует мельницу и может убрать одну фигуру соперника с доски. Для удаления можно выбрать любую фигуру, но предпочтительно фигуру, не являющуюся частью мельницы соперника.

### Перемещение
Игроки продолжают чередовать ходы, перемещая свои фигуры на соседние точки. Фигуры не могут «перепрыгивать» через другие фигуры. Игроки стараются сформировать мельницы и убрать фигуры соперника так же, как в фазе размещения. Игрок может «сломать» мельницу, переместив фигуру из нее, а затем вернуть фигуру обратно, чтобы снова образовать ту же мельницу и убрать еще одну фигуру соперника. Это действие иногда называют «ударом». Когда у одного из игроков остается три фигуры, начинается третья фаза.

---

## Сборка и запуск

### Требования 
Для компиляции проекта требуется:
- Компилятор **g++**;
- ОС **Linux**.

*В директории находится уже скомпилированный файл **main***

### Компиляция
Чтобы скомпилировать проект, необходимо ввести команду:
```
g++ main.cpp -o main
```
### Запуск
Чтобы запустить программу, необходимо сначала скомпилировать проект, затем ввести команду:
```
./main
```
