// Основная функция для рисования композиции
function drawArtComposition() {
    // Фон - большой светло-голубой прямоугольник
    drawRectangle(0, 0, 600, 400, "lightblue");
    
    // Солнце в правом верхнем углу
    drawCircle(500, 80, 40, "yellow", true);
    
    // Лучи солнца
    drawLine(500, 40, 500, 120, "gold", 3);
    drawLine(460, 80, 540, 80, "gold", 3);
    drawLine(480, 50, 520, 110, "gold", 3);
    drawLine(520, 50, 480, 110, "gold", 3);
    
    
    // Дом
    drawRectangle(150, 220, 120, 100, "sienna");
    
    // Крыша дома (треугольник)
    drawTriangle(140, 220, 210, 150, 280, 220, "brown");
    
    // Окно в доме
    drawRectangle(180, 250, 60, 40, "lightyellow", true);
    drawRectangle(180, 250, 60, 40, "black", false);
    
    // Перекладины окна
    drawLine(210, 250, 210, 290, "black", 2);
    drawLine(180, 270, 240, 270, "black", 2);
    
    // Дверь
    drawRectangle(220, 280, 40, 40, "darkred", true);
    
    // Ручка двери
    drawCircle(230, 300, 3, "gold", true);
    
    // Дерево
    drawRectangle(400, 200, 30, 100, "saddlebrown", true);
    
    // Крона дерева (три круга)
    drawCircle(415, 170, 40, "forestgreen", true);
    drawCircle(380, 190, 35, "forestgreen", true);
    drawCircle(450, 190, 35, "forestgreen", true);
    
    
    // Трава (зеленый прямоугольник внизу)
    drawRectangle(0, 320, 600, 80, "limegreen", true);
}
// Запускаем рисование
drawArtComposition();