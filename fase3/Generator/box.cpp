//
// Created by goncalo on 24-02-2024.
//

#include "headers/box.h"

Box::Box(float s, int d) {
    size = s;
    div = d;
}

// face de cima
std::vector<Point> Box::genPoints(){
    std::vector<Point> points;

    float length = size / 2.0;
    float square_size = size / div;
    float x1 = 0, x2 = 0, z1 = 0, z2 = 0;

    for (int i = 0; i < div; i++) {
        for (int j = 0; j < div; j++) {
            x1 = -length + (i * square_size);
            x2 = -length + ((i + 1) * square_size);
            z1 = -length + (j * square_size);
            z2 = -length + ((j + 1) * square_size);

            //Face de cima == plano XZ -> +TranslaçãoY(length) (parte de fora)
            points.push_back(Point(x1,length,-z1));
            points.push_back(Point(x2,length,-z1));
            points.push_back(Point(x1,length,-z2));

            points.push_back(Point(x2,length,-z1));
            points.push_back(Point(x2,length,-z2));
            points.push_back(Point(x1,length,-z2));

            // Face de baixo == plano XZ -> -TranslaçãoY(length) (parte de dentro)
            points.push_back(Point(x1,-length,z1));
            points.push_back(Point(x2,-length,z1));
            points.push_back(Point(x1,-length,z2));

            points.push_back(Point(x2,-length,z1));
            points.push_back(Point(x2,-length,z2));
            points.push_back(Point(x1,-length,z2));

            // Face lateral da frente == plano XZ -> RotZ(-90º) + TranslaçãoX(length) (parte de dentro)
            points.push_back(Point(length,-x1,-z1));
            points.push_back(Point(length,-x2,-z1));
            points.push_back(Point(length,-x1,-z2));


            points.push_back(Point(length,-x2,-z1));
            points.push_back(Point(length,-x2,-z2));
            points.push_back(Point(length,-x1,-z2));


            // Face lateral de trás == plano XZ -> RotZ(90º) - TranslaçãoX(length) (parte de dentro)
            points.push_back(Point(-length,x1,-z1));
            points.push_back(Point(-length,x2,-z1));
            points.push_back(Point(-length,x1,-z2));

            points.push_back(Point(-length,x2,-z1));
            points.push_back(Point(-length,x2,-z2));
            points.push_back(Point(-length,x1,-z2));

            // Face lateral lado esquerdo == plano XZ -> RotX(90º) + TranslaçãoZ(length) (parte da frente)
            points.push_back(Point(x1,z1,length));
            points.push_back(Point(x2,z1,length));
            points.push_back(Point(x1,z2,length));

            points.push_back(Point(x2,z1,length));
            points.push_back(Point(x2,z2,length));
            points.push_back(Point(x1,z2,length));

            // Face lateral lado direito == plano XZ -> RotX(-90º) - TranslaçãoZ(length) (parte da frente)
            points.push_back(Point(x1,-z1,-length));
            points.push_back(Point(x2,-z1,-length));
            points.push_back(Point(x1,-z2,-length));

            points.push_back(Point(x2,-z1,-length));
            points.push_back(Point(x2,-z2,-length));
            points.push_back(Point(x1,-z2,-length));
        }
    }
    return points;
}




