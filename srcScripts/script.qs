function drawAllShapes() {
    // 1. Синий залитый круг (x, y, радиус, цвет, заливка)
    drawCircle(100, 100, 50, "blue", true);
    
    // 2. Незалитый круг с бирюзовым контуром
    drawCircle(250, 100, 50, "cyan", false);
    
    // 3. Треугольник (x1, y1, x2, y2, x3, y3, цвет)
    drawTriangle(400, 50, 350, 150, 450, 150, "green");
    
    // 4. Прямоугольник (x, y, ширина, высота, цвет)
    drawRectangle(100, 200, 150, 100, "red");
    
    // 5. 
    drawLine(300, 200, 400, 200, "magenta");
    drawLine(300, 220, 400, 220, "yellow");
    drawLine(300, 240, 400, 240, "darkGreen");
    drawLine(300, 260, 400, 260, "darkBlue");
}

drawAllShapes();