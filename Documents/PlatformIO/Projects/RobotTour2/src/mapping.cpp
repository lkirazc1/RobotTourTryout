class Point {
    public: //constructor
        Point(int x, int y) {
            xCord = x;
            yCord = y;
        }

        int getX() {
            return xCord;
        }

        int getY() {
            return yCord;
        }

    private:
        int xCord;
        int yCord;
};

// Point point1 = Point(0,0);
Point list[2] = {Point(0,1), Point(1,1)} ;

void Path(Point cords[], Point pInitial, int initial_Direction) { //note that the input points must draw ONLY vertical or horizontal lines
    int deltaX;
    int deltaY;
    int final_Direction;
    for(int i = 0; i < sizeof(cords) / sizeof(cords[0]); i++) { //goes through all the list of cords
        deltaX = cords[i].getX() - pInitial.getX();
        deltaY = cords[i].getY() - pInitial.getY();

        if(deltaX > 0) { //right
            final_Direction = 90;
        } else if(deltaX < 0) { //left
            final_Direction = 270;
        } else if(deltaY > 0) { //up
            final_Direction = 0;
        } else if(deltaY < 0) { //down
            final_Direction = 180;
        } else {
            final_Direction = initial_Direction;
        }

        //turn in final direction, move

    }
}

//take in a list of points
//take in initial point
//take in initial direction

//calculate how many right turns it takes to move in the right direction

//move a certain amount of times forward

//set current direction and point as initial point and direction

