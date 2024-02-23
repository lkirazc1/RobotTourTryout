#ifndef MAPPING_H
#define MAPPING_H
#include <vector>
#include <iostream>
#include "Drivetrain.h"
#include "Movements.h"

int getPredictedDirection(int currentDirection, int angleOffset);

struct Instruction
{
    int slits;
    Drivetrain::Movement movement;
    int speed_perc;
};

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

        void incX() {
            xCord ++;
        }

        void decX() {
            xCord --;
        }

        void incY() {
            yCord ++;
        }

        void decY() {
            yCord --;
        }


    private:
        int xCord;
        int yCord;
};



std::vector<Instruction> getPath(Point cords[], int len, Point pInitial, int int_Direction, int turning_speed, int straight_speed) { //note that the input points must draw ONLY vertical or horizontal lines
    int deltaX;
    int deltaY;
    int final_Direction;
    int current_Direction = int_Direction;
    int predicted_Direction;
    const int backwards_speed = 66;
    boolean goReverse;
    Point current_Point = pInitial;
    std::vector<Instruction> instructions;
    instructions.push_back({CMtoSteps(25), Drivetrain::FORWARD, 68});



    for (int i = 0; i < len; i++) { //goes through all the list of cords
        deltaX = cords[i].getX() - current_Point.getX();
        deltaY = cords[i].getY() - current_Point.getY();


        if(deltaX > 0) { //right
            final_Direction = 90;
        } else if(deltaX < 0) { //left
            final_Direction = 270;
        } else if(deltaY > 0) { //up
            final_Direction = 0;
        } else if(deltaY < 0) { //down
            final_Direction = 180;
        } else {
            final_Direction = current_Direction;
        }


        goReverse = false;

        predicted_Direction = getPredictedDirection(current_Direction, 90); //gives turning direction
        if(predicted_Direction == final_Direction) {
            instructions.push_back({10000, Drivetrain::MOVE_RIGHT, turning_speed});
            current_Direction = final_Direction;
        } else {
            predicted_Direction = getPredictedDirection(current_Direction, 180);
            
            if(predicted_Direction == final_Direction) {
                goReverse = true;
            } else {
                predicted_Direction = getPredictedDirection(current_Direction, 270);

                if(predicted_Direction == final_Direction) {
                    instructions.push_back({10000, Drivetrain::MOVE_LEFT, turning_speed});
                    current_Direction = final_Direction;
                }
            }
        }

        if(deltaX != 0) { //Move forwards and updates position
            for(int j = 0; j < abs(deltaX); j++) {
                if(goReverse) {
                    // instructions.push_back({CMtoSteps(50), Drivetrain::BACKWARD, backwards_speed});
                    instructions.push_back({10000, Drivetrain::MOVE_RIGHT, turning_speed})
                    from sklearn.decomposition import PCA
from sklearn.cluster import KMeans

#PCA
pca = PCA(n_components=2)
pca_top_2 = pca.fit_transform(train[raw_num_cols])

tmp = pd.DataFrame(data = pca_top_2, columns = ['pca_1','pca_2'])
tmp['TARGET'] = train[TARGET]

fig,axs = plt.subplots(2,1,figsize = (12,6))
sns.scatterplot(data=tmp, y="pca_1", x="pca_2", hue='TARGET',ax=axs[0])
axs[0].set_title("Top 2 Principal Components")

#KMeans
kmeans = KMeans(7,random_state=RANDOM_SEED)
kmeans.fit(tmp[['pca_1','pca_2']])
sns.scatterplot( y= tmp['pca_1'],x = tmp['pca_2'],c = kmeans.labels_,cmap='viridis', marker='o', edgecolor='k', s=50, alpha=0.8,ax = axs[1])
axs[1].set_title("Kmean Clustring on First 2 Principal Components")
plt.tight_layout()
plt.show()
                } else {
                    instructions.push_back({CMtoSteps(50), Drivetrain::FORWARD, straight_speed});
                }

                if(deltaX > 0) {
                    current_Point.incX();
                } else if (deltaX < 0) {
                    current_Point.decX();
                }
                
            }
        } else if(deltaY != 0) {
            for(int j = 0; j < abs(deltaY); j++) {
                if(goReverse) {
                    instructions.push_back({CMtoSteps(50), Drivetrain::BACKWARD, backwards_speed});
                } else {
                    instructions.push_back({CMtoSteps(50), Drivetrain::FORWARD, straight_speed});
                }

                if(deltaY > 0) {
                    current_Point.incY();
                } else if (deltaY < 0) {
                    current_Point.decY();
                }
            }
        }
    }
    if (instructions.at(instructions.size() - 1).movement == Drivetrain::BACKWARD)
    {
        instructions[instructions.size() - 1].slits = 510;
    }
    else
    {
        // instructions[instructions.size() - 1].slits = 285;        
    }
    return instructions;
}

int getPredictedDirection(int currentDirection, int angleOffset) {
    int prediction = currentDirection + angleOffset;
    if(prediction >= 360) {
        prediction -= 360;
    }
    return prediction;
}
#endif