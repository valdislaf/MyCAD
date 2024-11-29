import os

# Получаем список всех файлов в текущей директории
files = os.listdir('.')

# Открываем файл files.txt для записи
with open('files.txt', 'w') as f:
    # Записываем имена файлов в files.txt
    for file in files:
        f.write(file + '\n')
