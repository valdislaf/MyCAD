echo Компиляция программы...
clang++ -v -O2 -std=c++17 -L"C:/Qt/Tools/llvm-mingw1706_64/x86_64-w64-mingw32/lib" -L"C:/Qt/llvm-mingw_64/lib"  -lQt6Graphs -lQt6Core -lQt6Gui -lQt6Widgets -stdlib=libc++ -Wall -I"C:/Qt/llvm-mingw_64/include/QtGraphs"  -I"C:/Qt/llvm-mingw_64/include" -I"C:/Qt/llvm-mingw_64/include/QtCore" -I"C:/Qt/llvm-mingw_64/include/QtGui" -I"C:/Qt/llvm-mingw_64/include/QtWidgets" -L"C:/Qt/llvm-mingw_64/lib" -stdlib=libstdc++ -L"C:\msys64\mingw64\x86_64-w64-mingw32\lib" -o mycad ShapeManager.cpp DrawingManager.cpp DrawingWidget.cpp EventHandling.cpp Grid.cpp HandleManager.cpp main.cpp MenuInit.cpp MenuMain.cpp MyCAD.cpp Shape.cpp ShapeCircle.cpp ShapeLine.cpp



echo Компиляция завершена.
pause


clang++ -v -O2 -std=c++17 -Wall -I"C:/Qt/llvm-mingw_64/include" -I"C:/Qt/llvm-mingw_64/include/QtCore" -I"C:/Qt/llvm-mingw_64/include/QtGui"  -L"C:/Qt/llvm-mingw_64/lib" -stdlib=libstdc++ -c ShapeLine.cpp

g++ -std=c++17  -I"C:\Qt\mingw_64\include" -L"C:\Qt\Tools\MinGW\lib" -I"C:\Qt\Tools\MinGW\include\QtWidgets" -I"C:\Qt\Tools\MinGW\QtGui" -I"C:\Qt\mingw_64\include\QtCore" -lQt6Core -lQt6Gui -lQt6Widgets -o mycad ShapeManager.cpp DrawingManager.cpp DrawingWidget.cpp EventHandling.cpp Grid.cpp HandleManager.cpp main.cpp MenuInit.cpp MenuMain.cpp MyCAD.cpp Shape.cpp ShapeCircle.cpp ShapeLine.cpp


g++ -std=c++17 -lstdc++ -I"C:/Qt/mingw_64/include" -L"C:/Qt/mingw_64/lib" -I"C:/Qt/mingw_64/include/QtWidgets" -I"C:/Qt/mingw_64/include/QtGui" -I"C:/Qt/mingw_64/include/QtCore" -lQt6Core -lQt6Widgets -lQt6Gui -static -static-libgcc -static-libstdc++ -o mycad ShapeManager.cpp DrawingManager.cpp DrawingWidget.cpp EventHandling.cpp Grid.cpp HandleManager.cpp main.cpp MenuInit.cpp MenuMain.cpp MyCAD.cpp Shape.cpp ShapeCircle.cpp ShapeLine.cpp



g++ -std=c++17 -lstdc++ -I"/mnt/c/Qt/mingw_64/include" -L"/mnt/c/Qt/mingw_64/lib" -I"/mnt/c/Qt/mingw_64/include/QtWidgets" -I"/mnt/c/Qt/mingw_64/include/QtGui" -I"/mnt/c/Qt/mingw_64/include/QtCore" -lQt6Core -lQt6Widgets -lQt6Gui -static -static-libgcc -static-libstdc++ -o mycad ShapeManager.cpp DrawingManager.cpp DrawingWidget.cpp EventHandling.cpp Grid.cpp HandleManager.cpp main.cpp MenuInit.cpp MenuMain.cpp MyCAD.cpp Shape.cpp ShapeCircle.cpp ShapeLine.cpp

g++ -std=c++17 -o mycad \
    ShapeManager.cpp DrawingManager.cpp DrawingWidget.cpp EventHandling.cpp Grid.cpp HandleManager.cpp \
    main.cpp MenuInit.cpp MenuMain.cpp MyCAD.cpp Shape.cpp ShapeCircle.cpp ShapeLine.cpp \
    -I"/mnt/c/Qt/mingw_64/include" \
    -I"/mnt/c/Qt/mingw_64/include/QtWidgets" \
    -I"/mnt/c/Qt/mingw_64/include/QtGui" \
    -I"/mnt/c/Qt/mingw_64/include/QtCore" \
    -L"/mnt/c/Qt/mingw_64/lib" \
    -lQt6Widgets -lQt6Gui -lQt6Core

g++ -std=c++17 -fPIC -o mycad \
    ShapeManager.cpp DrawingManager.cpp DrawingWidget.cpp EventHandling.cpp Grid.cpp HandleManager.cpp \
    main.cpp MenuInit.cpp MenuMain.cpp MyCAD.cpp Shape.cpp ShapeCircle.cpp ShapeLine.cpp \
    moc_MyCAD.cpp moc_MenuInit.cpp moc_MenuMain.cpp moc_DrawingWidget.cpp \
    -I"/mnt/c/Qt/mingw_64/include" \
    -I"/mnt/c/Qt/mingw_64/include/QtWidgets" \
    -I"/mnt/c/Qt/mingw_64/include/QtGui" \
    -I"/mnt/c/Qt/mingw_64/include/QtCore" \
    -L"/mnt/c/Qt/mingw_64/lib" \
    -lQt6Widgets -lQt6Gui -lQt6Core
